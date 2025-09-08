#include <arch/x86_64/cpu.h>
#include <arch/x86_64/printf.h>

void initialiseCPU() {
    printf("Initialising GDT\n");
    initialiseGDT();
    printf("GDT Initialised\n");
    printf("Initialising IDT\n");
    initialiseIDT();
    printf("IDT Initialised\n");
}