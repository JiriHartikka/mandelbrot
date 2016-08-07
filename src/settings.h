#define DEFAULT_CANVAS_H 800
#define DEFAULT_CANVAS_W 1400

#define DEFAULT_MAX_ITER 2000

#define GET_R(x,y,width) ((width) * (y) + (x)) * 3
#define GET_G(x,y,width) ((width) * (y) + (x)) * 3 + 1
#define GET_B(x,y,width) ((width) * (y) + (x)) * 3 + 2

typedef long double mandelbrot_f;
