#include <sys.h>
#include <cpu.h>

typedef struct {
    void (*workItem)(void*);
    void *arg;
} work_item_t;

void queueDefferedWork(void (*workItem)(void*), void *arg) {
    cpu_t *currentCPU = currentCPU;
    if (!currentCPU) {
        return;
    }

    work_item_t *item = (work_item_t *)malloc(sizeof(work_item_t));
    item->workItem = workItem;
    item->arg = arg;

    listPush(currentCPU->defferedWorkItems, item);
}

void runDefferedWork() {
    cpu_t *currentCPU = currentCPU;
    if (!currentCPU) {
        return;
    }

    disableInterrupts();

    list_t *workList = currentCPU->defferedWorkItems;
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
    for (size_t i = 0; i < cpuCount; i++) {
        cpus[i]->defferedWorkItems = createList();
    }
}