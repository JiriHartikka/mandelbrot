typedef float (*ESCAPE_TIME_FUNC)(mandelbrot_f, mandelbrot_f, uint32_t);

struct Canvas {
  uint32_t w;
  uint32_t h;
  GLuint texture;
  ESCAPE_TIME_FUNC escape_time;
  GLubyte *data;
};


void init_canvas(uint32_t w, uint32_t h, ESCAPE_TIME_FUNC escape_time_func);
void draw_canvas(void);
void calculate_fractal(GLubyte *color_buffer, mandelbrot_f x0, mandelbrot_f x1, mandelbrot_f y0, mandelbrot_f y1, uint32_t max_iter);
