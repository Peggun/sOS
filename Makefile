# Define variables
NASM = nasm
LD = ld -m elf_i386
GCC = gcc
QEMU = qemu-system-i386
BUILD_DIR = build
ASM_DIR = src/asm
KERNEL_DIR = src/c/kernel
VGA_DIR = src/c/drivers/vga
KEYBOARD_DIR = src/c/drivers/keyboard
DISK_DIR = src/c/drivers/disk
EXT2_DIR = src/c/fs
ISO = $(BUILD_DIR)/boot.iso
GRUB_CFG = $(BUILD_DIR)/isodir/boot/grub/grub.cfg
KERNEL = $(BUILD_DIR)/kernel.bin
KERNEL_OBJ = $(BUILD_DIR)/kernel.o
VGA_OBJ = $(BUILD_DIR)/vga.o
CURSOR_OBJ = $(BUILD_DIR)/cursor.o
PORT_OBJ = $(BUILD_DIR)/port.o
ISR_OBJ = $(BUILD_DIR)/isr.o
IDT_OBJ = $(BUILD_DIR)/idt.o
KEYBOARD_OBJ = $(BUILD_DIR)/keyboard.o
UTIL_OBJ = $(BUILD_DIR)/util.o
INTERRUPT_OBJ = $(BUILD_DIR)/interrupt.o
CMDS_OBJ = $(BUILD_DIR)/cmds.o
DISK_OBJ = $(BUILD_DIR)/disk.o
EXT2_SUPERBLOCK_OBJ = $(BUILD_DIR)/ext2_superblock.o
EXT2_INODE_OBJ = $(BUILD_DIR)/ext2_inode.o
EXT2_DIR_OBJ = $(BUILD_DIR)/ext2_directory.o
EXT2_OBJ = $(BUILD_DIR)/ext2.o
MBR = $(BUILD_DIR)/mbr.bin
OS_IMAGE = $(BUILD_DIR)/os-image.bin

# Default target
all: $(ISO) $(OS_IMAGE)

# Compile MBR
$(MBR): $(ASM_DIR)/mbr.asm $(ASM_DIR)/gdt.asm $(ASM_DIR)/disk.asm $(ASM_DIR)/switch-to-32bit.asm $(ASM_DIR)/print-16bit.asm $(ASM_DIR)/print-32bit.asm
	@mkdir -p $(BUILD_DIR)
	$(NASM) -f bin -o $(MBR) $< -I $(ASM_DIR)

# Compile kernel.c into kernel.o
$(KERNEL_OBJ): $(KERNEL_DIR)/kernel.c
	$(GCC) -m32 -ffreestanding  -fno-pie -c $(KERNEL_DIR)/kernel.c -o $(KERNEL_OBJ)

# Compile vga.c into vga.o
$(VGA_OBJ): $(VGA_DIR)/vga.c $(VGA_DIR)/cursor.h
	$(GCC) -m32 -ffreestanding  -fno-pie -c $(VGA_DIR)/vga.c -o $(VGA_OBJ)

# Compile cursor.c into cursor.o
$(CURSOR_OBJ): $(VGA_DIR)/cursor.c $(VGA_DIR)/ports.c
	$(GCC) -m32 -ffreestanding  -fno-pie -c $(VGA_DIR)/cursor.c -o $(CURSOR_OBJ)

# Compile ports.c into port.o
$(PORT_OBJ): $(VGA_DIR)/ports.c
	$(GCC) -m32 -ffreestanding  -fno-pie -c $(VGA_DIR)/ports.c -o $(PORT_OBJ)

# Compile isr.c into isr.o
$(ISR_OBJ): $(KERNEL_DIR)/isr.c $(KERNEL_DIR)/isr.h $(KERNEL_DIR)/idt.h $(VGA_DIR)/ports.h
	$(GCC) -m32 -ffreestanding  -fno-pie -c $(KERNEL_DIR)/isr.c -o $(ISR_OBJ)

# Compile idt.c into idt.o
$(IDT_OBJ): $(KERNEL_DIR)/idt.c $(KERNEL_DIR)/idt.h
	$(GCC) -m32 -ffreestanding  -fno-pie -c $(KERNEL_DIR)/idt.c -o $(IDT_OBJ)

# Compile keyboard.c into keyboard.o
$(KEYBOARD_OBJ): $(KEYBOARD_DIR)/keyboard.c $(KERNEL_DIR)/isr.h $(KERNEL_DIR)/idt.h $(VGA_DIR)/ports.h
	$(GCC) -m32 -ffreestanding  -fno-pie -c $(KEYBOARD_DIR)/keyboard.c -o $(KEYBOARD_OBJ)

