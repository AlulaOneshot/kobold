#include <khalt.h>

void khalt() {
    asm volatile("wfi");
}