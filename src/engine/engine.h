#ifndef ENGINE_ENGINE_H
#define ENGINE_ENGINE_H

#include <engine/graphics/renderer.h>

void engine_init(const char *pName, int argc, const char *argv[]);
void engine_quit();
int engine_run();

// TODO: Add engine events
int engine_on_tick();

#endif

