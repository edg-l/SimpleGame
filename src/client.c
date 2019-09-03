#include <engine/color.h>
#include <engine/list.h>
#include <engine/graphics/renderer.h>
#include <engine/logger.h>
#include <engine/ui/button.h>
#include <engine/ui/switch.h>
#include <engine/ui/textbox.h>
#include <engine/settings.h>
#include <engine/camera.h>
#include <engine/graphics/shader.h>
#include <engine/tilemap.h>
#include <SDL.h>
#include <stdlib.h>
#include "config.h"

int main(int argc, const char* argv[]) {

	if(!render_init(1024, 768, "SimpleGame")) {
		log_write(LOG_ERROR, "Error creating renderer: %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	util_init();

	Rect screen = util_rect(0, 0, settings_get_int("window_width"),
			settings_get_int("window_height"));

	render_text_color(20, 20, 200, 255);

	int ws, hs;
	render_text_size("hello world\ntest\ntest2", 38, STYLE_REGULAR, &ws, &hs);

	Point mouse;

	Camera *camera = camera_create();
	camera_move(camera, 30, 2);

	Tilemap *t = tilemap_create(50, 50, 16, TILE_AIR);
	tilemap_set(t, 11, 10, TILE_WALL);
	//tilemap_set_rect(t, util_rect(4, 4, 5, 2), TILE_WALL);
	// use deltatime

	while(1) {
		// TODO: wrap this
		util_update_keyboard();
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				goto cleanup;
			}
		}

		// Update
		util_update();
		double delta = util_delta_time();
		log_info("Delta: %f ms\n", delta);
		mouse = util_mouse_pos();
		if(util_is_keyup(SDL_SCANCODE_D))
			camera_move(camera, 5, 0);

		shader_update_camera(camera);
		Point out;
		screen_to_coords(camera, t, mouse, &out);

		// Render
		render_color(200, 46, 46, 255);
		render_clear();

		render_tilemap(t);

		render_present();
		SDL_Delay(1);
	}

cleanup:
	util_quit();
	settings_save("settings.ini");
	settings_quit();
	render_quit();
	return EXIT_SUCCESS;
}
