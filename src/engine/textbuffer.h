#ifndef ENGINE_TEXTBUFFER_H
#define ENGINE_TEXTBUFFER_H

typedef struct TextBuffer {
	char *buffer;
	unsigned long size;
	unsigned long cur_size;
} TextBuffer;

TextBuffer *tb_create(unsigned long initial_size);

void tb_add(TextBuffer *tb, const char *text);

void tb_free(TextBuffer *tb);

#endif
