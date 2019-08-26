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
#include "shader.h"
#include <cglm/cglm.h>
#include <ft2build.h>
#include FT_FREETYPE_H

static FT_Library ft;

static SDL_Window *pWindow = NULL;
static SDL_GLContext glContext;
static SDL_Renderer *pRenderer = NULL;
static List *pFontCache = NULL;
static List *pTextCache = NULL;
static SDL_Color fontColor = {0,0,0,255};
static Shader quadShader;
static Shader textShader;
static mat4 projection;
static RenderID quadVAO;
static RenderID textVAO;
static GLuint textVBO;

static const char *pQuadShaderVert = "#version 330 core\n\
									  layout (location = 0) in vec2 vertex;\n\
									  uniform mat4 projection;\n\
									  uniform mat4 model;\n\
									  void main () {\n\
										  gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);\n\
											  //gl_Position = vec4(vertex.xy, 0.0, 1.0);\n\
									  }";

static const char *pQuadShaderFrag = "#version 330 core\n\
									  uniform vec4 quadColor;\n\
									  out vec4 FragColor;\n\
									  void main() {\n\
										  FragColor = quadColor;\n\
									  }";

static const char *pTextShaderVert = "#version 330 core\n\
									  layout (location = 0) in vec4 vertex;\n\
									  out vec2 TexCoords;\n\
									  uniform mat4 projection;\n\
									  void main() {\n\
										  gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n\
											  TexCoords = vertex.zw;\n\
									  }";

static const char *pTextShaderFrag = "#version 330 core\n\
									  in vec2 TexCoords;\n\
									  uniform vec4 textColor;\n\
									  uniform sampler2D text;\n\
									  void main() {\n\
										  gl_FragColor = textColor * vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\
									  }";

typedef struct Glyph {
	FT_ULong code;
	GLuint advance;
	GLuint bx, by;
	GLuint width, height;
	GLuint tx, ty;
} Glyph;

typedef struct CachedFont {
	int pt;
	int style;
	GLuint tex;
	GLuint atlas_width;
	GLuint atlas_height;
	List *pCharList;
	FT_Face ft;
} CachedFont;

typedef struct CachedTexture {
	int w, h;
	GLuint texID;
} CachedTexture;

void GLAPIENTRY opengl_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, 
		GLsizei length, const GLchar* message, const void* userParam) {
	log_write(type == GL_DEBUG_TYPE_ERROR ? LOG_ERROR : LOG_INFO,
			"OpenGL message type=%d, severity=%d, message: %s\n", type, severity, message);
}

static void free_font(void* p) {
	CachedFont *c = p;
	glDeleteTextures(1, &c->tex);
	FT_Done_Face(c->ft);
	list_free(c->pCharList);
	free(c);
}

static void free_texture(void *p) {
	CachedTexture *pCachedTexture = p;
	glDeleteTextures(1, &pCachedTexture->texID);
	free(pCachedTexture);
}

