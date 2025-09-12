use crate::arch::x86_64::cpu::{lgdt, load_selectors};

#[repr(C, packed)]
struct Gdt {
    null: GdtEntry,
    kernel_code: GdtEntry,
    kernel_data: GdtEntry,
    user_code: GdtEntry,
    user_data: GdtEntry,
}

#[repr(C, packed)]
struct GdtEntry {
    limit_0: u16,
    base_0: u16,
    base_1: u8,
    access: u8,
    flags_limit_2: u8,
    base_2: u8,
}

#[repr(transparent)]
struct GdtAccessByte(u8);

impl GdtAccessByte {
    const fn new(
        accessed: bool,
        read_write: bool,
        direction_conforming: bool,
        executable: bool,
        descriptor_type: bool,
        dpl: u8,
        present: bool,
    ) -> GdtAccessByte {
        GdtAccessByte(
            (accessed as u8) & 0b00000001
                | ((read_write as u8) << 1) & 0b00000010
                | ((direction_conforming as u8) << 2) & 0b00000100
                | ((executable as u8) << 3) & 0b00001000
                | ((descriptor_type as u8) << 4) & 0b00010000
                | ((dpl & 0b11) << 5) & 0b01100000
                | ((present as u8) << 7) & 0b10000000,
        )
    }
}

impl GdtEntry {
    const fn new(
        base: u64,
        limit: u32,
        long_mode_code: bool,
        granularity: bool,
        size: bool,
        access: GdtAccessByte,
    ) -> GdtEntry {
        GdtEntry {
            limit_0: (limit & 0xFFFF) as u16,
            base_0: (base & 0xFFFF) as u16,
            base_1: ((base >> 16) & 0xFF) as u8,
            access: access.0,
            flags_limit_2: ((((((size as u8) << 2) & 0b0100)
                | (((granularity as u8) << 3) & 0b1000)
                | (((long_mode_code as u8) << 1) & 0b0010))
                & 0x0F)
                << 4)
                | (((limit >> 16) & 0x0F) as u8),
            base_2: ((base >> 24) & 0xFF) as u8,
        }
    }
}

#[repr(C, packed)]
pub struct GdtPointer {
    limit: u16,
    base: u64,
}

static GDT: Gdt = Gdt {
    null: GdtEntry::new(
        0,
        0,
        false,
        false,
        false,
        GdtAccessByte::new(false, false, false, false, false, 0, false),
    ),
    kernel_code: GdtEntry::new(
        0,
        0xFFFFFFFF,
        true,
        true,
        false,
        GdtAccessByte::new(true, true, false, true, true, 0, true),
    ),
    kernel_data: GdtEntry::new(
        0,
        0xFFFFFFFF,
        false,
        true,
        true,
        GdtAccessByte::new(true, true, false, false, true, 0, true),
    ),
    user_code: GdtEntry::new(
        0,
        0xFFFFFFFF,
        true,
        true,
        false,
        GdtAccessByte::new(true, true, false, true, true, 3, true),
    ),
    user_data: GdtEntry::new(
        0,
        0xFFFFFFFF,
        false,
        true,
        true,
        GdtAccessByte::new(true, true, false, false, true, 3, true),
    ),
};

pub fn init() {
    let descriptor = GdtPointer {
        limit: core::mem::size_of::<Gdt>() as u16 - 1,
        base: &GDT as *const _ as u64,
    };

    lgdt(&descriptor);

    load_selectors();
}
