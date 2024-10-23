#include "../../../include/ata.h"
#include "../../../include/ports.h"
#include "../../../include/disk.h"
#include "../../../include/ext2.h"
#include "../../../include/vga.h"

void initialize_ata_disk() {
    // ATA initialization process
    ata_initialize();

    // Wait until the drive is ready for commands
    ata_wait_for_ready();
}

int initialize_ext2(uint8_t *buffer) {
    // Try to read the ext2 superblock
    if (read_superblock(buffer) != 0) {
        // If the superblock is invalid, return an error code
        return -1;
    }

    // If the superblock is valid, the disk contains an ext2 filesystem
    return 0;
}

void kernel_disk_init() {
    // Buffer to store data read from the disk (superblock, etc.)
    uint8_t buffer[512];

    print_string("[DEBUG] Starting disk and filesystem initialization...\n");

    // Step 1: Initialize the ATA disk
    ata_initialize();

    // Step 2: Initialize the ext2 filesystem
    if (read_superblock(buffer) != 0) {
        print_string("[ERROR] ext2 filesystem initialization failed!\n");
        return;
    }

    print_string("[DEBUG] ext2 filesystem initialization succeeded.\n");

    // Step 3: Read and debug inode
    read_inode(buffer, 2, /* inode table block */ 5);

    // Step 4: Optionally, read the root directory and debug
    read_directory(buffer, /* root directory block number */ 8);

    print_string("[DEBUG] Disk and filesystem initialization complete.\n");
}
