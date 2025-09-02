#include <uacpi/kernel_api.h>
#include <uacpi/acpi.h>
#include <uacpi/uacpi.h>
#include <uacpi/sleep.h>
#include <uacpi/resources.h>
#include <uacpi/tables.h>
#include <uacpi/uacpi.h>
#include <uacpi/utilities.h>
#include <arch/x86_64/cpu.h>
#include <arch/x86_64/mm.h>
// Start with some UACPI Glue

void panic(const char *msg) {
    // For now, just a simple infinite loop
    // In a real kernel, you'd want to log this and halt the system properly
    printf("PANIC: %s\n", msg);
    while (1) { asm volatile("hlt"); }
}

uint64_t rsdpAddress = 0;

uacpi_status uacpi_kernel_get_rsdp(uacpi_phys_addr *out_rsdp_address) {
    if (rsdpAddress == 0) {
        return UACPI_STATUS_NOT_FOUND;
    }

    *out_rsdp_address = rsdpAddress;
    return UACPI_STATUS_OK;
}

void *uacpi_kernel_map(uacpi_phys_addr addr, uacpi_size len) {
    return vmMapPageRange(kernelPML4, addr, len);
}

void uacpi_kernel_unmap(void *addr, uacpi_size len) {
    vmUnmapPageRange(kernelPML4, addr, len);
}

void uacpi_kernel_log(uacpi_log_level level, const uacpi_char *msg) {
    switch(level) {
        case UACPI_LOG_DEBUG:
            printf("ACPI DEBUG: %s\n", msg);
            break;
        case UACPI_LOG_INFO:
            printf("ACPI INFO: %s\n", msg);
            break;
        case UACPI_LOG_WARN:
            printf("ACPI WARNING: %s\n", msg);
            break;
        case UACPI_LOG_ERROR:
            printf("ACPI ERROR: %s\n", msg);
            break;
        case UACPI_LOG_TRACE:
            printf("ACPI TRACE: %s\n", msg);
            break;
    }
}

uacpi_status uacpi_kernel_pci_device_open(
    uacpi_pci_address address, uacpi_handle *out_handle
) {
    uacpi_pci_address *addr = (uacpi_pci_address *)vmGetPages(NULL, sizeof(uacpi_pci_address));
    if (!addr) return UACPI_STATUS_OUT_OF_MEMORY;
    *addr = address;
    *out_handle = (uacpi_handle *)addr;
}

void uacpi_kernel_pci_device_close(uacpi_handle handle) {
    vmUnmapPageRange(kernelPML4, handle, sizeof(uacpi_pci_address));
}

