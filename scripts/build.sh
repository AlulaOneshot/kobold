#!/bin/bash
set -e

rustup component add rust-src --toolchain nightly-x86_64-unknown-linux-gnu
cargo build
python3 scripts/makeImage.py image.iso x86_64 target/x86_64-kobold/debug/kobold res/limine.conf
clear
qemu-system-x86_64 -cdrom image.iso -m 512M -smp 2 -debugcon stdio -display none