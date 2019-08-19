#include "button.h"
#include <graphics/renderer.h>
#include <string.h>

Button *button_create(float w, float h, float outline,
		const char* text, SDL_Color fg, SDL_Color bg) {
	Button *button;

	button = malloc(sizeof(Button));
	memset(button, 0, sizeof(Button));

	button->w = w;
	button->h = h;
	button->o = outline;
	button->fg = fg;
	button->bg = bg;

	int len = strlen(text) + 1;
	button->pText = malloc(len);
	memcpy(button->pText, text, len);

	render_text_size(button->pText, h / 3, STYLE_REGULAR, &button->textSizeW, &button->textSizeH);

	return button;
}

void button_free(Button *button) {
	free(button->pText);
	free(button);
}

void render_button(Button *button) {
	SDL_Rect buttonRect = {button->x, button->y, button->w, button->h};
	SDL_Color outline = button->fg;
	if(button->o > 0) {
		render_color(button->fg.r / 20, button->fg.g / 20, button->fg.b / 20, 255);
		render_rect(button->x - button->o,
				button->y - button->o,
				button->w + button->o * 2,
				button->h + button->o * 2,
				1);
	}
	int mx, my;
	SDL_Point m = {0,0};
	SDL_GetMouseState(&m.x, &m.y);

	if(SDL_PointInRect(&m, &buttonRect))
		render_color(button->bg.r + 10, button->bg.g + 10, button->bg.b + 10, button->bg.a);
	else
		render_color(button->bg.r, button->bg.g, button->bg.b, button->bg.a);
	render_rect(button->x, button->y, button->w, button->h,	1);

	render_text_color(button->fg.r, button->fg.g, button->fg.b, button->fg.a);
	render_text(button->h / 3, STYLE_REGULAR, button->pText,
			button->x + button->w / 2.f - button->textSizeW / 4.f,
			button->y + button->h / 2.f - button->textSizeH / 4.f);
}