uacpi_status uacpi_kernel_pci_read8(uacpi_handle device, uacpi_size offset, uacpi_u8 *value) {
    uacpi_pci_address *address = (uacpi_pci_address *)device;
    
    *value = configReadWordPCI(address->bus, address->device, address->function,
                          offset & ~1);
    *value = (*value >> ((offset & 1) * 8)) & 0xFF;
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_pci_read16(uacpi_handle device, uacpi_size offset, uacpi_u16 *value) {
    uacpi_pci_address *address = (uacpi_pci_address *)device;

    *value = configReadWordPCI(address->bus, address->device, address->function,
                          offset & ~1);
    *value = (*value >> ((offset & 1) * 8));
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_pci_read32(uacpi_handle device, uacpi_size offset, uacpi_u32 *value) {
    if (!value) return UACPI_STATUS_INVALID_ARGUMENT;
    uacpi_pci_address *a = (uacpi_pci_address *)device;
    *value = configReadDoubleWordPCI(a->bus, a->device, a->function, offset); // FIX
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_pci_write8(uacpi_handle device, uacpi_size offset, uacpi_u8 v) {
    uacpi_pci_address *a = (uacpi_pci_address *)device;
    uacpi_u32 d = configReadDoubleWordPCI(a->bus, a->device, a->function, offset & ~0x3); // FIX
    uacpi_u32 sh = (offset & 0x3) * 8;
    d = (d & ~(0xFFu << sh)) | ((uacpi_u32)v << sh);
    configWriteDoubleWordPCI(a->bus, a->device, a->function, offset & ~0x3, d);
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_pci_write16(uacpi_handle device, uacpi_size offset, uacpi_u16 v) {
    uacpi_pci_address *a = (uacpi_pci_address *)device;
    uacpi_u32 d = configReadDoubleWordPCI(a->bus, a->device, a->function, offset & ~0x3); // FIX
    uacpi_u32 sh = (offset & 0x2) * 8;  // 0 or 16
    d = (d & ~(0xFFFFu << sh)) | ((uacpi_u32)v << sh);
    configWriteDoubleWordPCI(a->bus, a->device, a->function, offset & ~0x3, d);
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_pci_write32(uacpi_handle device, uacpi_size offset, uacpi_u32 v) {
    uacpi_pci_address *a = (uacpi_pci_address *)device;
    configWriteDoubleWordPCI(a->bus, a->device, a->function, offset, v);
    return UACPI_STATUS_OK; // FIX
}


uacpi_status uacpi_kernel_io_map(uacpi_io_addr base, uacpi_size len, uacpi_handle *out_handle) {
    *out_handle = (uacpi_handle) base;
	return UACPI_STATUS_OK;
}

void uacpi_kernel_io_unmap(uacpi_handle handle) {
    // No action needed for x86 I/O ports
}

uacpi_status uacpi_kernel_io_read8(uacpi_handle handle, uacpi_size offset, uacpi_u8 *out_value) {
    uacpi_io_addr target = (uacpi_io_addr)handle + offset;
    *out_value = inb(target);
}

uacpi_status uacpi_kernel_io_read16(uacpi_handle handle, uacpi_size offset, uacpi_u16 *out_value) {
    uacpi_io_addr target = (uacpi_io_addr)handle + offset;
    *out_value = inw(target);
}

uacpi_status uacpi_kernel_io_read32(uacpi_handle handle, uacpi_size offset, uacpi_u32 *out_value) {
    uacpi_io_addr target = (uacpi_io_addr)handle + offset;
    *out_value = inl(target);
}

uacpi_status uacpi_kernel_io_write8(uacpi_handle handle, uacpi_size offset, uacpi_u8 in_value) {
    uacpi_io_addr target = (uacpi_io_addr)handle + offset;
    outb(target, in_value);
}

uacpi_status uacpi_kernel_io_write16(uacpi_handle handle, uacpi_size offset, uacpi_u16 in_value) {
    uacpi_io_addr target = (uacpi_io_addr)handle + offset;
    outw(target, in_value);
}

uacpi_status uacpi_kernel_io_write32(uacpi_handle handle, uacpi_size offset, uacpi_u32 in_value) {
    uacpi_io_addr target = (uacpi_io_addr)handle + offset;
    outl(target, in_value);
}

void *uacpi_kernel_alloc(uacpi_size size) {
    return vmGetPages(NULL, size);
}

void uacpi_kernel_free(void *mem, uacpi_size size_hint) {
    if (mem) {
        vmUnmapPageRange(kernelPML4, mem, size_hint);
    }
}

// Return ns since boot using TSC (bring-up quality)
uacpi_u64 uacpi_kernel_get_nanoseconds_since_boot(void) {
    // TODO: replace 3 GHz with calibrated TSC frequency
    return (rdtsc() * 1000000000ULL) / 3000000000ULL;
}

void uacpi_kernel_stall(uacpi_u32 usec) {         // FIX: widen type
    uacpi_u64 start = uacpi_kernel_get_nanoseconds_since_boot();
    uacpi_u64 target_ns = (uacpi_u64)usec * 1000ULL;
    while (uacpi_kernel_get_nanoseconds_since_boot() - start < target_ns) {
        __asm__ __volatile__("pause");
    }
}

void uacpi_kernel_sleep(uacpi_u64 msec) {         // FIX: convert units
    // Bring-up: busy-wait using stall
    if (msec > 0) {
        // capped loops avoid 32-bit overflow
        while (msec > 1000) { uacpi_kernel_stall(1000 * 1000); msec -= 1000; }
        uacpi_kernel_stall((uacpi_u32)(msec * 1000ULL));
    }
}


// We're just going to use a simple spinlock for now
uacpi_handle uacpi_kernel_create_mutex() {
    return uacpi_kernel_create_spinlock();
}

void uacpi_kernel_free_mutex(uacpi_handle handle) {
    uacpi_kernel_free_spinlock(handle);
}

uacpi_handle uacpi_kernel_create_event(void) {
    panic("uACPI events are not implemented yet");
    return NULL;
}

void uacpi_kernel_free_event(uacpi_handle handle) {
    panic("uACPI events are not implemented yet");
}

uacpi_thread_id uacpi_kernel_get_thread_id(void) {
    return 0; // Single-threaded for now
}

uacpi_status uacpi_kernel_acquire_mutex(uacpi_handle handle, uacpi_u16 timeout) {
    bool *lock = (bool *)handle;
    size_t start = uacpi_kernel_get_nanoseconds_since_boot();
    size_t target = timeout == 0xFFFF ? (size_t)-1 : timeout * 1000000ULL;

    if (timeout == 0) {
        // Non-blocking attempt
        if (__atomic_test_and_set(lock, __ATOMIC_ACQUIRE) == false) {
            return UACPI_STATUS_OK;
        } else {
            return UACPI_STATUS_TIMEOUT;
        }
    } else {
        while (true) {
            if (__atomic_test_and_set(lock, __ATOMIC_ACQUIRE) == false) {
                return UACPI_STATUS_OK;
            }
            if (timeout != 0xFFFF && (uacpi_kernel_get_nanoseconds_since_boot() - start) >= target) {
                return UACPI_STATUS_TIMEOUT;
            }
            // Briefly yield to avoid busy-waiting too aggressively
            uacpi_kernel_stall(1);
        }
    }
}

void uacpi_kernel_release_mutex(uacpi_handle handle) {
    bool *lock = (bool *)handle;
    __atomic_clear(lock, __ATOMIC_RELEASE);
}

uacpi_bool uacpi_kernel_wait_for_event(uacpi_handle, uacpi_u16) {
    panic("uACPI events are not implemented yet");
    return UACPI_FALSE;
}

void uacpi_kernel_signal_event(uacpi_handle) {
    panic("uACPI events are not implemented yet");
}

void uacpi_kernel_reset_event(uacpi_handle) {
    panic("uACPI events are not implemented yet");
}

uacpi_status uacpi_kernel_handle_firmware_request(uacpi_firmware_request*) {
    panic("uACPI firmware requests are not implemented yet");
    return UACPI_STATUS_UNIMPLEMENTED;
}

uacpi_status uacpi_kernel_install_interrupt_handler(uacpi_u32 irq, uacpi_interrupt_handler handler, uacpi_handle ctx, uacpi_handle *out_irq_handle) {
    setIDTEntry(irq, (void *)handler, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_DPL0 | IDT_PRESENT | IDT_TYPE_INTERRUPT_GATE);
    *out_irq_handle = (uacpi_handle)(uintptr_t)irq;
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_uninstall_interrupt_handler(uacpi_interrupt_handler, uacpi_handle irq_handle) {
    uacpi_u32 irq = (uacpi_u32)(uintptr_t)irq_handle;
    setIDTEntry(irq, NULL, makeSegmentSelector(0, 0, 0), IDT_NO_IST, ~IDT_PRESENT);
    return UACPI_STATUS_OK;
}

uacpi_handle uacpi_kernel_create_spinlock() {
    bool *lock = (bool *)vmGetPages(NULL, sizeof(bool));
    if (lock) {
        *lock = false;
    }
    return (uacpi_handle)lock;
}

void uacpi_kernel_free_spinlock(uacpi_handle handle) {
    vmUnmapPageRange(kernelPML4, handle, sizeof(bool));
}

uacpi_cpu_flags uacpi_kernel_lock_spinlock(uacpi_handle handle) {
    int intState = areInterruptsEnabled();
    while (__atomic_test_and_set((bool *)handle, __ATOMIC_ACQUIRE));
    return intState;
}

void uacpi_kernel_unlock_spinlock(uacpi_handle handle, uacpi_cpu_flags intState) {
    __atomic_clear((bool *)handle, __ATOMIC_RELEASE);

    if (intState) {
        asm volatile("sti");
    }
    else {
        asm volatile("cli");
    }
}

uacpi_status uacpi_kernel_schedule_work(uacpi_work_type, uacpi_work_handler, uacpi_handle ctx) {
    panic("uACPI work scheduling is not implemented yet");
    return UACPI_STATUS_UNIMPLEMENTED;
}

uacpi_status uacpi_kernel_wait_for_work_completion(void) {
    panic("uACPI work scheduling is not implemented yet");
    return UACPI_STATUS_UNIMPLEMENTED;
}

// End UACPI Glue

void initACPI(uint64_t address) {
    rsdpAddress = address;

    uacpi_status ret = uacpi_initialize(0);

    ret = uacpi_namespace_load();

    uacpi_set_interrupt_model(UACPI_INTERRUPT_MODEL_PIC);

    ret = uacpi_namespace_initialize();
}