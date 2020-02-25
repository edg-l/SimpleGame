#include "input.h"
#include <SDL.h>

static Uint32 prev_mouse_state;
static Uint32 mouse_state;
static Uint8 *prev_keyboard_status = NULL;
static const Uint8 *keyboard_status = NULL;
static int keyboard_status_length = 0;
static int oldMX = 0, oldMY = 0;
static int currentMX = 0, currentMY = 0;

void engine_input_init() {
	keyboard_status = SDL_GetKeyboardState(&keyboard_status_length);
	prev_keyboard_status = malloc(keyboard_status_length);
	engine_input_update();
}

void engine_input_update() {
	prev_mouse_state = mouse_state;
	oldMX = currentMX;
	oldMY = currentMY;
	mouse_state = SDL_GetMouseState(&currentMX, &currentMY);

	if (keyboard_status) {
		memcpy(prev_keyboard_status, keyboard_status, keyboard_status_length);
	}
}

void engine_input_mouse_pos(int *x, int *y) {
	*x = currentMX;
	*y = currentMY;
}

void engine_input_mouse_delta(int *x, int *y) {
	*x = currentMX - oldMX;
	*y = currentMY - oldMY;
}
int engine_input_mouse_click(MouseButton button) {
	return (prev_mouse_state & SDL_BUTTON(button)) &&
		   !(mouse_state & SDL_BUTTON(button));
}

int engine_input_keyup(int code) {
	if (!prev_keyboard_status)
		return 0;
	return prev_keyboard_status[code] && !keyboard_status[code];
}

int engine_input_keypress(int code) {
	return keyboard_status[code];
}
