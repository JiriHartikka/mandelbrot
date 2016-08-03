#include <GL/gl.h>
#include <stdlib.h>  
#include <complex.h>
#include <assert.h>
#include <stdio.h>
#include "settings.h"
#include "mandelbrot.h"

int textureId;

GLubyte *make_canvas(uint32_t w, uint32_t h ) {
	glGenTextures (1, &textureId);
	return malloc(w * h * 3 * sizeof(GLubyte));
}

void draw_canvas(GLvoid* canvas, uint32_t w, uint32_t h) {
	if(WINDOW_H != CANVAS_H || WINDOW_W != CANVAS_W) {
		const float x_scale = WINDOW_W / (float)CANVAS_W;
		const float y_scale = WINDOW_H / (float)CANVAS_H;
		glPixelZoom(x_scale, y_scale);
	}
	glDrawPixels( w, h, GL_RGB, GL_UNSIGNED_BYTE, canvas);

}

inline uint16_t escape_time(long double x0, long double y0) {
	long double x = 0.0;
	long double y = 0.0;
	long double xtemp;
	uint16_t iteration = 0;
	while((x*x + y*y < 4) && (iteration < MAX_ITER)) {
		xtemp = x*x - y*y + x0;
		y = 2*x*y + y0;
		x = xtemp;
		iteration++;
	}
	return iteration;
}

void calculate_mandelbrot(GLubyte *canvas, GLubyte *color_buffer, uint32_t w, uint32_t h, long double x0, long double x1, long double y0, long double y1) {
	const long double dx = (x1 - x0) / w;
	const long double dy = (y1 - y0) / h;
	uint16_t esc_time;
	int i, j;
	long double x, y;

	for(i = 0; i < w; ++i) {
		x = x0 + i * dx;
		for(j = 0; j < h; ++j) {
			y = y1 - j * dy;
			esc_time = escape_time(x, y);

			canvas[ GET_R(i, j, w) ] = color_buffer[esc_time * 3];
			canvas[ GET_G(i, j, w) ] = color_buffer[esc_time * 3 + 1];
			canvas[ GET_B(i, j, w) ] = color_buffer[esc_time * 3 + 2];

    	}
    } 
}


