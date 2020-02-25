#include "tilemap.h"
#include <GL/glew.h>
#include <GL/glu.h>
#include <cglm/cglm.h>
#include <engine/graphics/renderer.h>
#include <engine/graphics/shader.h>
#include <engine/logger.h>
#include <stdlib.h>

static Shader shader;

static Color tileColors[] = {
	{0, 0, 0, 255},
	{90, 77, 65, 255},
	{194, 178, 128, 255},
	{54, 69, 79, 255},
	{207, 16, 32, 255},
	{64, 164, 223, 255},
	{226, 88, 34, 255},
};

typedef struct Vertex {
	GLfloat x;
	GLfloat y;
	GLfloat r, g, b, a;
} Vertex;

void on_free(Entity *entity) {
	Tilemap *t = (Tilemap*)entity;
	glDeleteVertexArrays(1, &t->vao);
	glDeleteBuffers(1, &t->vbo);
	for (int y = 0; y < t->h; y++) {
		free(t->tiles[y]);
	}
	free(t->tiles);
	free(t);
}

void engine_tilemap_set(Tilemap *t, int x, int y, TileType type) {
	if (x >= t->w || x < 0 || y >= t->h || y < 0)
		return;

	t->tiles[y][x].type = type;

	Vertex vertices[6];

	int ox = x * t->tileSize;
	int oy = y * t->tileSize;
	int s = t->tileSize;

	Color c = tileColors[type];

	vertices[0] = (Vertex){ox, oy, c.r, c.g, c.b, c.a};
	vertices[1] = (Vertex){ox + s, oy, c.r, c.g, c.b, c.a};
	vertices[2] = (Vertex){ox, oy + s, c.r, c.g, c.b, c.a};

	vertices[3] = (Vertex){ox + s, oy, c.r, c.g, c.b, c.a};
	vertices[4] = (Vertex){ox + s, oy + s, c.r, c.g, c.b, c.a};
	vertices[5] = (Vertex){ox, oy + s, c.r, c.g, c.b, c.a};

	glBindVertexArray(t->vao);
	glBindBuffer(GL_ARRAY_BUFFER, t->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, (unsigned long)(t->w * y + x) * 6 * (2 * sizeof(GLfloat) + 4 * sizeof(GLfloat)),	sizeof(vertices), vertices);
}

void engine_tilemap_set_rect(Tilemap *t, Rect2Di r, TileType type) {
	if (r.x + r.w <= t->w && r.y + r.h <= t->h && r.x >= 0 && r.y >= 0) {
		for (int y = (int)r.y; y < r.y + r.h; y++) {
			for (int x = (int)r.x; x < r.x + r.w; x++) {
				engine_tilemap_set(t, x, y, type);
			}
		}
	}
}

void engine_tilemap_set_rect_wall(Tilemap *t, Rect2Di r, TileType type) {
	if (r.x + r.w <= t->w && r.y + r.h <= t->h && r.x >= 0 && r.y >= 0) {
		for (int y = (int)r.y; y < r.y + r.h; y++) {
			for (int x = (int)r.x; x < r.x + r.w; x++) {
				if (x == r.x || x == r.x + r.w - 1 || y == r.y || y == r.y + r.h - 1)
					engine_tilemap_set(t, x, y, type);
			}
		}
	}
}

Tile *engine_tilemap_get(Tilemap *t, int x, int y) {
	if (x >= t->w || x < 0 || y >= t->h || y < 0)
		return NULL;
	return &t->tiles[y][x];
}

static void on_render(Entity *entity, double delta) {
	Tilemap *t = (Tilemap*)entity;
	engine_shader_use(shader);
	glBindVertexArray(t->vao);
	glDrawArrays(GL_TRIANGLES, 0, t->w * t->h * 6);
	glBindVertexArray(0);
}

void engine_tilemap_get_tile_rect(Tilemap *t, int x, int y, Rect2Di *out) {
	SDL_assert(out);
	*out = (Rect2Di){x * t->tileSize, y * t->tileSize, t->tileSize, t->tileSize};
}

Tilemap *engine_tilemap_create(int w, int h, int tile_size, TileType fill) {
	Tilemap *t = malloc(sizeof(Tilemap));
	memset(t, 0, sizeof(Tilemap));

	t->w = w;
	t->h = h;
	t->tiles = malloc(sizeof(Tile *) * (unsigned long)h);
	t->tileSize = tile_size;
	t->entity.on_render = on_render;
	t->entity.on_free = on_free;

	Vertex *vertices = malloc(sizeof(Vertex) * w * h * 6);

	srand(2);

	int s = tile_size;
	int n = 0;
	for (int y = 0; y < h; y++) {
		t->tiles[y] = malloc(sizeof(Tile) * (unsigned long)w);
		for (int x = 0; x < w; x++) {
			int type = rand() % NUM_TILE_TYPES;
			Color c = tileColors[type];
			t->tiles[y][x].type = type; //fill;
			int ox = x * s;
			int oy = y * s;
			vertices[n++] = (Vertex){ox, oy, c.r, c.g, c.b, c.a};
			vertices[n++] = (Vertex){ox + s, oy, c.r, c.g, c.b, c.a};
			vertices[n++] = (Vertex){ox, oy + s, c.r, c.g, c.b, c.a};

			vertices[n++] = (Vertex){ox + s, oy, c.r, c.g, c.b, c.a};
			vertices[n++] = (Vertex){ox + s, oy + s, c.r, c.g, c.b, c.a};
			vertices[n++] = (Vertex){ox, oy + s, c.r, c.g, c.b, c.a};
		}
	}

	glGenVertexArrays(1, &t->vao);
	glGenBuffers(1, &t->vbo);

	glBindVertexArray(t->vao);

	glBindBuffer(GL_ARRAY_BUFFER, t->vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * w * h * 6, vertices, GL_DYNAMIC_DRAW);
	free(vertices);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat) + 4 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat) + 4 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	if (!shader) {
		shader = engine_shader_load("resources/shaders/tilemap.vert", "resources/shaders/tilemap.frag", NULL);
		engine_shader_use(shader);
		mat4 proj;
		engine_render_projection(proj);
		engine_shader_set_mat4(shader, "projection", proj);
	}

	return t;
}
