#ifndef BUTTON_UI_H
#define BUTTON_UI_H

#include <engine/graphics/renderer.h>

#define DEFAULT_FG (SDL_Color){255,255,255,255}
#define DEFAULT_BG (SDL_Color){70, 70, 70, 255}

typedef struct Button {
	Rect rect;
	int textSizeW, textSizeH;
	int textpt;
	Color fg;
	Color bg;
	char *pText;
} Button;

Button *button_create(int w, int h, const char* text, Color fg, Color bg);

void button_free(Button *button);

void render_button(Button *button);
#endif
