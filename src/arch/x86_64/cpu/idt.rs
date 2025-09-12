use spin::Mutex;

use crate::{arch::x86_64::{cpu::{isr::isr0, lidt, make_segment_selector}, dev::qemu_debug}, qemu_println};

#[repr(C, packed)]
pub struct IdtPointer {
    limit: u16,
    base: u64,
}

#[derive(Clone, Copy)]
#[repr(C, packed)]
struct IdtDescriptor {
    offset_0: u16,
    selector: u16,
    ist_reserved: u8,
    type_attr: u8,
    offset_1: u16,
    offset_2: u32,
    reserved: u32,
}

lazy_static::lazy_static! {
    static ref IDT: Mutex<[IdtDescriptor; 256]> = Mutex::new([IdtDescriptor {
        offset_0: 0,
        selector: 0,
        ist_reserved: 0,
        type_attr: 0,
        offset_1: 0,
        offset_2: 0,
        reserved: 0,
    }; 256]);
}

#[repr(C, packed)]
struct Registers {
    ds: u64,

    r15: u64,
    r14: u64,
    r13: u64,
    r12: u64,
    r11: u64,
    r10: u64,
    r9: u64,
    r8: u64,
    rbp: u64,
    rdi: u64,
    rsi: u64,
    rdx: u64,
    rcx: u64,
    rbx: u64,
    rax: u64,

    int_no: u64,
    err_code: u64,

    rip: u64,
    cs: u64,
    rflags: u64,
    rsp: u64,
    ss: u64,
}

pub enum IDTGateType {
    InterruptGate = 0b1110,
    TrapGate = 0b1111,
}

pub fn set_idt_gate(vector: u8, handler: extern "C" fn(), segment: u16, ist: u8, gate_type: IDTGateType, dpl: u8, present: bool) {
    let mut idt = IDT.lock();
    idt[vector as usize].offset_0 = (handler as u64 & 0xFFFF) as u16;
    idt[vector as usize].offset_1 = ((handler as u64 >> 16) & 0xFFFF) as u16;
    idt[vector as usize].offset_2 = (handler as u64 >> 32) as u32;
    idt[vector as usize].selector = segment;
    idt[vector as usize].ist_reserved = ist;
    idt[vector as usize].type_attr = (gate_type as u8 & 0x0F) | ((dpl & 0b11) << 5) | ((present as u8) << 7);
}

pub fn init() {
    set_idt_gate(0, isr0, make_segment_selector(1, 0, 0), 0, IDTGateType::InterruptGate, 0, true);

    let idt_ptr = IdtPointer {
        limit: (core::mem::size_of::<IdtDescriptor>() * 256 - 1) as u16,
        base: &IDT.lock()[0] as *const IdtDescriptor as u64,
    };

    lidt(&idt_ptr);
}

#[unsafe(no_mangle)]
extern "C" fn common_isr(registers: &mut Registers) {
    let int_no = registers.int_no;
    qemu_println!("Received interrupt: {}", int_no);
    panic!("Unhandled interrupt: {}", int_no);
}