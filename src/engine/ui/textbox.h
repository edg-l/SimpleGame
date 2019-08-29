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
	int text_pt;
} Textbox;

Textbox *textbox_create(int w, int h, int pt, int text_length, Color fg, Color bg, Color outline);

void textbox_update(Textbox *t);
void textbox_on_textinput(Textbox *t, const char *text);
void textbox_free(Textbox *t);

void render_textbox(Textbox *t);

#endif
