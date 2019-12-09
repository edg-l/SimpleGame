#include "util.h"
#include "math.h"
#include <SDL.h>
#include <engine/logger.h>
#include <engine/settings.h>
#include <engine/tilemap.h>
#include <engine/input.h>
#include <stdarg.h>
#include <stdio.h>

static double last_time = 0;
static double now_time = 0;

Color engine_util_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	return (Color){r, g, b, a};
}

Rect engine_util_rect(int x, int y, int w, int h) { return (Rect){x, y, w, h}; }

Point engine_util_point(int x, int y) { return (Point){x, y}; }

void engine_util_rect_outline(Rect *outline, Rect *rect, int outline_size) {
	SDL_assert(outline);
	SDL_assert(rect);

	outline->x = rect->x - outline_size;
	outline->y = rect->y - outline_size;
	outline->w = rect->w + outline_size * 2;
	outline->h = rect->h + outline_size * 2;
}

void engine_util_rect_padding(Rect *padding, Rect *rect, int padding_size) {
	SDL_assert(padding);
	SDL_assert(rect);

	padding->x = rect->x + padding_size;
	padding->y = rect->y + padding_size;
	padding->w = rect->w - padding_size * 2;
	padding->h = rect->h - padding_size * 2;
}

int engine_util_point_in_rect(Rect *rect, Point *point) {
	SDL_assert(rect);
	if (point->x > rect->x && point->x < rect->x + rect->w &&
			point->y > rect->y && point->y < rect->y + rect->h)
		return 1;
	return 0;
}

float engine_util_point_distance(Point *point1, Point *point2) {
	return sqrtf(pow(point1->x - point2->x, 2) + pow(point1->y - point2->y, 2));
}

int engine_util_rect_intersects(Rect *rect1, Rect *rect2, Rect *out) {
	SDL_assert(rect1);
	SDL_assert(rect2);

	int leftx = max(rect1->x, rect2->x);
	int rightx = min(rect1->x + rect1->w, rect2->x + rect2->w);
	int topy = max(rect1->y, rect2->y);
	int bottomy = min(rect1->y + rect1->h, rect2->y + rect2->h);

	if (leftx < rightx && topy < bottomy) {
		if (out) {
			*out = engine_util_rect(leftx, topy, rightx - leftx, bottomy - topy);
		}
		return 1;
	}

	return 0;
}

Rect engine_util_screen() {
	return engine_util_rect(0, 0, engine_settings_get_int("window_width"),
			engine_settings_get_int("window_height"));
}

int max(int x, int y) { return x > y ? x : y; }

int min(int x, int y) { return x < y ? x : y; }

void engine_util_update() {
	last_time = now_time;
	now_time = SDL_GetPerformanceCounter();
}

Uint32 engine_util_tick() { return SDL_GetTicks(); }

int engine_util_tick_passed(Uint32 a) { return SDL_TICKS_PASSED(engine_util_tick(), a); }

int engine_util_mouse_in_rect(Rect *rect) {
	SDL_assert(rect);
	int x,y;
	engine_input_mouse_pos(&x, &y);
	Point a = engine_util_point(x, y);
	return engine_util_point_in_rect(rect, &a);
}

double engine_util_delta_time() {
	return (double)((now_time - last_time) * 1000) / SDL_GetPerformanceFrequency();
}

double engine_util_fps() { return 1 / (engine_util_delta_time() / 1000); }

void engine_util_str_format(char *buf, size_t size, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, size, fmt, args);
	va_end(args);
}

void engine_util_rect_margin(Margin margin, int distance, Rect *rect1, Rect *rect2) {
	if (margin == MARGIN_RIGHT) {
		rect2->x = rect1->x + rect1->w + distance;
		rect2->y = rect1->y;
		rect2->h = rect1->h;
	} else if (margin == MARGIN_LEFT) {
		rect2->x = rect1->x - distance - rect2->w;
		rect2->y = rect1->y;
		rect2->h = rect1->h;
	} else if (margin == MARGIN_BOTTOM) {
		rect2->x = rect1->x;
		rect2->w = rect1->w;
		rect2->y = rect1->y + rect1->h + distance;
	} else if (margin == MARGIN_TOP) {
		rect2->x = rect1->x;
		rect2->w = rect1->w;
		rect2->y = rect1->y - rect2->h - distance;
	}
}

void engine_util_rect_center(Rect *rect1, Rect *rect2) {
	rect2->x = rect1->x + (rect1->w - rect2->w) / 2;
	rect2->y = rect1->y + (rect1->h - rect2->h) / 2;
}

Point engine_util_world_to_tilemap(Tilemap *t, Point world) {
	return engine_util_point(floor(world.x / (float)t->tileSize), floor(world.y / (float)t->tileSize));
}
