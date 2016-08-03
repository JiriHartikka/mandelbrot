#include <GL/gl.h>
#include <stdlib.h> 
#include "color_tools.h" 

/*GLubyte *make_color_gradient(
	uint32_t size, 
	GLubyte r1, GLubyte g1, GLubyte b1, 
	GLubyte r2, GLubyte g2, GLubyte b2
) {
	GLubyte *buffer = malloc(3 * (size + 1) * sizeof(GLubyte));
	double diff_r = (r2 - r1)/(double)size;
	double diff_g = (g2 - g1)/(double)size;
	double diff_b = (b2 - b1)/(double)size;

	uint32_t i;
	for(i=0; i<size; i++) {

		buffer[3 * i] = (diff_r * (i + 1) + r1);
		buffer[3 * i + 1] =  (diff_g * (i + 1) + g1);
		buffer[3 * i + 2] = (diff_b * (i + 1) + b1);
	}
	buffer[size * 3] = 0;
	buffer[size * 3 + 1] = 0;
	buffer[size * 3 + 2] = 0;
	return buffer;
}*/

void make_color_gradient(
	GLubyte *color_buffer,
	uint32_t offset,
	uint32_t size, 
	GLubyte r1, GLubyte g1, GLubyte b1, 
	GLubyte r2, GLubyte g2, GLubyte b2
) {
	double diff_r = (r2 - r1)/(double)size;
	double diff_g = (g2 - g1)/(double)size;
	double diff_b = (b2 - b1)/(double)size;
	uint32_t i;
	for(i=0; i<size; i++) {
		color_buffer[3 * (offset + i)] = (diff_r * (i + 1) + r1);
		color_buffer[3 * (offset + i) + 1] =  (diff_g * (i + 1) + g1);
		color_buffer[3 * (offset + i) + 2] = (diff_b * (i + 1) + b1);
	}
}

GLubyte *init_colors(uint16_t size) {
	GLubyte *buffer = malloc(3 * (size + 1) * sizeof(GLubyte));
	uint32_t chunk_size = (int) size / 5;
	int i;
	make_color_gradient(
		buffer,
		0,
		chunk_size,
		255, 255, 255,
		237, 117, 33
	);
	make_color_gradient(
		buffer,
		chunk_size,
		chunk_size,
		237, 117, 33,
		129, 132, 243
	);
	make_color_gradient(
		buffer,
		2 * chunk_size,
		chunk_size,
		129, 132, 243,
		49, 155, 56
	);
	make_color_gradient(
		buffer,
		3 * chunk_size,
		chunk_size,
		49, 155, 56,
		209, 16, 16
	);
	make_color_gradient(
		buffer,
		4 * chunk_size,
		chunk_size,
		209, 16, 16,
		41, 17, 44
	);
	for(i = 5 * chunk_size; i <= size; i++) {
		buffer[i * 3] = 0;
		buffer[i * 3 + 1] = 0;
		buffer[i * 3 + 2] = 0;
	}
	return buffer;
}