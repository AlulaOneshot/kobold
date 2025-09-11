#include <kmain.h>
#include <limine.h>
#include <arch/x86_64/cpu.h>
#include <arch/x86_64/dev.h>
#include <memory.h>
#include <cpu.h>
#include <arch/x86_64/printf.h>
#include <filesystem.h>

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

extern void setupNewStack(void *stackStart) __attribute__((noreturn));

void _start() {
    initialiseCPU();
    
    hhdmOffset = hhdm_request.response->offset;
    rsdpAddress = (uint64_t)rsdp_request.response->address;

    initialisePMM(memmap_request.response);
    initialiseVMM(memmap_request.response, kernel_address_request.response);
    initialiseAllocator();

    void *stack = (void*)((((uint64_t)vmGetSpace(pmmAlloc(256), 256) + (256 * PAGE_SIZE)) & ~((uintptr_t)0xF))-8);

    memset(stack, 0, 256 * PAGE_SIZE);

    printf("Jumping to new stack at %p\n", stack);

    setupNewStack(stack);
}

void initStage2() {
    initialiseFPU();

    printf("Initialising Deffered Work System\n");
    initialiseDefferedWork();
    printf("Deffered work initialised\n");

    printf("Initialising PCI\n");
    initialisePCI();
    printf("PCI initialised\n");

    printf("Initialising ACPI\n");
    initialiseACPI(rsdpAddress);
    printf("ACPI initialised\n");

    printf("Initialising VFS\n");
    initialiseVFS();
    printf("VFS initialised\n");

    vfs_node_t *rootNode = resolvePathVFS("/");

    for (size_t i = 0; i < rootNode->children->length; i++) {
        vfs_node_t *child = (vfs_node_t *)listGetNode(rootNode->children, i)->data;
        if (child) {
            printf("Child %zu: %s\n", i, child->name);
            printf("Type: %s\n", child->type == VFS_NODE_TYPE_DIRECTORY ? "Directory" : "File");
            printf("Permissions: 0x%X\n", child->permissions);
            printf("Filesystem: %s\n", getFSString(child->filesystem));
        }
    }

    printf("Jumping to kernel main function\n");

    kmain();
}