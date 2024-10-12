# Define variables
NASM = nasm
LD = ld -m elf_i386
GCC = gcc
QEMU = qemu-system-i386
BUILD_DIR = build
ASM_DIR = src/asm/bootloader
KERNEL_DIR = src/c/kernel
VGA_DIR = src/c/drivers/vga
BOOTLOADER = $(BUILD_DIR)/bootloader.bin
BOOTLOADER_OBJ = $(BUILD_DIR)/bootloader.o
ISO = $(BUILD_DIR)/boot.iso
GRUB_CFG = $(BUILD_DIR)/isodir/boot/grub/grub.cfg
KERNEL = $(BUILD_DIR)/kernel.bin
KERNEL_OBJ = $(BUILD_DIR)/kernel.o
VGA_OBJ = $(BUILD_DIR)/vga.o
LINKER_SCRIPT = $(ASM_DIR)/linker.ld

# Default target
all: $(ISO)

# Assemble the bootloader using the linker script
$(BOOTLOADER): $(ASM_DIR)/bootloader.asm $(LINKER_SCRIPT)
	@mkdir -p $(BUILD_DIR)
	$(NASM) -f elf -o $(BOOTLOADER_OBJ) $<
	$(LD) -T $(LINKER_SCRIPT) -o $(BOOTLOADER) $(BOOTLOADER_OBJ) --oformat binary

# Compile kernel source file
$(KERNEL): $(KERNEL_DIR)/kernel.c $(VGA_DIR)/vga.c
	@mkdir -p $(BUILD_DIR)
	$(GCC) -m32 -ffreestanding -nostdlib -fno-pie -c $(KERNEL_DIR)/kernel.c -o $(KERNEL_OBJ)
	$(GCC) -m32 -ffreestanding -nostdlib -fno-pie -c $(VGA_DIR)/vga.c -o $(VGA_OBJ)
	$(LD) -m elf_i386 -Ttext 0x1000 --oformat binary -nostdlib -e kernel_main -o $(KERNEL) $(KERNEL_OBJ) $(VGA_OBJ)

# Create GRUB config file
$(GRUB_CFG):
	@mkdir -p $(BUILD_DIR)/isodir/boot/grub
	echo 'set timeout=0' > $(GRUB_CFG)
	echo 'set default=0' >> $(GRUB_CFG)
	echo 'menuentry "Shitty OS" {' >> $(GRUB_CFG)
	echo 'multiboot /boot/kernel.bin' >> $(GRUB_CFG)
	echo 'boot' >> $(GRUB_CFG)
	echo '}' >> $(GRUB_CFG)

# Create ISO image with GRUB
$(ISO): $(BOOTLOADER) $(KERNEL) $(GRUB_CFG)
	@mkdir -p $(BUILD_DIR)/isodir/boot
	cp $(KERNEL) $(BUILD_DIR)/isodir/boot/
	grub-mkrescue -o $@ $(BUILD_DIR)/isodir

# Run in QEMU
run: $(ISO)
	$(QEMU) -kernel $(ISO) -boot d

# Clean build files
clean:
	rm -rf $(BUILD_DIR)/*