#ifndef GRAPHICS_SHADER_H
#define GRAPHICS_SHADER_H


typedef int Shader;

// Geometry is optional, pass null if not required.
Shader shader_load(const char *vertexPath, const char *fragmentPath, const char *geometryPath);

void shader_use(Shader shader);
void shader_delete(Shader shader);

#endif
