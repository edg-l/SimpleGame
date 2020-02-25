#include "shader.h"
#include <GL/glew.h>
#include <SDL_rwops.h>
#include <engine/io.h>
#include <engine/list.h>
#include <engine/logger.h>
#include <string.h>

static List *shaders;

static void check_errors(GLuint id, int is_program) {
	int success;
	char info[1024];

	if (is_program) {
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(id, 1024, NULL, info);
			engine_log_write(LOG_WARNING, "Error compiling shader: %s\n", info);
		}
	} else {
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(id, 1024, NULL, info);
			engine_log_write(LOG_WARNING, "Error compiling shader: %s\n", info);
		}
	}
}

static unsigned int load_shader_from_src(const char *vertS, const char *fragS,
										 const char *geoS) {
	GLuint vert, frag, geo, program;

	vert = glCreateShader(GL_VERTEX_SHADER);
	frag = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vert, 1, &vertS, NULL);
	glCompileShader(vert);

	check_errors(vert, 0);

	glShaderSource(frag, 1, &fragS, NULL);
	glCompileShader(frag);

	check_errors(frag, 0);

	if (geoS) {
		geo = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geo, 1, &geoS, NULL);
		glCompileShader(geo);

		check_errors(geo, 0);
	}

	program = glCreateProgram();
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	if (geoS)
		glAttachShader(program, geo);

	glLinkProgram(program);
	check_errors(program, 1);

	glDeleteShader(vert);
	glDeleteShader(frag);
	if (geoS)
		glDeleteShader(geo);

	if (!shaders) {
		shaders = engine_list_create();
	}

	GLuint *proc = malloc(sizeof(GLuint));
	*proc = program;

	engine_list_push_back(shaders, proc, sizeof(unsigned int));

	return program;
}

Shader engine_shader_load(const char *vertexPath, const char *fragmentPath,
						  const char *geometryPath) {
	char *vertS = engine_io_load(vertexPath);
	char *fragS = engine_io_load(fragmentPath);
	char *geoS = NULL;

	if (geometryPath)
		geoS = engine_io_load(geometryPath);

	unsigned int program = load_shader_from_src(vertS, fragS, geoS);

	free(vertS);
	free(fragS);
	free(geoS);

	return program;
}

Shader engine_shader_load_str(const char *vertexSrc, const char *fragmentSrc,
							  const char *geometrySrc) {
	return load_shader_from_src(vertexSrc, fragmentSrc, geometrySrc);
}

void engine_shader_delete(Shader shader) {
	glDeleteProgram(shader);
}

void engine_shader_use(Shader shader) {
	glUseProgram(shader);
}

void engine_shader_update_camera(Camera *c) {
	Node *current = shaders->head;

	while (current) {
		unsigned int *val = current->value;
		engine_shader_set_mat4(*val, "view", c->view);
		current = current->next;
	}
	c->should_update = 0;
}

int engine_shader_has_uniform(Shader shader, const char *name) {
	engine_shader_use(shader);
	engine_log_info("uniform: %d\n", glGetUniformLocation(shader, name));
	return glGetUniformLocation(shader, name) != -1;
}

void engine_shader_set_int(Shader shader, const char *name, int x) {
	engine_shader_use(shader);
	glUniform1i(glGetUniformLocation(shader, name), x);
}

void engine_shader_set_float(Shader shader, const char *name, float x) {
	engine_shader_use(shader);
	glUniform1f(glGetUniformLocation(shader, name), x);
}

void engine_shader_set_vec3(Shader shader, const char *name, float x, float y,
							float z) {
	engine_shader_use(shader);
	glUniform3f(glGetUniformLocation(shader, name), x, y, z);
}

void engine_shader_set_vec4(Shader shader, const char *name, float x, float y, float z,
							float w) {
	engine_shader_use(shader);
	glUniform4f(glGetUniformLocation(shader, name), x, y, z, w);
}

void engine_shader_set_mat4(Shader shader, const char *name, mat4 mat) {
	engine_shader_use(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader, name), 1, GL_FALSE, mat[0]);
}
