#include "config.h"
#include <engine/engine.h>


int main(int argc, const char *argv[]) {
	engine_init("SimpleGame");

	while (1) {
		engine_on_tick();

		render_clear();

		render_present();
		// SDL_Delay(1);
	}

	engine_quit();
	return EXIT_SUCCESS;
}
