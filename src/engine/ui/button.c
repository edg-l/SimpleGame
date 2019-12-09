#include "button.h"
#include <engine/graphics/renderer.h>
#include <engine/util.h>
#include <engine/input.h>
#include <stdlib.h>
#include <string.h>

Button *button_create(unsigned int w, unsigned int h, int pt, int style, const char *text,
		Color fg, Color bg) {
	Button *button;

	button = malloc(sizeof(Button));
	memset(button, 0, sizeof(Button));

	button->rect = engine_util_rect(0, 0, w, h);

	button->fg = fg;
	button->bg = bg;
	button->textpt = pt;
	button->textStyle = style;

	int len = strlen(text) + 1;
	button->pText = malloc(sizeof(char) * len);
	strcpy(button->pText, text);

	engine_render_text_size(button->pText, button->textpt, STYLE_REGULAR,
			&button->textSizeW, &button->textSizeH);

	return button;
}

void button_free(Button *button) {
	free(button->pText);
	free(button);
}

void engine_render_button(Button *button) {
	Point mouse;
	engine_input_mouse_pos(&mouse.x, &mouse.y);

	if (engine_util_point_in_rect(&button->rect, &mouse))
		engine_render_color(button->bg.r + 40, button->bg.g, button->bg.b,
				button->bg.a);
	else
		engine_render_color(button->bg.r, button->bg.g, button->bg.b, button->bg.a);

	engine_render_rect_s(&button->rect, 1);

	engine_render_text_color(button->fg.r, button->fg.g, button->fg.b, button->fg.a);
	engine_render_text(
			button->textpt, button->textStyle, button->pText,
			(int)(button->rect.x + (button->rect.w - button->textSizeW) / 2.f),
			(int)(button->rect.y + (button->rect.h - button->textSizeH) / 2.f));
}

int button_is_pressed(Button *button) {
	Point mouse;
	engine_input_mouse_pos(&mouse.x, &mouse.y);
	return engine_util_point_in_rect(&button->rect, &mouse) &&
		engine_input_mouse_click(BUTTON_LEFT);
}
