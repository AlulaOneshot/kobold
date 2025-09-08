#pragma once

#include <stddef.h>
#include <sys.h>

extern size_t cpuCount;
extern size_t currentCPU;

typedef struct cpu {
    //scheduler_t scheduler;

    list_t *defferedWorkItems;

    void *parent;
} cpu_t;

extern cpu_t **cpus;

void disableInterrupts();
void enableInterrupts();