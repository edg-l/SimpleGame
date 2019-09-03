#include "camera.h"
#include <stdlib.h>
#include <engine/settings.h>
#include <math.h>

Camera *camera_create() {
	Camera *c = malloc(sizeof(Camera));
	c->pos[0] = 0;
	c->pos[1] = 0;
	c->pos[2] = 0;
	c->should_update = 1;

	glm_mat4_identity(c->mat);
	glm_translate(c->mat, c->pos);
	return c;
}

void camera_move(Camera *c, float offX, float offY) {
	c->pos[0] -= offX;
	c->pos[1] -= offY;
	glm_mat4_identity(c->mat);
	glm_translate(c->mat, c->pos);
	c->should_update = 1;
}

void screen_to_coords(Camera *c, Tilemap *t, Point screen, Point *out) {
	int width, height;
	out->x = floor((screen.x / (float)t->tileSize) - (c->pos[0] / (float)t->tileSize));
	out->y = floor((screen.y / (float)t->tileSize) - (c->pos[1] / (float)t->tileSize));
}
