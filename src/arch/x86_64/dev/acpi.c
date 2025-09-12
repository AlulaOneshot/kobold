#include <arch/x86_64/dev.h>
#include <arch/x86_64/cpu.h>
#include <arch/x86_64/printf.h>
#include <arch/x86_64/time.h>
#include <uacpi/kernel_api.h>
#include <uacpi/acpi.h>
#include <uacpi/uacpi.h>
#include <memory.h>
#include <math.h>

static uint64_t rsdpAddress;

void initialiseACPI(uint64_t rsdpPhysicalAddr) {
    rsdpAddress = rsdpPhysicalAddr;

    uacpi_status status = uacpi_initialize(0);
    if (uacpi_unlikely_error(status)) {
        printf("ACPI initialization failed");
        asm volatile("cli");
        while (1) asm volatile("hlt");
    }

    status = uacpi_namespace_load();
    if (uacpi_unlikely_error(status)) {
        printf("ACPI initialization failed");
        asm volatile("cli");
        while (1) asm volatile("hlt");
    }
}

// Begin uACPI glue

uacpi_status uacpi_kernel_get_rsdp(uacpi_phys_addr *out_rsdp_address) {
    if (rsdpAddress == 0) return UACPI_STATUS_NOT_FOUND;
    *out_rsdp_address = rsdpAddress;
    return UACPI_STATUS_OK;
}

void *uacpi_kernel_map(uacpi_phys_addr addr, uacpi_size len) {
    printf("Mapping physical address %p, len %lu\n", (void *)addr, len);
    uacpi_phys_addr aligned = ALIGN_DOWN(addr, PAGE_SIZE);
    len += addr - aligned;
    len = ALIGN_UP(len, PAGE_SIZE);
    void *vaddr = (void *)((uint64_t)vmGetSpace(aligned, len / PAGE_SIZE) + (addr - aligned));
    printf("Mapped to virtual address %p\n", vaddr);
    return vaddr;
}

void uacpi_kernel_unmap(void *vaddr, uacpi_size len) {
    printf("Unmapping virtual address %p, len %lu\n", vaddr, len);
    uint64_t aligned = ALIGN_DOWN((uint64_t)vaddr, PAGE_SIZE);
    len += (uint64_t)vaddr - aligned;
    len = ALIGN_UP(len, PAGE_SIZE);
    vmFreeSpace((void *)aligned, len / PAGE_SIZE);
    printf("Unmapped virtual address %p\n", vaddr);
}

void uacpi_kernel_log(uacpi_log_level level, const uacpi_char *msg) {
    switch (level) {
        case UACPI_LOG_ERROR:
            printf("uACPI ERROR: %s\n", msg);
            break;
        case UACPI_LOG_WARN:
            printf("uACPI WARNING: %s\n", msg);
            break;
        case UACPI_LOG_INFO:
            printf("uACPI INFO: %s\n", msg);
            break;
        case UACPI_LOG_DEBUG:
            printf("uACPI DEBUG: %s\n", msg);
            break;
        case UACPI_LOG_TRACE:
            printf("uACPI TRACE: %s\n", msg);
            break;
        default:
            printf("uACPI LOG[%d]: %s\n", level, msg);
            break;
    }
}

uacpi_status uacpi_kernel_pci_device_open(uacpi_pci_address address, uacpi_handle *out_handle) {
    pci_device_t *device = malloc(sizeof(pci_device_t));
    device->segment = address.segment;
    device->bus = address.bus;
    device->slot = address.device;
    device->function = address.function;

    // Comment copied bc its literally the same lol
    // TODO: this should not just create a device and push it,
    // it REALLY needs to first check if we are aware of such a device
    addPCIDevice(device);

    *out_handle = device;

    return UACPI_STATUS_OK;
}

void uacpi_kernel_pci_device_close(uacpi_handle handle) {
    removePCIDevice((pci_device_t *)handle);
    free(handle);
}

