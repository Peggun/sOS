#include "ext2_superblock.h"
#include "../drivers/disk/disk.h"

#define EXT2_SUPERBLOCK_OFFSET 1024

void read_superblock(struct ext2_super_block *sb) {
    // Superblock starts at offset 1024 (block 1)
    disk_read(EXT2_SUPERBLOCK_OFFSET, (void *)sb, sizeof(struct ext2_super_block));
}

void write_superblock(const struct ext2_super_block *sb) {
    // Superblock starts at offset 1024 (block 1)
    disk_write(EXT2_SUPERBLOCK_OFFSET, (const void *)sb, sizeof(struct ext2_super_block));
}
