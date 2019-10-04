#include "camera.h"
#include <stdlib.h>
#include <engine/settings.h>
#include <engine/graphics/shader.h>
#include <math.h>

Camera *camera_create() {
	Camera *c = malloc(sizeof(Camera));
	glm_vec3_zero(c->pos);
	glm_vec3_zero(c->dir);
	glm_vec3_zero(c->up);
	c->dir[2] = 1;
	c->up[1] = 1;

	c->should_update = 1;
	return c;
}

void camera_move(Camera *c, Movement m, float speed) {
	vec3 cross;
	vec3 dest;
	switch(m) {
		case MOVE_FORWARD:
			glm_vec3_scale(c->dir, speed, dest);
			glm_vec3_add(c->pos, dest, c->pos);
			break;
		case MOVE_BACKWARD:
			glm_vec3_scale(c->dir, speed, dest);
			glm_vec3_sub(c->pos, dest, c->pos);
			break;
		case STRAFE_LEFT:
			glm_vec3_scale(c->dir, speed, dest);
			glm_vec3_cross(dest, c->up, cross);
			glm_vec3_add(c->pos, cross, c->pos);
			break;
		case STRAFE_RIGHT:
			glm_vec3_scale(c->dir, speed, dest);
			glm_vec3_cross(dest, c->up, cross);
			glm_vec3_sub(c->pos, cross, c->pos);
			break;
	}
	c->should_update = 1;
}

void camera_rotate(Camera *c, float angle, vec3 axis) {
	glm_vec3_rotate(c->dir, angle, axis);
	c->should_update = 1;
}

void camera_update(Camera *c) {
	if(c->should_update) {
		glm_mat4_identity(c->view);
		vec3 up = {0,1,0};
		glm_look(c->pos, c->dir, up, c->view);
		shader_update_camera(c);
	}
}
