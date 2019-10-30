#include "button.h"
#include <engine/graphics/renderer.h>
#include <engine/util.h>
#include <stdlib.h>
#include <string.h>

Button *button_create(int w, int h, int pt, int style, const char *text,
                      Color fg, Color bg) {
    Button *button;

    button = malloc(sizeof(Button));
    memset(button, 0, sizeof(Button));

    button->rect = util_rect(0, 0, w, h);

    button->fg = fg;
    button->bg = bg;
    button->textpt = pt;
    button->textStyle = style;

    int len = strlen(text) + 1;
    button->pText = malloc(len);
    memcpy(button->pText, text, len);

    render_text_size(button->pText, button->textpt, STYLE_REGULAR,
                     &button->textSizeW, &button->textSizeH);

    return button;
}

void button_free(Button *button) {
    free(button->pText);
    free(button);
}

void render_button(Button *button) {
    Point mouse = util_mouse_pos();

    if (util_point_in_rect(&button->rect, &mouse))
        render_color(button->bg.r + 20, button->bg.g + 20, button->bg.b + 20,
                     button->bg.a);
    else
        render_color(button->bg.r, button->bg.g, button->bg.b, button->bg.a);
    render_rect_s(&button->rect, 1);

    render_text_color(button->fg.r, button->fg.g, button->fg.b, button->fg.a);
    render_text(
        button->textpt, button->textStyle, button->pText,
        (int)(button->rect.x + (button->rect.w - button->textSizeW) / 2.f),
        (int)(button->rect.y + (button->rect.h - button->textSizeH) / 2.f));
}

int button_is_pressed(Button *button) {
    Point mouse = util_mouse_pos();
    return util_point_in_rect(&button->rect, &mouse) &&
           util_is_mouse_click(BUTTON_LEFT);
}
