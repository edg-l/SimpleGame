#ifndef ENGINE_UI_SWITCH_H
#define ENGINE_UI_SWITCH_H

#include <engine/graphics/renderer.h>

typedef struct Switch {
	Rect rect;
	Color bg;
	Color off_color;
	Color on_color;
	int value;
	int padding;
	int animate;
	double current_animation_time;
	double total_animation_time; // in ms
} Switch;

Switch *switch_create(int w, int h, Color bg, Color offColor, Color onColor);

void engine_ui_switch_update(Switch *s);
void engine_render_switch(Switch *s);

void engine_ui_switch_free(Switch *s);

#endif
