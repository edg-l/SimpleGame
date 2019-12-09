#include "config.h"
#include <engine/engine.h>


int main(int argc, const char *argv[]) {
	engine_init("SimpleGame");

	int stop = 0;
	while (!stop) {
		stop = engine_on_tick();

		engine_render_clear();

		engine_render_present();
		// SDL_Delay(1);
	}

	engine_quit();
	return EXIT_SUCCESS;
}
