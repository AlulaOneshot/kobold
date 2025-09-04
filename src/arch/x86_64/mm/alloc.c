#include <arch/x86_64/mm.h>

slab_t *first;

void *malloc(size_t size) {
    slab_t *current = first;
    while (current) {
        if (current->blockSize >= size && current->freeBlocks > 0) {
            // Find a free block
            for (uint16_t i = 0; i < current->totalBlocks; i++) {
                if (bitmapGetBit(current->freeMap, i) == 0) {
                    // Found a free block
                    bitmapSetBit(current->freeMap, i);
                    current->freeBlocks -= 1;
                    return (void *)((uint64_t)current->memory + (i * current->blockSize));
                }
            }
        }
        current = current->next;
    }
    {
        uint16_t blockCount;

        if (size >= 0x10000) {
            blockCount = 8;
        }
        else {
            blockCount = 64;
        }
        first = newSlab(current, ALIGN_UP(size, 16), blockCount); // Create a new slab with 32 blocks. Round block size to multiples of 16
    }
    return malloc(size); // Current.next should now be an empty slab of a suitible size.
}

void free(void *addr) {
    uint64_t numAddr = (uint64_t)addr;
    slab_t *current = first;
    while (current) {
        if (numAddr >= (uint64_t)current->memory && numAddr < ((uint64_t)current->memory + current->blockSize * current->totalBlocks)) {
            // Address resides within this block
            if (current->freeBlocks == current->totalBlocks - 1) {
                // Just release the whole block
                slab_t *prev = current->prev;
                slab_t *next = current->next;

                if (prev != NULL) {
                    prev->next = next;
                    if (next != NULL) {
                        next->prev = prev;
                    }
                }

                uint64_t slabSize = sizeof(slab_t) + (current->totalBlocks / 8) + (current->totalBlocks * current->blockSize);
                uint64_t phys = vmGetPhysical(kernelPML4, (void *)current);
                vmUnmapRange(kernelPML4, (void *)current, slabSize);
                pmFree((void *)phys, slabSize);
                current = NULL;
            }
            else {
                uint64_t offset = numAddr - (uint64_t)current->memory;
                bitmapClearBit(current->freeMap, offset / current->blockSize);
                current = NULL;
            }
        }
    }
}