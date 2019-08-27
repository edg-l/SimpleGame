#include "io.h"
#include "logger.h"
#include <SDL_rwops.h>
#include <SDL_filesystem.h>
#include <string.h>

static char *app_path = NULL;

static char *combine_path(const char *path1, const char *path2) {
	char* combined_path = malloc(strlen(path1) + strlen(path2) + 1);
	strncpy(combined_path, path1, strlen(path1) + 1);
	strncat(combined_path, path2, strlen(path2) + 1);
	return combined_path;
}

char *io_load(const char *path) {
	SDL_RWops *file = SDL_RWFromFile(path, "r");

	if(!file) {
		log_write(LOG_ERROR, "Loading file with path: %s (%s)\n", path, SDL_GetError());
		return NULL;
	}

	char *source;
	size_t size = SDL_RWsize(file) + 1;

	source = malloc(size);

	int total_read = 0, read = 1;
	char *buf = source;

	while(total_read < size && read != 0) {
		read = SDL_RWread(file, buf, sizeof(char), size - total_read);
		total_read += read;
		buf += read;
	}

	SDL_RWclose(file);

	if(total_read + 1 != size) {
		free(source);
		return NULL;
	}

	source[total_read] = '\0';
	return source;
}

char *io_load_app(const char *path) {
	if(!app_path)
		app_path = SDL_GetPrefPath("Ryozuki", "SimpleGame");

	char *combined_path = combine_path(app_path, path);
	char *value = io_load(combined_path);

	free(combined_path);
	return value;
}

void io_save(const char *path, char *value) {
	if(!app_path)
		app_path = SDL_GetPrefPath("Ryozuki", "SimpleGame");

	char* combined_path = combine_path(app_path, path);

	SDL_RWops *file = SDL_RWFromFile(combined_path, "w");

	if(!file) {
		log_write(LOG_ERROR, "Loading file with path: %s (%s)\n", path, SDL_GetError());
		return;
	}

	size_t real_len = strlen(value);
	size_t len = SDL_RWwrite(file, value, sizeof(char), real_len);

	if(len != real_len) {
		log_write(LOG_WARNING, "Couldn't write all the string to path (%d/%d): %s\n", combined_path,
				len, real_len);
	}

	free(combined_path);
	SDL_RWclose(file);
}

int io_file_exists(const char *path) {
	if(!app_path)
		app_path = SDL_GetPrefPath("Ryozuki", "SimpleGame");

	char* combined_path = combine_path(app_path, path);

	SDL_RWops *file = SDL_RWFromFile(combined_path, "r+");

	free(combined_path);

	if(!file) {
		log_write(LOG_INFO, "called\n");
		SDL_GetError();
		return 0;
	}

	int size = SDL_RWtell(file);

	if(!size) {
		log_write(LOG_INFO, "size: %d\n", size);
		return 0;
	}

	return 1;
}
