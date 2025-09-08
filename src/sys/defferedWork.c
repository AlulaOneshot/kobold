#include <sys.h>
#include <cpu.h>

list_t *defferedWorkItems;

typedef struct {
    void (*workItem)(void*);
    void *arg;
} work_item_t;

void queueDefferedWork(void (*workItem)(void*), void *arg) {
    work_item_t *item = (work_item_t *)malloc(sizeof(work_item_t));
    item->workItem = workItem;
    item->arg = arg;

    listPush(defferedWorkItems, item);
}

void runDefferedWork() {
    disableInterrupts();

    list_t *workList = defferedWorkItems;
    while (workList->length > 0) {
        list_node_t *node = listPop(workList);
        if (node) {
            work_item_t *item = (work_item_t *)node->data;
            if (item && item->workItem) {
                item->workItem(item->arg);
            }
            free(item);
            free(node);
        }
    }

    enableInterrupts();
}

void initialiseDefferedWork() {
    defferedWorkItems = createList();
}