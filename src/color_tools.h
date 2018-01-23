#define GET_R(x,y,width) (((width) * (y) + (x)) * 3)
#define GET_G(x,y,width) (((width) * (y) + (x)) * 3 + 1)
#define GET_B(x,y,width) (((width) * (y) + (x)) * 3 + 2)

#define MAX_COLOR_INDEX 10000

GLubyte *init_colors(uint16_t size);
