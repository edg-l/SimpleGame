#include "textbox.h"
#include <stdlib.h>
#include <string.h>
#include <engine/graphics/renderer.h>
#include <engine/logger.h>

Textbox *textbox_create(int w, int h, int pt, int text_length, Color fg, Color bg, Color outline) {
	Textbox *textbox = malloc(sizeof(Textbox));
	textbox->length = text_length + 1;
	textbox->pText = malloc(sizeof(char) * textbox->length);
	memset(textbox->pText, 0, sizeof(char) * textbox->length);

	textbox->rect = util_rect(0, 0, w, h);
	textbox->fg = fg;
	textbox->bg = bg;
	textbox->focused = 0;
	textbox->cursor_pos = 0;
	textbox->text_pt = pt;
	textbox->padding = 5;
	textbox->outline = outline;
	textbox->outline_size = 2;
	return textbox;
}

void textbox_free(Textbox *t) {
	free(t->pText);
	free(t);
}

void textbox_update(Textbox *t) {
	if(!t->focused && util_mouse_in_rect(&t->rect) && util_is_mouse_click(BUTTON_LEFT)) {
		t->focused = 1;
	}
	else if(!util_mouse_in_rect(&t->rect) && util_is_mouse_click(BUTTON_LEFT)) {
		t->focused = 0;
	}

	if(t->focused && util_is_key_click(SDL_SCANCODE_BACKSPACE)) {
		log_write(LOG_INFO, "pressed\n");
		t->pText[max(0, strlen(t->pText) - 1)] = '\0';
	}
}

void textbox_on_textinput(Textbox *t, const char *text) {
	if(!t->focused)
		return;

	int len = strlen(text);
	int current = strlen(t->pText);

	if(current + len < t->length) {
		strcat(t->pText, text);
	}
}

void render_textbox(Textbox *t) {
	render_color_s(t->outline);
	Rect r;
	util_rect_outline(&r, &t->rect, t->outline_size);
	render_rect_s(&r, 1);

	render_color_s(t->bg);
	render_rect_s(&t->rect, 1);

	if(strlen(t->pText) > 0) {
		Point s;
		render_text_size_s(t->pText, t->text_pt, STYLE_REGULAR, &s);
		render_text(t->text_pt, STYLE_REGULAR, t->pText,
				t->rect.x + t->padding,
				(int)(t->rect.y + (t->rect.h - s.y) / 2));
	}
}
