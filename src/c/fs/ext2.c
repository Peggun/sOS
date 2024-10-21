#include "ext2.h"
#include "../drivers/disk/disk.h"
#include "../drivers/vga/vga.h"

// Global variable to hold the superblock for easy access
struct ext2_super_block ext2_sb;

void ext2_mount(void) {
    read_superblock(&ext2_sb);

    // Check if it's a valid ext2 file system
    if (ext2_sb.s_magic != EXT2_MAGIC_NUMBER) {
        print_string("Invalid ext2 filesystem\n");
        return;
    }

    print_string("ext2 filesystem mounted\n");
}

void ext2_umount(void) {
    // Optionally, save state or clean up resources here
    print_string("ext2 filesystem unmounted\n");
}