# Compile util.c into util.o
$(UTIL_OBJ): $(KERNEL_DIR)/util.c
	$(GCC) -m32 -ffreestanding  -fno-pie -c $(KERNEL_DIR)/util.c -o $(UTIL_OBJ)

# Compile interrupt.asm into interrupt.o
$(INTERRUPT_OBJ): $(KERNEL_DIR)/interrupt.asm
	$(NASM) -f elf32 -o $(INTERRUPT_OBJ) $(KERNEL_DIR)/interrupt.asm

# Compile cmds.c into cmds.o
$(CMDS_OBJ): $(KERNEL_DIR)/cmds.c
	$(GCC) -m32 -ffreestanding  -fno-pie -c $(KERNEL_DIR)/cmds.c -o $(CMDS_OBJ)

# Compile disk.c into disk.o
$(DISK_OBJ): $(DISK_DIR)/disk.c $(DISK_DIR)/disk.h
	$(GCC) -m32 -ffreestanding -fno-pie -c $(DISK_DIR)/disk.c -o $(DISK_OBJ)

# Compile ext2_superblock.c into ext2_superblock.o
$(EXT2_SUPERBLOCK_OBJ): $(EXT2_DIR)/ext2_superblock.c $(EXT2_DIR)/ext2.h
	$(GCC) -m32 -ffreestanding -fno-pie -c $(EXT2_DIR)/ext2_superblock.c -o $(EXT2_SUPERBLOCK_OBJ)

# Compile ext2_inode.c into ext2_inode.o
$(EXT2_INODE_OBJ): $(EXT2_DIR)/ext2_inode.c $(EXT2_DIR)/ext2.h
	$(GCC) -m32 -ffreestanding -fno-pie -c $(EXT2_DIR)/ext2_inode.c -o $(EXT2_INODE_OBJ)

# Compile ext2_directory.c into ext2_directory.o
$(EXT2_DIR_OBJ): $(EXT2_DIR)/ext2_directory.c $(EXT2_DIR)/ext2.h
	$(GCC) -m32 -ffreestanding -fno-pie -c $(EXT2_DIR)/ext2_directory.c -o $(EXT2_DIR_OBJ)

# Compile ext2.c into ext2.o
$(EXT2_OBJ): $(EXT2_DIR)/ext2.c $(EXT2_DIR)/ext2.h
	$(GCC) -m32 -ffreestanding -fno-pie -c $(EXT2_DIR)/ext2.c -o $(EXT2_OBJ)


# Link Kernel with all object files
$(KERNEL): $(KERNEL_OBJ) $(VGA_OBJ) $(CURSOR_OBJ) $(PORT_OBJ) $(ISR_OBJ) $(IDT_OBJ) $(KEYBOARD_OBJ) $(UTIL_OBJ) $(INTERRUPT_OBJ) $(CMDS_OBJ) $(DISK_OBJ) $(EXT2_SUPERBLOCK_OBJ) $(EXT2_INODE_OBJ) $(EXT2_DIR_OBJ) $(EXT2_OBJ)
	@mkdir -p $(BUILD_DIR)
	$(LD) -Ttext 0x1000 --oformat binary -e kernel_main -o $(KERNEL) $(KERNEL_OBJ) $(VGA_OBJ) $(CURSOR_OBJ) $(PORT_OBJ) $(ISR_OBJ) $(IDT_OBJ) $(KEYBOARD_OBJ) $(UTIL_OBJ) $(INTERRUPT_OBJ) $(CMDS_OBJ) $(DISK_OBJ) $(EXT2_SUPERBLOCK_OBJ) $(EXT2_INODE_OBJ) $(EXT2_DIR_OBJ) $(EXT2_OBJ)

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
	cp $(OS_IMAGE) $(BUILD_DIR)/isodir/boot/
	grub-mkrescue -o $@ $(BUILD_DIR)/isodir

# Create image for testing
$(OS_IMAGE): $(MBR) $(KERNEL)
	cat $^ > $@

$(BUILD_DIR)/disk.img:
#qemu-img create -f raw $(BUILD_DIR)/disk.img 64M
	dd if=/dev/zero of=$(BUILD_DIR)/disk.img bs=1M count=16
	mkfs.ext2 $(BUILD_DIR)/disk.img

# Run in QEMU 
run: $(OS_IMAGE) $(BUILD_DIR)/disk.img
	$(QEMU) -hda $(BUILD_DIR)/disk.img -fda $(OS_IMAGE) -d guest_errors,int,cpu_reset -boot a

# Clean
clean:
	rm -rf $(BUILD_DIR)/*