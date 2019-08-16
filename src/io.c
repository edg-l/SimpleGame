#include "io.h"
#include "logger.h"
#include <SDL_rwops.h>
#include <string.h>

char *io_load(const char *path) {
	SDL_RWops *file = SDL_RWFromFile(path, "r");

	if(!file) {
		log_write(LOG_WARNING, "Error loading file with path: %s (%s)\n", path, SDL_GetError());
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

	if(total_read != size) {
		free(source);
		return NULL;
	}

	source[total_read] = '\0';
	return source;
}
