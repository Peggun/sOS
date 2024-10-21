#ifndef EXT2_DIRECTORY_H
#define EXT2_DIRECTORY_H

#include "../types.h"
#include "ext2_inode.h"

// Directory entry structure definition
struct ext2_dir_entry {
    uint32_t inode;                 // Inode number
    uint16_t rec_len;               // Length of this directory record
    uint8_t name_len;               // Length of the name
    uint8_t file_type;              // Type of file (0 for unknown, 1 for regular file, 2 for directory, etc.)
    char name[];                    // Filename (null-terminated)
};

int find_file_in_directory(struct ext2_inode *dir_inode, const char *filename, struct ext2_dir_entry *dir_entry);
void create_directory_entry(struct ext2_inode *dir_inode, const char *filename, uint32_t inode);

#endif // EXT2_DIRECTORY_H
