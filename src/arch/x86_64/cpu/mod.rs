use core::arch::asm;

use crate::arch::x86_64::cpu::idt::IdtPointer;

pub mod gdt;
pub mod idt;
pub mod isr;

#[inline]
pub fn lgdt(gdt_ptr: &gdt::GdtPointer) {
    unsafe {
        asm!("lgdt [{}]", in(reg) gdt_ptr, options(nostack, readonly));
    }
}

#[inline]
pub fn lidt(idt_ptr: &IdtPointer) {
    unsafe {
        asm!("lidt [{}]", in(reg) idt_ptr, options(nostack, readonly));
    }
}

#[inline]
pub const fn make_segment_selector(index: u16, table: u8, rpl: u8) -> u16 {
    (index << 3) | (((table as u16) & 0b1) << 2) | (rpl as u16 & 0b11)
}

#[unsafe(no_mangle)]
pub fn load_selectors() {
    unsafe {
        core::arch::asm!(
            "mov ax, {data_segment}",
            "mov ds, ax",
            "mov es, ax",
            "mov fs, ax",
            "mov ss, ax",
            "mov gs, ax",
            data_segment = const make_segment_selector(2, 0, 0),
            options(nostack)
        );
        load_cs();
    }
}

#[unsafe(no_mangle)]
extern "C" fn load_cs() {
    unsafe {
        asm!(
            "push {code_segment}",
            "lea rax, [2f + rip]",
            "push rax",
            "retfq",
            "2:",
            code_segment = const make_segment_selector(1, 0, 0),
            options(nostack),
            lateout("rax") _
        );
    }
}

pub fn outb(port: u16, value: u8) {
    unsafe {
        asm!("out dx, al", in("dx") port, in("al") value, options(nomem, nostack, preserves_flags));
    }
}
