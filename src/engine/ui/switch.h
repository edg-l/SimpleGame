#ifndef ENGINE_UI_SWITCH_H
#define ENGINE_UI_SWITCH_H

#include <engine/entity.h>
#include <engine/graphics/renderer.h>

typedef struct Switch {
	Entity entity;
	Rect2Df rect;
	Color bg;
	Color off_color;
	Color on_color;
	int value;
	int padding;
	int animate;
	double current_animation_time;
	double total_animation_time; // in ms
} Switch;

Switch *engine_ui_switch_create(int w, int h, Color bg, Color offColor, Color onColor);

#endif
