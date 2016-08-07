#include <stdlib.h> 
#include <stdio.h>
#include <stdint.h>
#include <GL/gl.h>
#include <GL/glut.h> 
#include "settings.h"
#include "zoom_state.h"

zoom_state *make_zoom_state(uint32_t w, uint32_t h, char debug, mandelbrot_f x0, mandelbrot_f x1, mandelbrot_f y0, mandelbrot_f y1, uint32_t max_iter, mandelbrot_f window_scale) {
	zoom_state *zoom = malloc(sizeof(zoom_state));
	zoom->w = w;
	zoom->h = h;
	zoom->debug = debug;
	zoom->x0 = x0;
	zoom->x1 = x1;
	zoom->y0 = y0;
	zoom->y1 = y1;
	zoom->max_iter = max_iter;
	zoom->window_scale = window_scale;
	return zoom;
}

void zoom_focus(zoom_state *zoom, mandelbrot_f x, mandelbrot_f y, float zoomFactor) {
	mandelbrot_f lx = (zoom->x1 - zoom->x0) / (zoomFactor * 2);
	mandelbrot_f ly = (zoom->y1 - zoom->y0) / (zoomFactor * 2);
	zoom->x0 = x - lx;
	zoom->x1 = x + lx;
	zoom->y0 = y - ly;
	zoom->y1 = y + ly;
}

void display_depth(zoom_state *zoom) {
	glPushAttrib(GL_CURRENT_BIT);
	char message[75];
	glColor3ub(0, 0, 0);
	glRasterPos2f(-1.0, 0.9);
	double dx = zoom->x1 - zoom->x0;
	double dy = zoom->y1 - zoom->y0;
	sprintf(message, "dx = %.3le	dy = %.3le", dx, dy);
	glutBitmapString(GLUT_BITMAP_HELVETICA_12, message);
	glPopAttrib();
}