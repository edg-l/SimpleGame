#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#include <cglm/cglm.h>
#include <engine/util.h>
#include <engine/tilemap.h>

typedef struct Camera {
	vec3 pos;
	mat4 mat;
	int should_update;
} Camera;

Camera *camera_create();

void camera_move(Camera *c, float offX, float offY);

void screen_to_coords(Camera *c, Tilemap *t, Point screen, Point *out);

#endif
