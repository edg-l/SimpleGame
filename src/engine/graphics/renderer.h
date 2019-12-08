#ifndef RENDERER_H
#define RENDERER_H

#include <cglm/cglm.h>
#include <engine/util.h>

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

int engine_render_init(int width, int height, const char *title);
void engine_render_quit();

void engine_render_clear();
void engine_render_present();
void engine_render_color(int r, int g, int b, int a);
void engine_render_color_s(Color color);
void engine_render_rect(float x, float y, float width, float height, int filled);
void engine_render_rect_s(Rect *rect, int filled);
void engine_render_texture2D(float x, float y, float width, float height, unsigned int tex);
void engine_render_line(float x1, float y1, float x2, float y2);
void engine_render_line_s(Point p1, Point p2);
void engine_render_text_color(int r, int g, int b, int a);
void engine_render_text_color_s(Color color);
void engine_render_text_size(const char *text, unsigned int pt, int style, unsigned int *w, unsigned int *h);
void engine_render_text_size_s(const char *text, unsigned int pt, int style, Point *point);
void engine_render_text_size_len(const char *text, unsigned int pt, int style, Point *point, size_t len);
void engine_render_text(unsigned int pt, int style, const char *text, float x, float y);
void engine_render_text_s(unsigned int pt, int style, const char *text, Point point);
void engine_render_use_camera(int enable);
void engine_render_clear_color(Color c);
void engine_render_projection(mat4 proj);

#endif
