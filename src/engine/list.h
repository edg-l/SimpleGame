#ifndef LIST_H
#define LIST_H

#include <stddef.h>

typedef void(*LIST_FREE_FN)(void*);
typedef int(*LIST_SORT_FN)(void *a, void *b);
typedef int(*LIST_REMOVE_FN)(void *a);

typedef struct Node {
	void *value;
	size_t size;
	struct Node *prev;
	struct Node *next;
} Node;

typedef struct List {
	Node *head;
	Node *tail;
	LIST_FREE_FN freeFunc;
} List;

List *engine_list_create();

// Create a list with a custom free function for the element values.
List *engine_list_create_fn(LIST_FREE_FN f);

void engine_list_push_front(List *list, void *value, size_t size);
void engine_list_push_back(List *list, void *value, size_t size);

// Returns 0 if value was not inserted.
int engine_list_insert(List *list, void *value, size_t size, unsigned int index);

int engine_list_pop_front(List *list);
int engine_list_pop_back(List *list);

// Returns 1 if a element was deleted
int engine_list_delete(List *list, int index);

void* engine_list_get(List *list, int index);

int engine_list_index(List *list, void *value);

// Returns 1 if the list is empty
int engine_list_empty(List *list);

int engine_list_size(List *list);

// Clears all the elements.
void engine_list_clear(List *list);

void engine_list_free(List *list);

// TODO: reverse, sort, merge

void engine_list_swap_index(List *list, unsigned index1, unsigned index2);

// Removes items where the remove function returns true. Returns the count of removed items.
int engine_list_remove_if(List *list, LIST_REMOVE_FN check);

#define engine_list_for(list_name, node) for(Node *node = list_name->head; node; node = node->next)

#endif
