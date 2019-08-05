#include <stddef.h>
#include "renderer.h"
#include <SDL.h>
#include <SDL_ttf.h>

static SDL_Renderer *pRenderer = NULL;
static TTF_Font *pFont = NULL;
static TTF_Font *pBoldFont = NULL;
static TTF_Font *pBigBoldFont = NULL;

// Maybe implement a font cache for diferent sizes.
// maybe improve the list to handle void*?
// cant do this in 5 mins

int render_init(SDL_Window *pWindow) {
	pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);

	if(!pRenderer) {
		fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
		return 0;
	}

	if(TTF_Init() == -1) {
		fprintf(stderr, "Error initializing SDL_ttf: %s\n", TTF_GetError());
		return 0;
	}

	pFont = TTF_OpenFont("resources/OpenSans-Regular.ttf", 18);

	return 1;
}

void render_quit() {
	TTF_Quit();
	SDL_DestroyRenderer(pRenderer);
	pRenderer = NULL;
}

void render_clear() {
	SDL_RenderClear(pRenderer);
}

void render_present() {
	SDL_RenderPresent(pRenderer);
}

void render_color(int r, int g, int b, int a) {
	SDL_SetRenderDrawColor(pRenderer, r, g, b, a);
}

void render_rect(int x1, int y1, int x2, int y2) {
	SDL_Rect x = {x1, y1, x2, y2};
	SDL_RenderDrawRect(pRenderer, &x);
}

void render_line(int x1, int y1, int x2, int y2) {
	SDL_RenderDrawLine(pRenderer, x1, y1, x2, y2);
}
