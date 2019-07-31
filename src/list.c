#include "list.h"
#include <stdlib.h>
#include <string.h>

static void strlist_freeelem(StrListValue *elem) {
    if(elem != NULL) {
        free(elem->value);
        free(elem);
    }
}

StrList *strlist_create() {
    StrList *list = (StrList*)malloc(sizeof(StrList));
    list->head = list->tail = NULL;
    return list;
}

void strlist_push_front(StrList *list, char *value, size_t size) {
    char* buf = (char*)malloc(size);
    memcpy(buf, value, size);

    StrListValue *oldFirst = list->head;

    list->head = (StrListValue*)malloc(sizeof(StrListValue));

    if(oldFirst) 
        oldFirst->prev = list->head;

    list->head->next = oldFirst;
    list->head->prev = NULL;

    list->head->value = buf;

    if(list->tail == NULL)
        list->tail = list->head;
}


void strlist_push_back(StrList *list, char *value, size_t size) {
    char* buf = (char*)malloc(size);
    memcpy(buf, value, size);

    StrListValue *newVal = (StrListValue*)malloc(sizeof(StrListValue));

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

int strlist_pop_front(StrList *list) {
    StrListValue *oldHead = list->head;

    if(!oldHead)
        return 0;
    
    list->head = oldHead->next;

    if(list->head)
        list->head->prev = NULL;

    if(list->tail == oldHead)
        list->tail = list->head;

    strlist_freeelem(oldHead);
}

int strlist_pop_back(StrList *list) {
    StrListValue *oldTail = list->tail;

    if(oldTail == NULL)
        return 0;

    list->tail = oldTail->prev;

    if(oldTail->prev)
        oldTail->prev->next = NULL;

    if(list->head == oldTail)
        list->head = list->tail;

    strlist_freeelem(oldTail);
}

int strlist_delete(StrList *list, int index) {
    int i = 0;

    StrListValue *current = list->head;

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
            strlist_freeelem(current);
        } 
        else if(current->next && list->head == current) {
            list->head = current->next;
            strlist_freeelem(current);
        }
        else {
            list->head = NULL;
            list->tail = NULL;
            strlist_freeelem(current);
        }

        return 1;
    }

    return 0;
}

char* strlist_get(StrList *list, int index) {
    int i = 0;

    StrListValue *current = list->head;

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

int strlist_empty(StrList *list) {
    return list->head == NULL;
}

int strlist_size(StrList *list) {
    int i = 0;

    StrListValue *current = list->head;
    
    while(current) {
        current = current->next;
        i++;
    }

    return i;
}

void strlist_clear(StrList *list) {
    while(list->head) {
        StrListValue *next = list->head->next;
        strlist_freeelem(list->head);
        list->head = next;
    }

    list->head = list->tail = NULL;
}

void strlist_free(StrList *list) {
    strlist_clear(list);
    free(list);
}