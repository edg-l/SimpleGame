#include <stddef.h>
#include <SDL.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL_opengl.h>
#include <SDL_ttf.h>
#include "renderer.h"
#include "list.h"
#include "color.h"
#include "logger.h"

static SDL_Window *pWindow = NULL;
static SDL_GLContext glContext;
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

int render_init(int width, int height, const char *title) {
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) == -1) {
		log_write(LOG_ERROR, "Error initializing SDL2: %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	pWindow = SDL_CreateWindow("SimpleGame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
			width, height, SDL_WINDOW_OPENGL);

	if(!pWindow) {
		log_write(LOG_ERROR, "Error creating window: %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	glContext = SDL_GL_CreateContext(pWindow);

	if(!glContext) {
		log_write(LOG_ERROR, "Error creating renderer: %s\n", SDL_GetError());
		return 0;
	}

	if(TTF_Init() == -1) {
		log_write(LOG_ERROR, "Error initializing SDL_ttf: %s\n", TTF_GetError());
		return 0;
	}

	// Initialize GLEW
	glewExperimental = GL_TRUE; 
	GLenum glewError = glewInit();

	if(glewError != GLEW_OK) {
		log_write(LOG_ERROR, "Error initializing GLEW: %s\n", glewGetErrorString(glewError));
		return 0;
	}

	// Set vsync
	SDL_GL_SetSwapInterval(1);

	glClearColor(0, 0, 0, 1);

	pFontCache = list_create_fn(free_font);
	pTextCache = list_create_fn(free_texture);

	log_write(LOG_INFO, "Renderer initialized.\n");

	return 1;
}

void render_quit() {
	list_clear(pFontCache);
	TTF_Quit();
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyRenderer(pRenderer);
	pRenderer = NULL;
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
}

void render_clear() {
	glClear(GL_COLOR_BUFFER_BIT);
}

void render_present() {
	SDL_GL_SwapWindow(pWindow);
}

void render_color(int r, int g, int b, int a) {
	SDL_SetRenderDrawColor(pRenderer, r, g, b, a);
}

void render_rect(float x1, float y1, float x2, float y2, int filled) {
	SDL_FRect x = {x1, y1, x2, y2};
	if(filled)
		SDL_RenderFillRectF(pRenderer, &x);
	else
		SDL_RenderDrawRectF(pRenderer, &x);
}

void render_line(float x1, float y1, float x2, float y2) {
	SDL_RenderDrawLineF(pRenderer, x1, y1, x2, y2);
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

int render_create_cached_text(int pt, int style, const char *text, int *w, int *h) {
	int cacheId = -1;

	SDL_Texture *t = create_text(pt, style, text);

	if(t) {
		cacheId = list_size(pTextCache);
		CachedTexture *pCachedTexture = malloc(sizeof(CachedTexture));
		pCachedTexture->pTexture = t;

		SDL_QueryTexture(t, NULL, NULL, &pCachedTexture->w, &pCachedTexture->h);
		render_text_size(text, pt, style, w, h);

		list_push_back(pTextCache, pCachedTexture, sizeof(CachedTexture));
		free(pCachedTexture);
	}

	return cacheId;
}

void render_text(float x, float y, int pt, int style, const char* text) {
	SDL_Texture *t = create_text(pt, style, text);

	if(t) {
		int w, h;
		SDL_QueryTexture(t, NULL, NULL, &w, &h);
		SDL_FRect dst = {x, y, w, h};
		SDL_RenderCopyF(pRenderer, t, NULL, &dst);
		SDL_DestroyTexture(t);
	}
}

void render_cached_text(int id, float x, float y) {
	CachedTexture *c = list_get(pTextCache, id);

	if(c) {
		SDL_FRect dst = {x, y, c->w, c->h};
		SDL_RenderCopyF(pRenderer, c->pTexture, NULL, &dst);
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

