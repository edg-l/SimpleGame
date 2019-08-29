#include <engine/color.h>
#include <engine/list.h>
#include <engine/graphics/renderer.h>
#include <engine/logger.h>
#include <engine/ui/button.h>
#include <engine/settings.h>
#include <SDL.h>
#include <stdlib.h>
#include "config.h"

int main(int argc, const char* argv[]) {

	if(!render_init(1024, 768, "SimpleGame")) {
		log_write(LOG_ERROR, "Error creating renderer: %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	util_update();

	Rect screen = util_rect(0, 0, settings_get_int("window_width"),
			settings_get_int("window_height"));

	render_text_color(20, 20, 200, 255);

	int ws, hs;
	render_text_size("hello world\ntest\ntest2", 38, STYLE_REGULAR, &ws, &hs);

	Point mouse;

	Button *button = button_create(200, 100, 40, STYLE_REGULAR, "Exit",
			util_color(255, 255, 255, 255), util_color(100, 50, 0, 255));
	button->rect.x = 600;
	button->rect.y = 400;


	util_rect_center(&screen, &button->rect);

	while(1) {
		// TODO: wrap this
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				goto cleanup;
			}
		}

		// Update
		util_update();
		mouse = util_mouse_pos();

		if(button_is_pressed(button))
			goto cleanup;

		// Render
		render_color(200, 46, 46, 255);
		render_clear();

		render_rect(20, 200, ws, hs, 0);
		render_text(38, STYLE_REGULAR, "hello world\ntest\ntest2", 20, 200);

		render_line(20, 20, mouse.x, mouse.y);

		render_text(24, STYLE_SEMIBOLD_ITALIC, VERSION_STR, 10, 10);

		render_button(button);

		render_color(0, 200, 0, 255);
		render_rect_s(&rect1, 1);
		render_color(200, 0, 0, 255);
		render_rect_s(&rect2, 1);
		render_color(0, 0, 200, 255);
		render_rect_s(&rect3, 1);

		render_present();
		SDL_Delay(1);
	}

cleanup:
	settings_save("settings.ini");
	settings_quit();
	render_quit();
	return EXIT_SUCCESS;
}
