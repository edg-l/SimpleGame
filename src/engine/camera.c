#include "camera.h"
#include <engine/graphics/shader.h>
#include <engine/settings.h>
#include <math.h>
#include <stdlib.h>

Camera *camera_create() {
	Camera *c = malloc(sizeof(Camera));
	glm_vec3_zero(c->pos);

	c->should_update = 1;
	return c;
}

void camera_move(Camera *c, float offX, float offY, float speed) {
	c->pos[0] -= offX;
	c->pos[1] -= offY;
	c->should_update = 1;
}

void camera_update(Camera *c) {
	if (c->should_update) {
		glm_mat4_identity(c->view);
		glm_translate(c->view, c->pos);
	 engine_shader_update_camera(c);
	}
}
