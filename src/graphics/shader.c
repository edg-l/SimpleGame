#include "shader.h"
#include <GL/glew.h>
#include <SDL_rwops.h>
#include <string.h>
#include <io.h>
#include <logger.h>

static Shader last_shader_used = -1;

static void check_errors(GLuint id, int is_program) {
	int success;
	char info[1024];

	if(is_program)
		glGetProgramInfoLog(id, 1024, NULL, info);
	else
		glGetShaderInfoLog(id, 1024, NULL, info);
	log_write(LOG_WARNING, "Error compiling shader: %s\n", info);
}

Shader shader_load(const char *vertexPath, const char *fragmentPath, const char *geometryPath) {
	char *vertS = io_load(vertexPath);
	char *fragS = io_load(fragmentPath);
	char *geoS = NULL;

	if(geometryPath)
		geoS = io_load(geometryPath);

	int vert, frag, geo, program;

	vert = glCreateShader(GL_VERTEX_SHADER);
	frag = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vert, 1, (const GLchar **)&vertS, NULL);
	glCompileShader(vert);

	check_errors(vert, 0);

	glShaderSource(frag, 1, (const GLchar **)&fragS, NULL);
	glCompileShader(frag);

	check_errors(frag, 0);

	if(geoS) {
		glShaderSource(geo, 1, (const GLchar **)&geoS, NULL);
		glCompileShader(geo);

		check_errors(geo, 0);
	}

	program = glCreateProgram();
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	if(geoS)
		glAttachShader(program, geo);

	glLinkProgram(program);
	check_errors(program, 1);

	glDeleteShader(vert);
	glDeleteShader(frag);
	if(geoS)
		glDeleteShader(geo);

	free(vertS);
	free(fragS);
	free(geoS);

	return program;
}

void shader_delete(Shader shader) {
	if(last_shader_used == shader)
		last_shader_used = -1;
	glDeleteProgram(shader);
}

void shader_use(Shader shader) {
	glUseProgram(shader);
	last_shader_used = shader;
}
