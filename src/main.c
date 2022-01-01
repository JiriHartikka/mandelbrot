
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <argp.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <time.h>
#include "settings.h"
#include "mandelbrot.h"
#include "zoom_state.h"
#include "color_tools.h"
#include "canvas.h"



static char doc[] =
  "Mandelbrot -- a simple interactive Mandelbrot set zoomer in openGL.";

static char args_doc[] = "";

static struct argp_option options[] = {
   {"debug",   'd', 0,  0, "Display debug information on screen and stdout."},
   {"max-iteration",   'm', "INT",  0, "Maximum number of iterations in escape time calculation."},
   {"x0", 'x', "FLOAT", 0, "Smallest x coordinate on the screen."},
   {"x1", 'y', "FLOAT", 0,  "Largest x coordinate on the screen."},
   {"y0", 'z', "FLOAT", 0, "Smallest y coordinate on the screen."},
   {"y1", 'q', "FLOAT", 0, "Largest y coordinate on the screen."},
   {"width", 'w', "INT", 0, "Width of the canvas."},
   {"height", 'h', "INT", 0, "Height of the canvas."},
   {"scale-window", 's', "FLOAT", 0, "Scales the window by the given factor (without affecting the resolution)."},
   {"fractal-name", 'f', "STRING", 0, "Fractal to use. Default = mandelbrot."},
   { 0 }
};

// lazy fix implicit declaration warning in build 
extern int setenv (const char *__name, const char *__value, int __replace);

#define SLEEP() nanosleep((const struct timespec[]){{0, 20000000L}}, NULL)

struct arguments {
   mandelbrot_f x0, x1, y0, y1;
   int debug;
   uint32_t w, h, max_iteration;
   mandelbrot_f window_scale;
   char *fractal_name;
};

void try_parse_f(mandelbrot_f *to_parse, char *input) {
   sscanf(input, "%Lf", to_parse);
}

void try_parse_i(uint32_t *to_parse, char *input) {
   sscanf(input, "%d", to_parse);
}

void try_parse_s(char **to_parse, char *input) {
   *to_parse = malloc(strlen(input) + 1);
   strcpy(*to_parse, input);
}

static error_t parse_opt (int key, char *arg, struct argp_state *state) {
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;

  mandelbrot_f parsed_f;
  uint32_t parsed_i;

   switch (key) {
      case 'x':
         try_parse_f(&arguments->x0, arg);
      break;
      case 'y':
         try_parse_f(&arguments->x1, arg);
      break;
      case 'z':
         try_parse_f(&arguments->y0, arg);
      break;
      case 'q':
         try_parse_f(&arguments->y1, arg);
      break;
      case 'w':
         try_parse_i(&arguments->w, arg);
      break;
      case 'h':
         try_parse_i(&arguments->h, arg);
      break;
      case 'm':
         try_parse_i(&arguments->max_iteration, arg);
      break;
      case 's':
         try_parse_f(&arguments->window_scale, arg);
      break;
      case 'd':
         arguments->debug = 1;
      break;
      case 'f':
         try_parse_s(&arguments->fractal_name, arg);
      break;
      case ARGP_KEY_END:
      break;
      default:
      return ARGP_ERR_UNKNOWN;
   }
  return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };


void adjust_aspect_ratio(struct arguments *arguments) {
   //note: width needs to be divisible by 4 (glDrawPixels limitation)
   //quick hack: force width to nearest num divisible by 4.
   mandelbrot_f dx = arguments->x1 - arguments->x0;
   mandelbrot_f dy = arguments->y1 - arguments->y0;
   if(arguments->h == 0 && arguments->w == 0) {
      arguments->h = DEFAULT_CANVAS_H;
      arguments->w = DEFAULT_CANVAS_W;
   } else if (arguments->h == 0) {
      arguments->w = arguments->w + (4 - arguments->w % 4);
      //adjust height so that the image is not distorted
      arguments->h = arguments->w * dy / dx;
   } else if(arguments->w == 0) {
      //adjust width so that the image is not distorted
      arguments->w = arguments->h * dx / dy;
      arguments->w = arguments->w + (4 - arguments->w % 4);
      //readjust height once more
      arguments->h = arguments->w * dy / dx;
   }
}

GLubyte *color_buffer;
zoom_state *zoom;
uint16_t max_iteration;

void display() {
   draw_canvas();
   glutSwapBuffers();

   if(zoom->debug) {
      display_depth(zoom);
   }
}

void idle() {
   if (zoom->is_ready && zoom->is_display_ready) {
      SLEEP();
      return;
   }

   if (zoom->is_ready && !zoom->is_display_ready) {
      glutPostRedisplay();
      zoom->is_display_ready = true;
   }

   SLEEP();
   glutPostRedisplay();
}

