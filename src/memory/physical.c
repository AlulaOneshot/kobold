#include <limine.h>
#include <memory.h>
#include <string.h>
#include <math.h>
#include <arch/x86_64/printf.h>

uint64_t hhdmOffset;
bitmap_t *pmmBitmap;
uint64_t highestAddress;
uint64_t bitmapSize;
uint64_t freePages;

void initialisePMM(struct limine_memmap_response *memmap) {
    struct limine_memmap_entry **entry = memmap->entries;
    uint64_t entryCount = memmap->entry_count;

    // Find the highest address in the memory map
    for (uint64_t i = 0; i < entryCount; i++) {
        if (entry[i]->type != LIMINE_MEMMAP_USABLE) {
            continue;
        }

        uint64_t topAddress = entry[i]->base + entry[i]->length;

        if (topAddress > highestAddress) {
            highestAddress = topAddress;
        }
    }

    bitmapSize = ALIGN_UP((highestAddress / 0x1000) / 8, 0x1000);

    // Now we need to find a place to put the bitmap
    for (uint64_t i = 0; i < entryCount; i++) {
        if (entry[i]->type != LIMINE_MEMMAP_USABLE) {
            continue;
        }
        if (entry[i]->length >= bitmapSize) {
            pmmBitmap = (bitmap_t *)(entry[i]->base + hhdmOffset);

            memset(pmmBitmap, 0xFF, bitmapSize);

            entry[i]->base += bitmapSize;
            entry[i]->length -= bitmapSize;

            break;
        }
    }

    if (!pmmBitmap) {
        printf("Failed to find space for PMM bitmap\n");
        asm volatile("cli");
        while (1) {
            asm volatile("hlt");
        }
    }

    for (uint64_t i = 0; i < entryCount; i++) {
        if (entry[i]->type != LIMINE_MEMMAP_USABLE) {
            continue;
        }

        uint64_t basePage = entry[i]->base / 0x1000;
        uint64_t pageCount = entry[i]->length / 0x1000;

        for (uint64_t j = 0; j < pageCount; j++) {
            bitmapClearBit(pmmBitmap, basePage + j);
            freePages++;
        }
    }

    printf("PMM Initialised. Free Memory: %d MiB\n", (freePages * 0x1000) / (1024 * 1024));
}

/// @brief Get a range of free physical memory pages.
/// @param pageCount The amount of pages to allocate.
/// @return The physical start address of the pages allocated, or 0 in case of error.
uint64_t pmmAlloc(size_t pageCount) {
    if (pageCount == 0 || pageCount >= freePages) {
        return 0;
    }

    size_t currentStreak = 0;

    for (size_t i = 0; i < (highestAddress / 0x1000); i++) {
        if (!bitmapGetBit(pmmBitmap, i)) {
            currentStreak++;
            if (currentStreak >= pageCount) {
                uint64_t startPage = i + 1 - pageCount;
                for (size_t j = 0; j < pageCount; j++) {
                    bitmapSetBit(pmmBitmap, startPage + j);
                }
                freePages -= pageCount;
                return startPage * 0x1000;
            }
        } else {
            currentStreak = 0;
        }
    }

    return 0;
}

/// @brief Free a range of physical memory pages.
/// @param address The page aligned physical start address of the pages to free.
/// @param pageCount The number of pages to free.
void pmmFree(uint64_t address, size_t pageCount) {
    if (address % 0x1000 != 0 || pageCount == 0) {
        return;
    }

    uint64_t startPage = address / 0x1000;

    for (size_t i = 0; i < pageCount; i++) {
        if (bitmapGetBit(pmmBitmap, startPage + i)) {
            bitmapClearBit(pmmBitmap, startPage + i);
            freePages++;
        }
    }
}