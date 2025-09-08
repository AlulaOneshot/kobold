#include <kmain.h>
#include <limine.h>
#include <arch/x86_64/cpu.h>
#include <memory.h>
#include <cpu.h>
#include <arch/x86_64/printf.h>

__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests")))
static volatile struct limine_smp_request smp_request = {
    .id = LIMINE_SMP_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_rsdp_request rsdp_request = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_kernel_address_request kernel_address_request = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;

/// @brief Physical Address of the RSDP
uint64_t rsdpAddress;
cpu_t **cpus;

void _start() {
    initialiseCPU();
    
    hhdmOffset = hhdm_request.response->offset;
    rsdpAddress = (uint64_t)rsdp_request.response->address;

    uint64_t highestID = 0;
    cpuCount = 0;
    for (size_t i = 0; i < smp_request.response->cpu_count; i++) {
        if (smp_request.response->cpus[i]->processor_id > highestID) {
            highestID = smp_request.response->cpus[i]->processor_id;
        }
        cpuCount++;
    }
    if (cpuCount - 1 != highestID) {
        printf("ERROR: Highest CPU Sequential ID to CPU Count Mismatch\n");
        asm volatile ("cli");
        while (1) {
            asm volatile ("hlt");
        }
    }

    initialisePMM(memmap_request.response);
    initialiseVMM(memmap_request.response, kernel_address_request.response);
    initialiseAllocator();
    initialiseFPU();

    cpus = (cpu_t**)malloc(sizeof(cpu_t*) * cpuCount);
    for (size_t i = 0; i < cpuCount; i++) {
        cpus[i] = (cpu_t*)malloc(sizeof(cpu_t));
        cpus[i]->parent = (x86_64_cpu_t *)malloc(sizeof(x86_64_cpu_t));
        initialiseLocalCPU((x86_64_cpu_t*)cpus[i]->parent, i);
    }

    printf("CPU Count: %u\n", cpuCount);

    printf("Initialising Deffered Work System\n");
    initialiseDefferedWork();
    printf("Deffered work initialised\n");

    kmain();
}