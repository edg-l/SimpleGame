#include "tilemap.h"
#include <stdlib.h>
#include <engine/graphics/shader.h>
#include <engine/graphics/renderer.h>
#include <engine/logger.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <cglm/cglm.h>

static Shader shader;
static const char* vertShader = "#version 330 core\n"
"layout (location = 0) in vec2 vertex;\n"
"layout (location = 1) in int inTileType;\n"
"flat out int tileType;\n"
"uniform mat4 projection;\n"
"uniform mat4 view;\n"
"void main() {\n"
"	gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
"	tileType = inTileType;"
"}";
static const char *fragShader = "#version 330 core\n"
"flat in int tileType;\n"
"//uniform vec4 colors[2];\n"
"void main() {\n"
"	//gl_FragColor = colors[tileType];\n"
"	if(tileType == 0) gl_FragColor = vec4(1, 0, 0, 1);\n"
"	else gl_FragColor = vec4(0, 1, 0, 1);\n"
"}";

Tilemap *tilemap_create(int w, int h, int tile_size, TileType fill) {
	Tilemap *t = malloc(sizeof(Tilemap));

	t->w = w;
	t->h = h;
	t->tiles = malloc(sizeof(Tile*) * h);
	t->tileSize = tile_size;

	struct vertex {
		GLfloat x;
		GLfloat y;
		GLint type;
	} vertices[w*h * 6];

	int s = tile_size;
	int n = 0;
	for(int y = 0; y < h; y++) {
		t->tiles[y] = malloc(sizeof(Tile) * w);
		for(int x = 0; x < w; x++) {
			t->tiles[y][x].type = fill;
			int ox = x * s;
			int oy = y * s;
			vertices[n++] = (struct vertex){ox, oy, TILE_AIR};
			vertices[n++] = (struct vertex){ox + s, oy, TILE_AIR};
			vertices[n++] = (struct vertex){ox, oy + s, TILE_AIR};

			vertices[n++] = (struct vertex){ox + s, oy, TILE_AIR};
			vertices[n++] = (struct vertex){ox + s, oy + s, TILE_AIR};
			vertices[n++] = (struct vertex){ox, oy + s, TILE_AIR};
		}
	}

	glGenVertexArrays(1, &t->vao);
	glGenBuffers(1, &t->vbo);

	glBindVertexArray(t->vao);

	glBindBuffer(GL_ARRAY_BUFFER, t->vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat) + sizeof(GLint), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 1, GL_INT, GL_FALSE,
			2 * sizeof(GLfloat) + sizeof(GLint), (void*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	if(!shader) {
		shader = shader_load_str(vertShader, fragShader, NULL);
		shader_use(shader);
		mat4 projection;
		render_projection(projection);
		shader_set_mat4(shader, "projection", projection);
		mat4 view;
		glm_mat4_identity(view);
		shader_set_mat4(shader, "view", projection);
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

void render_tilemap(Tilemap *t) {
	shader_use(shader);
	glBindVertexArray(t->vao);
	glDrawArrays(GL_TRIANGLES, 0, t->w * t->h * 6);
	glBindVertexArray(0);
}
