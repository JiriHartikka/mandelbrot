extern uint16_t max_iteration;

inline float escape_time_mandelbrot(mandelbrot_f x0, mandelbrot_f y0, uint32_t max_iter);
inline float escape_time_burning_ship(mandelbrot_f x0, mandelbrot_f y0, uint32_t max_iter);
inline float escape_time_z3(mandelbrot_f x0, mandelbrot_f y0, uint32_t max_iter);
inline float escape_time_tan_mixture(mandelbrot_f x0, mandelbrot_f y0, uint32_t max_iter);
