#include <engine/color.h>
#include <engine/list.h>
#include <engine/graphics/renderer.h>
#include <engine/logger.h>
#include <engine/ui/button.h>
#include <engine/ui/switch.h>
#include <engine/ui/textbox.h>
#include <engine/settings.h>
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

	Button *button = button_create(200, 100, 40, STYLE_REGULAR, "Exit",
			util_color(255, 255, 255, 255), util_color(100, 50, 0, 255));
	button->rect.x = 600;
	button->rect.y = 400;


	util_rect_center(&screen, &button->rect);
	button->rect.y += 300;

	Switch *s = switch_create(100, 50, util_color(63, 45, 31, 255),
			util_color(210, 210, 210, 255),
			util_color(134, 96, 60, 255));
	util_rect_center(&screen, &s->rect);

	char aBuf[64];
	memset(aBuf, 0, sizeof(aBuf));

	Textbox *tb = textbox_create(200, 50, 20, 30, util_color(255, 255, 255, 255),
			util_color(0, 100, 0, 255),
			util_color(255, 255, 255, 255));
	tb->rect.x = 100;
	tb->rect.y = 400;

	Tilemap *t = tilemap_create(20, 20, 16, TILE_AIR);
	tilemap_set_rect(t, util_rect(4, 4, 2, 2), TILE_WALL);

	while(1) {
		// TODO: wrap this
		util_update_keyboard();
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				goto cleanup;
			}
			else if(event.type == SDL_TEXTINPUT) {
				textbox_on_textinput(tb, event.text.text);
			}
		}

		// Update
		util_update();
		mouse = util_mouse_pos();

		if(button_is_pressed(button))
			goto cleanup;

		switch_update(s);
		sprintf(aBuf, "Switch: %s", s->value ? "On": "Off");

		textbox_update(tb);

		// Render
		render_color(200, 46, 46, 255);
		render_clear();

		render_text_color_s(COLOR_GOLD);
		render_text(78, STYLE_EXTRABOLD_ITALIC, "Hello twitch!", 20, 200);

		render_text_color_s(COLOR_CYAN);
		render_text(24, STYLE_BOLD, VERSION_STR, 10, 10);

		render_button(button);
		render_text(24, STYLE_REGULAR, aBuf, 400, 480);
		render_switch(s);

		render_textbox(tb);
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
