#include <sys.h>

list_t *createList() {
    list_t *list = (list_t *)malloc(sizeof(list_t));
    list->length = 0;
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void listPush(list_t *list, void *data) {
    list_node_t *node = (list_node_t *)malloc(sizeof(list_node_t));
    node->data = data;
    node->next = NULL;
    node->prev = list->tail;

    if (list->tail) {
        list->tail->next = node;
    }

    list->tail = node;
    list->length++;
}

list_node_t *listPop(list_t *list) {
    if (list->length == 0) {
        return NULL;
    }

    list_node_t *node = list->tail;
    list->tail = node->prev;

    if (list->tail) {
        list->tail->next = NULL;
    }

    list->length--;
    return node;
}

list_node_t *listGetNode(list_t *list, size_t index) {
    if (index > list->length) {
        return NULL;
    }

    list_node_t *current = list->head;
    for (size_t i = 0; i < index && current != NULL; i++) {
        current = current->next;
    }

    return current;
}