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

void engine_util_screen(Rect2Df *out) {
	SDL_assert(out);
	out->x = 0;
	out->y = 0;
	out->w = engine_settings_get_int("window_width");
	out->h = engine_settings_get_int("window_height");
}

void engine_util_update() {
	last_time = now_time;
	now_time = SDL_GetPerformanceCounter();
}

Uint32 engine_util_tick() { return SDL_GetTicks(); }

int engine_util_tick_passed(Uint32 a) { return SDL_TICKS_PASSED(engine_util_tick(), a); }

double engine_util_delta_time() {
	return (double)((now_time - last_time) * 1000) / SDL_GetPerformanceFrequency();
}

double engine_util_fps() { return 1 / (engine_util_delta_time() / 1000); }

void engine_util_str_format(char *buf, size_t size, const char *fmt, ...) {
	SDL_assert(buf);

	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, size, fmt, args);
	va_end(args);
}

void engine_util_mouse_tile_pos(Tilemap *t, Vector2Di *out) {
	SDL_assert(t);
	SDL_assert(out);
	int x, y;
	engine_input_mouse_pos(&x, &y);
	out->x = floorf(x / (float)t->tileSize);
	out->y = floorf(y / (float)t->tileSize);
}

Tile *engine_util_mouse_tile(Tilemap *t) {
	SDL_assert(t);

	Vector2Di pos;
	engine_util_mouse_tile_pos(t, &pos);
	return engine_tilemap_get(t, pos.x, pos.y);
}
