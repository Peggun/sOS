# Define variables
NASM = nasm
LD = ld -m elf_i386
GCC = gcc
QEMU = qemu-system-i386
BUILD_DIR = build
ASM_DIR = src/asm
KERNEL_DIR = src/c/kernel
VGA_DIR = src/c/drivers/vga
ISO = $(BUILD_DIR)/boot.iso
GRUB_CFG = $(BUILD_DIR)/isodir/boot/grub/grub.cfg
KERNEL = $(BUILD_DIR)/kernel.bin
KERNEL_OBJ = $(BUILD_DIR)/kernel.o
VGA_OBJ = $(BUILD_DIR)/vga.o
MBR = $(BUILD_DIR)/mbr.bin
OS_IMAGE = $(BUILD_DIR)/os-image.bin

# Default target
all: $(ISO) $(OS_IMAGE)

# Compile MBR
$(MBR): $(ASM_DIR)/mbr/mbr.asm $(ASM_DIR)/gdt/gdt.asm $(ASM_DIR)/disk/disk.asm $(ASM_DIR)/switchbitmode/switch-to-32bit.asm
	@mkdir -p $(BUILD_DIR)
	$(NASM) -f bin -o $(MBR) $< -I $(ASM_DIR)/gdt/ -I $(ASM_DIR)/disk/ -I $(ASM_DIR)/switchbitmode/

# Compile kernel
$(KERNEL): $(KERNEL_DIR)/kernel.c $(VGA_DIR)/vga.c
	@mkdir -p $(BUILD_DIR)
	$(GCC) -m32 -ffreestanding -nostdlib -fno-pie -c $(KERNEL_DIR)/kernel.c -o $(KERNEL_OBJ)
	$(GCC) -m32 -ffreestanding -nostdlib -fno-pie -c $(VGA_DIR)/vga.c -o $(VGA_OBJ)
	$(LD) -m elf_i386 -Ttext 0x1000 --oformat binary -nostdlib -e kernel_main -o $(KERNEL) $(KERNEL_OBJ) $(VGA_OBJ)

# GRUB configuration
$(GRUB_CFG):
	@mkdir -p $(BUILD_DIR)/isodir/boot/grub
	echo 'set timeout=0' > $(GRUB_CFG)
	echo 'set default=0' >> $(GRUB_CFG)
	echo 'menuentry "Shitty OS" {' >> $(GRUB_CFG)
	echo 'multiboot /boot/kernel.bin' >> $(GRUB_CFG)
	echo 'boot' >> $(GRUB_CFG)
	echo '}' >> $(GRUB_CFG)

# Create ISO
$(ISO): $(MBR) $(KERNEL) $(GRUB_CFG)
	@mkdir -p $(BUILD_DIR)/isodir/boot
	cp $(KERNEL) $(BUILD_DIR)/isodir/boot/
	grub-mkrescue -o $@ $(BUILD_DIR)/isodir

# Create image for testing for now. 
$(OS_IMAGE): $(MBR) $(KERNEL)
	cat $^ > $@

# Run in QEMU
run: $(P)
	$(QEMU) -fda $(OS_IMAGE)

# Clean
clean:
	rm -rf $(BUILD_DIR)/*