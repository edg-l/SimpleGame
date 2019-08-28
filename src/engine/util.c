#include "util.h"
#include "math.h"
#include <SDL.h>
#include <engine/logger.h>


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
	Point a;
	SDL_GetMouseState(&a.x, &a.y);
	return a;
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
