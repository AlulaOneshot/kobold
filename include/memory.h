#pragma once

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <limine.h>
#include <bitmaps.h>
#include <math.h>

#define PAGE_SIZE 4096

extern uint64_t hhdmOffset;
extern bitmap_t *pmmBitmap;
extern uint64_t bitmapSize;

void initialisePMM(struct limine_memmap_response *memmap);

uint64_t pmmAlloc(size_t pageCount);
void pmmFree(uint64_t address, size_t pageCount);

void initialiseVMM(struct limine_memmap_response *memmap, struct limine_kernel_address_response *kernelAddressResponse);
void *vmGetSpace(uint64_t physicalAddress, size_t pageCount);
void vmFreeSpace(void *virtualAddress, size_t pageCount);

typedef struct slab {
    bitmap_t *freeMap;
    uint64_t blockSize;
    uint16_t blockCount;
    uint16_t freeCount;
    struct slab *previous;
    struct slab *next;
    void *memory;
} __attribute__((packed)) slab_t;

void initialiseAllocator();
void *malloc(size_t size);
void free(void *ptr);

static inline slab_t *newSlab(slab_t *previous, uint16_t blockSize, uint16_t totalBlocks) {
    size_t sizeRequred = sizeof(slab_t) + totalBlocks / 8 + (blockSize * totalBlocks);
    uint64_t area = pmmAlloc(ALIGN_UP(sizeRequred, PAGE_SIZE) / PAGE_SIZE);
    void *virtual = vmGetSpace(area, ALIGN_UP(sizeRequred, PAGE_SIZE) / PAGE_SIZE);
    slab_t *slab = (slab_t *)virtual;
    slab->blockSize = blockSize;
    slab->blockCount = totalBlocks;
    slab->freeCount = totalBlocks;
    if (previous) {
        previous->next = slab;
    }
    slab->previous = previous;
    slab->next = NULL;
    slab->memory = (void *)((uint64_t)virtual + sizeof(slab_t) + (totalBlocks / 8));
    slab->freeMap = (bitmap_t *)((uint64_t)virtual + sizeof(slab_t));
    memset(slab->freeMap, 0, totalBlocks / 8);
    memset(slab->memory, 0, blockSize * totalBlocks);
    return slab;
}