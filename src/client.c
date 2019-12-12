#include "config.h"
#include <engine/engine.h>
#include <engine/logger.h>
#include <engine/ui/button.h>

static void on_start_click() {
	engine_log_info("Start clicked!");
}

static void on_exit_click() {
	engine_quit();
}

int main(int argc, const char *argv[]) {
	engine_init("SimpleGame", argc, argv);

	Button *start = engine_button_default(200, 100, 30, "Start", on_start_click);
	Button *exit = engine_button_default(200, 100, 30, "Quit", on_exit_click);

	Rect2Df screen;
	engine_util_screen(&screen);

	engine_math_rect2df_center(&screen, &start->rect);

	engine_math_rect2df_margin(MARGIN_BOTTOM, 10, &start->rect, &exit->rect);

	engine_entity_add((Entity *)start);
	engine_entity_add((Entity *)exit);

	return engine_run();
}
