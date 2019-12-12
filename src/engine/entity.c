#include "entity.h"
#include <engine/list.h>
#include <engine/logger.h>
#include <engine/util.h>
#include <SDL_assert.h>
#include <SDL_events.h>

static List *entity_list;

static void entity_free(void *data) {
	if(!data)
		return;

	Entity *entity = (Entity *)data;

	if(!entity->on_free) {
		engine_log_warning("Entity %p has no free function, possible memory leak.", entity);
		return;
	}

	entity->on_free(entity);
}

static Entity *entity_to_check = NULL;
static int entity_equals(void *data) {
	return (Entity *)data == entity_to_check;
}

void engine_entity_init() {
	entity_list = engine_list_create_fn(entity_free);
}

void engine_entity_add(Entity *entity) {
	SDL_assert(entity_list);

	// Add sorted

	unsigned int i = 0;
	Node *node = entity_list->head;

	if(!node) {
		engine_list_push_front(entity_list, entity, sizeof(Entity));
		return;
	}

	while(node) {
		Entity *current_entity = (Entity *)node->value;

		if(current_entity->render_priority > entity->render_priority) {
			engine_list_insert(entity_list, entity, sizeof(Entity), i);
			return;
		}

		node = node->next;
		i++;
	}

	engine_list_push_back(entity_list, entity, sizeof(Entity));
}

void engine_entity_remove(Entity *entity) {
	SDL_assert(entity);
	entity_to_check = entity;
	engine_list_remove_if(entity_list, entity_equals);
	entity_to_check = NULL;
}

void engine_entity_onupdate() {
	double delta = engine_util_delta_time();

	Node *node = entity_list->head;

	while(node) {
		Entity *entity = (Entity *)node->value;
		entity->on_update(entity, delta);
		node = node->next;
	}
}

void engine_entity_onrender() {
	double delta = engine_util_delta_time();

	Node *node = entity_list->head;

	while(node) {
		Entity *entity = (Entity *)node->value;
		entity->on_render(entity, delta);
		node = node->next;
	}
}

void engine_entity_onevent(union SDL_Event *event) {
	Node *node = entity_list->head;

	while(node) {
		Entity *entity = (Entity *)node->value;
		if(event->type == SDL_KEYDOWN) {
			entity->on_keydown(entity, event->key.keysym.scancode,
					event->key.keysym.mod);
		}
		else if(event->type == SDL_MOUSEBUTTONUP) {
			entity->on_mouse_click(entity, event->button.button,
					event->button.x, event->button.y);
		}
		node = node->next;
	}
}
