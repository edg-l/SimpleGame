#include "config.h"
#include <engine/engine.h>
#include <engine/logger.h>
#include <engine/ui/button.h>

static void on_hello_click() {
	engine_log_info("Button clicked!");
}

int main(int argc, const char *argv[]) {
	engine_init("SimpleGame", argc, argv);

	Button *start = engine_button_default(200, 100, 30, "Hello", on_hello_click);

	Rect2Df screen;
	engine_util_screen(&screen);

	engine_math_rect2df_center(&screen, &start->rect);

	engine_entity_add((Entity *)start);

	return engine_run();
}
