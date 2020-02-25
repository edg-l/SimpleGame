#include "textbox.h"
#include <engine/graphics/renderer.h>
#include <engine/input.h>
#include <engine/logger.h>
#include <stdlib.h>
#include <string.h>

// TODO: Fix double delete by waiting x time after first key down.

#define INPUT_DELAY_MS 150
#define CURSOR_BLINK_MS 500
static Tick next_input_tick = 0;

static void update_input_tick() {
	next_input_tick = engine_util_tick() + INPUT_DELAY_MS;
}

static void on_free(Entity *e) {
	Textbox *t = (Textbox *)e;
	free(t->pText);
	free(t);
}

static void on_update(Entity *e, double delta) {
	Textbox *t = (Textbox *)e;

	if (!t->focused && engine_math_mouse_in_rect2df(&t->rect) &&
		engine_input_mouse_click(BUTTON_LEFT)) {
		t->focused = 1;
	} else if (!engine_math_mouse_in_rect2df(&t->rect) &&
			   engine_input_mouse_click(BUTTON_LEFT)) {
		t->focused = 0;
	}

	if (t->focused) {
		if (engine_util_tick_passed(t->cursor_blink_tick)) {
			t->cursor_blink = !t->cursor_blink;
			t->cursor_blink_tick = engine_util_tick() + CURSOR_BLINK_MS;
		}
		if (engine_util_tick_passed(next_input_tick)) {
			if (engine_input_keypress(SDL_SCANCODE_LCTRL) &&
				engine_input_keypress(SDL_SCANCODE_BACKSPACE)) {
				char *last_space = t->pText;
				for (char *p = t->pText; *p && (p - t->pText) < t->cursor_pos;
					 p++) {
					if (*p == ' ')
						last_space = p;
				}

				memset(last_space, 0, t->cursor_pos - (last_space - t->pText));
				t->cursor_pos = last_space - t->pText;
				t->update_cursor_x = 1;
				update_input_tick();
			} else if (engine_input_keypress(SDL_SCANCODE_BACKSPACE)) {
				t->pText[SDL_max(0, strlen(t->pText) - 1)] = '\0';
				t->cursor_pos = SDL_max(0, t->cursor_pos - 1);
				t->update_cursor_x = 1;
				update_input_tick();
			}
		}
	}

	if (t->update_cursor_x) {
		size_t len = strlen(t->pText);
		Vector2Df size;

		if (t->cursor_pos == 0)
			size.x = 0;
		else if (t->cursor_pos >= len)
			engine_render_text_size_s(t->pText, t->text_pt, STYLE_REGULAR, &size);
		else {
			// Cursor is in the middle.
			engine_render_text_size_len(t->pText, t->text_pt, STYLE_REGULAR, &size,
										t->cursor_pos);
		}

		t->cursor_x = t->rect.x + t->padding + size.x;
		t->update_cursor_x = 0;
	}
}

static void on_textinput(Entity *e, const char *text) {
	Textbox *t = (Textbox *)e;

	if (!t->focused)
		return;

	int len = strlen(text);
	int current = strlen(t->pText);

	if (current + len < t->length) {
		strcat(t->pText, text);
		t->cursor_pos++;
		t->update_cursor_x = 1;
	}
}

static void on_textediting(Entity *e, const char *text, int start, int length) {
	engine_log_debug("textediting event: '%s' %d %d", text, start, length);
}

void on_render(Entity *e, double delta) {
	Textbox *t = (Textbox *)e;

	engine_render_color_s(t->outline);
	Rect2Df r;
	engine_math_rect2df_outline(&r, &t->rect, t->outline_size);
	engine_render_rect_s(&r, 1);

	engine_render_color_s(t->bg);
	engine_render_rect_s(&t->rect, 1);

	if (strlen(t->pText) > 0) {
		Vector2Df s;
		engine_render_text_color_s(t->fg);
		engine_render_text_size_s(t->pText, t->text_pt, STYLE_REGULAR, &s);
		engine_render_text(t->text_pt, STYLE_REGULAR, t->pText, t->rect.x + t->padding,
						   (int)(t->rect.y + (t->rect.h - s.y) / 2));
	}

	Rect2Df cursor = (Rect2Df){t->cursor_x, t->rect.y + t->padding / 2 + (t->rect.h - t->cursor_size) / 2, 2, t->cursor_size};

	if (t->focused && t->cursor_blink) {
		engine_render_color_s(t->fg);
		engine_render_rect_s(&cursor, 1);
	}
}

Textbox *engine_ui_textbox_create(int w, int h, int pt, int text_length, Color fg,
								  Color bg, Color outline) {

	Textbox *textbox = malloc(sizeof(Textbox));
	memset(textbox, 0, sizeof(Textbox));

	textbox->entity.on_free = on_free;
	textbox->entity.on_update = on_update;
	textbox->entity.on_render = on_render;
	textbox->entity.on_textinput = on_textinput;
	textbox->entity.on_textediting = on_textediting;

	textbox->length = text_length + 1;
	textbox->pText = malloc(sizeof(char) * textbox->length);
	memset(textbox->pText, 0, sizeof(char) * textbox->length);

	textbox->rect = (Rect2Df){0, 0, w, h};
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
	textbox->cursor_blink_tick = engine_util_tick();

	Vector2Df cursor_size;

	engine_render_text_size_s("|", textbox->text_pt + 10, STYLE_REGULAR, &cursor_size);
	textbox->cursor_size = cursor_size.y;
	return textbox;
}
