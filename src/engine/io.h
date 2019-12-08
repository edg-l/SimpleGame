#ifndef IO_H
#define IO_H

#include <stdlib.h>

// Loads a file, remember to free the pointer.
char *io_load(const char *path);

char *io_load_app(const char *path);
void engine_io_save(const char *path, char *value);

int engine_io_file_exists(const char *path);

#endif
