#include <kmain.h>
#include <limine.h>
#include <string.h>
#include <stdbool.h>
#include <arch/x86_64/cpu.h>
#include <arch/x86_64/serial.h>
#include <arch/x86_64/printf.h>
#include <arch/x86_64/mm.h>

__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_kernel_address_request kernel_address_request = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_rsdp_request rsdp_request = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;

void _start() {
    initSerial();
    printf("Serial Initialized\n");
    if (LIMINE_BASE_REVISION_SUPPORTED) {
        printf("Limine Base Protocol Supported\n");
    } else {
        printf("Limine Base Protocol Not Supported\n");
        while (1);
    }

    printf("Got Higher Half Direct Map of %p\n", hhdm_request.response->offset);

    printf("Initializing GDT\n");
    initGDT();
    printf("GDT Initialized\n");
    printf("Initializing IDT\n");
    initIDT();
    printf("IDT Initialized\n");
    printf("Initializing PMM\n");
    initPMM(memmap_request.response, hhdm_request.response->offset);
    printf("PMM Initialized\n");
    printf("Initializing VMM\n");
    initVMM(memmap_request.response, kernel_address_request.response);
    printf("VMM Initialized\n");
    printf("Initializing ACPI\n");
    initACPI((uint64_t)rsdp_request.response->address);
    printf("ACPI Initialized\n");

    kmain();
}