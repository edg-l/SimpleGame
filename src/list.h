#ifndef LIST_H
#define LIST_H

#include <stddef.h>

typedef struct StrListValue {
    char *value;
    struct StrListValue *prev;
    struct StrListValue *next;
} StrListValue;

typedef struct StrList {
    StrListValue *head;
    StrListValue *tail;
} StrList;

StrList *strlist_create();

void strlist_push_front(StrList *list, char *value, size_t size);
void strlist_push_back(StrList *list, char *value, size_t size);

int strlist_pop_front(StrList *list);
int strlist_pop_back(StrList *list);

// Returns 1 if a element was deleted
int strlist_delete(StrList *list, int index);

char* strlist_get(StrList *list, int index);

// Returns 1 if the list is empty
int strlist_empty(StrList *list);

int strlist_size(StrList *list);

// Clears all the elements.
void strlist_clear(StrList *list);

void strlist_free(StrList *list);

// TODO: Add swap, reverse, sort, merge and remove if val is x

#endif