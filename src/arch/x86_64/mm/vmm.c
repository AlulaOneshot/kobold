#include <arch/x86_64/mm.h>
#include <arch/x86_64/printf.h>
#include <arch/x86_64/cpu.h>
#include <math.h>

extern char limineStart[], limineEnd[];
extern char textStart[], textEnd[];
extern char rodataStart[], rodataEnd[];
extern char dataStart[], dataEnd[];

pagemap_t *kernelPML4;

vmmaps_t **kernelMaps = NULL;
uint64_t mapsCount = 0; // Starts at 0
uint64_t mapsCapacity = 0;

static void vmEnsureCapacity(vmmaps_t ***mapsPtr, uint64_t *mapsCapacity, uint64_t countNeeded) {
    if (*mapsCapacity >= countNeeded) {
        return; // enough space already
    }

    uint64_t newCap = *mapsCapacity ? *mapsCapacity * 2 : 16; // grow 2x or start at 16
    if (newCap < countNeeded) {
        newCap = countNeeded; // make sure it's enough
    }

    // Allocate new array
    vmmaps_t **newMaps = (vmmaps_t **)((uint64_t)pmAlloc(sizeof(vmmaps_t *) * newCap) + hhdmOffset);
    if (!newMaps) {
        printf("vmEnsureCapacity: Failed to allocate vmmaps array\n");
    }

    // Copy old entries if any
    if (*mapsPtr) {
        memcpy(newMaps, *mapsPtr, sizeof(vmmaps_t *) * (*mapsCapacity));
        pmFree((void *)((uint64_t)*mapsPtr - hhdmOffset), sizeof(vmmaps_t *) * (*mapsCapacity));
    }

    *mapsPtr = newMaps;
    *mapsCapacity = newCap;
}

static pagemap_t *vmMapsFindFreeSlot(vmmaps_t *maps[], uint64_t mapsCount) {
    if (mapsCount == 0) return NULL;
    for (uint64_t x = 0; x < mapsCount; x++) {
        for (uint64_t i = 0; i < VMMAPS_MAPS; i++) {
            if (bitmapGetBit(maps[x]->avalibleMaps, i) == 0) {
                bitmapSetBit(maps[x]->avalibleMaps, i);
                return &maps[x]->pagemaps[i];
            }
        }
    }
    return NULL;
}

static pagemap_t *vmGetOrAllocTable(pagemap_t *parent, uint64_t index, uint64_t flags) {
    vmEnsureCapacity(&kernelMaps, &mapsCapacity, mapsCount + 1);
    if (!((uint64_t)((*parent)[index]) & PAGE_PRESENT)) {
        pagemap_t *freetable = vmMapsFindFreeSlot(kernelMaps, mapsCount);
        if (!freetable) {
            uint64_t phys = (uint64_t)pmAlloc(sizeof(vmmaps_t));
            kernelMaps[mapsCount] = (vmmaps_t *)(phys + hhdmOffset);
            memset(kernelMaps[mapsCount], 0, sizeof(vmmaps_t));
            kernelMaps[mapsCount]->pml4 = kernelPML4;
            freetable = &kernelMaps[mapsCount]->pagemaps[0];
            bitmapSetBit(kernelMaps[mapsCount]->avalibleMaps, 0);
            mapsCount++;
        }
        (*parent)[index] = ((uint64_t)freetable - hhdmOffset) | flags | PAGE_PRESENT;
    }
    return (pagemap_t *)(((uint64_t)(*parent)[index] & PTE_ADDR_MASK) + hhdmOffset);
}

void vmMapPage(pagemap_t *map, void *virtual, uint64_t phys, uint64_t flag_mask) {
    uint64_t entry = (phys & PTE_ADDR_MASK) | flag_mask;
    uint64_t pml4Index = ((uint64_t)virtual >> 39) & 0x1FF;
    uint64_t pdptIndex = ((uint64_t)virtual >> 30) & 0x1FF;
    uint64_t pdIndex   = ((uint64_t)virtual >> 21) & 0x1FF;
    uint64_t ptIndex   = ((uint64_t)virtual >> 12) & 0x1FF;

    pagemap_t *pdpt = vmGetOrAllocTable(map, pml4Index, (PAGE_PRESENT | PAGE_WRITABLE));

    pagemap_t *pd = vmGetOrAllocTable(pdpt, pdptIndex, (PAGE_PRESENT | PAGE_WRITABLE));

    pagemap_t *pt = vmGetOrAllocTable(pd, pdIndex, (PAGE_PRESENT | PAGE_WRITABLE));

    if (((*pt)[ptIndex]) & PAGE_PRESENT) {
        // Already mapped
        printf("vmMapPage: Virtual address %p is already mapped!\n", virtual);
    }

    (*pt)[ptIndex] = entry | PAGE_PRESENT;
}

