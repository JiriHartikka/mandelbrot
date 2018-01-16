#include <GL/gl.h>
#include <stdlib.h>
#include <complex.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "settings.h"
#include "mandelbrot.h"

GLubyte *make_canvas(uint32_t w, uint32_t h ) {
	return malloc(w * h * 3 * sizeof(GLubyte));
}


void init_canvas(GLubyte *canvas, GLuint *tex, uint32_t w, uint32_t h) {

  glGenTextures(1, tex);
  glBindTexture(GL_TEXTURE_2D, *tex);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, canvas);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void draw_canvas(GLubyte* canvas, GLuint texture, uint32_t w, uint32_t h, mandelbrot_f window_scale) {

  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, w, 0, h, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glClear(GL_COLOR_BUFFER_BIT);

  glBindTexture(GL_TEXTURE_2D, texture);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, canvas);
  glEnable(GL_TEXTURE_2D);

  glBegin( GL_QUADS );
        glTexCoord2i(0, 0);	  glVertex2i(0, 0);
        glTexCoord2i(0, 1); 	glVertex2i(0, h);
        glTexCoord2i(1, 1); 	glVertex2i(w, h);
        glTexCoord2i(1, 0); 	glVertex2i(w, 0);
  glEnd();

  glDisable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  //glFlush();
  //glutSwapBuffers();
}

/*void draw_canvas(GLvoid* canvas, uint32_t w, uint32_t h, mandelbrot_f window_scale) {
	glPixelZoom(window_scale, window_scale);
	glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_BYTE, canvas);

}*/

void calculate_mandelbrot(GLubyte *canvas, GLubyte *color_buffer, uint32_t w, uint32_t h, mandelbrot_f x0, mandelbrot_f x1, mandelbrot_f y0, mandelbrot_f y1, uint32_t max_iter) {
	mandelbrot_f dx = (x1 - x0) / w;
	mandelbrot_f dy = (y1 - y0) / h;

  #pragma omp parallel for schedule(static,1) collapse(2)
	for(int i = 0; i < w; ++i) {
		for(int j = 0; j < h; ++j) {
      uint16_t esc_time = (*escape_time)(x0 + i * dx, y0 + j * dy, max_iter);

      canvas[ GET_R(i, j, w) ] = color_buffer[esc_time * 3];
			canvas[ GET_G(i, j, w) ] = color_buffer[esc_time * 3 + 1];
			canvas[ GET_B(i, j, w) ] = color_buffer[esc_time * 3 + 2];
    	}
    }
}

inline uint16_t escape_time_mandelbrot(mandelbrot_f x0, mandelbrot_f y0, uint32_t max_iter) {
	mandelbrot_f x = 0.0;
	mandelbrot_f y = 0.0;
	mandelbrot_f xtemp;
	uint16_t iteration = 0;

  //mandelbrot_f modulus_squared = x*x + y*y;

	//while((x*x + y*y < 4) && (iteration < max_iter)) {
  while(( x*x + y*y < 4) && (iteration < max_iter)) {
		xtemp = x*x - y*y + x0;
		y = 2*x*y + y0;
		x = xtemp;
		iteration++;
	}

  if(iteration == max_iter) {
    //return max_iter;
    return 10000;
  }

  for(int i = 0; i<2; i++) {
    xtemp = x*x - y*y + x0;
		y = 2*x*y + y0;
		x = xtemp;
		iteration++;
  }

  mandelbrot_f renormalized = iteration + 1 - log(log( sqrt(x*x + y*y) )) / log(2);

  return (renormalized / max_iter) * 10000;

  //return MIN(max_iter, iteration + 1 - log(log( sqrt(x*x + y*y) )) / log(2));
  //uint16_t renormalized = iteration + 1 - log(log( sqrt(x*x + y*y) )) / log(2);
  //return MIN(max_iter - 1, renormalized);
}


inline uint16_t escape_time_burning_ship(mandelbrot_f x0, mandelbrot_f y0, uint32_t max_iter) {
  mandelbrot_f x = 0.0;
  mandelbrot_f y = 0.0;
  mandelbrot_f xtemp;
  uint16_t iteration = 0;

  while((x*x + y*y < 4) && (iteration < max_iter)) {
    xtemp = x*x - y*y - x0;
    y = 2*fabs(x*y) - y0;
    x = xtemp;
    iteration++;
  }

  return iteration;
}

inline uint16_t escape_time_z3(mandelbrot_f x0, mandelbrot_f y0, uint32_t max_iter) {
  mandelbrot_f x = 0.0;
  mandelbrot_f y = 0.0;
  mandelbrot_f xtemp;
  uint16_t iteration = 0;

  while((x*x + y*y < 4) && (iteration < max_iter)) {
    xtemp = x*x*x - 3*x*y + x0;
    y = 3*x*x - y*y*y + y0;
    x = xtemp;
    iteration++;
  }

  return iteration;
}

