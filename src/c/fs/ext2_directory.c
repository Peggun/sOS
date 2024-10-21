#include "ext2_directory.h"
#include "../drivers/disk/disk.h"

// Custom strncmp function
int strncmp(const char *str1, const char *str2, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (str1[i] != str2[i] || str1[i] == '\0' || str2[i] == '\0') {
            return (unsigned char)str1[i] - (unsigned char)str2[i];
        }
    }
    return 0;
}

// Custom strlen function
size_t strlen(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

// Custom strcpy function
char *strcpy(char *dest, const char *src) {
    char *d = dest;
    while ((*d++ = *src++) != '\0');
    return dest;
}

int find_file_in_directory(struct ext2_inode *dir_inode, const char *filename, struct ext2_dir_entry *dir_entry) {
    // Loop through the directory's data blocks and look for the file
    for (int i = 0; i < dir_inode->i_size / EXT2_BLOCK_SIZE; i++) {
        uint32_t block = dir_inode->i_block[i];
        disk_read(block * EXT2_BLOCK_SIZE, dir_entry, sizeof(struct ext2_dir_entry));
        
        if (strncmp(dir_entry->name, filename, dir_entry->name_len) == 0) {
            return 1; // File found
        }
    }
    return 0; // File not found
}

void create_directory_entry(struct ext2_inode *dir_inode, const char *filename, uint32_t inode) {
    struct ext2_dir_entry new_entry;
    new_entry.inode = inode;
    new_entry.name_len = strlen(filename);
    strcpy(new_entry.name, filename);
    
    // Write the new directory entry to the disk
    for (int i = 0; i < dir_inode->i_size / EXT2_BLOCK_SIZE; i++) {
        uint32_t block = dir_inode->i_block[i];
        disk_write(block * EXT2_BLOCK_SIZE, &new_entry, sizeof(struct ext2_dir_entry));
        break; // For now, we assume the directory has free space at the first block
    }
}
