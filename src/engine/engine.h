#ifndef ENGINE_ENGINE_H
#define ENGINE_ENGINE_H

#include <engine/graphics/renderer.h>

void engine_init(const char *pName);
void engine_quit();

// TODO: Add engine events
void engine_on_tick();

#endif

