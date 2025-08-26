#!/usr/bin/env python3
import os, sys, subprocess, shutil, tempfile

if len(sys.argv) != 5:
    print(f"Usage: {sys.argv[0]} <output.iso> <arch> <kernel> <cfg>")
    sys.exit(1)

out_iso, arch, kernel, cfg = sys.argv[1:]

# Which EFI binary we need
limine_bin = {
    "x86_64": "BOOTX64.EFI",
    "riscv64": "BOOTRISCV64.EFI",
    "aarch64": "BOOTAA64.EFI",
}[arch]

# Clone & build limine (9.x branch)
limine_dir = os.path.abspath("limine")
if not os.path.isdir(limine_dir):
    subprocess.run([
        "git", "clone", "--depth=1",
        "--branch=v9.x-binary",
        "https://github.com/limine-bootloader/limine.git",
        limine_dir
    ], check=True)

subprocess.run(["make", "-C", limine_dir], check=True)

with tempfile.TemporaryDirectory() as iso_root:
    os.makedirs(os.path.join(iso_root, "boot"), exist_ok=True)
    shutil.copy(kernel, os.path.join(iso_root, "boot", "kernel"))
    os.makedirs(os.path.join(iso_root, os.path.join("boot", "limine")), exist_ok=True)
    shutil.copy(cfg, os.path.join(iso_root, "boot", "limine", "limine.cfg"))
    os.makedirs(os.path.join(iso_root, "EFI", "BOOT"), exist_ok=True)
    if arch == "x86_64":
        shutil.copy(os.path.join(limine_dir, "limine-bios.sys"), os.path.join(iso_root, "boot", "limine", "limine-bios.sys"))
        shutil.copy(os.path.join(limine_dir, "limine-bios-cd.bin"), os.path.join(iso_root, "boot", "limine", "limine-bios-cd.bin"))
        shutil.copy(os.path.join(limine_dir, "limine-uefi-cd.bin"), os.path.join(iso_root, "boot", "limine", "limine-uefi-cd.bin"))
        shutil.copy(os.path.join(limine_dir, "BOOTX64.EFI"), os.path.join(iso_root, "EFI", "BOOT", "BOOTX64.EFI"))
        shutil.copy(os.path.join(limine_dir, "BOOTIA32.EFI"), os.path.join(iso_root, "EFI", "BOOT", "BOOTIA32.EFI"))
        subprocess.run([
           "xorriso", "-as", "mkisofs", "-R", "-r", "-J", "-b", "boot/limine/limine-bios-cd.bin",
            "-no-emul-boot", "-boot-load-size", "4", "-boot-info-table", "-hfsplus",
            "-apm-block-size", "2048", "--efi-boot", f"boot/limine/limine-uefi-cd.bin",
            "-efi-boot-part", "--efi-boot-image", "--protective-msdos-label",
            iso_root, "-o", out_iso
        ])
        subprocess.run(
            [os.path.join(limine_dir, "limine"), "bios-install", out_iso],
        )
    elif arch == "aarch64":
        shutil.copy(os.path.join(limine_dir, "limine-uefi-cd.bin"), os.path.join(iso_root, "boot", "limine", "limine-uefi-cd.bin"))
        shutil.copy(os.path.join(limine_dir, "BOOTAA64.EFI"), os.path.join(iso_root, "EFI", "BOOT", "BOOTAA64.EFI"))
        subprocess.run([
            "xorriso", "-as", "mkisofs", "-R", "-r", "-J",
		    "-hfsplus", "-apm-block-size", "2048",
		    "--efi-boot", "boot/limine/limine-uefi-cd.bin",
		    "-efi-boot-part", "--efi-boot-image", "--protective-msdos-label",
		    iso_root, "-o", out_iso
            ])
    elif arch == "riscv64":
        shutil.copy(os.path.join(limine_dir, "limine-uefi-cd.bin"), os.path.join(iso_root, "boot", "limine", "limine-uefi-cd.bin"))
        shutil.copy(os.path.join(limine_dir, "BOOTRISCV64.EFI"), os.path.join(iso_root, "EFI", "BOOT", "BOOTRISCV64.EFI"))
        subprocess.run([
            "xorriso", "-as", "mkisofs", "-R", "-r", "-J",
		    "-hfsplus", "-apm-block-size", "2048",
		    "--efi-boot", "boot/limine/limine-uefi-cd.bin",
		    "-efi-boot-part", "--efi-boot-image", "--protective-msdos-label",
		    iso_root, "-o", out_iso
            ])