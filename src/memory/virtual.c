#include <memory.h>
#include <string.h>
#if ARCH_X86_64 == 1
    #include <arch/x86_64/cpu.h>
    #include <arch/x86_64/printf.h>
#endif

#if ARCH_X86_64 == 1
    pagemap_t *kernelPagemap;
#endif

void initialiseVMM(struct limine_memmap_response *memmap, struct limine_kernel_address_response *kernelAddressResponse) {
    #if ARCH_X86_64 == 1
        printf("Initialising VMM\n");

        kernelPagemap = (pagemap_t *)(pmmAlloc(1) + hhdmOffset);

        memset(kernelPagemap, 0, sizeof(pagemap_t));

        initialiseMMU(kernelPagemap, memmap, kernelAddressResponse);
        printf("VMM Initialised\n");
    #endif
}

void *vmGetSpace(uint64_t physicalAddress, size_t pageCount) {
    #if ARCH_X86_64 == 1
        void *virtualAddress = findFreeRange(kernelPagemap, pageCount);
        mapRange(kernelPagemap, virtualAddress, physicalAddress, pageCount, PAGEMAP_PRESENT | PAGEMAP_WRITABLE | PAGEMAP_NO_EXECUTE);
        return virtualAddress;
    #endif
}