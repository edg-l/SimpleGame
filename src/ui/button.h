#ifndef BUTTON_UI_H
#define BUTTON_UI_H

#include <SDL.h>

#define DEFAULT_FG (SDL_Color){255,255,255,255}
#define DEFAULT_BG (SDL_Color){70, 70, 70, 255}

typedef struct Button {
	float x, y;
	float w, h, o;
	int textID;
	int textSizeW, textSizeH;
	SDL_Color fg;
	SDL_Color bg;
} Button;

Button *button_create(float w, float h, float outline, 
		const char* text, SDL_Color fg, SDL_Color bg);

void button_free(Button *button);

void render_button(Button *button);
#endif
