#include <arch/x86_64/cpu.h>
#include <arch/x86_64/printf.h>
#include <string.h>
#include <memory.h>
#include <math.h>

#define PTE_ADDR_MASK 0x000FFFFFFFFFF000ULL

/// @brief Get a pagetable entry from a virtual address and a pagemap level 4.
/// @param pml4 The Level 4 pagemap to search
/// @param virtual The virtual address to search with
/// @return The pagetable entry, or NULL if none found
uint64_t *mmuGetPage(pagemap_t *pml4, void *virtual) {
    uint64_t pml4Index = ((uint64_t)virtual >> 39) & 0x1FF;
    uint64_t pdptIndex = ((uint64_t)virtual >> 30) & 0x1FF;
    uint64_t pdIndex   = ((uint64_t)virtual >> 21) & 0x1FF;
    uint64_t ptIndex   = ((uint64_t)virtual >> 12) & 0x1FF;

    // Check PML4 present first
    uint64_t pml4e = (*pml4)[pml4Index];
    if (!(pml4e & PAGEMAP_PRESENT)) return NULL;
    pagemap_t *pdpt = (pagemap_t *)((pml4e & PTE_ADDR_MASK) + hhdmOffset);

    uint64_t pdpte = (*pdpt)[pdptIndex];
    if (!(pdpte & PAGEMAP_PRESENT)) return NULL;
    pagemap_t *pd = (pagemap_t *)((pdpte & PTE_ADDR_MASK) + hhdmOffset);

    uint64_t pde = (*pd)[pdIndex];
    if (!(pde & PAGEMAP_PRESENT)) return NULL;
    pagemap_t *pt = (pagemap_t *)((pde & PTE_ADDR_MASK) + hhdmOffset);

    uint64_t *result = &(*pt)[ptIndex];
    if (!(*result & PAGEMAP_PRESENT)) return NULL;

    return result;
}

static pagemap_t *mmuGetOrAllocateTable(pagemap_t *parent, uint16_t index, uint64_t flagsMask) {
    if (!((*parent)[index] & PAGEMAP_PRESENT)) {
        uint64_t newTable = pmmAlloc(1);
        if (newTable == 0) {
            return NULL;
        }
        memset((void *)(newTable + hhdmOffset), 0, sizeof(pagemap_t));
        (*parent)[index] = (newTable & PTE_ADDR_MASK) | (flagsMask & ~0xFFFULL) | PAGEMAP_PRESENT;
    }
    return (pagemap_t *)(((*parent)[index] & PTE_ADDR_MASK) + hhdmOffset);
}

bool mmuMapPage(pagemap_t *pml4, void *virtual, uint64_t physical, uint64_t flagsMask) {
    uint64_t entry = (physical & PTE_ADDR_MASK) | (flagsMask & ~PTE_ADDR_MASK) | PAGEMAP_PRESENT;

    uint16_t pml4Index = ((uint64_t)virtual >> 39) & 0x1FF;
    uint16_t pdptIndex = ((uint64_t)virtual >> 30) & 0x1FF;
    uint16_t pdIndex   = ((uint64_t)virtual >> 21) & 0x1FF;
    uint16_t ptIndex   = ((uint64_t)virtual >> 12) & 0x1FF;

    pagemap_t *pdpt = mmuGetOrAllocateTable(pml4, pml4Index, flagsMask);
    if (pdpt == NULL) return false;

    pagemap_t *pd = mmuGetOrAllocateTable(pdpt, pdptIndex, flagsMask);
    if (pd == NULL) return false;

    pagemap_t *pt = mmuGetOrAllocateTable(pd, pdIndex, flagsMask);
    if (pt == NULL) return false;

    (*pt)[ptIndex] = entry;
    asm volatile ("invlpg (%0)" : : "r"(virtual) : "memory");
    return true;
}

void mmuUnmapPage(pagemap_t *pml4, void *virtual) {
    uint64_t *entry = mmuGetPage(pml4, virtual);
    if (entry == NULL) return;
    *entry = 0;
    asm volatile("invlpg (%0)" : : "r"(virtual) : "memory");
}

void mmuSwitchPagemap(pagemap_t *pml4) {
    uint64_t physicalAddress = ((uint64_t)pml4 - hhdmOffset) & ~0xFFFULL;
    
    asm volatile("mov %0, %%cr3" : : "r"(physicalAddress));
    printf("Loaded!\n");
}

void *findFreeRange(pagemap_t *pml4, size_t pageCount) {
    size_t streak = 0;
    uint64_t startAddress = 0;

    for (uint64_t address = 0xFFFF800000000000; address < 0xFFFFFFFFFFFFFFFF; address += PAGE_SIZE) {
        uint64_t *entry = mmuGetPage(pml4, (void *)address);
        if (entry == NULL || !(*entry & PAGEMAP_PRESENT)) {
            if (streak == 0) {
                startAddress = address;
            }
            streak++;
            if (streak >= pageCount) {
                return (void *)startAddress;
            }
        }
        else {
            streak = 0;
        }
    }
    return NULL;
}

