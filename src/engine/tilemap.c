#include "tilemap.h"
#include <stdlib.h>
#include <engine/graphics/shader.h>
#include <engine/graphics/renderer.h>
#include <engine/logger.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <cglm/cglm.h>

static Shader shader;

static Color tileColors[NUM_TILE_TYPES] = {
	(Color){0, 0, 200, 255},
	(Color){0, 200, 0, 255},
};

Tilemap *tilemap_create(int w, int h, int tile_size, TileType fill) {
	Tilemap *t = malloc(sizeof(Tilemap));

	t->w = w;
	t->h = h;
	t->tiles = malloc(sizeof(Tile*) * h);
	t->tileSize = tile_size;

	struct vertex {
		GLfloat x;
		GLfloat y;
		GLint r,g,b,a;
	} vertices[w*h * 6];

	Color c = tileColors[fill];

	int s = tile_size;
	int n = 0;
	for(int y = 0; y < h; y++) {
		t->tiles[y] = malloc(sizeof(Tile) * w);
		for(int x = 0; x < w; x++) {
			t->tiles[y][x].type = fill;
			int ox = x * s;
			int oy = y * s;
			vertices[n++] = (struct vertex){ox, oy, c.r, c.g, c.b, c.a};
			vertices[n++] = (struct vertex){ox + s, oy, c.r, c.g, c.b, c.a};
			vertices[n++] = (struct vertex){ox, oy + s, c.r, c.g, c.b, c.a};

			vertices[n++] = (struct vertex){ox + s, oy, c.r, c.g, c.b, c.a};
			vertices[n++] = (struct vertex){ox + s, oy + s, c.r, c.g, c.b, c.a};
			vertices[n++] = (struct vertex){ox, oy + s, c.r, c.g, c.b, c.a};
		}
	}

	glGenVertexArrays(1, &t->vao);
	glGenBuffers(1, &t->vbo);

	glBindVertexArray(t->vao);

	glBindBuffer(GL_ARRAY_BUFFER, t->vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat) + 4 * sizeof(GLint), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_INT, GL_FALSE,
			2 * sizeof(GLfloat) + 4 * sizeof(GLint), (void*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	if(!shader) {
		shader = shader_load("resources/shaders/tilemap.vert", "resources/shaders/tilemap.frag", NULL);
		shader_use(shader);
		mat4 projection;
		render_projection(projection);
		shader_set_mat4(shader, "projection", projection);
	}

	return t;
}

void tilemap_free(Tilemap *t) {
	glDeleteVertexArrays(1, &t->vao);
	glDeleteBuffers(1, &t->vbo);
	for(int y = 0; y < t->h; y++) {
		free(t->tiles[y]);
	}
	free(t->tiles);
	free(t);
}

void tilemap_set(Tilemap *t, int x, int y, TileType type) {
	if(x >= t->w || x < 0 || y >= t->h || y < 0)
		return;

	t->tiles[y][x].type = type;

	struct vertex {
		GLfloat x;
		GLfloat y;
		GLint r,g,b,a;
	} vertices[6];

	int ox = x * t->tileSize;
	int oy = y * t->tileSize;
	int s = t->tileSize;

	Color c = tileColors[type];

	vertices[0] = (struct vertex){ox, oy, c.r, c.g, c.b, c.a};
	vertices[1] = (struct vertex){ox + s, oy, c.r, c.g, c.b, c.a};
	vertices[2] = (struct vertex){ox, oy + s, c.r, c.g, c.b, c.a};

	vertices[3] = (struct vertex){ox + s, oy, c.r, c.g, c.b, c.a};
	vertices[4] = (struct vertex){ox + s, oy + s, c.r, c.g, c.b, c.a};
	vertices[5] = (struct vertex){ox, oy + s, c.r, c.g, c.b, c.a};

	glBindVertexArray(t->vao);
	glBindBuffer(GL_ARRAY_BUFFER, t->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, (t->w*y + x) * 6 * (2 * sizeof(GLfloat) + 4 * sizeof(GLint)),
			sizeof(vertices), vertices);
}

void tilemap_set_rect(Tilemap *t, Rect r, TileType type) {
	if(r.x + r.w <= t->w && r.y + r.h <= t->h && r.x >= 0 && r.y >= 0) {
		for(int y = r.y; y < r.y + r.h; y++) {
			for(int x = r.x; x < r.x + r.w; x++) {
				tilemap_set(t, x, y, type);
			}
		}
	}
}

void tilemap_set_rect_wall(Tilemap *t, Rect r, TileType type) {
	if(r.x + r.w <= t->w && r.y + r.h <= t->h && r.x >= 0 && r.y >= 0) {
		for(int y = r.y; y < r.y + r.h; y++) {
			for(int x = r.x; x < r.x + r.w; x++) {
				if(x == r.x || x == r.x + r.w - 1 || y == r.y || y == r.y + r.h - 1)
					tilemap_set(t, x, y, type);
			}
		}
	}
}

Tile *tilemap_get(Tilemap *t, int x, int y) {
	if(x >= t->w || x < 0 || y >= t->h || y < 0)
		return NULL;
	return &t->tiles[y][x];
}

void render_tilemap(Tilemap *t) {
	shader_use(shader);
	glBindVertexArray(t->vao);
	glDrawArrays(GL_TRIANGLES, 0, t->w * t->h * 6);
	glBindVertexArray(0);
}

Rect tilemap_get_tile_rect(Tilemap *t, int x, int y) {
	return util_rect(x * t->tileSize, y * t->tileSize, t->tileSize, t->tileSize);
}
