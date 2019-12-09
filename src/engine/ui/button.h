#ifndef BUTTON_UI_H
#define BUTTON_UI_H

#include <engine/graphics/renderer.h>

typedef struct Button {
	Rect rect;
	unsigned int textSizeW, textSizeH;
	int textpt;
	int textStyle;
	Color fg;
	Color bg;
	char *pText;
} Button;

Button *button_create(unsigned int w, unsigned int h, int pt, int style, const char* text, Color fg, Color bg);

int button_is_pressed(Button *button);

void button_free(Button *button);

void engine_render_button(Button *button);
#endif