void* worker_main() {

   while (true) {

      if (!zoom->is_interrupted) {
         // wait for a new zoom event
         SLEEP();
         continue;
      } else {
         // zoom event, start working and "consume" the event
         zoom->is_interrupted = false;
      }

      uint32_t chunk_size = zoom->w / 2;
      zoom->is_ready = false;
      zoom->is_display_ready = false;

      do {
         chunk_size /= 2;
         calculate_fractal_iterative(color_buffer, zoom->x0, zoom->x1, zoom->y0, zoom->y1, zoom->max_iter, chunk_size);

         if (zoom->is_interrupted) {
            // zoom was interrupted, start over
            zoom->is_ready = false;
            zoom->is_display_ready = false;
            break;
         }
      } while(chunk_size > 1);
      // zooming has finished
      zoom->is_ready = true;
   }

   pthread_exit(0);
}

void onMouseClick(int button, int state, int x, int y) {
   mandelbrot_f dx = (zoom->x1 - zoom->x0) / zoom->w;
   mandelbrot_f dy = (zoom->y1 - zoom->y0) / zoom->h;
   mandelbrot_f x_new = zoom->x0 + x * (1 / zoom->window_scale) * dx;
   mandelbrot_f y_new = zoom->y0 + (zoom->h - y) * (1 / zoom->window_scale) * dy;

   if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
      zoom_focus(zoom, x_new, y_new, 2.0);
   } else if ( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
      zoom_focus(zoom, zoom->x0 + (zoom->x1 - zoom->x0) / 2 , zoom->y0 + (zoom->y1 - zoom->y0) / 2, 0.5);
   }

   zoom->is_interrupted = true;
}

void check_and_set_omp_cancellation(char** argv) {
   char *hasCancel = getenv("OMP_CANCELLATION");
   if (hasCancel == NULL) {
      printf("Bootstrapping OMP with cancellation capacity, restarting\n");
      setenv("OMP_CANCELLATION", "true", 1);
      // Restart the program
      int output = execvp(argv[0], argv);
      // Execution should not continue past here
      printf("Bootstrapping failed with code %d\n",output);
      exit(1);
   } else {
      printf("OMP_CANCELLATION is enabled\n");
   }

}

int main(int argc, char** argv) {
   struct arguments arguments;
   arguments.x0 = -2.5;
   arguments.x1 = 1.0;
   arguments.y0 = -1.0;
   arguments.y1 = 1.0;
   arguments.w = 0; //set by adjust_aspect_ratio
   arguments.h = 0; //set by adjust_aspect_ratio
   arguments.max_iteration = DEFAULT_MAX_ITER;
   arguments.debug = 0;
   arguments.window_scale = 1.0;
   arguments.fractal_name = "mandelbrot";
   float (*escape_time)(mandelbrot_f, mandelbrot_f, uint32_t);
   pthread_t worker_id;

   check_and_set_omp_cancellation(argv);

   argp_parse(&argp, argc, argv, 0, 0, &arguments);

   printf("fractal name: %s\n", arguments.fractal_name);

   if(strcmp(arguments.fractal_name, "mandelbrot") == 0) {
      escape_time = escape_time_mandelbrot;
   }
   else if(strcmp(arguments.fractal_name, "burning_ship") == 0) {
      escape_time = escape_time_burning_ship;
   }
   else if(strcmp(arguments.fractal_name, "z3") == 0) {
      escape_time = escape_time_z3;
   }
   else if(strcmp(arguments.fractal_name, "tan_mixture") == 0) {
     escape_time = escape_time_tan_mixture;
   }
   else {
      printf("Error: Invalid argument --fractal-name %s\n", arguments.fractal_name);
      return 2;
   }


   adjust_aspect_ratio(&arguments);
   zoom = make_zoom_state(arguments.w, arguments.h, arguments.debug, arguments.x0, arguments.x1, arguments.y0, arguments.y1, arguments.max_iteration, arguments.window_scale);
   // signal worker to compute
   zoom->is_interrupted = true;
   color_buffer = init_colors(MAX_COLOR_INDEX + 1);

   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE);
   glutInitWindowSize(arguments.w * arguments.window_scale, arguments.h * arguments.window_scale);
   glutCreateWindow("Mandelbrot");
   init_canvas(arguments.w, arguments.h, escape_time);
   pthread_create(&worker_id, NULL, worker_main, NULL);

   glutDisplayFunc(display);
   glutMouseFunc(onMouseClick);
   glutIdleFunc(idle);
   glutMainLoop();

   return 0;
}