static unsigned int count_glyphs(FT_Face face) {
	FT_ULong c;
	FT_UInt gindex;
	c = FT_Get_First_Char(face, &gindex);
	int count = 0;
	while(gindex != 0) {
		c = FT_Get_Next_Char(face, c, &gindex);
		count++;
	}
	return count;
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
		CachedFont *cfont = malloc(sizeof(CachedFont));
		cfont->pt = pt;
		cfont->style = style;
		cfont->pCharList = list_create();
		FT_Error fterr = FT_New_Face(ft, font_path(style), 0, &cfont->ft);

		if(fterr) {
			log_write(LOG_ERROR, "Error initializing Freetype: %s\n", FT_Error_String(fterr));
			return NULL;
		}

		FT_Set_Pixel_Sizes(cfont->ft, 0 , pt);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &cfont->tex);
		glBindTexture(GL_TEXTURE_2D, cfont->tex);

		// Calculate atlas size
		int max_dim = (1 + (cfont->ft->size->metrics.height >> 6)) * ceilf(sqrtf(count_glyphs(cfont->ft)));
		int tex_width = 1;
		while(tex_width < max_dim) tex_width <<= 1;
		int tex_height = tex_width;

		cfont->atlas_width = tex_width;
		cfont->atlas_height = tex_height;

		log_write(LOG_INFO, "Creating texture atlas for a new font with size: %dx%d\n", tex_width, tex_height);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, tex_width, tex_height, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		FT_ULong c;
		FT_UInt gindex;

		c = FT_Get_First_Char(cfont->ft, &gindex);

		int count = 0;
		int x = 0;
		int y = 0;
		int max_row_h = 0;

		FT_GlyphSlot g = cfont->ft->glyph;

		while(gindex != 0) {
			FT_Error ftcerr = FT_Load_Char(cfont->ft, c, FT_LOAD_RENDER);

			if(ftcerr) {
				log_write(LOG_ERROR, "Error loading char (%c): %s\n", c, FT_Error_String(fterr));
				continue;
			}

			max_row_h = g->bitmap.rows > max_row_h ? g->bitmap.rows : max_row_h;

			if(x + g->bitmap.width + 1 >= tex_width) {
				y += max_row_h + 1;
				max_row_h = 0;
				x = 0;
			}
			else {
				x += g->bitmap.width + 1;
			}

			glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

			Glyph *glyph = malloc(sizeof(Glyph));
			glyph->advance = g->advance.x >> 6;
			glyph->bx = g->bitmap_left;
			glyph->by = g->bitmap_top;
			glyph->width = g->bitmap.width;
			glyph->height = g->bitmap.rows;
			glyph->tx = (float)x;// / tex_width;
			glyph->ty = (float)y;// / tex_height;
			glyph->code = c;

			list_push_back(cfont->pCharList, glyph, sizeof(Glyph));
			free(glyph);

			c = FT_Get_Next_Char(cfont->ft, c, &gindex);
			count++;
		}
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		list_push_back(pFontCache, cfont, sizeof(CachedFont));
		log_write(LOG_INFO, "Added font (%dpt, %d glyphs, %d style) to cache\n", cfont->pt, count, cfont->style);
		free(cfont);
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
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	glContext = SDL_GL_CreateContext(pWindow);

	if(!glContext) {
		log_write(LOG_ERROR, "Error creating renderer: %s\n", SDL_GetError());
		return 0;
	}

	FT_Error fterr = FT_Init_FreeType(&ft);
	if(fterr) {
		log_write(LOG_ERROR, "Error initializing Freetype: %s\n", FT_Error_String(fterr));
		return 0;
	}

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();

	if(glewError != GLEW_OK) {
		log_write(LOG_ERROR, "Error initializing GLEW: %s\n", glewGetErrorString(glewError));
		return 0;
	}

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(opengl_message_callback, 0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, width, height);

	// Set vsync
	SDL_GL_SetSwapInterval(1);

	glClearColor(0, 0, 0, 1);

	pFontCache = list_create_fn(free_font);
	pTextCache = list_create_fn(free_texture);

	glm_ortho(0, width, height, 0, -1, 1, projection);

	quadShader = shader_load_str(pQuadShaderVert, pQuadShaderFrag, NULL);
	shader_use(quadShader);
	shader_set_mat4(quadShader, "projection", projection);

	textShader = shader_load_str(pTextShaderVert, pTextShaderFrag, NULL);
	shader_use(textShader);
	shader_set_mat4(textShader, "projection", projection);

	{
		// Setup the quad VAO
		GLuint vbo;
		GLuint ebo;

		GLfloat vertices[] = {
			0, 1, // top left
			1, 1, // top right
			1, 0, // bottom right
			0, 0, // bottom left
		};

		GLuint indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		glBindVertexArray(quadVAO);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
	}

	{
		glGenVertexArrays(1, &textVAO);
		glGenBuffers(1, &textVBO);

		glBindVertexArray(textVAO);

		glBindBuffer(GL_ARRAY_BUFFER, textVBO);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	log_write(LOG_INFO, "Renderer initialized.\n");

	return 1;
}


void render_quit() {
	list_clear(pFontCache);
	FT_Done_FreeType(ft);
	SDL_GL_DeleteContext(glContext);
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
	shader_use(quadShader);
	shader_set_vec4(quadShader, "quadColor", r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

void render_rect(float x, float y, float width, float height, int filled) {
	shader_use(quadShader);

	mat4 model;
	glm_mat4_identity(model);
	vec3 pos;
	pos[0] = x;
	pos[1] = y;
	pos[2]= 0;
	glm_translate(model, pos);

	vec3 scale;
	scale[0] = width;
	scale[1] = height;
	scale[2] = 1;
	glm_scale(model, scale);

	shader_set_mat4(quadShader, "model", model);

	glBindVertexArray(quadVAO);

	if(filled)
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	else
		glDrawElements(GL_LINE_STRIP, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void render_line(float x1, float y1, float x2, float y2) {
	// TODO: do this
}

void render_text_color(int r, int g, int b, int a) {
	shader_use(textShader);
	glUniform4f(glGetUniformLocation(textShader, "textColor"), r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

void render_text(int pt, int style, const char *text, float x, float y) {
	CachedFont *cfont = search_font(pt, style);

	if(!cfont)
		return;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	shader_use(textShader);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(textVAO);
	glBindTexture(GL_TEXTURE_2D, cfont->tex);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);

	struct point {
		GLfloat x;
		GLfloat y;
		GLfloat tx;
		GLfloat ty;
	} coords[6 * strlen(text)];

	int n = 0;
	for(const char *c = text; *c; c++) {
		ListValue *current = cfont->pCharList->head;

		while(current) {
			Glyph *glyph = current->value;
			if(glyph->code == *c && glyph->width && glyph->height) {
				float ox = x + glyph->bx;
				float oy = y - (glyph->height - glyph->by);
				coords[n++] = (struct point){ox, oy, glyph->tx, glyph->ty};
				coords[n++] = (struct point){ox + glyph->width, oy, glyph->tx + glyph->width, glyph->ty};
				coords[n++] = (struct point){ox, oy + glyph->height, glyph->tx, glyph->ty + glyph->height};
				coords[n++] = (struct point){ox, oy + glyph->height, glyph->tx, glyph->ty + glyph->height};
				coords[n++] = (struct point){ox + glyph->width, oy + glyph->height, glyph->tx + glyph->width, glyph->ty + glyph->height};
				coords[n++] = (struct point){ox + glyph->width, oy, glyph->tx + glyph->width, glyph->ty};

				x += glyph->advance;
			}
			current = current->next;
		}
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_DYNAMIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, n);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

void render_clear_text_cache() {
	list_clear(pTextCache);
}

void render_text_size(const char* text, int pt, int style, int *w, int *h) {
}