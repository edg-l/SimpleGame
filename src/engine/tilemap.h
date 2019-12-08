#ifndef ENGINE_TILEMAP_H
#define ENGINE_TILEMAP_H

#include <engine/util.h>

typedef enum TileType {
	TILE_AIR,
	TILE_WALL,
	NUM_TILE_TYPES
} TileType;

typedef struct Tile {
	TileType type;
} Tile;

typedef struct Tilemap {
	Tile **tiles;
	int w, h;
	int tileSize;
	unsigned int vao, vbo;
} Tilemap;

Tilemap *tilemap_create(int w, int h, int tile_size, TileType fill);
void engine_tilemap_free(Tilemap *t);

void engine_tilemap_set(Tilemap *t, int x, int y, TileType type);

void engine_tilemap_set_rect(Tilemap *t, Rect r, TileType type);
void engine_tilemap_set_rect_wall(Tilemap *t, Rect r, TileType type);
Tile *tilemap_get(Tilemap *t, int x, int y);
Rect engine_tilemap_get_tile_rect(Tilemap *t, int x, int y);

void engine_render_tilemap(Tilemap *t);

#endif
