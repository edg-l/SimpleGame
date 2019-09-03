#include "util.h"
#include "math.h"
#include <SDL.h>
#include <engine/logger.h>

static Uint32 prev_mouse_state;
static Uint32 mouse_state;
static Uint8 *prev_keyboard_status = NULL;
static const Uint8 *keyboard_status = NULL;
static int keyboard_status_length = 0;
static Point mouse_pos;
static double last_time = 0;
static double now_time = 0;

void util_init() {
	keyboard_status = SDL_GetKeyboardState(&keyboard_status_length);
	prev_keyboard_status = malloc(keyboard_status_length);
	util_update();
}

void util_quit() {
	free(prev_keyboard_status);
}

Color util_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	return (Color) {r, g, b, a};
}

Rect util_rect(int x, int y, int w, int h) {
	return (Rect){x, y, w, h};
}

Point util_point(int x, int y) {
	return (Point){x, y};
}

Point util_mouse_pos() {
	return mouse_pos;
}

void util_rect_outline(Rect *outline, Rect *rect, int outline_size) {
	outline->x = rect->x - outline_size;
	outline->y = rect->y - outline_size;
	outline->w = rect->w + outline_size * 2;
	outline->h = rect->h + outline_size * 2;
}

int util_point_in_rect(Rect *rect, Point *point) {
	if(point->x > rect->x && point->x < rect->x + rect->w &&
			point->y > rect->y && point->y < rect->y + rect->h)
		return 1;
	return 0;
}

float util_point_distance(Point *point1, Point *point2) {
	return sqrtf(pow(point1->x - point2->x, 2) + pow(point1->y - point2->y, 2));
}

int util_rect_intersects(Rect *rect1, Rect *rect2, Rect *out) {
	Rect *inside;
	Rect *outside;

	int is_inside = 0;

	Rect i;

	int leftx = max(rect1->x, rect2->x);
	int rightx = min(rect1->x + rect1->w, rect2->x + rect2->w);
	int topy = max(rect1->y, rect2->y);
	int bottomy = min(rect1->y + rect1->h, rect2->y + rect2->h);

	if(leftx < rightx && topy < bottomy) {
		if(out) {
			*out = util_rect(leftx, topy, rightx-leftx, bottomy - topy);
		}
		return 1;
	}

	return 0;
}

int max(int x, int y) {
	return x > y ? x : y;
}

int min(int x, int y) {
	return x < y ? x : y;
}

void util_update() {
	prev_mouse_state = mouse_state;
	mouse_state = SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
	last_time = now_time;
	now_time = SDL_GetPerformanceCounter();
}

void util_update_keyboard() {
	if(keyboard_status) {
		memcpy(prev_keyboard_status, keyboard_status, keyboard_status_length);
	}
}

int util_is_mouse_click(MouseButton button) {
	return (prev_mouse_state & SDL_BUTTON(button)) && !(mouse_state & SDL_BUTTON(button));
}

int util_is_keyup(int code) {
	if(!prev_keyboard_status)
		return 0;
	return prev_keyboard_status[code] && !keyboard_status[code];
}

int util_is_keypress(int code) {
	return keyboard_status[code];
}

Uint32 util_tick() {
	return SDL_GetTicks();
}

int util_tick_passed(Uint32 a) {
	return SDL_TICKS_PASSED(util_tick(), a);
}

int util_mouse_in_rect(Rect *rect) {
	return util_point_in_rect(rect, &mouse_pos);
}

double util_delta_time() {
	return (double)((now_time - last_time) * 1000) / SDL_GetPerformanceFrequency();
}