uacpi_status uacpi_kernel_pci_read8(uacpi_handle handle, uacpi_size offset, uacpi_u8 *value) {
    pci_device_t *device = (pci_device_t *)handle;
    *value = readPCIConfig8(device, offset);
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_pci_read16(uacpi_handle handle, uacpi_size offset, uacpi_u16 *value) {
    pci_device_t *device = (pci_device_t *)handle;
    *value = readPCIConfig16(device, offset);
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_pci_read32(uacpi_handle handle, uacpi_size offset, uacpi_u32 *value) {
    pci_device_t *device = (pci_device_t *)handle;
    *value = readPCIConfig32(device, offset);
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_pci_write8(uacpi_handle handle, uacpi_size offset, uacpi_u8 value) {
    pci_device_t *device = (pci_device_t *)handle;
    writePCIConfig8(device, offset, value);
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_pci_write16(uacpi_handle handle, uacpi_size offset, uacpi_u16 value) {
    pci_device_t *device = (pci_device_t *)handle;
    writePCIConfig16(device, offset, value);
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_pci_write32(uacpi_handle handle, uacpi_size offset, uacpi_u32 value) {
    pci_device_t *device = (pci_device_t *)handle;
    writePCIConfig32(device, offset, value);
    return UACPI_STATUS_OK;
}

typedef struct {
    uacpi_io_addr base;
    uacpi_size len;
} uacpi_io_range_t;

uacpi_status uacpi_kernel_io_map(uacpi_io_addr base, uacpi_size len, uacpi_handle *out_handle) {
    if(base + len > 0xFFFF) return UACPI_STATUS_INVALID_ARGUMENT;
    uacpi_io_range_t *range = malloc(sizeof(uacpi_io_range_t));
    range->base = base;
    range->len = len;
    *out_handle = range;
    return UACPI_STATUS_OK;
}

void uacpi_kernel_io_unmap(uacpi_handle handle) {
    uacpi_io_range_t *range = (uacpi_io_range_t *)handle;
    free(range);
}

uacpi_status uacpi_kernel_io_read8(uacpi_handle handle, uacpi_size offset, uacpi_u8 *out_value) {
    uacpi_io_range_t *range = (uacpi_io_range_t *)handle;
    if(offset >= range->len) return UACPI_STATUS_INVALID_ARGUMENT;
    *out_value = inb(range->base + offset);
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_io_read16(uacpi_handle handle, uacpi_size offset, uacpi_u16 *out_value) {
    uacpi_io_range_t *range = (uacpi_io_range_t *)handle;
    if(offset >= range->len) return UACPI_STATUS_INVALID_ARGUMENT;
    *out_value = inw(range->base + offset);
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_io_read32(uacpi_handle handle, uacpi_size offset, uacpi_u32 *out_value) {
    uacpi_io_range_t *range = (uacpi_io_range_t *)handle;
    if(offset >= range->len) return UACPI_STATUS_INVALID_ARGUMENT;
    *out_value = inl(range->base + offset);
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_io_write8(uacpi_handle handle, uacpi_size offset, uacpi_u8 in_value) {
    uacpi_io_range_t *range = (uacpi_io_range_t *)handle;
    if(offset >= range->len) return UACPI_STATUS_INVALID_ARGUMENT;
    outb(range->base + offset, in_value);
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_io_write16(uacpi_handle handle, uacpi_size offset, uacpi_u16 in_value) {
    uacpi_io_range_t *range = (uacpi_io_range_t *)handle;
    if(offset >= range->len) return UACPI_STATUS_INVALID_ARGUMENT;
    outw(range->base + offset, in_value);
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_io_write32(uacpi_handle handle, uacpi_size offset, uacpi_u32 in_value) {
    uacpi_io_range_t *range = (uacpi_io_range_t *)handle;
    if(offset >= range->len) return UACPI_STATUS_INVALID_ARGUMENT;
    outl(range->base + offset, in_value);
    return UACPI_STATUS_OK;
}

void *uacpi_kernel_alloc(uacpi_size size) {
    printf("Allocating %lu bytes\n", size);
    void *mem = malloc(size);
    printf("Allocated at %p\n", mem);
    return mem;
}

void uacpi_kernel_free(void *mem) {
    free(mem);
}

uacpi_u64 uacpi_kernel_get_nanoseconds_since_boot(void) {
    return getTime() * 1000000; // Convert milliseconds to nanoseconds
}

void uacpi_kernel_stall(uacpi_u8 usec) {
    uint64_t start = getTime();
    while(getTime() < start + (usec / 1000) + 1); // +1 to ensure we wait at least the requested time
}

void uacpi_kernel_sleep(uacpi_u64 msec) {
    uint64_t start = getTime();
    while(getTime() < start + msec) {
        asm volatile("hlt"); // Sleep until the next interrupt
    }
}

typedef bool mutex_t;

uacpi_handle uacpi_kernel_create_mutex(void) {
    mutex_t *mutex = malloc(sizeof(mutex_t));
    *mutex = false; // Unlocked
    return mutex;
}

void uacpi_kernel_free_mutex(uacpi_handle handle) {
    mutex_t *mutex = (mutex_t *)handle;
    free(mutex);
}

typedef struct {
    uacpi_u16 counter; // Number of signals
    uacpi_u16 waiters; // Number of waiters
} event_t;

uacpi_handle uacpi_kernel_create_event(void) {
    event_t *event = malloc(sizeof(event_t));
    event->counter = 0; // Counter starts at 0
    event->waiters = 0; // No waiters initially
    return event;
}

void uacpi_kernel_free_event(uacpi_handle handle) {
    event_t *event = (event_t *)handle;
    free(event);
}

uacpi_thread_id uacpi_kernel_get_thread_id(void) {
    return (uacpi_thread_id) 0; // Since kernel is single-threaded for now, and especially when running ACPI code
}

uacpi_status uacpi_kernel_acquire_mutex(uacpi_handle handle, uacpi_u16 timeout) {
    mutex_t *mutex = (mutex_t *)handle;
    uint64_t start = getTime();
    if (timeout == 0) { // Non-blocking
        if (!*mutex) {
            *mutex = true;
            return UACPI_STATUS_OK;
        } else {
            return UACPI_STATUS_TIMEOUT;
        }
    } else if (timeout == 0xFFFF) { // Infinite wait
        while (*mutex) {
            asm volatile("hlt"); // Sleep until the next interrupt
        }
        *mutex = true;
        return UACPI_STATUS_OK;
    } else { // Wait for timeout milliseconds
        while (*mutex) {
            if (getTime() >= start + timeout) {
                return UACPI_STATUS_TIMEOUT;
            }
            asm volatile("hlt"); // Sleep until the next interrupt
        }
        *mutex = true;
        return UACPI_STATUS_OK;
    }
}

void uacpi_kernel_release_mutex(uacpi_handle handle) {
    mutex_t *mutex = (mutex_t *)handle;
    *mutex = false;
}

uacpi_bool uacpi_kernel_wait_for_event(uacpi_handle handle, uacpi_u16 timeout) {
    event_t *event = (event_t *)handle;
    uint64_t start = getTime();
    if (timeout == 0xFFFF) { // Infinite wait
        event->waiters++;
        while (event->counter == 0) {
            asm volatile("hlt"); // Sleep until the next interrupt
        }
        event->waiters--;
        event->counter--;
        return UACPI_TRUE;
    } else { // Wait for timeout milliseconds
        event->waiters++;
        while (event->counter == 0) {
            if (getTime() >= start + timeout) {
                event->waiters--;
                return UACPI_FALSE; // Timeout
            }
            asm volatile("hlt"); // Sleep until the next interrupt
        }
        event->waiters--;
        event->counter--;
        return UACPI_TRUE;
    }
}

void uacpi_kernel_signal_event(uacpi_handle handle) {
    event_t *event = (event_t *)handle;
    event->counter++;
    // If there are waiters, we could potentially wake one up here.
    // However, since our kernel is single-threaded for now, we don't need to do anything special.
}

void uacpi_kernel_reset_event(uacpi_handle handle) {
    event_t *event = (event_t *)handle;
    event->counter = 0;
}

uacpi_status uacpi_kernel_handle_firmware_request(uacpi_firmware_request *request) {
    switch (request->type) {
        case UACPI_FIRMWARE_REQUEST_TYPE_BREAKPOINT:
            printf("ACPI Breakpoint: Method Context: %p\n", request->breakpoint.ctx);
            return UACPI_STATUS_OK;
        case UACPI_FIRMWARE_REQUEST_TYPE_FATAL:
            printf("ACPI Fatal Error: Code: %d, Arg: %d, Type: %d\n", request->fatal.code, request->fatal.arg, request->fatal.type);
            asm volatile ("cli"); // Disable interrupts
            while (1) {
                asm volatile ("hlt");
            }
            return UACPI_STATUS_OK; // Never reached
        default:
            return UACPI_STATUS_INVALID_ARGUMENT;
    }
}

uacpi_status uacpi_kernel_install_interrupt_handler(uacpi_u32 irq, uacpi_interrupt_handler handler, uacpi_handle ctx, uacpi_handle *out_irq_handle) {
    printf("UNIMPLIMENTED:\n");
    return UACPI_STATUS_UNIMPLEMENTED; // TODO: Implement this
}

uacpi_status uacpi_kernel_uninstall_interrupt_handler(uacpi_interrupt_handler handler, uacpi_handle irq_handle) {
    printf("UNIMPLIMENTED:\n");
    return UACPI_STATUS_UNIMPLEMENTED; // TODO: Implement this
}

uacpi_handle uacpi_kernel_create_spinlock() {
    mutex_t *spinlock = malloc(sizeof(mutex_t));
    *spinlock = false; // Unlocked
    return spinlock;
}

void uacpi_kernel_free_spinlock(uacpi_handle handle) {
    mutex_t *spinlock = (mutex_t *)handle;
    free(spinlock);
}

uacpi_cpu_flags uacpi_kernel_lock_spinlock(uacpi_handle handle) {
    mutex_t *spinlock = (mutex_t *)handle;
    uacpi_cpu_flags flags = areInterruptsEnabled() ? 1 : 0;
    asm volatile ("cli"); // Disable interrupts
    while (__atomic_test_and_set(spinlock, __ATOMIC_ACQUIRE)) {
        // Spin-wait (do nothing)
    }
    return flags;
}

void uacpi_kernel_unlock_spinlock(uacpi_handle handle, uacpi_cpu_flags flags) {
    mutex_t *spinlock = (mutex_t *)handle;
    __atomic_clear(spinlock, __ATOMIC_RELEASE);
    if (flags & 1) {
        asm volatile ("sti"); // Re-enable interrupts if they were enabled before
    }
}

uacpi_status uacpi_kernel_schedule_work(uacpi_work_type type, uacpi_work_handler handler, uacpi_handle ctx) {
    printf("UNIMPLIMENTED:\n");
    // Implementation goes here
    return UACPI_STATUS_UNIMPLEMENTED;
}

uacpi_status uacpi_kernel_wait_for_work_completion(void) {
    // Implementation goes here
    printf("UNIMPLIMENTED:\n");
    return UACPI_STATUS_UNIMPLEMENTED;
}