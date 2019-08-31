#include "textbox.h"
#include <stdlib.h>
#include <string.h>
#include <engine/graphics/renderer.h>
#include <engine/logger.h>

#define INPUT_DELAY_MS 100
#define CURSOR_BLINK_MS 500
static Tick next_input_tick = 0;

static void update_input_tick() {
	next_input_tick = util_tick() + INPUT_DELAY_MS;
}

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
	textbox->update_cursor_x = 1;
	textbox->cursor_blink = 0;
	textbox->cursor_blink_tick = util_tick();
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

	if(t->focused) {
		if(util_tick_passed(t->cursor_blink_tick)) {
			t->cursor_blink = !t->cursor_blink;
			t->cursor_blink_tick = util_tick() + CURSOR_BLINK_MS;
		}
		if(util_tick_passed(next_input_tick)) {
			if(util_is_keypress(SDL_SCANCODE_LCTRL) && util_is_keypress(SDL_SCANCODE_BACKSPACE)) {
				char *last_space = t->pText;
				for(char *p = t->pText; *p && (p - t->pText) < t->cursor_pos; p++) {
					if(*p == ' ')
						last_space = p;
				}

				memset(last_space, 0, t->cursor_pos - (last_space - t->pText));
				t->cursor_pos = last_space - t->pText;
				t->update_cursor_x = 1;
				update_input_tick();
			}
			else if(util_is_keypress(SDL_SCANCODE_BACKSPACE)) {
				t->pText[max(0, strlen(t->pText) - 1)] = '\0';
				t->cursor_pos = max(0, t->cursor_pos - 1);
				t->update_cursor_x = 1;
				update_input_tick();
			}
		}
	}

	if(t->update_cursor_x) {
		size_t len = strlen(t->pText);
		Point size;

		if(t->cursor_pos == 0)
			size.x = 0;
		else if(t->cursor_pos >= len)
			render_text_size_s(t->pText, t->text_pt, STYLE_REGULAR, &size);
		else {
			// Cursor is in the middle.
			render_text_size_len(t->pText, t->text_pt, STYLE_REGULAR, &size, t->cursor_pos);
		}

		t->cursor_x = t->rect.x + t->padding + size.x;
		t->update_cursor_x = 0;
	}
}

void textbox_on_textinput(Textbox *t, const char *text) {
	if(!t->focused)
		return;

	int len = strlen(text);
	int current = strlen(t->pText);

	if(current + len < t->length) {
		strcat(t->pText, text);
		t->cursor_pos++;
		t->update_cursor_x = 1;
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

	Rect cursor = util_rect(t->cursor_x, t->rect.y + t->rect.h / 4, 2, t->rect.h / 2);
	if(t->focused && t->cursor_blink) {
		render_color(40, 40, 40, 255);
		render_rect_s(&cursor, 1);
	}
}
