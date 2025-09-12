use core::arch::asm;

use limine::{
    BaseRevision,
    request::{RequestsEndMarker, RequestsStartMarker},
};

use crate::{arch::x86_64::cpu::{gdt, idt}, qemu_println};

#[used]
#[unsafe(link_section = ".requests")]
static BASE_REVISION: BaseRevision = BaseRevision::new();

#[used]
#[unsafe(link_section = ".requests_start_marker")]
static _START_MARKER: RequestsStartMarker = RequestsStartMarker::new();

#[used]
#[unsafe(link_section = ".requests_end_marker")]
static _END_MARKER: RequestsEndMarker = RequestsEndMarker::new();

#[unsafe(no_mangle)]
pub extern "C" fn _start() -> ! {
    assert!(BASE_REVISION.is_supported());

    init();
}

pub fn init() -> ! {
    qemu_println!("Initializing GDT...");
    gdt::init();
    qemu_println!("GDT initialized.");
    qemu_println!("Initializing IDT...");
    idt::init();
    qemu_println!("IDT initialized.");

    unsafe {
        asm!("int 0x00");
    }

    crate::kmain();
}
