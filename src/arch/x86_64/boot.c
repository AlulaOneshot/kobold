#include <kmain.h>
#include <khalt.h>
#include <limine.h>
#include <string.h>
#include <stdbool.h>
#include <arch/x86_64/drivers/serial.h>
#include <arch/x86_64/panic.h>
#include <cpu.h>

__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;

void _start() {
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        panic();
    }

    initSerial();
    initIDT();
    serialDebugLn("[Kobold/Boot] Booting...");

    panic();
}