#![no_std]
#![no_main]

pub mod arch;

use core::panic::PanicInfo;

pub fn kmain() -> ! {
    loop {}
}

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}