void vmUnmapPage(pagemap_t *map, void *virtual) {
    uint64_t pml4Index = ((uint64_t)virtual >> 39) & 0x1FF;
    uint64_t pdptIndex = ((uint64_t)virtual >> 30) & 0x1FF;
    uint64_t pdIndex   = ((uint64_t)virtual >> 21) & 0x1FF;
    uint64_t ptIndex   = ((uint64_t)virtual >> 12) & 0x1FF;

    if (!((uint64_t)((*map)[pml4Index]) & PAGE_PRESENT)) {
        printf("vmUnmapPage: Virtual address %p is not mapped (PML4)!\n", virtual);
    }
    pagemap_t *pdpt = (pagemap_t *)(((uint64_t)(*map)[pml4Index] & PTE_ADDR_MASK) + hhdmOffset);

    if (!((uint64_t)((*pdpt)[pdptIndex]) & PAGE_PRESENT)) {
        printf("vmUnmapPage: Virtual address %p is not mapped (PDPT)!\n", virtual);
    }
    pagemap_t *pd = (pagemap_t *)(((uint64_t)(*pdpt)[pdptIndex] & PTE_ADDR_MASK) + hhdmOffset);

    if (!((uint64_t)((*pd)[pdIndex]) & PAGE_PRESENT)) {
        printf("vmUnmapPage: Virtual address %p is not mapped (PD)!\n", virtual);
    }
    pagemap_t *pt = (pagemap_t *)(((uint64_t)(*pd)[pdIndex] & PTE_ADDR_MASK) + hhdmOffset);

    if (!((uint64_t)((*pt)[ptIndex]) & PAGE_PRESENT)) {
        printf("vmUnmapPage: Virtual address %p is not mapped (PT)!\n", virtual);
    }

    (*pt)[ptIndex] = 0; // Unmap
    __asm__ volatile("invlpg (%0)" ::"r"(virtual) : "memory");
}

void vmMapRange(pagemap_t *map, void *virtual, uint64_t phys, uint64_t size, uint64_t flag_mask) {
    uint64_t pages = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    for (uint64_t i = 0; i < pages; i++) {
        vmMapPage(map, (void *)((uint64_t)virtual + i * PAGE_SIZE), phys + i * PAGE_SIZE, flag_mask);
    }
}

void vmUnmapRange(pagemap_t *map, void *virtual, uint64_t size) {
    uint64_t pages = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    for (uint64_t i = 0; i < pages; i++) {
        vmUnmapPage(map, (void *)((uint64_t)virtual + i * PAGE_SIZE));
    }
}

bool vmIsMapped(pagemap_t *map, void *virtual) {
    uint64_t address = (uint64_t)virtual;
    uint64_t pml4Index = (address >> 39) & 0x1FF;
    uint64_t pdptIndex = (address >> 30) & 0x1FF;
    uint64_t pdIndex   = (address >> 21) & 0x1FF;
    uint64_t ptIndex   = (address >> 12) & 0x1FF;

    if (!((uint64_t)((*map)[pml4Index]) & PAGE_PRESENT)) {
        return false;
    }
    pagemap_t *pdpt = (pagemap_t *)(((uint64_t)(*map)[pml4Index] & PTE_ADDR_MASK) + hhdmOffset);

    if (!((uint64_t)((*pdpt)[pdptIndex]) & PAGE_PRESENT)) {
        return false;
    }
    pagemap_t *pd = (pagemap_t *)(((uint64_t)(*pdpt)[pdptIndex] & PTE_ADDR_MASK) + hhdmOffset);

    if (!((uint64_t)((*pd)[pdIndex]) & PAGE_PRESENT)) {
        return false;
    }
    pagemap_t *pt = (pagemap_t *)(((uint64_t)(*pd)[pdIndex] & PTE_ADDR_MASK) + hhdmOffset);

    if (!((uint64_t)((*pt)[ptIndex]) & PAGE_PRESENT)) {
        return false;
    }

    return true;
}

uint64_t vmGetPhysical(pagemap_t *map, void *virtual) {
    uint64_t pml4Index = ((uint64_t)virtual >> 39) & 0x1FF;
    uint64_t pdptIndex = ((uint64_t)virtual >> 30) & 0x1FF;
    uint64_t pdIndex   = ((uint64_t)virtual >> 21) & 0x1FF;
    uint64_t ptIndex   = ((uint64_t)virtual >> 12) & 0x1FF;

    if (!((uint64_t)((*map)[pml4Index]) & PAGE_PRESENT)) {
        printf("vmGetPhysical: Virtual address %p is not mapped (PML4)!\n", virtual);
    }
    pagemap_t *pdpt = (pagemap_t *)(((uint64_t)(*map)[pml4Index] & PTE_ADDR_MASK) + hhdmOffset);

    if (!((uint64_t)((*pdpt)[pdptIndex]) & PAGE_PRESENT)) {
        printf("vmGetPhysical: Virtual address %p is not mapped (PDPT)!\n", virtual);
    }
    pagemap_t *pd = (pagemap_t *)(((uint64_t)(*pdpt)[pdptIndex] & PTE_ADDR_MASK) + hhdmOffset);

    if (!((uint64_t)((*pd)[pdIndex]) & PAGE_PRESENT)) {
        printf("vmGetPhysical: Virtual address %p is not mapped (PD)!\n", virtual);
    }
    pagemap_t *pt = (pagemap_t *)(((uint64_t)(*pd)[pdIndex] & PTE_ADDR_MASK) + hhdmOffset);

    if (!((uint64_t)((*pt)[ptIndex]) & PAGE_PRESENT)) {
        printf("vmGetPhysical: Virtual address %p is not mapped (PT)!\n", virtual);
    }

    return ((*pt)[ptIndex] & PTE_ADDR_MASK) | ((uint64_t)virtual & 0xFFF);
}

