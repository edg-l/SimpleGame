#ifndef ENGINE_UTIL_H
#define ENGINE_UTIL_H

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

Color util_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
Rect util_rect(int x, int y, int w, int h);
Point util_point(int x, int y);


int util_point_in_rect(Rect *rect, Point *point);
float util_point_distance(Point *point1, Point *point2);
int util_rect_intersects(Rect *rect1, Rect *rect2, Rect *out);

int max(int x, int y);
int min(int x, int y);

Point util_mouse_pos();

#endif
