#include <arch/x86_64/cpu.h>
#include <arch/x86_64/mm.h>

#define PCI_CONFIG_ADDRESS_PORT 0xCF8
#define PCI_CONFIG_DATA_PORT 0xCFC

// TODO: Convert to Linked List
pci_device_t devices[256];
uint64_t deviceCount = 0;

void initPCI() {

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
    uint32_t original = readPCIConfig32(device, offset);

    
}