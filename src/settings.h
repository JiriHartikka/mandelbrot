#define WINDOW_H 800
#define WINDOW_W 1400

#define CANVAS_H 800
#define CANVAS_W 1400

#define MAX_ITER 2000

#define GET_R(x,y,width) ((width) * (y) + (x)) * 3
#define GET_G(x,y,width) ((width) * (y) + (x)) * 3 + 1
#define GET_B(x,y,width) ((width) * (y) + (x)) * 3 + 2