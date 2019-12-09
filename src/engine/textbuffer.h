#ifndef ENGINE_TEXTBUFFER_H
#define ENGINE_TEXTBUFFER_H

typedef struct TextBuffer {
	char *buffer;
	unsigned long size;
	unsigned long cur_size;
} TextBuffer;

TextBuffer *engine_buffer_create(unsigned long initial_size);

void engine_buffer_add_str(TextBuffer *tb, const char *text);

void engine_buffer_free(TextBuffer *tb);

#endif
