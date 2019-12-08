#ifndef GRAPHICS_SHADER_H
#define GRAPHICS_SHADER_H

#include <cglm/cglm.h>
#include <engine/camera.h>

typedef unsigned int Shader;

// Geometry is optional, pass null if not required.
Shader engine_shader_load(const char *vertexPath, const char *fragmentPath, const char *geometryPath);
Shader engine_shader_load_str(const char *vertexSrc, const char *fragmentSrc, const char *geometrySrc);

void engine_shader_use(Shader shader);
void engine_shader_delete(Shader shader);
int engine_shader_has_uniform(Shader shader, const char *name);
void engine_shader_update_camera(Camera *c);

void engine_shader_set_int(Shader shader, const char *name, int x);
void engine_shader_set_float(Shader shader, const char *name, float x);
void engine_shader_set_vec3(Shader shader, const char *name, float x, float y, float z);
void engine_shader_set_vec4(Shader shader, const char *name, float x, float y, float z, float w);
void engine_shader_set_mat4(Shader shader, const char *name, mat4 mat);

#endif
