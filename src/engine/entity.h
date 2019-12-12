#ifndef ENGINE_ENTITY_H
#define ENGINE_ENTITY_H

struct Entity;

typedef void (*ENTITY_RENDER_FN)(struct Entity *entity, double delta);
typedef void (*ENTITY_UPDATE_FN)(struct Entity *entity, double deltaTime);
typedef void (*ENTITY_EVENT_MOUSE_CLICK_FN)(struct Entity *entity, unsigned char button, 
		int x, int y);
typedef void (*ENTITY_EVENT_KEYDOWN_FN)(struct Entity *entity, int keycode, unsigned short mod);
typedef void (*ENTITY_FREE_FN)(struct Entity *entity);

// TODO: Add more events 

typedef struct Entity {
	unsigned int render_priority; // less means later, which means will be on top.
	ENTITY_UPDATE_FN on_update;
	ENTITY_RENDER_FN on_render;
	ENTITY_EVENT_MOUSE_CLICK_FN on_mouse_click;
	ENTITY_EVENT_KEYDOWN_FN on_keydown;
	ENTITY_FREE_FN on_free;
} Entity;

// Initializes the entity engine.
void engine_entity_init();

// Note: Once added, changing the render priority in the entity has no effect.
void engine_entity_add(Entity *entity);

// Removes and FREES the entity.
void engine_entity_remove(Entity *entity);

// Used internally
void engine_entity_onupdate();

// Used internally
void engine_entity_onrender();

union SDL_Event;

// Used internally
void engine_entity_onevent(union SDL_Event *event);

#endif
