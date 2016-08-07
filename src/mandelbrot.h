
GLubyte *make_canvas(uint32_t w, uint32_t h );
void draw_canvas(GLvoid* canvas, uint32_t w, uint32_t h, mandelbrot_f window_scale);
void calculate_mandelbrot(GLubyte *canvas, GLubyte *color_buffer, uint32_t w, uint32_t h, mandelbrot_f x0, mandelbrot_f x1, mandelbrot_f y0, mandelbrot_f y1, uint32_t max_iter);