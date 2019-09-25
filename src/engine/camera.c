#include "camera.h"
#include <stdlib.h>
#include <engine/settings.h>
#include <math.h>

Camera *camera_create() {
	Camera *c = malloc(sizeof(Camera));
	c->pos[0] = 0;
	c->pos[1] = 0;
	c->pos[2] = 0;

	c->dir[0] = 0;
	c->dir[1] = 0;
	c->dir[2] = 1;
	c->should_update = 1;

	glm_mat4_identity(c->mat);
	glm_translate(c->mat, c->pos);
	return c;
}

void camera_move(Camera *c, float offX, float offY, float offZ) {
	c->pos[0] += offX * c->dir[0];
	c->pos[1] += offY * c->dir[1];
	c->pos[2] += offZ * c->dir[2];
	vec3 off = {offX * c->dir[0], offY * c->dir[1], offZ * c->dir[2]};
	glm_translate(c->mat, off);
	c->should_update = 1;
}


void camera_lookat(Camera *c, float x, float y, float z) {
	vec3 p = {x,y,z};

	vec3 up = {0, 1, 0};

	glm_lookat(c->pos, p, up, c->mat);
	c->should_update = 1;
}

void screen_to_coords(Camera *c, Tilemap *t, Point screen, Point *out) {
	int width, height;
	out->x = floor((screen.x / (float)t->tileSize) - (c->pos[0] / (float)t->tileSize));
	out->y = floor((screen.y / (float)t->tileSize) - (c->pos[1] / (float)t->tileSize));
}
