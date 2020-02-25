#ifndef ENGINE_UI_BAR_H
#define ENGINE_UI_BAR_H

#include <engine/entity.h>
#include <engine/util.h>

typedef struct ProgressBar {
	Entity entity;
	Rect2Df rect;
	double progress;
	double initial_progress;
	double next_progress;
	Color bg;
	Color start;
	Color end;
	int padding;
	double animate;
	double total_animation_time;
	double current_animation_time;
} ProgressBar;

ProgressBar *engine_ui_progressbar_create(int w, int h, Color bg, Color start, Color end);

void engine_ui_progressbar_set_progress(ProgressBar *p, double progress);

#endif
