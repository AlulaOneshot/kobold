/*
    The Kobold Kernel
    vmm.c - The virtual memory manager
*/

#include <arch/x86_64/mm.h>

pagemap_t *kernelPML4; // The kernel's PML4

/// @brief Gets or allocates a page table
/// @param parent The parent page table
/// @param index The index in the parent table
/// @param flags The flags to set on the new table
/// @return The page table, or NULL on failure
static pagemap_t *getOrAllocateTable(pagemap_t *parent, uint64_t index, uint8_t flags) {
    if (!((uint64_t)((*parent)[index]) & PAGE_PRESENT)) {
        void *page = pmAlloc(PAGE_SIZE);
        if (!page) return NULL;
        memset((void *)(hhdmOffset + (uint64_t)page), 0, PAGE_SIZE);
        (*parent)[index] = (((uint64_t)page & 0x000FFFFFFFFFF000) | PAGE_PRESENT | PAGE_WRITABLE | (flags & 0xFFF));
    } else {
        // Only add safe flags
        (*parent)[index] |= (flags & 0xFFF);
    }
    return (pagemap_t *)(hhdmOffset + ((*parent)[index] & 0x000FFFFFFFFFF000));
}


bool isPageMapped(uint64_t vaddr) {
    if (!kernelPML4) return false;
    
    uint64_t pml4Index = (vaddr >> 39) & 0x1FF;
    uint64_t pdptIndex = (vaddr >> 30) & 0x1FF;
    uint64_t pdIndex   = (vaddr >> 21) & 0x1FF;
    uint64_t ptIndex   = (vaddr >> 12) & 0x1FF;

    // Check PML4 entry
    if (!((*kernelPML4)[pml4Index] & PAGE_PRESENT)) return false;
    
    // Check PDPT entry
    pagemap_t *pml3 = (pagemap_t *)(hhdmOffset + ((*kernelPML4)[pml4Index] & 0x000FFFFFFFFFF000));
    if (!((*pml3)[pdptIndex] & PAGE_PRESENT)) return false;
    
    // Check PD entry
    pagemap_t *pml2 = (pagemap_t *)(hhdmOffset + ((*pml3)[pdptIndex] & 0x000FFFFFFFFFF000));
    if (!((*pml2)[pdIndex] & PAGE_PRESENT)) return false;
    
    // Check PT entry
    pagemap_t *pml1 = (pagemap_t *)(hhdmOffset + ((*pml2)[pdIndex] & 0x000FFFFFFFFFF000));
    return ((*pml1)[ptIndex] & PAGE_PRESENT) != 0;
}


bool vmMapPage(pagemap_t *pagemap, uint64_t vaddr, uint64_t paddr, uint64_t flags) {
    uint64_t pml4Index = (vaddr >> 39) & 0x1FF;
    uint64_t pdptIndex = (vaddr >> 30) & 0x1FF;
    uint64_t pdIndex   = (vaddr >> 21) & 0x1FF;
    uint64_t ptIndex   = (vaddr >> 12) & 0x1FF;

    pagemap_t *pml3 = getOrAllocateTable(pagemap, pml4Index, flags);
    if (!pml3) return false;
    
    pagemap_t *pml2 = getOrAllocateTable(pml3, pdptIndex, flags);
    if (!pml2) return false;
    
    pagemap_t *pml1 = getOrAllocateTable(pml2, pdIndex, flags);
    if (!pml1) return false;
    
    if ((*pml1)[ptIndex] & PAGE_PRESENT) {
        // Page already mapped
        return false;
    }


    (*pml1)[ptIndex] = (paddr & 0x000FFFFFFFFFF000) | flags | PAGE_PRESENT;
    return true;  
}

void vmUnmapPage(pagemap_t *pagemap, uint64_t vaddr) {
    uint64_t pml4Index = (vaddr >> 39) & 0x1FF;
    uint64_t pdptIndex = (vaddr >> 30) & 0x1FF;
    uint64_t pdIndex   = (vaddr >> 21) & 0x1FF;
    uint64_t ptIndex   = (vaddr >> 12) & 0x1FF;

    pagemap_t *pml3 = (pagemap_t *)(hhdmOffset + ((*pagemap)[pml4Index] & 0x000FFFFFFFFFF000));
    if (!pml3) return;
    
    pagemap_t *pml2 = (pagemap_t *)(hhdmOffset + ((*pml3)[pdptIndex] & 0x000FFFFFFFFFF000));
    if (!pml2) return;
    
    pagemap_t *pml1 = (pagemap_t *)(hhdmOffset + ((*pml2)[pdIndex] & 0x000FFFFFFFFFF000));
    if (!pml1) return;

    (*pml1)[ptIndex] = 0;
    asm volatile ("invlpg (%0)" :: "r"(vaddr) : "memory");
}

