#include "list.h"
#include <stdlib.h>
#include <string.h>

static void engine_list_free_node(List *list, Node *elem) {
	if (elem != NULL) {
		list->freeFunc(elem->value);
		free(elem);
	}
}

static Node *list_get_node(List *list, unsigned int index) {
	int i = 0;

	Node *current = list->head;

	if (!current)
		return NULL;

	while (current && i != index) {
		current = current->next;
		i++;
	}

	if (i == index && current) {
		return current;
	}

	return NULL;

}

List *engine_list_create_fn(LIST_FREE_FN f) {
	List *list = (List *)malloc(sizeof(List));
	list->head = list->tail = NULL;
	list->freeFunc = f;
	return list;
}

List *engine_list_create() { return engine_list_create_fn(free); }

void engine_list_push_front(List *list, void *value, size_t size) {

	Node *oldFirst = list->head;

	list->head = (Node *)malloc(sizeof(Node));

	if (oldFirst)
		oldFirst->prev = list->head;

	list->head->next = oldFirst;
	list->head->prev = NULL;
	list->head->value = value;
	list->head->size = size;

	if (list->tail == NULL)
		list->tail = list->head;
}

void engine_list_push_back(List *list, void *value, size_t size) {
	Node *newVal = (Node *)malloc(sizeof(Node));

	newVal->next = NULL;
	newVal->prev = list->tail;
	newVal->value = value;
	newVal->size = size;

	if (list->tail) {
		list->tail->next = newVal;
	}

	list->tail = newVal;

	if (list->head == NULL) {
		list->head = list->tail;
	}
}

int engine_list_pop_front(List *list) {
	Node *oldHead = list->head;

	if (!oldHead)
		return 0;

	list->head = oldHead->next;

	if (list->head)
		list->head->prev = NULL;

	if (list->tail == oldHead)
		list->tail = list->head;

	engine_list_free_node(list, oldHead);
	return 1;
}

int engine_list_pop_back(List *list) {
	Node *oldTail = list->tail;

	if (oldTail == NULL)
		return 0;

	list->tail = oldTail->prev;

	if (oldTail->prev)
		oldTail->prev->next = NULL;

	if (list->head == oldTail)
		list->head = list->tail;

	engine_list_free_node(list, oldTail);
	return 1;
}

int engine_list_insert(List *list, void *value, size_t size, unsigned int index) {

	int list_size = engine_list_size(list);

	if(index > list_size)
		return 0;

	if(index == list_size) {
		engine_list_push_back(list, value, size);
		return 1;
	}
	else if(index == 0) {
		engine_list_push_front(list, value, size);
		return 1;
	}

	int i = 0;

	Node *current = list->head;

	while (current && i != index) {
		current = current->next;
		i++;
	}

	// Already checked if it was on the head or tail.
	if (i == index && current) {
		Node *newVal = (Node *)malloc(sizeof(Node));

		newVal->prev = current;
		newVal->next = current->next;
		newVal->value = value;
		newVal->size = size;

		current->next = newVal;
		return 1;
	}

	return 0;
}

int engine_list_delete(List *list, int index) {
	int i = 0;

	Node *current = list->head;

	if (!current)
		return 0;

	while (current && i != index) {
		current = current->next;
		i++;
	}

	if (i == index && current) {
		if(!current->prev && !current->next) {
			list->head = NULL;
			list->tail = NULL;
		}
		else if(!current->prev) {
			list->head = current->next;
			list->head->prev = NULL;
		}
		else if(!current->next) {
			list->tail = current->prev;
			list->tail->next = NULL;
		}
		else {
			current->prev->next = current->next;
			current->next->prev = current->prev;
		}

		engine_list_free_node(list, current);

		return 1;
	}

	return 0;
}

void *engine_list_get(List *list, int index) {
	int i = 0;

	Node *current = list->head;

	if (!current)
		return NULL;

	while (current && i != index) {
		current = current->next;
		i++;
	}

	if (i == index && current) {
		return current->value;
	}

	return NULL;
}

int engine_list_index(List *list, void *value) {
	int i = 0;

	Node *current = list->head;

	if (!current)
		return -1;

	while (current) {
		if(current->value == value)
			return i;
		current = current->next;
		i++;
	}

	return -1;

}

int engine_list_empty(List *list) { return list->head == NULL; }

int engine_list_size(List *list) {
	int i = 0;

	Node *current = list->head;

	while (current) {
		current = current->next;
		i++;
	}

	return i;
}

void engine_list_clear(List *list) {
	while (list->head) {
		Node *next = list->head->next;
		engine_list_free_node(list, list->head);
		list->head = next;
	}

	list->head = list->tail = NULL;
}

void engine_list_free(List *list) {
	engine_list_clear(list);
	free(list);
}

void engine_list_swap_index(List *list, unsigned index1, unsigned index2) {
	int size = engine_list_size(list);

	if(index1 >= size || index2 >= size || index1 == index2) {
		return;
	}

	Node *first;
	Node *second;
	Node *prev, *next;

	first = list_get_node(list, index1);
	second = list_get_node(list, index2);

	prev = first->prev;
	next = first->next;

	first->prev = second->prev;
	first->next = second->next;

	second->prev = prev;
	second->next = next;
}

int engine_list_remove_if(List *list, LIST_REMOVE_FN check) {
	if(!check)
		return 0;

	Node *current = list->head;
	Node *tofree;
	int count = 0;

	while(current) {
		if(check(current->value)) {
			count++;

			tofree = current;

			if(!current->prev && !current->next) {
				list->head = NULL;
				list->tail = NULL;
			}
			else if(!current->prev) {
				list->head = current->next;
				list->head->prev = NULL;
			}
			else if(!current->next) {
				list->tail = current->prev;
				list->tail->next = NULL;
			}
			else {
				current->prev->next = current->next;
				current->next->prev = current->prev;
			}

			current = current->next;
			engine_list_free_node(list, tofree);

			tofree = NULL;
		}
	}

	return count;
}
