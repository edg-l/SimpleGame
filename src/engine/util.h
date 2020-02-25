#ifndef ENGINE_UTIL_H
#define ENGINE_UTIL_H

#include "util_colors.h"
#include <SDL.h>
#include <engine/color.h>
#include <engine/math/rect.h>
#include <engine/math/vector.h>

typedef Uint32 Tick;

typedef struct Rect {
	int x, y;
	unsigned int w, h;
} Rect;

struct Tilemap;

Color engine_util_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

void engine_util_screen(Rect2Df *out);

void engine_util_mouse_tile_pos(struct Tilemap *t, Vector2Di *out);
struct Tile *engine_util_mouse_tile(struct Tilemap *t);

int max(int x, int y);
int min(int x, int y);

void engine_util_update();

void engine_util_str_format(char *buf, size_t size, const char *fmt, ...);

// In ms
Uint32 engine_util_tick();

// Returns 1 if current tick passed a
int engine_util_tick_passed(Uint32 a);

double engine_util_delta_time();
double engine_util_fps();
#endif
