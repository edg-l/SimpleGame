#include "switch.h"
#include <engine/input.h>
#include <engine/logger.h>
#include <engine/util.h>
#include <stdlib.h>
#include <string.h>

Switch *engine_ui_switch_create(int w, int h, Color bg, Color offColor, Color onColor) {
	Switch *s = malloc(sizeof(Switch));

	memset(s, 0, sizeof(Switch));

	s->entity.render_priority = 1000;
	s->entity.on_render = on_render;
	s->entity.on_update = on_update;
	s->entity.on_mouse_button_up = on_mouse_button_up;
	s->entity.on_free = on_free;

	s->rect = (Rect2Df){0, 0, w, h};
	s->bg = bg;
	s->off_color = offColor;
	s->on_color = onColor;
	s->value = 0;
	s->padding = 4;
	s->total_animation_time = 350;
	s->current_animation_time = 0;
	s->animate = 0;
	return s;
}

static void on_mouse_button_up(Entity *entity, unsigned char button_code, int x, int y) {
	Switch *s = (Switch *)entity;

	if (button_code == BUTTON_LEFT && engine_math_mouse_in_rect2df(&s->rect)) {
		s->animate = 1;
		s->value = !s->value;
		if (s->current_animation_time > 0)
			s->current_animation_time = s->total_animation_time - s->current_animation_time;
	}
}

static void on_update(Entity *entity) {
	Switch *s = (Switch *)entity;

	if (s->animate) {
		s->current_animation_time += engine_util_delta_time();
		if (s->current_animation_time >= s->total_animation_time) {
			s->animate = 0;
			s->current_animation_time = 0;
		}
	}
}

static void on_free(Entity *entity) {
	Switch *s = (Switch *)entity;

	free(entity);
}

static void on_render(Entity *entity) {
	Switch *s = (Switch *)entity;

	engine_render_color_s(s->bg);
	engine_render_rect_s(&s->rect, 1);

	if (!s->animate) {
		if (!s->value) {
			Rect2Df r = (Rect2Df){s->rect.x + s->padding, s->rect.y + s->padding,
								  s->rect.w / 2 - s->padding,
								  s->rect.h - s->padding * 2};
			engine_render_color_s(s->off_color);
			engine_render_rect_s(&r, 1);
		} else {
			Rect2Df r = (Rect2Df){s->rect.x + s->rect.w / 2, s->rect.y + s->padding,
								  s->rect.w / 2 - s->padding, s->rect.h - s->padding * 2};
			engine_render_color_s(s->on_color);
			engine_render_rect_s(&r, 1);
		}
	} else {
		double percent = s->current_animation_time / s->total_animation_time;
		double diff = (double)s->rect.w / 2;
		int current = round(diff * percent);

		if (s->value) {
			Color c;

			c.r = s->off_color.r + round((s->on_color.r - s->off_color.r) * percent);
			c.g = s->off_color.g + round((s->on_color.g - s->off_color.g) * percent);
			c.b = s->off_color.b + round((s->on_color.b - s->off_color.b) * percent);
			c.a = 255;

			Rect2Df r = (Rect2Df){s->rect.x + s->padding + current, s->rect.y + s->padding,
								  s->rect.w / 2 - s->padding, s->rect.h - s->padding * 2};
			engine_render_color_s(c);
			engine_render_rect_s(&r, 1);
		} else {
			Color c;

			c.r = s->on_color.r + round((s->off_color.r - s->on_color.r) * percent);
			c.g = s->on_color.g + round((s->off_color.g - s->on_color.g) * percent);
			c.b = s->on_color.b + round((s->off_color.b - s->on_color.b) * percent);
			c.a = 255;

			Rect2Df r = (Rect2Df){s->rect.x + s->rect.w / 2 - current, s->rect.y + s->padding,
								  s->rect.w / 2 - s->padding, s->rect.h - s->padding * 2};
			engine_render_color_s(c);
			engine_render_rect_s(&r, 1);
		}
	}
}
