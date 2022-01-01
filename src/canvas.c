#include <GL/gl.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "settings.h"
#include "mandelbrot.h"
#include "color_tools.h"
#include "canvas.h"

struct Canvas canvas;

void init_canvas(uint32_t w, uint32_t h, ESCAPE_TIME_FUNC escape_time_func) {
  canvas.w = w;
  canvas.h = h;
  canvas.data = malloc(w * h * 3 * sizeof(GLubyte));

  glGenTextures(1, &(canvas.texture));
  glBindTexture(GL_TEXTURE_2D, canvas.texture);

  canvas.escape_time = escape_time_func;

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, canvas.data);
  glBindTexture(GL_TEXTURE_2D, 0);
}


void draw_canvas(void) {
  const uint32_t w = canvas.w;
  const uint32_t h = canvas.h;

  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, w, 0, h, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glClear(GL_COLOR_BUFFER_BIT);

  glBindTexture(GL_TEXTURE_2D, canvas.texture);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, canvas.data);
  glEnable(GL_TEXTURE_2D);

  glBegin( GL_QUADS );
        glTexCoord2i(0, 0);	  glVertex2i(0, 0);
        glTexCoord2i(0, 1); 	glVertex2i(0, h);
        glTexCoord2i(1, 1); 	glVertex2i(w, h);
        glTexCoord2i(1, 0); 	glVertex2i(w, 0);
  glEnd();

  glDisable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}


void calculate_fractal(GLubyte *color_buffer, mandelbrot_f x0, mandelbrot_f x1, mandelbrot_f y0, mandelbrot_f y1, uint32_t max_iter) {
  const uint32_t w = canvas.w;
  const uint32_t h = canvas.h;

	const mandelbrot_f dx = (x1 - x0) / w;
	const mandelbrot_f dy = (y1 - y0) / h;

  #pragma omp parallel for schedule(static,1) collapse(2)
	for(int i = 0; i < w; ++i) {
		for(int j = 0; j < h; ++j) {

      float esc_time = (canvas.escape_time)(x0 + i * dx, y0 + j * dy, max_iter);
      //linear interpolation of color
      uint16_t color_index = (esc_time / max_iter) * MAX_COLOR_INDEX;

      canvas.data[ GET_R(i, j, w) ] = color_buffer[color_index * 3];
			canvas.data[ GET_G(i, j, w) ] = color_buffer[color_index * 3 + 1];
			canvas.data[ GET_B(i, j, w) ] = color_buffer[color_index * 3 + 2];
    	}
    }
}

void calculate_fractal_iterative(
  GLubyte *color_buffer,
  mandelbrot_f x0,
  mandelbrot_f x1,
  mandelbrot_f y0,
  mandelbrot_f y1,
  uint32_t max_iter,
  uint32_t chunk_size) {

  const uint32_t w = canvas.w;
  const uint32_t h = canvas.h;

	const mandelbrot_f dx = (x1 - x0) / w;
	const mandelbrot_f dy = (y1 - y0) / h;
  
  #pragma omp parallel for schedule(static,1) collapse(2)
	for(int i = 0; i < w; i += chunk_size) {
		for(int j = 0; j < h; j += chunk_size) {

      float esc_time = (canvas.escape_time)(x0 + (i + chunk_size / 2 ) * dx, y0 + (j + chunk_size / 2) * dy, max_iter);
      //linear interpolation of color
      uint16_t color_index = (esc_time / max_iter) * MAX_COLOR_INDEX;

      for(int x = i; x < i + chunk_size && x < w; x++) {
        for(int y = j; y < j + chunk_size && y < h; y++) {
          canvas.data[ GET_R(x, y, w) ] = color_buffer[color_index * 3];
          canvas.data[ GET_G(x, y, w) ] = color_buffer[color_index * 3 + 1];
          canvas.data[ GET_B(x, y, w) ] = color_buffer[color_index * 3 + 2];      
        } 
      }

    }
  }
}
