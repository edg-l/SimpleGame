#include "io.h"
#include "logger.h"
#include <SDL_filesystem.h>
#include <SDL_rwops.h>
#include <string.h>

static char *app_path = NULL;

static char *combine_path(const char *path1, const char *path2) {
    char *combined_path = malloc(sizeof(char) * (strlen(path1) + strlen(path2) + 1));
    strcpy(combined_path, path1);
    strcat(combined_path, path2);
    return combined_path;
}

char *engine_io_load(const char *path) {
    SDL_RWops *file = SDL_RWFromFile(path, "r");

    if (!file) {
        engine_log_write(LOG_ERROR, "Loading file with path: %s (%s)", path, SDL_GetError());
        return NULL;
    }

    char *source;
    size_t size = SDL_RWsize(file) + 1;

    source = malloc(size);

    int total_read = 0, read = 1;
    char *buf = source;

    while (total_read < size && read != 0) {
        read = SDL_RWread(file, buf, sizeof(char), size - total_read);
        total_read += read;
        buf += read;
    }

    SDL_RWclose(file);

    if (total_read + 1 != size) {
        free(source);
        return NULL;
    }

    source[total_read] = '\0';
    return source;
}

char *engine_io_load_app(const char *path) {
    if (!app_path)
        app_path = SDL_GetPrefPath("Ryozuki", "SimpleGame");

    char *combined_path = combine_path(app_path, path);
    char *value = engine_io_load(combined_path);

    free(combined_path);
    return value;
}

void engine_io_save(const char *path, char *value) {
    if (!app_path)
        app_path = SDL_GetPrefPath("Ryozuki", "SimpleGame");

    char *combined_path = combine_path(app_path, path);

    SDL_RWops *file = SDL_RWFromFile(combined_path, "w");

    if (!file) {
        engine_log_write(LOG_ERROR, "Loading file with path: %s (%s)", path, SDL_GetError());
        return;
    }

    size_t real_len = strlen(value);
    size_t len = SDL_RWwrite(file, value, sizeof(char), real_len);

    if (len != real_len) {
        engine_log_write(LOG_WARNING, "Couldn't write all the string to path (%d/%d): %s", combined_path, len, real_len);
    }

    free(combined_path);
    SDL_RWclose(file);
}

int engine_io_file_exists(const char *path) {
    if (!app_path)
        app_path = SDL_GetPrefPath("Ryozuki", "SimpleGame");

    char *combined_path = combine_path(app_path, path);

    SDL_RWops *file = SDL_RWFromFile(combined_path, "r+");

    free(combined_path);

    if (!file) {
        SDL_GetError();
        return 0;
    }

    return 1;
}
