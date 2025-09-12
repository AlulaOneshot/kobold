use core::fmt::Write;

use spin::Mutex;

use crate::arch::x86_64::cpu::outb;

pub fn putchar(c: char) {
    outb(0xE9, c as u8);
}

struct Writer {}

impl Writer {
    fn new() -> Self {
        Writer {}
    }

    fn write_str(&mut self, s: &str) {
        for c in s.chars() {
            putchar(c);
        }
    }
}

impl Write for Writer {
    fn write_str(&mut self, s: &str) -> core::fmt::Result {
        self.write_str(s);
        Ok(())
    }
}

lazy_static::lazy_static! {
    static ref WRITER: Mutex<Writer> = Mutex::new(Writer::new());
}

#[doc(hidden)]
pub fn _print(args: core::fmt::Arguments) {
    WRITER.lock().write_fmt(args).unwrap();
}

#[macro_export]
macro_rules! qemu_print {
    ($($arg:tt)*) => (crate::arch::x86_64::dev::qemu_debug::_print(format_args!($($arg)*)));
}

#[macro_export]
macro_rules! qemu_println {
    () => (print!("\n"));
    ($($arg:tt)*) => (crate::qemu_print!("{}\n", format_args!($($arg)*)));
}
