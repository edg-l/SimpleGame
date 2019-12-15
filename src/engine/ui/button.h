#ifndef ENGINE_UI_BUTTON_H
#define ENGINE_UI_BUTTON_H

#include <engine/entity.h>
#include <engine/math/rect.h>
#include <engine/color.h>
#include <engine/util_colors.h>
#include <engine/graphics/renderer.h>

typedef void (*BUTTON_ON_CLICK_FN)();

typedef struct Button {
	Entity entity;
	Rect2Df rect;
	float textSizeW, textSizeH;
	int textpt;
	int textStyle;
	Color fg;
	Color bg;
	char *pText;
	BUTTON_ON_CLICK_FN on_click;
} Button;

Button *engine_button_create(unsigned int w, unsigned int h, int pt,
		int style, const char* text, BUTTON_ON_CLICK_FN on_click, Color fg, Color bg);

#define engine_button_default(w, h, pt, text, on_click) engine_button_create((w), (h), (pt), STYLE_REGULAR, (text), on_click, COLOR_WHITE, COLOR_DARK)

#endif
