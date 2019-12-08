#ifndef ENGINE_UTIL_H
#define ENGINE_UTIL_H

#include <SDL.h>
#include "util_colors.h"

typedef Uint32 Tick;

typedef struct Color {
	unsigned char r, g, b, a;
} Color;

typedef struct Rect {
	int x, y;
	unsigned int w, h;
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

struct Tilemap;

Color engine_util_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
Rect engine_util_rect(int x, int y, int w, int h);
Point engine_util_point(int x, int y);


int engine_util_point_in_rect(Rect *rect, Point *point);
float engine_util_point_distance(Point *point1, Point *point2);
int engine_util_rect_intersects(Rect *rect1, Rect *rect2, Rect *out);

int engine_util_mouse_in_rect(Rect *rect);

void engine_util_rect_margin(Margin margin, int distance, Rect *rect1, Rect *rect2);
void engine_util_rect_center(Rect *rect1, Rect *rect2);
void engine_util_rect_outline(Rect *outline, Rect *rect, int outline_size);
void engine_util_rect_padding(Rect *padding, Rect *rect, int padding_size);

Rect engine_util_screen();

Point engine_util_world_to_tilemap(struct Tilemap *t, Point world);

int max(int x, int y);
int min(int x, int y);

void engine_util_init();
void engine_util_quit();
void engine_util_update();
void engine_util_update_keyboard();

Point engine_util_mouse_pos();
void engine_util_mouse_delta(int *x, int *y);
int engine_util_is_mouse_click(MouseButton button);
int engine_util_is_keyup(int code);
int engine_util_is_keypress(int code);

void engine_util_str_format(char* buf, size_t size, const char *fmt, ...);

// In ms
Uint32 engine_util_tick();

// Returns 1 if current tick passed a
int engine_util_tick_passed(Uint32 a);

double engine_util_delta_time();
double engine_util_fps();
#endif
