#ifndef EXT2_SUPERBLOCK_H
#define EXT2_SUPERBLOCK_H

#include "../types.h"

// EXT2 Magic Number
#define EXT2_MAGIC_NUMBER 0xEF53

// Superblock structure definition
struct ext2_super_block {
    uint32_t s_inodes_count;        // Total number of inodes
    uint32_t s_blocks_count;        // Total number of blocks
    uint32_t s_r_blocks_count;      // Number of reserved blocks
    uint32_t s_free_blocks_count;   // Free blocks count
    uint32_t s_free_inodes_count;   // Free inodes count
    uint32_t s_first_data_block;    // First Data Block
    uint32_t s_log_block_size;      // Block size (logarithmic)
    uint32_t s_log_frag_size;       // Fragment size (logarithmic)
    uint32_t s_blocks_per_group;    // Blocks per group
    uint32_t s_frags_per_group;     // Fragments per group
    uint32_t s_inodes_per_group;    // Inodes per group
    uint32_t s_mtime;               // Last mount time
    uint32_t s_wtime;               // Last write time
    uint16_t s_mnt_count;           // Mount count
    uint16_t s_max_mnt_count;       // Max mount count before a check
    uint16_t s_magic;               // Magic signature (0xEF53)
    uint16_t s_state;               // File system state
    uint16_t s_errors;              // Behaviour when detecting errors
    uint16_t s_minor_rev_level;     // Minor revision level
    uint32_t s_lastcheck;           // Time of last check
    uint32_t s_checkinterval;       // Max time between checks
    uint32_t s_creator_os;          // OS ID
    uint32_t s_rev_level;           // Revision level
    uint16_t s_def_resuid;          // Default UID for reserved blocks
    uint16_t s_def_resgid;          // Default GID for reserved blocks
    // Additional fields can be added as needed
};

void read_superblock(struct ext2_super_block *sb);
void write_superblock(const struct ext2_super_block *sb);

#endif // EXT2_SUPERBLOCK_H