#include "engine.h"
#include <engine/graphics/renderer.h>
#include <engine/logger.h>
#include <engine/settings.h>


void engine_init(const char *pName) {
	
	if (!render_init(1024, 768, pName)) {
	 engine_log_write(LOG_ERROR, "Error creating renderer: %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	
 engine_render_clear_color(COLOR_WHITE);
 engine_util_init();

	// TODO: Add entity manager and initialize it here.
	// TODO: Add function to set a active camera, create one by default?
};

void engine_on_tick() {
 engine_util_update_keyboard();

	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			engine_quit();
		}
	}

 engine_util_update();
}

void engine_quit() {
 engine_util_quit();
 engine_settings_save("settings.ini");
 engine_settings_quit();
 engine_render_quit();
}
