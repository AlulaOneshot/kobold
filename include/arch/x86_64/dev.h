#pragma once

#include <stdint.h>
#include <stddef.h>

void qemuDebugPutChar(char c);

typedef struct {
    uint16_t segment;
    uint8_t bus;
    uint8_t slot;
    uint8_t function;
    int64_t listIndex;
} pci_device_t;

void initialisePCI();
void addPCIDevice(pci_device_t *device);
void removePCIDevice(pci_device_t *device);
uint8_t readPCIConfig8(pci_device_t *device, size_t offset);
uint16_t readPCIConfig16(pci_device_t *device, size_t offset);
uint32_t readPCIConfig32(pci_device_t *device, size_t offset);
void writePCIConfig8(pci_device_t *device, size_t offset, uint8_t value);
void writePCIConfig16(pci_device_t *device, size_t offset, uint16_t value);
void writePCIConfig32(pci_device_t *device, size_t offset, uint32_t value);

void initialiseACPI(uint64_t rsdpPhysicalAddr);