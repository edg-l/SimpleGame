#ifndef ENGINE_TILEMAP_H
#define ENGINE_TILEMAP_H

#include <engine/math/rect.h>
#include <engine/entity.h>

typedef enum TileType {
	TILE_AIR,
	TILE_ROCK,
	TILE_SAND,
	TILE_COAL,
	TILE_LAVA,
	TILE_WATER,
	TILE_FIRE,
	NUM_TILE_TYPES
} TileType;

typedef struct Tile {
	TileType type;
} Tile;

typedef struct Tilemap {
	Entity entity;
	Tile **tiles;
	int w, h;
	int tileSize;
	unsigned int vao, vbo;
} Tilemap;

Tilemap *engine_tilemap_create(int w, int h, int tile_size, TileType fill);

void engine_tilemap_set(Tilemap *t, int x, int y, TileType type);

void engine_tilemap_set_rect(Tilemap *t, Rect2Di r, TileType type);
void engine_tilemap_set_rect_wall(Tilemap *t, Rect2Di r, TileType type);
Tile *engine_tilemap_get(Tilemap *t, int x, int y);
void engine_tilemap_get_tile_rect(Tilemap *t, int x, int y, Rect2Di *out);

#endif
