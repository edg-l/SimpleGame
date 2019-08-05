#include <stddef.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "renderer.h"
#include "list.h"
#include "color.h"
#include "logger.h"

static SDL_Renderer *pRenderer = NULL;
static List *pFontCache = NULL;
static List *pTextCache = NULL;
static SDL_Color fontColor = {0,0,0,255};

typedef struct CachedFont {
	int pt;
	int style;
	TTF_Font *pFont;
} CachedFont;

typedef struct CachedTexture {
	int w, h;
	SDL_Texture *pTexture;
} CachedTexture;

static void free_font(void* p) {
	CachedFont *c = p;
	TTF_CloseFont(c->pFont);
	c->pFont = NULL;
	free(c);
}

static void free_texture(void *p) {
	CachedTexture *pCachedTexture = p;
	SDL_DestroyTexture(pCachedTexture->pTexture);
	free(pCachedTexture);
}

static const char* font_path(int style) {
	switch(style) {
		case STYLE_LIGHT:
			return "resources/fonts/OpenSans-Light.ttf";
		case STYLE_LIGHT_ITALIC:
			return "resources/fonts/OpenSans-LightItalic.ttf";
		case STYLE_REGULAR:
			return "resources/fonts/OpenSans-Regular.ttf";
		case STYLE_ITALIC:
			return "resources/fonts/OpenSans-Italic.ttf";
		case STYLE_BOLD:
			return "resources/fonts/OpenSans-Bold.ttf";
		case STYLE_BOLD_ITALIC:
			return "resources/fonts/OpenSans-BoldItalic.ttf";
		case STYLE_EXTRABOLD:
			return "resources/fonts/OpenSans-ExtraBold.ttf";
		case STYLE_EXTRABOLD_ITALIC:
			return "resources/fonts/OpenSans-ExtraBoldItalic.ttf";
		case STYLE_SEMIBOLD:
			return "resources/fonts/OpenSans-Semibold.ttf";
		case STYLE_SEMIBOLD_ITALIC:
			return "resources/fonts/OpenSans-SemiboldItalic.ttf";
	}
	return font_path(STYLE_REGULAR);
}

static CachedFont* search_font(int pt, int style) {
	if(!pFontCache)
		return NULL;

	ListValue *pCurrent = pFontCache->head;

	while(pCurrent) {
		CachedFont *c = (CachedFont*)pCurrent->value;

		if(c->pt == pt && c->style == style)
			return c;

		pCurrent = pCurrent->next;
	}

	if(!pCurrent) {
		// Font is not cached, load it.
		CachedFont *c = malloc(sizeof(CachedFont));
		c->pt = pt;
		c->style = style;
		c->pFont = TTF_OpenFont(font_path(style), pt);
		if(!c->pFont) {
			log_write(LOG_ERROR, "Error opening font: %s\n", TTF_GetError());
			return NULL;
		}
		list_push_back(pFontCache, c, sizeof(CachedFont));
		log_write(LOG_INFO, "Added font (%dpt, %d) to cache\n", c->pt, c->style);
		free(c);
	}

	return pFontCache->tail->value;
}

int render_init(SDL_Window *pWindow) {
	pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);

	if(!pRenderer) {
		log_write(LOG_ERROR, "Error creating renderer: %s\n", SDL_GetError());
		return 0;
	}

	if(TTF_Init() == -1) {
		log_write(LOG_ERROR, "Error initializing SDL_ttf: %s\n", TTF_GetError());
		return 0;
	}

	pFontCache = list_create_fn(free_font);
	pTextCache = list_create_fn(free_texture);

	log_write(LOG_INFO, "Renderer initialized.\n");

	return 1;
}

void render_quit() {
	list_clear(pFontCache);
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

void render_text_color(int r, int g, int b, int a) {
	fontColor = (SDL_Color){r, g, b, a};
}

static SDL_Texture *create_text(int pt, int style, const char *text) {
	CachedFont *c = search_font(pt, style);

	if(c && c->pFont && text) {
		SDL_Surface * pTextSurface;
		pTextSurface = TTF_RenderText_Blended(c->pFont, text, fontColor);
		if(!pTextSurface)
			log_write(LOG_ERROR, "Error rendering text: %s\n", TTF_GetError());

		SDL_Texture *pTextTexture;

		pTextTexture = SDL_CreateTextureFromSurface(pRenderer, pTextSurface);

		SDL_FreeSurface(pTextSurface);
		pTextSurface = NULL;

		return pTextTexture;
	}
	else
		log_write(LOG_ERROR, "Tried to render text with either a NULL font or text.\n");
	return NULL;
}

int render_create_cached_text(int pt, int style, const char *text) {
	int cacheId = -1;

	SDL_Texture *t = create_text(pt, style, text);

	if(t) {
		cacheId = list_size(pTextCache);
		CachedTexture *pCachedTexture = malloc(sizeof(CachedTexture));
		pCachedTexture->pTexture = t;

		SDL_QueryTexture(t, NULL, NULL, &pCachedTexture->w, &pCachedTexture->h);

		list_push_back(pTextCache, pCachedTexture, sizeof(CachedTexture));
		free(pCachedTexture);
	}

	return cacheId;
}

void render_text(int x, int y, int pt, int style, const char* text) {
	SDL_Texture *t = create_text(pt, style, text);

	if(t) {
		int w, h;
		SDL_QueryTexture(t, NULL, NULL, &w, &h);
		SDL_Rect dst = {x, y, w, h};
		SDL_RenderCopy(pRenderer, t, NULL, &dst);
		SDL_DestroyTexture(t);
	}
}

void render_cached_text(int id, int x, int y) {
	CachedTexture *c = list_get(pTextCache, id);

	if(c) {
		SDL_Rect dst = {x, y, c->w, c->h};
		SDL_RenderCopy(pRenderer, c->pTexture, NULL, &dst);
	}
}

void render_clear_text_cache() {
	list_clear(pTextCache);
}

void render_text_size(const char* text, int pt, int style, int *w, int *h) {
	CachedFont *c = search_font(pt, style);

	if(TTF_SizeText(c->pFont, text, w, h)) {
		log_write(LOG_ERROR, "Error calculating text size: %s\n", TTF_GetError());
	}
}

