
typedef struct {
	//window width and height
	uint32_t w, h;
	//window position
	long double x0, x1, y0, y1;
	//display debug info
	char debug;
} zoom_state;

zoom_state *make_zoom_state(uint32_t w, uint32_t h, char debug, long double x0, long double x1, long double y0, long double y1);
void zoom_focus(zoom_state *zoom, long double x, long double y, float zoomFactor);
void display_depth(zoom_state *zoom);