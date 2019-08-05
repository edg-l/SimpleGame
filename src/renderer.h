#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>

// Info: https://wiki.libsdl.org/CategoryRender

int render_init(SDL_Window *pWindow);
void render_quit();

void render_clear();
void render_present();
void render_color(int r, int g, int b, int a);
void render_rect(int x1, int y1, int x2, int y2);
void render_line(int x1, int y1, int x2, int y2);

#endif
