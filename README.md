# Limine Meson C Starter Kernel

## Setup

To setup your environment, run the following commands. These are REQUIRED or your kernel may not build correctly. If you are not familiar with the terminal or dont have one, quit now. You aren't ready for OSDev.

```bash
chmod +x scripts/setupenv.sh
./scripts/setupenv.sh
```

## Build

Now that your environment is setup, you can build and run your kernel.
First, choose the architecture you want to build for. This template supports x86_64, aarch64, and riscv64. Once you have chosen your target architecture, you need to get the name of the cross file for you build. The cross file tells meson what tools to use to build your kernel. All cross files are located in the `res/cross-files/` folder. The cross file for x86_64, for example, would be `res/cross-files/x86_64-gcc.ini`.

To setup a meson build directory, run the following command.

```bash
meson setup builddir --cross-file=res/cross-files/{your_target_arch}-gcc.ini
```

Now, you should see a directory named builddir. This is where meson will place your built kernel and disk image. To build your kernel and a disk image, run the following command.

```bash
meson compile -C builddir
```

To run your image, you must run a different command depending on what architecture you built it for. If you are running in a graphical environment (i.e. KDE Plasma, Windows, MacOS, GNOME), you can omit the `-nographic`. This was added because I created this template in github codespaces.

### x86_64
```bash
qemu-system-x86_64 -M q35 -cdrom builddir/image.iso -nographic
```

### aarch64
```bash
qemu-system-aarch64 -M virt -cpu cortex-a72 -device qemu-xhci -device usb-kbd -cdrom builddir/image.iso -nographic
```

### riscv64
```bash
qemu-system-riscv64 -M virt -cpu rv64 -device qemu-xhci -device usb-kbd -cdrom builddir/image.iso -nographic
```