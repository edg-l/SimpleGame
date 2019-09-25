#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#include <cglm/cglm.h>
#include <engine/util.h>
#include <engine/tilemap.h>

typedef struct Camera {
	vec3 pos;
	vec3 dir;
	mat4 mat;
	int should_update;
} Camera;

Camera *camera_create();

void camera_move(Camera *c, float offX, float offY, float offZ);
void camera_lookat(Camera *c, float x, float y, float z);

void screen_to_coords(Camera *c, Tilemap *t, Point screen, Point *out);

#endif
