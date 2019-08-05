#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>

// Info: https://wiki.libsdl.org/CategoryRender

enum {
	STYLE_LIGHT,
	STYLE_LIGHT_ITALIC,
	STYLE_REGULAR,
	STYLE_ITALIC,
	STYLE_BOLD,
	STYLE_BOLD_ITALIC,
	STYLE_EXTRABOLD,
	STYLE_EXTRABOLD_ITALIC,
	STYLE_SEMIBOLD,
	STYLE_SEMIBOLD_ITALIC
};

int render_init(SDL_Window *pWindow);
void render_quit();

void render_clear();
void render_present();
void render_color(int r, int g, int b, int a);
void render_rect(int x1, int y1, int x2, int y2);
void render_line(int x1, int y1, int x2, int y2);
void render_text_color(int r, int g, int b, int a);
int render_create_cached_text(int pt, int style, const char *text);
void render_cached_text(int id, int x, int y);
void render_text(int x, int y, int pt, int style, const char* text);
void render_clear_text_cache();
void render_text_size(const char* text, int pt, int style, int *w, int *h);

#endif
