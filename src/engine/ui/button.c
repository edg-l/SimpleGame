#include "button.h"
#include <engine/graphics/renderer.h>
#include <engine/util.h>
#include <engine/logger.h>
#include <engine/input.h>
#include <stdlib.h>
#include <string.h>

static void on_render(Entity *entity, double delta) {
	Button *button = (Button *)entity;

	if (engine_math_mouse_in_rect2df(&button->rect))
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


	engine_render_text_color_s(COLOR_BLACK);
	Rect2Df rect;
	float w, h;

	const char *text = "Testing";
	engine_render_text_size(text, 20, STYLE_REGULAR, &w, &h);

	rect.x = 30;
	rect.y = 30;
	rect.w = w;
	rect.h = h;

	engine_render_color_s(COLOR_RED);
	engine_render_rect_s(&rect, 0);
	engine_render_text(20, STYLE_REGULAR, text, 30, 30);

	const char *text2 = "testTing";
	engine_render_text_size(text2, 20, STYLE_REGULAR, &w, &h);

	rect.x = 120;
	rect.y = 30;
	rect.w = w;
	rect.h = h;

	engine_render_color_s(COLOR_RED);
	engine_render_rect_s(&rect, 0);
	engine_render_text(20, STYLE_REGULAR, text2, 120, 30);
}

static void on_mouse_button_down(Entity *entity, unsigned char button_code, int x, int y) {
	Button *button = (Button *)entity;
	if(button->on_click && button_code == SDL_BUTTON_LEFT
			&& engine_math_mouse_in_rect2df(&button->rect)) {
		button->on_click();
	}
}

static void on_free(Entity *entity) {
	Button *button = (Button *)entity;
	free(button->pText);
	free(button);
}

Button *engine_button_create(unsigned int w, unsigned int h, int pt, int style, const char *text,
		BUTTON_ON_CLICK_FN on_click, Color fg, Color bg) {
	Button *button;

	button = malloc(sizeof(Button));
	memset(button, 0, sizeof(Button));

	button->entity.render_priority = 1000;
	button->entity.on_render = on_render;
	button->entity.on_mouse_button_down = on_mouse_button_down;
	button->entity.on_free = on_free;

	button->rect = (Rect2Df){0, 0, w, h};

	button->on_click = on_click;

	button->fg = fg;
	button->bg = bg;
	button->textpt = pt;
	button->textStyle = style;

	int len = strlen(text) + 1;
	button->pText = malloc(sizeof(char) * len);
	strcpy(button->pText, text);

	engine_render_text_size(button->pText, button->textpt, button->textStyle,
			&button->textSizeW, &button->textSizeH);

	return button;
}

