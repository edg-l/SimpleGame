#include "tilemap.h"
#include <stdlib.h>

Tilemap *tilemap_create(int w, int h, TileType fill) {
	Tilemap *t = malloc(sizeof(Tilemap));

	t->w = w;
	t->h = h;
	t->tiles = malloc(sizeof(Tile*));

	for(int y = 0; y < h; y++) {
		t->tiles[y] = malloc(sizeof(Tile) * w);
		for(int x = 0; x < w; x++) {
			t->tiles[y][x].type = fill;
		}
	}

	return t;
}

void tilemap_free(Tilemap *t) {
	for(int y = 0; y < t->h; y++) {
		free(t->tiles[y]);
	}
	free(t->tiles);
	free(t);
}

void tilemap_set(Tilemap *t, int x, int y, TileType type) {
	t->tiles[y][x].type = type;
}

void tilemap_set_rect(Tilemap *t, Rect r, TileType type) {
	if(r.w <= t->w && r.h <= t->h && r.x >= 0 && r.y >= 0) {
		for(int y = r.y; y < r.y + r.h; y++) {
			for(int x = r.x; x < r.x + r.w; x++) {
				tilemap_set(t, x, y, type);
			}
		}
	}
}

void tilemap_set_rect_wall(Tilemap *t, Rect r, TileType type) {
	if(r.w <= t->w && r.h <= t->h && r.x >= 0 && r.y >= 0) {
		for(int y = r.y; y < r.y + r.h; y++) {
			for(int x = r.x; x < r.x + r.w; x++) {
				if(x == r.x || x == r.x + r.w || y == r.y || y == r.y + r.h)
					tilemap_set(t, x, y, type);
			}
		}
	}
}

Tile *tilemap_get(Tilemap *t, int x, int y) {
	return &t->tiles[y][x];
}
