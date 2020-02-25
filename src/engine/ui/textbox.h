#ifndef ENGINE_UI_TEXTBOX_H
#define ENGINE_UI_TEXTBOX_H

#include <engine/color.h>
#include <engine/entity.h>
#include <engine/math/rect.h>

typedef struct Textbox {
	Entity entity;
	Rect2Df rect;
	float padding;
	char *pText;
	int length;
	Color fg;
	Color bg;
	Color outline;
	int outline_size;
	int focused;
	int cursor_pos;
	float cursor_size;
	int update_cursor_x;
	float cursor_x;
	int text_pt;
	unsigned int cursor_blink_tick;
	int cursor_blink;
} Textbox;

Textbox *engine_ui_textbox_create(int w, int h, int pt, int text_length, Color fg, Color bg, Color outline);

#endif
