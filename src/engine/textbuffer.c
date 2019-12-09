#include "textbuffer.h"

#include <engine/logger.h>
#include <stdlib.h>
#include <string.h>

TextBuffer *engine_buffer_create(unsigned long initial_size) {
	TextBuffer *tb = malloc(sizeof(TextBuffer));
	tb->size = initial_size;
	tb->cur_size = 0;

	tb->buffer = malloc(sizeof(char) * tb->size);
	memset(tb->buffer, 0, sizeof(char) * tb->size);
	return tb;
}

void engine_buffer_add_str(TextBuffer *tb, const char *text) {
	size_t len = strlen(text);

	// strcat uses the null char on existing strings, but if size is 0 we need to add it.
	if (tb->size == 0)
		len++;

	if (tb->cur_size + len >= tb->size) {
		tb->buffer = realloc(tb->buffer, sizeof(char) * (tb->cur_size + len));
		tb->size = tb->cur_size + len;
	}

	strcat(tb->buffer, text);
	tb->cur_size += len;
}

void engine_buffer_free(TextBuffer *tb) {
	free(tb->buffer);
	free(tb);
}
