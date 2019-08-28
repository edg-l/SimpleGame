#include <engine/color.h>
#include <engine/list.h>
#include <engine/graphics/renderer.h>
#include <engine/logger.h>
#include <engine/ui/button.h>
#include <engine/settings.h>
#include <engine/network/netclient.h>
#include "config.h"
#include "protocol.h"

int main(int argc, const char* argv[]) {

	if(!render_init(1024, 768, "SimpleGame")) {
		log_write(LOG_ERROR, "Error creating renderer: %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	netclient_init(1024);

	render_text_color(20, 20, 200, 255);
	int ws, hs;
	render_text_size("hello world\ntest\ntest2", 38, STYLE_REGULAR, &ws, &hs);

	while(1) {
		// TODO: wrap this
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				goto cleanup;
			}
		}

		// Update


		// Render
		render_color(200, 46, 46, 255);
		render_clear();

		render_rect(20, 200, ws, hs, 0);
		render_text(38, STYLE_REGULAR, "hello world\ntest\ntest2", 20, 200);

		render_present();
		SDL_Delay(1);
	}

cleanup:
	settings_save("settings.ini");
	settings_quit();
	netclient_quit();
	render_quit();
	return EXIT_SUCCESS;
}
