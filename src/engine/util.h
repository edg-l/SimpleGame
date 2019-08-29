#ifndef ENGINE_UTIL_H
#define ENGINE_UTIL_H

#include <SDL.h>

typedef struct Color {
	unsigned char r, g, b, a;
} Color;

typedef struct Rect {
	int x, y;
	int w, h;
} Rect;

typedef struct Point {
	int x, y;
} Point;

typedef enum Margin {
	MARGIN_LEFT,
	MARGIN_TOP,
	MARGIN_BOTTOM,
	MARGIN_RIGHT
} Margin;

typedef enum MouseButton {
	BUTTON_LEFT = SDL_BUTTON_LEFT,
	BUTTON_RIGHT = SDL_BUTTON_RIGHT,
	BUTTON_MIDDLE = SDL_BUTTON_MIDDLE
} MouseButton;

Color util_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
Rect util_rect(int x, int y, int w, int h);
Point util_point(int x, int y);


int util_point_in_rect(Rect *rect, Point *point);
float util_point_distance(Point *point1, Point *point2);
int util_rect_intersects(Rect *rect1, Rect *rect2, Rect *out);

void util_rect_margin(Margin margin, int distance, Rect *rect1, Rect *rect2);
void util_rect_center(Rect *rect1, Rect *rect2);

int max(int x, int y);
int min(int x, int y);

void util_update();

Point util_mouse_pos();
int util_is_mouse_click(MouseButton button);

#endif
