#include <limine.h>
#include <arch/x86_64/mm.h>

pagemap_t *kernelPML4;

void initVMM(struct limine_memmap_response *memmapResponse) {
    kernelPML4 = (pagemap_t *)((uint64_t)pmAlloc(PAGE_SIZE) + hhdmOffset);

    for (uint64_t i = 0; i < 512; i++) {
        (*kernelPML4)[i] = 0; // Ensure that all entries are clear to start.
    }

    for (size_t i = 0; i < memmapResponse->entry_count; i++) {
        if (memmapResponse->entries[i]->type == LIMINE_MEMMAP_USABLE) {
            
        }
    }
}