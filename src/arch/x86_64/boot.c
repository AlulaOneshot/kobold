#include <kmain.h>
#include <limine.h>
#include <string.h>
#include <stdbool.h>
#include <arch/x86_64/cpu.h>

__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;

void _start() {
    initGDT();

    kmain();
}