void initVMM(struct limine_memmap_response *memmapResponse, struct limine_kernel_address_response *kernelAddressResponse) {
    kernelPML4 = (pagemap_t *)((uint64_t)pmAlloc(sizeof(pagemap_t)) + hhdmOffset);
    memset(kernelPML4, 0, sizeof(pagemap_t));

    struct limine_memmap_entry **memmap = memmapResponse->entries;
    uint64_t memmap_entries = memmapResponse->entry_count;
    for (uint64_t i = 0; i < memmap_entries; i++) // Clear usable pages
    {
        for (uint64_t j = 0; j < memmap[i]->length; j += PAGE_SIZE)
        {
            if (!vmMapPage(kernelPML4, memmap[i]->base + j + hhdmOffset, memmap[i]->base + j, PAGE_PRESENT | PAGE_WRITABLE)) {
                printf("Failed to map memory at %p\n", (void *)(memmap[i]->base + j + hhdmOffset));
            }
        }
    }

    extern char limineStart[], limineEnd[];

    uint64_t limineStartAligned = ALIGN_DOWN((uint64_t)&limineStart, PAGE_SIZE);
    uint64_t limineEndAligned = ALIGN_UP((uint64_t)&limineEnd, PAGE_SIZE);

    printf("Mapping Limine from %p to %p\n", (void *)limineStartAligned, (void *)limineEndAligned);
    for (uint64_t i = limineStartAligned; i < limineEndAligned; i += PAGE_SIZE) {
        if (!vmMapPage(kernelPML4, i, i - kernelAddressResponse->virtual_base + kernelAddressResponse->physical_base, PAGE_PRESENT | PAGE_WRITABLE)) {
            printf("Failed to map Limine at %p\n", (void *)i);
        }
    }

    extern char textStart[], textEnd[];

    uint64_t textStartAligned = ALIGN_DOWN((uint64_t)&textStart, PAGE_SIZE);
    uint64_t textEndAligned = ALIGN_UP((uint64_t)&textEnd, PAGE_SIZE);

    printf("Mapping Kernel Text from %p to %p\n", (void *)textStartAligned, (void *)textEndAligned);
    for (uint64_t i = textStartAligned; i < textEndAligned; i += PAGE_SIZE) {
        if (!vmMapPage(kernelPML4, i, i - kernelAddressResponse->virtual_base + kernelAddressResponse->physical_base, PAGE_PRESENT | PAGE_WRITABLE)) {
            printf("Failed to map Kernel Text at %p\n", (void *)i);
        }
    }

    extern char rodataStart[], rodataEnd[];

    uint64_t rodataStartAligned = ALIGN_DOWN((uint64_t)&rodataStart, PAGE_SIZE);
    uint64_t rodataEndAligned = ALIGN_UP((uint64_t)&rodataEnd, PAGE_SIZE);

    printf("Mapping Kernel Rodata from %p to %p\n", (void *)rodataStartAligned, (void *)rodataEndAligned);
    for (uint64_t i = rodataStartAligned; i < rodataEndAligned; i += PAGE_SIZE) {
        if (!vmMapPage(kernelPML4, i, i - kernelAddressResponse->virtual_base + kernelAddressResponse->physical_base, PAGE_PRESENT)) {
            printf("Failed to map Kernel Rodata at %p\n", (void *)i);
        }
    }

    extern char dataStart[], dataEnd[];

    uint64_t dataStartAligned = ALIGN_DOWN((uint64_t)&dataStart, PAGE_SIZE);
    uint64_t dataEndAligned = ALIGN_UP((uint64_t)&dataEnd, PAGE_SIZE);
    printf("Mapping Kernel Data from %p to %p\n", (void *)dataStartAligned, (void *)dataEndAligned);
    for (uint64_t i = dataStartAligned; i < dataEndAligned; i += PAGE_SIZE) {
        if (!vmMapPage(kernelPML4, i, i - kernelAddressResponse->virtual_base + kernelAddressResponse->physical_base, PAGE_PRESENT | PAGE_WRITABLE)) {
            printf("Failed to map Kernel Data at %p\n", (void *)i);
        }
    }

    uint64_t bitmapStartAligned = ALIGN_DOWN((uint64_t)bitmap, PAGE_SIZE);
    uint64_t bitmapEndAligned = ALIGN_UP((uint64_t)bitmap + bitmapSize, PAGE_SIZE);

    printf("Mapping Bitmap from %p to %p\n", (void *)bitmapStartAligned, (void *)bitmapEndAligned);
    for (uint64_t i = bitmapStartAligned; i < bitmapEndAligned; i += PAGE_SIZE) {
        if (!vmMapPage(kernelPML4, i, i - hhdmOffset, PAGE_PRESENT | PAGE_WRITABLE)) {
            printf("Failed to map Bitmap at %p\n", (void *)i);
        }
    }

    printf("Loading new Pagemap at %p\n", kernelPML4);
    vmLoad(kernelPML4);
}

void vmLoad(pagemap_t *pml4) {
    uint64_t phys = (uint64_t)pml4 - hhdmOffset;
    asm volatile ("mov %0, %%cr3" :: "r"(phys & ~0xFFFULL));
}

