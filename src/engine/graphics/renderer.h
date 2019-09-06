#ifndef RENDERER_H
#define RENDERER_H

#include <engine/util.h>
#include <cglm/cglm.h>

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


int render_init(int width, int height, const char *title);
void render_quit();

void render_clear();
void render_present();
void render_color(int r, int g, int b, int a);
void render_color_s(Color color);
void render_rect(float x, float y, float width, float height, int filled);
void render_rect_s(Rect *rect, int filled);
void render_texture2D(float x, float y, float width, float height, unsigned int tex);
void render_line(float x1, float y1, float x2, float y2);
void render_line_s(Point p1, Point p2);
void render_text_color(int r, int g, int b, int a);
void render_text_color_s(Color color);
void render_text_size(const char* text, int pt, int style, int *w, int *h);
void render_text_size_s(const char* text, int pt, int style, Point *point);
void render_text_size_len(const char* text, int pt, int style, Point *point, size_t len);
void render_text(int pt, int style, const char *text, float x, float y);
void render_text_s(int pt, int style, const char *text, Point point);
void render_projection(mat4 m);
void render_use_camera(int enable);
void render_clear_color(Color c);

#endif
