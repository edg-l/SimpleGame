#ifndef ENGINE_MATH_RECT_H
#define ENGINE_MATH_RECT_H

#include <SDL_assert.h>
#include <math.h>
#include <engine/math/vector.h>
#include <engine/input.h>

typedef enum Margin {
	MARGIN_LEFT,
	MARGIN_TOP,
	MARGIN_BOTTOM,
	MARGIN_RIGHT
} Margin;

#define GenerateRect(name, type) \
	typedef struct Rect2D ## name { \
		type x, y;\
		type w, h;\
	} Rect2D ## name; \
	\
	inline int engine_math_rect2d ## name ## _intersects(Rect2D ## name *r1, Rect2D ## name *r2, Rect2D ## name *out) { \
		SDL_assert(r1); \
		SDL_assert(r2); \
		type leftx = r1->x > r2->x ? r1->x : r2->x; \
		type rightx = (r1->x + r1->w) > (r2->x + r2->w) ? (r1->x + r1->w) : (r2->x + r2->w); \
		type topy = r1->y > r2->y ? r1->y : r2->y; \
		type bottomy = (r1->y + r1->h) > (r2->y + r2->h) ? (r1->y + r1->h) : (r2->y + r2->h); \
		if(leftx < rightx && topy < bottomy) { \
			if(out) \
				*out = (Rect2D ## name){leftx, topy, rightx - leftx, bottomy - topy};\
			return 1; \
		} \
		return 0; \
	}; \
	inline int engine_math_rect2d ## name ## _has_point(Rect2D ## name *r1, Vector2D ## name *p) { \
		SDL_assert(r1); \
		SDL_assert(p); \
		if (p->x > r1->x && p->x < r1->x + r1->w && \
			p->y > r1->y && p->y < r1->y + r1->h) {\
			return 1; \
		} \
		return 0; \
	}; \
	inline void engine_math_rect2d ## name ## _padding(Rect2D ## name *padding, Rect2D ## name *rect, type padding_size) { \
		SDL_assert(padding); \
		SDL_assert(rect); \
		padding->x = rect->x + padding_size; \
		padding->y = rect->y + padding_size; \
		padding->w = rect->w - padding_size * 2; \
		padding->h = rect->h - padding_size * 2; \
	}; \
	inline void engine_math_rect2d ## name ## _outline(Rect2D ## name *outline, Rect2D ## name *rect, type outline_size) { \
		SDL_assert(outline); \
		SDL_assert(rect); \
		outline->x = rect->x - outline_size; \
		outline->y = rect->y - outline_size; \
		outline->w = rect->w + outline_size * 2; \
		outline->h = rect->h + outline_size * 2; \
	}; \
	inline void engine_math_rect2d ## name ## _margin(Margin margin, type distance, Rect2D ## name *rect1, Rect2D ## name *rect2) { \
		SDL_assert(rect1); \
		SDL_assert(rect2); \
		if (margin == MARGIN_RIGHT) { \
			rect2->x = rect1->x + rect1->w + distance; \
			rect2->y = rect1->y; \
			rect2->h = rect1->h; \
		} else if (margin == MARGIN_LEFT) { \
			rect2->x = rect1->x - distance - rect2->w; \
			rect2->y = rect1->y; \
			rect2->h = rect1->h; \
		} else if (margin == MARGIN_BOTTOM) { \
			rect2->x = rect1->x; \
			rect2->w = rect1->w; \
			rect2->y = rect1->y + rect1->h + distance; \
		} else if (margin == MARGIN_TOP) { \
			rect2->x = rect1->x; \
			rect2->w = rect1->w; \
			rect2->y = rect1->y - rect2->h - distance; \
		} \
	}; \
	inline void engine_math_rect2d ## name ## _center(Rect2D ## name *rect1, Rect2D ## name *rect2) { \
		SDL_assert(rect1); \
		SDL_assert(rect2); \
		rect2->x = rect1->x + (rect1->w - rect2->w) / 2; \
		rect2->y = rect1->y + (rect1->h - rect2->h) / 2; \
	} \
	inline int engine_math_mouse_in_rect2d ## name (Rect2D ## name *rect) { \
		SDL_assert(rect); \
		int x,y; \
		engine_input_mouse_pos(&x, &y); \
		Vector2D ## name point; \
		point.x = (type)x; \
		point.y = (type)y; \
		return engine_math_rect2d ## name ## _has_point(rect, &point); \
	}

GenerateRect(i, int);

GenerateRect(ui, unsigned int);

GenerateRect(l, long);

GenerateRect(ul, unsigned long);

GenerateRect(f, float);

GenerateRect(d, double);

#endif
