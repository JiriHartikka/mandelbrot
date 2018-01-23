#include <GL/gl.h>
#include <stdlib.h>
#include <complex.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "settings.h"
#include "mandelbrot.h"

inline float escape_time_mandelbrot(mandelbrot_f x0, mandelbrot_f y0, uint32_t max_iter) {
	mandelbrot_f x = 0.0;
	mandelbrot_f y = 0.0;
	mandelbrot_f xtemp;
	uint16_t iteration = 0;

  while(( x*x + y*y < 4) && (iteration < max_iter)) {
		xtemp = x*x - y*y + x0;
		y = 2*x*y + y0;
		x = xtemp;
		iteration++;
	}

  if(iteration == max_iter) {
    return max_iter;
  }

  for(int i = 0; i<2; i++) {
    xtemp = x*x - y*y + x0;
		y = 2*x*y + y0;
		x = xtemp;
		iteration++;
  }

  return iteration + 1 - log(log( sqrt(x*x + y*y) )) / log(2);
}


inline float escape_time_burning_ship(mandelbrot_f x0, mandelbrot_f y0, uint32_t max_iter) {
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

inline float escape_time_z3(mandelbrot_f x0, mandelbrot_f y0, uint32_t max_iter) {
  mandelbrot_f x = 0.0;
  mandelbrot_f y = 0.0;
  mandelbrot_f xtemp;
  uint16_t iteration = 0;

  while((x*x + y*y < 4) && (iteration < max_iter)) {
    xtemp = x*x*x - 3*x*y*y + x0;
    y = 3*x*x*y - y*y*y + y0;
    x = xtemp;
    iteration++;
  }

  if(iteration == max_iter) {
    return max_iter;
  }

  for(int i = 0; i<2; i++) {
    xtemp = x*x*x - 3*x*y*y + x0;
    y = 3*x*x*y - y*y*y + y0;
    x = xtemp;
    iteration++;
  }

  return iteration + 1 - log(log( sqrt(x*x + y*y) )) / log(3);


  //return iteration;
}

inline void complex_tan(mandelbrot_f real_part, mandelbrot_f img_part, mandelbrot_f *real_out, mandelbrot_f *img_out) {
  mandelbrot_f tmp_real_out = sin(2 * real_part) / ( cos(2 * real_part) + cosh(2 * img_part) );
  mandelbrot_f tmp_img_out = sinh(2 * img_part) / ( cos(2 * real_part ) + cosh(2 * img_part) );
  *real_out = tmp_real_out;
  *img_out = tmp_img_out;
}

inline float escape_time_tan_mixture(mandelbrot_f x0, mandelbrot_f y0, uint32_t max_iter) {
  mandelbrot_f x = 0.0;
  mandelbrot_f y = 0.0;

  uint16_t iteration = 0;

  while((x*x + y*y < 4) && (iteration < max_iter)) {
    complex_tan(x*x - y*y, 2*x*y, &x, &y);
    x = x + x0;
    y = y + y0;

    iteration++;
  }

  return iteration;
}
