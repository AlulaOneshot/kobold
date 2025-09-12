use core::arch::{asm, naked_asm};

#[unsafe(naked)]
#[unsafe(no_mangle)]
pub extern "C" fn isr_common_asm() {
    naked_asm!(
        "cli",
        "push rax",
        "push rbx",
        "push rcx",
        "push rdx",
        "push rsi",
        "push rdi",
        "push rbp",
        "push r8",
        "push r9",
        "push r10",
        "push r11",
        "push r12",
        "push r13",
        "push r14",
        "push r15",

        "mov rbp, ds",
        "push rbp",

        "mov rdi, rsp",
        "call common_isr",

        "pop rbp",
        "mov ds, rbp",

        "pop r15",
        "pop r14",
        "pop r13",
        "pop r12",
        "pop r11",
        "pop r10",
        "pop r9",
        "pop r8",
        "pop rbp",
        "pop rdi",
        "pop rsi",
        "pop rdx",
        "pop rcx",
        "pop rbx",
        "pop rax",

        "add rsp, 16", // pop errcode + vector

        "sti",
        "iretq",
    );
}

macro_rules! isr_stub {
    ($name:ident, $num:expr) => {
        #[unsafe(naked)]
        #[unsafe(no_mangle)]
        pub extern "C" fn $name() {
                naked_asm!(
                    "push 0",
                    concat!("push ", stringify!($num)),
                    "jmp isr_common_asm",
                );
        }
    };
}

macro_rules! isr_stub_err {
    ($name:ident, $num:expr) => {
        #[unsafe(naked)]
        #[unsafe(no_mangle)]
        pub extern "C" fn $name() {
                naked_asm!(
                    concat!("push ", stringify!($num)),
                    "jmp isr_common_asm",
                );
        }
    };
}

isr_stub!(isr0, 0);