#include "config.h"
#include <engine/engine.h>
#include <engine/logger.h>
#include <engine/ui/button.h>
#include <engine/ui/textbox.h>
#include <engine/tilemap.h>

int main(int argc, const char *argv[]) {
	engine_init("SimpleGame", argc, argv);


	Tilemap *tilemap = engine_tilemap_create(100, 100, 8, TILE_FIRE);
	Rect2Di c = (Rect2Di){0, 0, 100, 100};
	engine_tilemap_set_rect_wall(tilemap, c, TILE_SAND);
	engine_entity_add((Entity *)tilemap);


	return engine_run();
}
