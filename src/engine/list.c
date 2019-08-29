#include "list.h"
#include <stdlib.h>
#include <string.h>


static void list_freeelem(List *list, ListValue *elem) {
    if(elem != NULL) {
        list->freeFunc(elem->value);
        free(elem);
    }
}

List *list_create_fn(FREE_FN f) {
    List *list = (List*)malloc(sizeof(List));
    list->head = list->tail = NULL;
	list->freeFunc = f;
    return list;
}

List *list_create() {
	return list_create_fn(free);
}

void list_push_front(List *list, void *value, size_t size) {
    void* buf = malloc(size);
    memcpy(buf, value, size);

    ListValue *oldFirst = list->head;

    list->head = (ListValue*)malloc(sizeof(ListValue));

    if(oldFirst)
        oldFirst->prev = list->head;

    list->head->next = oldFirst;
    list->head->prev = NULL;

    list->head->value = buf;

    if(list->tail == NULL)
        list->tail = list->head;
}


void list_push_back(List *list, void *value, size_t size) {
    void *buf = malloc(size);
    memcpy(buf, value, size);

    ListValue *newVal = (ListValue*)malloc(sizeof(ListValue));

    newVal->next = NULL;
    newVal->prev = list->tail;
    newVal->value = buf;

    if(list->tail) {
        list->tail->next = newVal;
    }

    list->tail = newVal;

    if(list->head == NULL) {
        list->head = list->tail;
    }
}

int list_pop_front(List *list) {
    ListValue *oldHead = list->head;

    if(!oldHead)
        return 0;

    list->head = oldHead->next;

    if(list->head)
        list->head->prev = NULL;

    if(list->tail == oldHead)
        list->tail = list->head;

    list_freeelem(list, oldHead);
	return 1;
}

int list_pop_back(List *list) {
    ListValue *oldTail = list->tail;

    if(oldTail == NULL)
        return 0;

    list->tail = oldTail->prev;

    if(oldTail->prev)
        oldTail->prev->next = NULL;

    if(list->head == oldTail)
        list->head = list->tail;

    list_freeelem(list, oldTail);
	return 1;
}

int list_delete(List *list, int index) {
    int i = 0;

    ListValue *current = list->head;

    if(!current)
        return 0;

    while(current && i != index) {
        current = current->next;
        i++;
    }

    if(i == index && current) {
        if(current->prev) {
            current->prev->next = current->next;
            current->next->prev = current->prev;
            list_freeelem(list, current);
        }
        else if(current->next && list->head == current) {
            list->head = current->next;
            list_freeelem(list, current);
        }
        else {
            list->head = NULL;
            list->tail = NULL;
            list_freeelem(list, current);
        }

        return 1;
    }

    return 0;
}

void* list_get(List *list, int index) {
    int i = 0;

    ListValue *current = list->head;

    if(!current)
        return NULL;

    while(current && i != index) {
        current = current->next;
        i++;
    }

    if(i == index && current) {
        return current->value;
    }

    return NULL;
}

int list_empty(List *list) {
    return list->head == NULL;
}

int list_size(List *list) {
    int i = 0;

    ListValue *current = list->head;

    while(current) {
        current = current->next;
        i++;
    }

    return i;
}

void list_clear(List *list) {
    while(list->head) {
        ListValue *next = list->head->next;
        list_freeelem(list, list->head);
        list->head = next;
    }

    list->head = list->tail = NULL;
}


void list_free(List *list) {
    list_clear(list);
    free(list);
}
