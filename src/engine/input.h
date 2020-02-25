#ifndef ENGINE_INPUT_H
#define ENGINE_INPUT_H

typedef enum MouseButton {
	BUTTON_LEFT = 1,
	BUTTON_MIDDLE,
	BUTTON_RIGHT,
	BUTTON_X1,
	BUTTON_X2
} MouseButton;

void engine_input_init();
void engine_input_update();

void engine_input_mouse_pos(int *x, int *y);
void engine_input_mouse_delta(int *x, int *y);
int engine_input_mouse_click(MouseButton button);
int engine_input_keyup(int code);
int engine_input_keypress(int code);

#endif
