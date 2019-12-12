#include "engine.h"
#include <engine/graphics/renderer.h>
#include <engine/input.h>
#include <engine/io.h>
#include <engine/logger.h>
#include <engine/settings.h>
#include <engine/entity.h>
#include <engine/math/vector.h>


// TODO: Render circle
// TODO: Render texture
// TODO: Resource manager
// TODO: Fix text rendering, for example "Quit"

void engine_init(const char *pName, int argc, const char *argv[]) {

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1) {
		engine_log_error("Error initializing SDL2: %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	engine_settings_init();

	// TODO: Set default fullscreen with display size.
	engine_settings_add_int("window_width", 1024, 5, 5000);
	engine_settings_add_int("window_height", 768, 5, 5000);

	engine_settings_add_int("msaa_enable", 1, 0, 1);
	engine_settings_add_int("msaa_value", 2, 0, 4);
	engine_settings_add_int("vsync", 1, 0, 1);

	if (!engine_io_file_exists("settings.ini")) {
		engine_log_info("Settings doesn't exist, creating it.\n");
		engine_settings_save("settings.ini");
	}
	engine_settings_load("settings.ini");

	if (!engine_render_init(pName)) {
		engine_log_write(LOG_ERROR, "Error creating renderer: %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	engine_input_init();
	engine_entity_init();
	engine_render_clear_color(COLOR_WHITE);

	// TODO: Add entity manager and initialize it here.
	// TODO: Add function to set a active camera, create one by default?
};

int engine_on_tick() {
	int stop = 0;

	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			stop = 1;
		}
		engine_entity_onevent(&event);
	}

	engine_input_update();
	engine_util_update();
	engine_entity_onupdate();
	return stop;
}

int engine_run() {
	int stop = 0;
	while (!stop) {
		stop = engine_on_tick();

		engine_render_clear();

		engine_entity_onrender();

		engine_render_present();
		// SDL_Delay(1);
	}

	engine_quit();
	return EXIT_SUCCESS;
}

void engine_quit() {
	engine_settings_save("settings.ini");
	engine_settings_quit();
	engine_render_quit();
}
