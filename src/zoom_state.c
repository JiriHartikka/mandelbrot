#include <stdlib.h> 
#include <stdio.h>
#include <stdint.h>
#include <GL/gl.h>
#include <GL/glut.h> 
#include "settings.h"
#include "zoom_state.h"

zoom_state *make_zoom_state(uint32_t w, uint32_t h, char debug, long double x0, long double x1, long double y0, long double y1) {
	zoom_state *zoom = malloc(sizeof(zoom_state));
	zoom->w = w;
	zoom->h = h;
	zoom->debug = debug;
	zoom->x0 = x0;
	zoom->x1 = x1;
	zoom->y0 = y0;
	zoom->y1 = y1;
	return zoom;
}

void zoom_focus(zoom_state *zoom, long double x, long double y, float zoomFactor) {
	long double lx = (zoom->x1 - zoom->x0) / (zoomFactor * 2);
	long double ly = (zoom->y1 - zoom->y0) / (zoomFactor * 2);
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
	long double dx = zoom->x1 - zoom->x0;
	long double dy = zoom->y1 - zoom->y0;
	sprintf(message, "dx = %.3Le	dy = %.3Le", dx, dy);
	glutBitmapString(GLUT_BITMAP_HELVETICA_12, message);
	glPopAttrib();
}