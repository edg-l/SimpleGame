#include "switch.h"
#include <engine/logger.h>
#include <engine/util.h>
#include <engine/input.h>
#include <stdlib.h>
#include <string.h>

Switch *engine_ui_switch_create(int w, int h, Color bg, Color offColor, Color onColor) {
    Switch *s = malloc(sizeof(Switch));
    s->rect = engine_util_rect(0, 0, w, h);
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

void engine_ui_switch_update(Switch *s) {
    if (engine_util_mouse_in_rect(&s->rect) && engine_input_mouse_click(BUTTON_LEFT)) {
        s->animate = 1;
        s->value = !s->value;
        if (s->current_animation_time > 0)
            s->current_animation_time = s->total_animation_time - s->current_animation_time;
    } else if (s->animate) {
        s->current_animation_time += engine_util_delta_time();
        if (s->current_animation_time >= s->total_animation_time) {
            s->animate = 0;
            s->current_animation_time = 0;
        }
    }
}

void engine_ui_switch_free(Switch *s) { free(s); }

void engine_render_switch(Switch *s) {
    engine_render_color_s(s->bg);
    engine_render_rect_s(&s->rect, 1);

    if (!s->animate) {
        if (!s->value) {
            Rect r = engine_util_rect(s->rect.x + s->padding, s->rect.y + s->padding,
                               s->rect.w / 2 - s->padding,
                               s->rect.h - s->padding * 2);
            engine_render_color_s(s->off_color);
            engine_render_rect_s(&r, 1);
        } else {
            Rect r = engine_util_rect(
                s->rect.x + s->rect.w / 2, s->rect.y + s->padding,
                s->rect.w / 2 - s->padding, s->rect.h - s->padding * 2);
            engine_render_color_s(s->on_color);
            engine_render_rect_s(&r, 1);
        }
    } else {
        double percent = s->current_animation_time / s->total_animation_time;
        double diff = (double)s->rect.w / 2;
        int current = round(diff * percent);
        if (s->value) {
            Color c;

            c.r = s->off_color.r +
                  round((s->on_color.r - s->off_color.r) * percent);
            c.g = s->off_color.g +
                  round((s->on_color.g - s->off_color.g) * percent);
            c.b = s->off_color.b +
                  round((s->on_color.b - s->off_color.b) * percent);
            c.a = 255;

            Rect r = engine_util_rect(
                s->rect.x + s->padding + current, s->rect.y + s->padding,
                s->rect.w / 2 - s->padding, s->rect.h - s->padding * 2);
            engine_render_color_s(c);
            engine_render_rect_s(&r, 1);
        } else {
            Color c;

            c.r = s->on_color.r +
                  round((s->off_color.r - s->on_color.r) * percent);
            c.g = s->on_color.g +
                  round((s->off_color.g - s->on_color.g) * percent);
            c.b = s->on_color.b +
                  round((s->off_color.b - s->on_color.b) * percent);
            c.a = 255;

            Rect r = engine_util_rect(
                s->rect.x + s->rect.w / 2 - current, s->rect.y + s->padding,
                s->rect.w / 2 - s->padding, s->rect.h - s->padding * 2);
            engine_render_color_s(c);
            engine_render_rect_s(&r, 1);
        }
    }
}
