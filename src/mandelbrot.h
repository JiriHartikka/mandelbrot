
GLubyte *make_canvas(uint32_t w, uint32_t h );
void init_canvas(GLubyte* canvas, GLuint *texture, uint32_t w, uint32_t h);
void draw_canvas(GLubyte* canvas, GLuint texture, uint32_t w, uint32_t h, mandelbrot_f window_scale);
void calculate_mandelbrot(GLubyte *canvas, GLubyte *color_buffer, uint32_t w, uint32_t h, mandelbrot_f x0, mandelbrot_f x1, mandelbrot_f y0, mandelbrot_f y1, uint32_t max_iter);
extern uint16_t (*escape_time)(mandelbrot_f, mandelbrot_f, uint32_t);
inline uint16_t escape_time_mandelbrot(mandelbrot_f x0, mandelbrot_f y0, uint32_t max_iter);
inline uint16_t escape_time_burning_ship(mandelbrot_f x0, mandelbrot_f y0, uint32_t max_iter);
inline uint16_t escape_time_z3(mandelbrot_f x0, mandelbrot_f y0, uint32_t max_iter);
extern uint16_t max_iteration;
