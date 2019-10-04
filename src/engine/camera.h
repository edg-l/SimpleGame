#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#include <cglm/cglm.h>
#include <engine/util.h>
#include <engine/tilemap.h>

typedef struct Camera {
	vec3 pos;
	vec3 dir;
	vec3 up;
	mat4 view;
	int should_update;
} Camera;

typedef enum Movement {
	MOVE_FORWARD,
	MOVE_BACKWARD,
	STRAFE_LEFT,
	STRAFE_RIGHT
} Movement;

Camera *camera_create();

void camera_move(Camera *c, Movement m, float speed);
void camera_rotate(Camera *c, float angle, vec3 axis);
void camera_update(Camera *c);

#endif