void vmModifyFlags(pagemap_t *map, void *virtual, uint64_t flag_mask, bool set) {
    uint64_t pml4Index = ((uint64_t)virtual >> 39) & 0x1FF;
    uint64_t pdptIndex = ((uint64_t)virtual >> 30) & 0x1FF;
    uint64_t pdIndex   = ((uint64_t)virtual >> 21) & 0x1FF;
    uint64_t ptIndex   = ((uint64_t)virtual >> 12) & 0x1FF;

    if (!((uint64_t)((*map)[pml4Index]) & PAGE_PRESENT)) {
        printf("vmModifyFlags: Virtual address %p is not mapped (PML4)!\n", virtual);
    }
    pagemap_t *pdpt = (pagemap_t *)(((uint64_t)(*map)[pml4Index] & PTE_ADDR_MASK) + hhdmOffset);

    if (!((uint64_t)((*pdpt)[pdptIndex]) & PAGE_PRESENT)) {
        printf("vmModifyFlags: Virtual address %p is not mapped (PDPT)!\n", virtual);
    }
    pagemap_t *pd = (pagemap_t *)(((uint64_t)(*pdpt)[pdptIndex] & PTE_ADDR_MASK) + hhdmOffset);

    if (!((uint64_t)((*pd)[pdIndex]) & PAGE_PRESENT)) {
        printf("vmModifyFlags: Virtual address %p is not mapped (PD)!\n", virtual);
    }
    pagemap_t *pt = (pagemap_t *)(((uint64_t)(*pd)[pdIndex] & PTE_ADDR_MASK) + hhdmOffset);

    if (!((uint64_t)((*pt)[ptIndex]) & PAGE_PRESENT)) {
        printf("vmModifyFlags: Virtual address %p is not mapped (PT)!\n", virtual);
    }

    if (set) {
        (*pt)[ptIndex] |= flag_mask;
    } else {
        (*pt)[ptIndex] &= ~flag_mask;
    }
    __asm__ volatile("invlpg (%0)" ::"r"(virtual) : "memory");
}

void *vmGetSpace(uint64_t physicalAddress, uint64_t size) {
    uint64_t pages = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    if (physicalAddress == 0) {
        physicalAddress = (uint64_t)pmAlloc(pages * PAGE_SIZE);
        if (physicalAddress == 0) {
            printf("vmGetSpace: Failed to allocate physical memory\n");
            return NULL;
        }
    }
    uint64_t range = 0;
    for (uint64_t i = 0x1000; i < 0xFFFFFFFFFFFFF000; i += PAGE_SIZE) {
        if (!vmIsMapped(kernelPML4, (void *)i)) {
            range += 1;
            if (range * PAGE_SIZE >= pages * PAGE_SIZE) {
                vmMapRange(kernelPML4, (void *)(i - range), physicalAddress, pages * PAGE_SIZE, PAGE_WRITABLE);
                return (void *)(i - range);
            }
        } else {
            range = 0;
        }
    }
    printf("vmGetSpace: Failed to find a large enough virtual memory range\n");
    return NULL;
}

void initVMM(struct limine_memmap_response *memmap, struct limine_kernel_address_response *kernelAddressResponse) {
    vmEnsureCapacity(&kernelMaps, &mapsCapacity, mapsCount + 1);

    // Setup the top page-table
    kernelPML4 = (pagemap_t *)((uint64_t)(pmAlloc(sizeof(pagemap_t))) + hhdmOffset);

    memset(kernelPML4, 0, sizeof(pagemap_t));

    pagemap_t *cur_pml4 = (pagemap_t *)((getCR3()) + hhdmOffset);
    memcpy(kernelPML4, cur_pml4, PAGE_SIZE);

    setCR3((uint64_t)kernelPML4 - hhdmOffset);

    if (!vmIsMapped(kernelPML4, &textStart)) {
    printf("Kernel text is not mapped!\n");
    }

    uint64_t phys = vmGetPhysical(kernelPML4, &dataStart);
    printf("Physical address of dataStart: 0x%llx\n", phys);
}