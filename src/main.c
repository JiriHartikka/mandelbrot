
#include <GL/gl.h>
#include <GL/glut.h> 
#include <stdlib.h>
#include <stdio.h>
#include "settings.h"
#include "mandelbrot.h"
#include "zoom_state.h"
#include "color_tools.h"

GLubyte *canvas;
GLubyte *color_buffer;
zoom_state *zoom;


void display() {
   int x,y;
   calculate_mandelbrot(canvas, color_buffer, zoom->w, zoom->h, zoom->x0, zoom->x1, zoom->y0, zoom->y1);
   draw_canvas(canvas, CANVAS_W, CANVAS_H);
   if(zoom->debug) {
      display_depth(zoom);
   }
   glFlush();
}

void onMouseClick(int button, int state, int x, int y) {
   const double dx = (zoom->x1 - zoom->x0) / zoom->w;
   const double dy = (zoom->y1 - zoom->y0) / zoom->h;
   const double x_scale = CANVAS_W / (double)WINDOW_W;
   const double y_scale = CANVAS_H / (double)WINDOW_H;
   const double x_new = zoom->x0 + x * x_scale * dx;
   const double y_new = zoom->y0 + y * y_scale * dy;

   if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
      zoom_focus(zoom, x_new, y_new, 2.0);
      display();
   } else if ( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
      zoom_focus(zoom, zoom->x0 + (zoom->x1 - zoom->x0) / 2 , zoom->y0 + (zoom->y1 - zoom->y0) / 2, 0.5);
      display();
   }
} 

int main(int argc, char** argv) {
   long double x0 = -2.5, x1 = 1.0, y0 = -1.0, y1 = 1.0;
   int debug = 0;
   if(argc == 6) { //debug, x0, x1, y0, y1
      debug = atoi(argv[1]);
      sscanf(argv[2], "%Le", &x0);
      sscanf(argv[3], "%Le", &x1);
      sscanf(argv[4], "%Le", &y0);
      sscanf(argv[5], "%Le", &y1);
   } else if(argc == 2) { //debug
      debug = atoi(argv[1]);
   } else {
      if(argc != 1) {
         printf("Usage: program | program <debug> | program <debug> <x0> <x1> <y0> <y1>\n");
         return 2;
      }
   }
   canvas = make_canvas(CANVAS_W, CANVAS_H);
   zoom = make_zoom_state(CANVAS_W, CANVAS_H, debug, -2.5, 1.0, -1.0, 1.0);
   color_buffer = init_colors(MAX_ITER);
   int i;
   glEnable(GL_TEXTURE_RECTANGLE);
   glutInit(&argc, argv);  
   glutInitWindowSize(WINDOW_W, WINDOW_H);
   glutInitWindowPosition(0, 0);
   glutCreateWindow("Mandelbrot");
   glutDisplayFunc(display); 
   glutMouseFunc(onMouseClick);
   glutMainLoop();           
   return 0;
}
