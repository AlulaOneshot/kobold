#include <arch/x86_64/dev.h>
#include <arch/x86_64/cpu.h>
#include <memory.h>

#define PCI_CONFIG_ADDRESS_PORT 0xCF8
#define PCI_CONFIG_DATA_PORT 0xCFC

// TODO: Convert to Linked List
pci_device_t devices[256];
uint64_t deviceCount = 0;

void initialisePCI() {

}

void addPCIDevice(pci_device_t *device) {
    device->listIndex = (int64_t)deviceCount;
    devices[deviceCount] = *device;
    deviceCount++;
}

void removePCIDevice(pci_device_t *device) {
    int64_t indexOfDevice = device->listIndex;
    if (indexOfDevice < 0 || (uint64_t)indexOfDevice >= deviceCount) {
        return;
    }
    device->listIndex = -1;
    
    memmove(&devices[indexOfDevice], &devices[indexOfDevice + 1], sizeof(pci_device_t) * (deviceCount - indexOfDevice - 1));
    for (uint64_t i = indexOfDevice; i < deviceCount - 1; i++) {
        devices[i].listIndex = i;
    }
    deviceCount--;
}

uint8_t readPCIConfig8(pci_device_t *device, size_t offset) {
    uint32_t address;
    uint32_t lbus = (uint32_t)device->bus;
    uint32_t lslot = (uint32_t)device->slot;
    uint32_t lfunc = (uint32_t)device->function;
    uint32_t tmp = 0;

    // IDK what this does, but elysium-os/cronus does it
    address = (offset & 0xFC) | (1 << 31);
    address |= lbus << 16;
    address |= (lslot & 0x1F) << 11;
    address |= (lfunc & 0x07) << 8;
    outl(PCI_CONFIG_ADDRESS_PORT, address);

    tmp = inl(PCI_CONFIG_DATA_PORT);

    return (uint8_t) (tmp >> ((offset & 3) * 8));
}

uint16_t readPCIConfig16(pci_device_t *device, size_t offset) {
    uint32_t address;
    uint32_t lbus = (uint32_t)device->bus;
    uint32_t lslot = (uint32_t)device->slot;
    uint32_t lfunc = (uint32_t)device->function;
    uint32_t tmp = 0;

    // IDK what this does, but elysium-os/cronus does it
    address = (offset & 0xFC) | (1 << 31);
    address |= lbus << 16;
    address |= (lslot & 0x1F) << 11;
    address |= (lfunc & 0x07) << 8;
    outl(PCI_CONFIG_ADDRESS_PORT, address);

    tmp = inl(PCI_CONFIG_DATA_PORT);

    return (uint16_t) (tmp >> ((offset & 2) * 8));
}

uint32_t readPCIConfig32(pci_device_t *device, size_t offset) {
    uint32_t address;
    uint32_t lbus = (uint32_t)device->bus;
    uint32_t lslot = (uint32_t)device->slot;
    uint32_t lfunc = (uint32_t)device->function;

    // IDK what this does, but elysium-os/cronus does it
    address = (offset & 0xFC) | (1 << 31);
    address |= lbus << 16;
    address |= (lslot & 0x1F) << 11;
    address |= (lfunc & 0x07) << 8;
    outl(PCI_CONFIG_ADDRESS_PORT, address);

    return inl(PCI_CONFIG_DATA_PORT);
}

void writePCIConfig8(pci_device_t *device, size_t offset, uint8_t value) {
    uint32_t current = readPCIConfig32(device, offset & ~3);
    switch (offset & 3) {
        case 0:
            current = (current & 0xFFFFFF00) | (uint32_t)value;
            break;
        case 1:
            current = (current & 0xFFFF00FF) | ((uint32_t)value << 8);
            break;
        case 2:
            current = (current & 0xFF00FFFF) | ((uint32_t)value << 16);
            break;
        case 3:
            current = (current & 0x00FFFFFF) | ((uint32_t)value << 24);
            break;
    }
    writePCIConfig32(device, offset & ~3, current);
}

void writePCIConfig16(pci_device_t *device, size_t offset, uint16_t value) {
    uint32_t current = readPCIConfig32(device, offset & ~2);
    if (offset & 2) {
        current = (current & 0x0000FFFF) | ((uint32_t)value << 16);
    } else {
        current = (current & 0xFFFF0000) | (uint32_t)value;
    }
    writePCIConfig32(device, offset & ~2, current);
}

void writePCIConfig32(pci_device_t *device, size_t offset, uint32_t value) {
    uint32_t address;
    uint32_t lbus = (uint32_t)device->bus;
    uint32_t lslot = (uint32_t)device->slot;
    uint32_t lfunc = (uint32_t)device->function;

    // IDK what this does, but elysium-os/cronus does it
    address = (offset & 0xFC) | (1 << 31);
    address |= lbus << 16;
    address |= (lslot & 0x1F) << 11;
    address |= (lfunc & 0x07) << 8;
    outl(PCI_CONFIG_ADDRESS_PORT, address);

    outl(PCI_CONFIG_DATA_PORT, value);
}