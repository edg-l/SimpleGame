#include "config.h"
#include "player.h"
#include <SDL.h>
#include <engine/camera.h>
#include <engine/color.h>
#include <engine/graphics/renderer.h>
#include <engine/graphics/shader.h>
#include <engine/list.h>
#include <engine/logger.h>
#include <engine/settings.h>
#include <engine/textbuffer.h>
#include <engine/tilemap.h>
#include <engine/ui/button.h>
#include <engine/ui/progress_bar.h>
#include <engine/ui/switch.h>
#include <engine/ui/textbox.h>
#include <stdlib.h>

// TODO: animated bar, dropdown

int main(int argc, const char *argv[]) {

	if (!render_init(1024, 768, "SimpleGame")) {
		log_write(LOG_ERROR, "Error creating renderer: %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	util_init();

	render_clear_color(COLOR_WHITE);

	Rect screen = util_rect(0, 0, settings_get_int("window_width"),
							settings_get_int("window_height"));

	Point mouse;

	Camera *camera = camera_create();
	// camera_lookat(camera, 0, 0, 0);

	char aFpsBuf[64];
	double delta = util_delta_time();
	double fps = util_fps();
	double averageFps = util_fps();
	double fpsTime = 0;
	int fpsTimesAdded = 1;

	Player *p = player_create(100, 500, 100, 100);
	p->rect.x = 500;
	p->rect.y = 400;

	ProgressBar *pb = pb_create(200, 50, COLOR_GOLD, COLOR_RED, COLOR_GREEN);
	Button *but = button_create(200, 50, 20, STYLE_REGULAR, "sjJAavier",
								COLOR_WHITE, COLOR_RED);
	but->rect.x = 400;
	but->rect.y = 200;

	pb_set_progress(pb, 100);

	Textbox *tb = textbox_create(200, 100, 20, 40, COLOR_BLUE, COLOR_GREY, COLOR_RED);
	tb->rect.x = 200;
	tb->rect.y = 50;

	Tilemap *map = tilemap_create(20, 20, 32, TILE_AIR);

	while (1) {
		// TODO: wrap this
		util_update_keyboard();
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				goto cleanup;
			}
			textbox_on_sdlevent(tb, &event);
		}

		// Update
		util_update();
		delta = util_delta_time();

		if (util_is_keyup(SDL_SCANCODE_K)) {
			pb_set_progress(pb, 60);
		}

		if (util_is_keyup(SDL_SCANCODE_G)) {
			pb_set_progress(pb, 20);
		}

		fpsTime += delta;
		if (fpsTime > 500) {
			averageFps = fps / fpsTimesAdded;
			fps = util_fps();
			fpsTimesAdded = 1;
			fpsTime = 0;
		} else {
			fps += util_fps();
			fpsTimesAdded++;
		}

		util_str_format(aFpsBuf, sizeof(aFpsBuf), "FPS: %.02f", averageFps);

		mouse = util_mouse_pos();

		camera_update(camera);
		pb_update(pb);
		textbox_update(tb);

		if (button_is_pressed(but))
			log_info("pressed");

		// Render
		render_clear();

		render_use_camera(0);
		render_button(but);
		render_textbox(tb);
		render_text_color_s(COLOR_RED);
		render_text(36, STYLE_REGULAR, "abcdefghijkmnlopkrstxyzw1234567890?!,.-_", 40, 600);
		render_text_color_s(COLOR_CYAN);
		render_text(36, STYLE_BOLD, "abcdefghijkmnlopkrstxyzw1234567890?!,.-_", 40, 680);
		render_text_color_s(COLOR_PURPLE);
		render_text(36, STYLE_ITALIC, "abcdefghijkmnlopkrstxyzw1234567890?!,.-_", 40, 720);
		render_pb(pb);

		render_use_camera(1);
		render_tilemap(map);

		render_present();
		// SDL_Delay(1);
	}

cleanup:
	util_quit();
	settings_save("settings.ini");
	settings_quit();
	render_quit();
	return EXIT_SUCCESS;
}
