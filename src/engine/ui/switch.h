#ifndef ENGINE_UI_SWITCH_H
#define ENGINE_UI_SWITCH_H

#include <engine/graphics/renderer.h>

typedef struct Switch {
	Rect rect;
	Color bg;
	Color off_color;
	Color on_color;
	int value;
	int clicked_tick;
	int padding;
	double animation_time; // in ms
} Switch;

Switch *switch_create(int w, int h, Color bg, Color offColor, Color onColor);

void switch_update(Switch *s);
void render_switch(Switch *s);

void switch_free(Switch *s);

#endif
