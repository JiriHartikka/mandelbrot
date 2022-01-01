
typedef struct {
	//window width and height
	uint32_t w, h;
	//window position
	mandelbrot_f x0, x1, y0, y1;
	//maximum number of iterations
	uint32_t max_iter;
	//display debug info
	char debug;
	//scaling factor for window
	mandelbrot_f window_scale;
	// is zooming ready, written by worker when a pass is ready and emptied by main
	bool is_ready;
	// is zooming interrupted, written by main on user input
	bool is_interrupted;
} zoom_state;

zoom_state *make_zoom_state(uint32_t w, uint32_t h, char debug, mandelbrot_f x0, mandelbrot_f x1, mandelbrot_f y0, mandelbrot_f y1, uint32_t max_iter, mandelbrot_f window_scale);
void zoom_focus(zoom_state *zoom, mandelbrot_f x, mandelbrot_f y, float zoomFactor);
void display_depth(zoom_state *zoom);