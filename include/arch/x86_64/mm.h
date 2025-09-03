#ifndef X86_64_MM_H
#define X86_64_MM_H

/*
    The Kobold Kernel
    mm.h - Pretty much everything Memory Management related for x86_64.
    Includes:
        PMM
        VMM
*/

#include <stdint.h>
#include <limine.h>
#include <stdbool.h>
#include <string.h>
#include <arch/x86_64/printf.h>
#include <math.h>
#include <bitmaps.h>

#define PAGE_SIZE 0x1000 // 4KB

/// @brief The offset where the higher half kernel is mapped
extern uint64_t hhdmOffset;
/// @brief A bitmap representing used/free physical memory pages
extern bitmap_t *bitmap;
/// @brief The size of the physical memory bitmap in bytes
extern uint64_t bitmapSize;

/// @brief Initialize the Physical Memory Manager
/// @param memmapResponse The Memory Map Returned by Limine
/// @param hhdm The Higher Half Direct Map Offset
void initPMM(struct limine_memmap_response *memmapResponse, uint64_t hhdm);
/// @brief Allocate physical memory
/// @param size Amount of physical memory to allocate in bytes
/// @return The physical address of the allocated memory, or NULL on failure
void *pmAlloc(uint64_t size);
/// @brief Free previously allocated physical memory
/// @param ptr The physical address of the memory to free
/// @param size The size of the memory block to free in bytes
void pmFree(void *ptr, uint64_t size);

/// @brief A page map (PML4, PML3, PML2, or PML1)
typedef uint64_t pagemap_t[512];

#define VMMAPS_MAPS 32
#define PTE_ADDR_MASK 0x000FFFFFFFFFF000ULL

// A place to store pagemaps. Useful to make sure the maps are mapped by storing them in one localized place.
typedef struct {
    pagemap_t *pml4; // The PML4 for this map
    bitmap_t avalibleMaps[VMMAPS_MAPS / 8];
    pagemap_t pagemaps[VMMAPS_MAPS]
} vmmaps_t;

//TODO: PML5

#define PAGE_PRESENT 0b1
#define PAGE_WRITABLE 0b10
#define PAGE_USER 0b100
#define PAGE_PWT 0b1000
#define PAGE_PCD 0b10000
#define PAGE_ACCESSED  0b100000
#define PAGE_DIRTY 0b1000000 // Only for PML1
#define PAGE_PAT 0b10000000 // Only for PML1
#define PAGE_GLOBAL 0b100000000 // Only for PML1
// Avalible
// Reserved (0)
// bits 8-11 Avalible
// bits 12-51 Physical Address
// bits 52-62 Avalible
// bits 59-62 PK on PML1
// bit 63 No Execute

extern pagemap_t *kernelPML4;

void initVMM();

#endif