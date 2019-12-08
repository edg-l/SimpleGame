#ifndef LIST_H
#define LIST_H

#include <stddef.h>

typedef void(*FREE_FN)(void*);

typedef struct ListValue {
    void *value;
	size_t size;
    struct ListValue *prev;
    struct ListValue *next;
} ListValue;

typedef struct List {
    ListValue *head;
    ListValue *tail;
	FREE_FN freeFunc;
} List;

List *list_create();

// Create a list with a custom free function for the element values.
List *list_create_fn(FREE_FN f);

void engine_list_push_front(List *list, void *value, size_t size);
void engine_list_push_back(List *list, void *value, size_t size);

int engine_list_pop_front(List *list);
int engine_list_pop_back(List *list);

// Returns 1 if a element was deleted
int engine_list_delete(List *list, int index);

void* engine_list_get(List *list, int index);

// Returns 1 if the list is empty
int engine_list_empty(List *list);

int engine_list_size(List *list);

// Clears all the elements.
void engine_list_clear(List *list);

void engine_list_free(List *list);

// TODO: Add swap, reverse, sort, merge and remove if val is x

#endif
