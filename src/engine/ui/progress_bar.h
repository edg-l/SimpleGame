#ifndef ENGINE_UI_BAR_H
#define ENGINE_UI_BAR_H

#include <engine/util.h>

typedef struct ProgressBar {
	Rect rect;
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

ProgressBar *pb_create(int w, int h, Color bg, Color start, Color end);
void pb_free(ProgressBar *p);

void pb_set_progress(ProgressBar *p, double progress);
void pb_update(ProgressBar *p);
void render_pb(ProgressBar *p);

#endif