/// @brief Find a free region of virtual memory of at least size bytes
/// @param size The minimum size of the region
/// @return The virtual address of the region
static void *findFreeRegion(size_t size) {
    uint64_t currentRunStart = 0;
    uint64_t currentRunLength = 0;

    for (uint64_t i = 0x1000; i < 0xFFFFFFFFFFFFF000; i += PAGE_SIZE) {
        if (isPageMapped(i)) {
            currentRunLength = 0;
            currentRunStart = 0;
        }
        else {
            currentRunStart = i;
            while (isPageMapped(i) == false && i < 0xFFFFFFFFFFFFF000) {
                currentRunLength += PAGE_SIZE;
                i += PAGE_SIZE;
                if (currentRunLength >= size) {
                    return (void *)currentRunStart;
                }
            }
        }
    }

    return NULL; // No suitable region found
}

void *vmGetPages(void *requestedAddress, size_t size) {
    if (requestedAddress == NULL) {
        // Find a place to put memory
        uint64_t addr = (uint64_t)findFreeRegion(size);
        if (addr == 0) {
            return NULL; // No space found
        }
        return vmGetPages((void *)addr, size);
    } 

    uint64_t start = ALIGN_DOWN((uint64_t)requestedAddress, PAGE_SIZE);
    uint64_t end = ALIGN_UP((uint64_t)requestedAddress + size, PAGE_SIZE);

    for (uint64_t addr = start; addr < end; addr += PAGE_SIZE) {
        if (!vmMapPage(kernelPML4, addr, (uint64_t)pmAlloc(PAGE_SIZE), PAGE_PRESENT | PAGE_WRITABLE)) {
            // Handle allocation failure (e.g., log an error)
            return NULL;
        }
    }

    return (void *)start;
}

void *vmMapPageRange(pagemap_t *pagemap, uint64_t paddr, size_t size) {
    uint64_t start = ALIGN_DOWN(paddr, PAGE_SIZE);
    uint64_t end = ALIGN_UP(paddr + size, PAGE_SIZE);

    void *vaddr = findFreeRegion(end - start);
    if (!vaddr) return NULL;

    for (uint64_t addr = start; addr < end; addr += PAGE_SIZE) {
        if (!vmMapPage(pagemap, (uint64_t)vaddr + (addr - start), addr, PAGE_PRESENT | PAGE_WRITABLE)) {
            // Handle allocation failure (e.g., log an error)
            return NULL;
        }
    }

    return vaddr;
}

void vmUnmapPageRange(pagemap_t *pagemap, void *address, size_t size) {
    uint64_t start = ALIGN_DOWN((uint64_t)address, PAGE_SIZE);
    uint64_t end = ALIGN_UP((uint64_t)address + size, PAGE_SIZE);

    for (uint64_t addr = start; addr < end; addr += PAGE_SIZE) {
        vmUnmapPage(pagemap, addr);
    }
}

uint64_t vmGetPhysical(pagemap_t *pagemap, uint64_t vaddr) {
    if (!pagemap) return 0;
    
    uint64_t pml4Index = (vaddr >> 39) & 0x1FF;
    uint64_t pdptIndex = (vaddr >> 30) & 0x1FF;
    uint64_t pdIndex   = (vaddr >> 21) & 0x1FF;
    uint64_t ptIndex   = (vaddr >> 12) & 0x1FF;

    if (!((*pagemap)[pml4Index] & PAGE_PRESENT)) return 0;
    
    pagemap_t *pml3 = (pagemap_t *)(hhdmOffset + ((*pagemap)[pml4Index] & 0x000FFFFFFFFFF000));
    if (!((*pml3)[pdptIndex] & PAGE_PRESENT)) return 0;
    
    pagemap_t *pml2 = (pagemap_t *)(hhdmOffset + ((*pml3)[pdptIndex] & 0x000FFFFFFFFFF000));
    if (!((*pml2)[pdIndex] & PAGE_PRESENT)) return 0;
    
    pagemap_t *pml1 = (pagemap_t *)(hhdmOffset + ((*pml2)[pdIndex] & 0x000FFFFFFFFFF000));
    if (!((*pml1)[ptIndex] & PAGE_PRESENT)) return 0;
    
    return ((*pml1)[ptIndex] & 0x000FFFFFFFFFF000) | (vaddr & 0xFFF);
}

void vmFreePages(void *address, size_t size) {
    uint64_t start = ALIGN_DOWN((uint64_t)address, PAGE_SIZE);
    uint64_t end = ALIGN_UP((uint64_t)address + size, PAGE_SIZE);

    for (uint64_t addr = start; addr < end; addr += PAGE_SIZE) {
        uint64_t phys = vmGetPhysical(kernelPML4, addr);
        if (phys) {
            pmFree((void *)phys, PAGE_SIZE);
            vmUnmapPage(kernelPML4, addr);
        }
    }
}