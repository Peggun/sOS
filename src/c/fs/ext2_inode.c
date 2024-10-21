#include "ext2_inode.h"
#include "../drivers/disk/disk.h"

// Function to get the starting block of the inode table (hardcoded for now)
uint32_t get_inode_table_block() {
    return 5; // Assume it's at block group 5 for now (you can calculate it from the block group descriptor table)
}

void read_inode(uint32_t inode_num, struct ext2_inode *inode) {
    uint32_t inode_table_block = get_inode_table_block();
    uint32_t inode_offset = (inode_num - 1) * sizeof(struct ext2_inode);
    
    disk_read(inode_table_block * EXT2_BLOCK_SIZE + inode_offset, inode, sizeof(struct ext2_inode));
}

void write_inode(uint32_t inode_num, const struct ext2_inode *inode) {
    uint32_t inode_table_block = get_inode_table_block();
    uint32_t inode_offset = (inode_num - 1) * sizeof(struct ext2_inode);
    
    disk_write(inode_table_block * EXT2_BLOCK_SIZE + inode_offset, inode, sizeof(struct ext2_inode));
}
