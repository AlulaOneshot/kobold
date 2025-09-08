#include <memory.h>

slab_t *first = NULL;

void initialiseAllocator() {
    first = newSlab(NULL, 64, 256); // Start with a slab of 16 byte blocks
}

void *malloc(size_t size) {
    slab_t *current = first;
    while (current) {
        if (current->blockSize >= size && current->freeCount > 0) {
            for (size_t i = 0; i < current->blockCount; i++) {
                if (bitmapGetBit(current->freeMap, i) == 0) {
                    bitmapSetBit(current->freeMap, i);
                    current->freeCount -= 1;
                    return (void *)((uint64_t)current->memory + (i * current->blockSize));
                }
            }
        }
        if (current->next) {
            current = current->next;
        }
        else {
            break;
        }
    }
    
    uint16_t blockCount;

    if (size >= 0x10000) {
        blockCount = 8;
    }
    else {
        blockCount = 64;
    }
    
    newSlab(current, ALIGN_UP(size, 16), blockCount);

    return malloc(size); // Retry now that we have a new slab of a suitable size
}

void free(void *ptr) {
    //TODO: Implement
}