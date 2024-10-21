#ifndef EXT2_INODE_H
#define EXT2_INODE_H

#include "../types.h"

// Inode structure definition
struct ext2_inode {
    uint16_t i_mode;                // File mode
    uint16_t i_uid;                 // Owner ID
    uint32_t i_size;                // Size in bytes
    uint32_t i_atime;               // Access time
    uint32_t i_ctime;               // Creation time
    uint32_t i_mtime;               // Modification time
    uint32_t i_dtime;               // Deletion time
    uint16_t i_gid;                 // Group ID
    uint16_t i_links_count;         // Links count
    uint32_t i_blocks;              // Block count
    uint32_t i_block[15];           // Pointers to data blocks (0-11 direct, 12 singly indirect, 13 doubly indirect, 14 triply indirect)
};

void read_inode(uint32_t inode_num, struct ext2_inode *inode);
void write_inode(uint32_t inode_num, const struct ext2_inode *inode);

#endif // EXT2_INODE_H
