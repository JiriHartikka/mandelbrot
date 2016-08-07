#include <GL/gl.h>
#include <stdlib.h>  
#include <complex.h>
#include <assert.h>
#include <stdio.h>
#include "settings.h"
#include "mandelbrot.h"

GLubyte *make_canvas(uint32_t w, uint32_t h ) {
	return malloc(w * h * 3 * sizeof(GLubyte));
}

void draw_canvas(GLvoid* canvas, uint32_t w, uint32_t h, mandelbrot_f window_scale) {
	glPixelZoom(window_scale, window_scale);
	glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_BYTE, canvas);

}

inline uint16_t escape_time(mandelbrot_f x0, mandelbrot_f y0, uint32_t max_iter) {
	mandelbrot_f x = 0.0;
	mandelbrot_f y = 0.0;
	mandelbrot_f xtemp;
	uint16_t iteration = 0;
	while((x*x + y*y < 4) && (iteration < max_iter)) {
		xtemp = x*x - y*y + x0;
		y = 2*x*y + y0;
		x = xtemp;
		iteration++;
	}
	return iteration;
}

void calculate_mandelbrot(GLubyte *canvas, GLubyte *color_buffer, uint32_t w, uint32_t h, mandelbrot_f x0, mandelbrot_f x1, mandelbrot_f y0, mandelbrot_f y1, uint32_t max_iter) {
	mandelbrot_f dx = (x1 - x0) / w;
	mandelbrot_f dy = (y1 - y0) / h;
	uint16_t esc_time;
	int i, j;
	mandelbrot_f x, y;

	for(i = 0; i < w; ++i) {
		x = x0 + i * dx;
		for(j = 0; j < h; ++j) {
			y = y1 - j * dy;
			esc_time = escape_time(x, y, max_iter);

			canvas[ GET_R(i, j, w) ] = color_buffer[esc_time * 3];
			canvas[ GET_G(i, j, w) ] = color_buffer[esc_time * 3 + 1];
			canvas[ GET_B(i, j, w) ] = color_buffer[esc_time * 3 + 2];

    	}
    } 
}


