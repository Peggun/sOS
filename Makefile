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
CURSOR_OBJ = $(BUILD_DIR)/cursor.o
PORT_OBJ = $(BUILD_DIR)/port.o
MBR = $(BUILD_DIR)/mbr.bin
OS_IMAGE = $(BUILD_DIR)/os-image.bin

# Default target
all: $(ISO) $(OS_IMAGE)

# Compile MBR
$(MBR): $(ASM_DIR)/mbr/mbr.asm $(ASM_DIR)/gdt/gdt.asm $(ASM_DIR)/disk/disk.asm $(ASM_DIR)/switchbitmode/switch-to-32bit.asm
	@mkdir -p $(BUILD_DIR)
	$(NASM) -f bin -o $(MBR) $< -I $(ASM_DIR)/gdt/ -I $(ASM_DIR)/disk/ -I $(ASM_DIR)/switchbitmode/

# Compile kernel.c into kernel.o
$(KERNEL_OBJ): $(KERNEL_DIR)/kernel.c
	$(GCC) -m32 -ffreestanding -nostdlib -fno-pie -c $(KERNEL_DIR)/kernel.c -o $(KERNEL_OBJ)

# Compile vga.c into vga.o
$(VGA_OBJ): $(VGA_DIR)/vga.c $(VGA_DIR)/cursor.h
	$(GCC) -m32 -ffreestanding -nostdlib -fno-pie -c $(VGA_DIR)/vga.c -o $(VGA_OBJ)

# Compile cursor.c into cursor.o
$(CURSOR_OBJ): $(VGA_DIR)/cursor.c $(VGA_DIR)/ports.c
	$(GCC) -m32 -ffreestanding -nostdlib -fno-pie -c $(VGA_DIR)/cursor.c -o $(CURSOR_OBJ)

# Compile ports.c into port.o
$(PORT_OBJ): $(VGA_DIR)/ports.c
	$(GCC) -m32 -ffreestanding -nostdlib -fno-pie -c $(VGA_DIR)/ports.c -o $(PORT_OBJ)

# Link Kernel with VGA, Cursor, and Port
$(KERNEL): $(KERNEL_OBJ) $(VGA_OBJ) $(CURSOR_OBJ) $(PORT_OBJ)
	@mkdir -p $(BUILD_DIR)
	$(LD) -Ttext 0x1000 --oformat binary -nostdlib -e kernel_main -o $(KERNEL) $(KERNEL_OBJ) $(VGA_OBJ) $(CURSOR_OBJ) $(PORT_OBJ)

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
$(ISO): $(OS_IMAGE) $(GRUB_CFG)
	@mkdir -p $(BUILD_DIR)/isodir/boot
	cp $(KERNEL) $(BUILD_DIR)/isodir/boot/
	grub-mkrescue -o $@ $(BUILD_DIR)/isodir

# Create image for testing
$(OS_IMAGE): $(MBR) $(KERNEL)
	cat $^ > $@

# Run in QEMU 
run: $(OS_IMAGE)
	$(QEMU) -fda $(OS_IMAGE) -boot d -d int,cpu_reset,exec -serial stdio -debugcon file:logs/debug.log

# Clean
clean:
	rm -rf $(BUILD_DIR)/*