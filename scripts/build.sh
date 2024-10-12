#!/bin/bash

# Navigate to the asm directory
cd src/asm/bootloader

# Create build directory if it doesn't exist
mkdir -p ../../../build

# Assemble the bootloader
if ! as -o bootloader.o bootloader.s; then
    echo "Error assembling bootloader"
    exit 1
fi

# Link the bootloader to create a raw binary output
if ! ld -o bootloader.bin --oformat binary -e init -Ttext 0x7c00 bootloader.o; then
    echo "Error linking bootloader"
    exit 1
fi

# Create a bootable image (ensure the image is large enough for the bootloader)
dd if=/dev/zero of=../../../build/boot.img bs=512 count=2048

# Copy the bootloader binary to the boot image
dd if=bootloader.bin of=../../../build/boot.img conv=notrunc

# Navigate to the C files directory
cd src/c

# Compile the kernel and vga files
gcc -m32 -ffreestanding -c src/c/kernel/kernel.c -o kernel.o
gcc -m32 -ffreestanding -c src/c/drivers/vga/vga.c -o vga.o

# Link the kernel and vga to create a kernel binary
if ! ld -o kernel.bin -Ttext 0x1000 --oformat binary kernel.o vga.o; then
    echo "Error linking kernel"
    exit 1
fi

# Append the kernel binary to the boot image (at the correct offset)
dd if=kernel.bin of=../../../build/boot.img conv=notrunc oflag=append bs=512 seek=2

# Run the bootloader in QEMU
qemu-system-x86_64 -drive format=raw,file=../../../build/boot.img
