#include <stddef.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "renderer.h"
#include "list.h"

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
			return "resources/OpenSans-Light.ttf";
		case STYLE_LIGHT_ITALIC:
			return "resources/OpenSans-LightItalic.ttf";
		case STYLE_NORMAL:
			return "resources/OpenSans-Normal.ttf";
		case STYLE_ITALIC:
			return "resources/OpenSans-Italic.ttf";
		case STYLE_BOLD:
			return "resources/OpenSans-Bold.ttf";
		case STYLE_BOLD_ITALIC:
			return "resources/OpenSans-BoldItalic.ttf";
		case STYLE_EXTRABOLD:
			return "resources/OpenSans-ExtraBold.ttf";
		case STYLE_EXTRABOLD_ITALIC:
			return "resources/OpenSans-ExtraBoldItalic.ttf";
		case STYLE_SEMIBOLD:
			return "resources/OpenSans-Semibold.ttf";
		case STYLE_SEMIBOLD_ITALIC:
			return "resources/OpenSans-SemiboldItalic.ttf";
	}
	return font_path(STYLE_NORMAL);
}

static CachedFont* search_font(int pt, int style) {
	if(!pFontCache || !pFontCache->head)
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
			fprintf(stderr, "Error opening font: %s\n", TTF_GetError());
			return NULL;
		}
		list_push_back(pFontCache, c, sizeof(CachedFont));
		free(c);
	}

	return pFontCache->tail->value;
}

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

	pFontCache = list_create_fn(free_font);
	pTextCache = list_create_fn(free_texture);

	return 1;
}

void render_quit() {

	if(pFontCache && pFontCache->head) {
		ListValue *pCurrent = pFontCache->head;

		while(pCurrent) {
			CachedFont *c = (CachedFont*)pCurrent->value;
			TTF_CloseFont(c->pFont);
			pCurrent = pCurrent->next;
		}
	}
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
			fprintf(stderr, "Error rendering text: %s\n", TTF_GetError());

		SDL_Texture *pTextTexture;

		pTextTexture = SDL_CreateTextureFromSurface(pRenderer, pTextSurface);

		SDL_FreeSurface(pTextSurface);
		pTextSurface = NULL;

		return pTextTexture;
	}
	else
		fprintf(stderr, "Tried to render text with either a NULL font or text.\n");
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
