#ifndef GRAPHICS_SHADER_H
#define GRAPHICS_SHADER_H

#include <cglm/cglm.h>
#include <engine/camera.h>

typedef int Shader;

// Geometry is optional, pass null if not required.
Shader shader_load(const char *vertexPath, const char *fragmentPath, const char *geometryPath);
Shader shader_load_str(const char *vertexSrc, const char *fragmentSrc, const char *geometrySrc);

void shader_use(Shader shader);
void shader_delete(Shader shader);
int shader_has_uniform(Shader shader, const char *name);
void shader_update_camera(Camera *c);

void shader_set_int(Shader shader, const char *name, int x);
void shader_set_float(Shader shader, const char *name, float x);
void shader_set_vec3(Shader shader, const char *name, float x, float y, float z);
void shader_set_vec4(Shader shader, const char *name, float x, float y, float z, float w);
void shader_set_mat4(Shader shader, const char *name, mat4 mat);

#endif
