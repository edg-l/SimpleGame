#ifndef ENGINE_UI_TEXTBOX_H
#define ENGINE_UI_TEXTBOX_H

#include <engine/util.h>

typedef struct Textbox {
	Rect rect;
	int padding;
	char *pText;
	int length;
	Color fg;
	Color bg;
	Color outline;
	int outline_size;
	int focused;
	int cursor_pos;
	int cursor_size;
	int update_cursor_x;
	int cursor_x;
	int text_pt;
	Tick cursor_blink_tick;
	int cursor_blink;
} Textbox;

Textbox *engine_ui_textbox_create(int w, int h, int pt, int text_length, Color fg, Color bg, Color outline);

void engine_ui_textbox_update(Textbox *t);
void engine_ui_textbox_on_sdlevent(Textbox *t, SDL_Event *event);
void engine_ui_textbox_free(Textbox *t);

void engine_render_textbox(Textbox *t);

#endif
