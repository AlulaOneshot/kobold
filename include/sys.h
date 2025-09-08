#pragma once

#include <memory.h>

void queueDefferedWork(void (*workItem)(void*), void *arg);
void runDefferedWork();
void initialiseDefferedWork();

typedef struct list_node {
    void *data;
    struct list_node *next;
    struct list_node *prev;
} list_node_t;

typedef struct {
    size_t length;
    list_node_t *head;
    list_node_t *tail;
} list_t;

list_t *createList();
void listPush(list_t *list, void *data);
list_node_t *listPop(list_t *list);
list_node_t *listGetNode(list_t *list, size_t index);