void mapRange(pagemap_t *pml4, void *virtualAddress, uint64_t physicalAddress, size_t pageCount, uint64_t flagsMask) {
    for (size_t i = 0; i < pageCount; i++) {
        if (!mmuMapPage(pml4, (void *)((uint64_t)virtualAddress + (i * PAGE_SIZE)), physicalAddress + (i * PAGE_SIZE), flagsMask)) {
            printf("ERROR: Failed to map page in range\n");
            return;
        }
    }
}

extern char limineStart[], limineEnd[];
extern char textStart[], textEnd[];
extern char rodataStart[], rodataEnd[];
extern char dataStart[], dataEnd[];

void initialiseMMU(pagemap_t *pml4, struct limine_memmap_response *memmap, struct limine_kernel_address_response *kernelAddressResponse) {
    uint64_t limineStartAligned = ALIGN_DOWN((uint64_t)&limineStart, PAGE_SIZE);
    uint64_t limineEndAligned = ALIGN_UP((uint64_t)&limineEnd, PAGE_SIZE);

    uint64_t textStartAligned = ALIGN_DOWN((uint64_t)&textStart, PAGE_SIZE);
    uint64_t textEndAligned = ALIGN_UP((uint64_t)&textEnd, PAGE_SIZE);

    uint64_t dataStartAligned = ALIGN_DOWN((uint64_t)&dataStart, PAGE_SIZE);
    uint64_t dataEndAligned = ALIGN_UP((uint64_t)&dataEnd, PAGE_SIZE);

    uint64_t rodataStartAligned = ALIGN_DOWN((uint64_t)&rodataStart, PAGE_SIZE);
    uint64_t rodataEndAligned = ALIGN_UP((uint64_t)&rodataEnd, PAGE_SIZE);

    uint64_t bitmapStartAligned = ALIGN_DOWN((uint64_t)pmmBitmap, PAGE_SIZE);
    uint64_t bitmapEndAligned = ALIGN_UP((uint64_t)pmmBitmap + bitmapSize, PAGE_SIZE);

    uint64_t oldCR3;
    asm volatile("mov %%cr3, %0" : "=r"(oldCR3));

    pagemap_t *limineProvidedPagemap = (pagemap_t *)((oldCR3 & PTE_ADDR_MASK) + hhdmOffset);
    memcpy(pml4, limineProvidedPagemap, PAGE_SIZE);

    printf("Initialising HHDM\n");
    // Initialize the HHDM mapping
    for (uint64_t j = 0; j < memmap->entry_count; j++) {
        if (memmap->entries[j]->type != LIMINE_MEMMAP_USABLE) {
            continue;
        }

        for (uint64_t i = 0; i < memmap->entries[j]->length; i += PAGE_SIZE) {
            mmuMapPage(pml4, (void *)(memmap->entries[j]->base + i + hhdmOffset), memmap->entries[j]->base + i, PAGEMAP_WRITABLE | PAGEMAP_PRESENT | PAGEMAP_NO_EXECUTE);
        }
    }
    printf("HHDM Initialised\n");

    printf("Mapping Limine\n");
    for (uint64_t i = limineStartAligned; i < limineEndAligned; i += PAGE_SIZE) {
        mmuMapPage(pml4, (void *)i, i - kernelAddressResponse->virtual_base + kernelAddressResponse->physical_base, PAGEMAP_PRESENT | PAGEMAP_NO_EXECUTE | PAGEMAP_WRITABLE);
    }
    printf("Limine Mapped\n");

    printf("Mapping Kernel Text\n");
    for (uint64_t i = textStartAligned; i < textEndAligned; i += PAGE_SIZE) {
        mmuMapPage(pml4, (void *)i, i - kernelAddressResponse->virtual_base + kernelAddressResponse->physical_base, PAGEMAP_PRESENT);
    }
    printf("Kernel Text Mapped\n");

    printf("Mapping Kernel ROData\n");
    for (uint64_t i = rodataStartAligned; i < rodataEndAligned; i += PAGE_SIZE) {
        mmuMapPage(pml4, (void *)i, i - kernelAddressResponse->virtual_base + kernelAddressResponse->physical_base, PAGEMAP_PRESENT | PAGEMAP_NO_EXECUTE);
    }
    printf("Kernel ROData Mapped\n");

    printf("Mapping Kernel Data\n");
    for (uint64_t i = dataStartAligned; i < dataEndAligned; i += PAGE_SIZE) {
        mmuMapPage(pml4, (void *)i, i - kernelAddressResponse->virtual_base + kernelAddressResponse->physical_base, PAGEMAP_PRESENT | PAGEMAP_WRITABLE | PAGEMAP_NO_EXECUTE);
    }
    printf("Kernel Data Mapped\n");

    printf("Mapping Bitmap\n");
    for (uint64_t i = bitmapStartAligned; i < bitmapEndAligned; i+= PAGE_SIZE) {
        mmuMapPage(pml4, (void *)i, i - hhdmOffset, PAGEMAP_PRESENT | PAGEMAP_WRITABLE | PAGEMAP_NO_EXECUTE);
    }
    printf("Bitmap Mapped\n");

    printf("Loading Kernel Pagemap at %p\n", pml4);
    mmuSwitchPagemap(pml4);
}