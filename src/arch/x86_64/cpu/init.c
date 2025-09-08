#include <arch/x86_64/cpu.h>
#include <arch/x86_64/printf.h>

size_t cpuCount;
size_t currentCPU = 0;

void initialiseLocalCPU(x86_64_cpu_t *cpu, size_t id) {
    cpu->self = cpu;

    cpu->lapicTimerFrequency = 0;
    cpu->tscTimerFrequency = 0;

    cpu->lapicID = 0;
    cpu->id = id;
    
    cpu->common.parent = (void *)cpu;
    cpu->common.defferedWorkItems = NULL;
}

/// @brief Initialises the Whole CPU, whereas initialiseLocalCPU initialises the core it is running on.
void initialiseCPU() {
    printf("Initialising GDT\n");
    initialiseGDT();
    printf("GDT Initialised\n");
    printf("Initialising IDT\n");
    initialiseIDT();
    printf("IDT Initialised\n");
}