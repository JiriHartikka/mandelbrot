
GLubyte *make_canvas(uint32_t w, uint32_t h );
void draw_canvas(GLvoid* canvas, uint32_t w, uint32_t h);
void calculate_mandelbrot(GLubyte *canvas, GLubyte *color_buffer, uint32_t w, uint32_t h, long double x0, long double x1, long double y0, long double y1);