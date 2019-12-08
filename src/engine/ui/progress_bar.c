#include "progress_bar.h"
#include <engine/graphics/renderer.h>
#include <engine/logger.h>
#include <math.h>

ProgressBar *pb_create(int w, int h, Color bg, Color start, Color end) {
    ProgressBar *p = malloc(sizeof(ProgressBar));
    p->rect = engine_util_rect(0, 0, w, h);
    p->bg = bg;
    p->start = start;
    p->end = end;
    p->animate = 0;
    p->total_animation_time = 2000;
    p->current_animation_time = 0;
    p->progress = 0;
    p->initial_progress = 0;
    p->next_progress = 0;
    p->padding = 2;
    return p;
}

void engine_ui_progressbar_free(ProgressBar *p) { free(p); }

void engine_ui_progressbar_set_progress(ProgressBar *p, double progress) {
    p->initial_progress = p->progress;
    p->next_progress = progress;
    p->animate = 1;
    p->current_animation_time = 0;
}

void engine_ui_progressbar_update(ProgressBar *p) {
    if (p->animate) {
        double delta = engine_util_delta_time();
        p->current_animation_time += delta;
        if (p->initial_progress < p->next_progress) {
            double diff = p->next_progress - p->initial_progress;
            double percent = p->current_animation_time /
                             (p->total_animation_time * (diff / 100.f));
            p->progress = p->initial_progress + (diff * percent);
            if (p->progress >= p->next_progress) {
                p->progress = p->next_progress;
                p->animate = 0;
                p->current_animation_time = 0;
            }
        } else {
            double diff = p->initial_progress - p->next_progress;
            double percent = p->current_animation_time /
                             (p->total_animation_time * (diff / 100.f));
            p->progress = p->initial_progress - (diff * percent);
            if (p->progress <= p->next_progress) {
                p->progress = p->next_progress;
                p->animate = 0;
                p->current_animation_time = 0;
            }
        }
    }
}
void engine_render_pb(ProgressBar *p) {
    engine_render_color_s(p->bg);
    engine_render_rect_s(&p->rect, 1);

    Rect inside;
    engine_util_rect_padding(&inside, &p->rect, p->padding);
    Color c;
    if (p->progress <= 0) {
        c = p->start;
    } else if (p->progress >= 100) {
        c = p->end;
    } else {
        c.r =
            p->start.r + round((p->end.r - p->start.r) * (p->progress / 100.f));
        c.g =
            p->start.g + round((p->end.g - p->start.g) * (p->progress / 100.f));
        c.b =
            p->start.b + round((p->end.b - p->start.b) * (p->progress / 100.f));
        c.a = p->start.a;
    }

    inside.w = round(inside.w * (p->progress / 100.f));

    engine_render_color_s(c);
    engine_render_rect_s(&inside, 1);
}
