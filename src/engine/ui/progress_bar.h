#ifndef ENGINE_UI_BAR_H
#define ENGINE_UI_BAR_H

#include <engine/util.h>

typedef struct ProgressBar {
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
void engine_ui_progressbar_free(ProgressBar *p);

void engine_ui_progressbar_set_progress(ProgressBar *p, double progress);
void engine_ui_progressbar_update(ProgressBar *p);
void engine_render_progressbar(ProgressBar *p);

#endif
