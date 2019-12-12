#include "config.h"
#include <engine/engine.h>


int main(int argc, const char *argv[]) {
	engine_init("SimpleGame", argc, argv);
	return engine_run();
}
