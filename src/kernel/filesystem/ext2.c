#include "../../include/stdint.h"
#include "../../include/ext2.h"
#include "../../include/vga.h"
#include "../../include/ata.h"
#include "../../include/cursor.h"
#include "../../include/util.h"

// Function to read and verify the ext2 superblock
int read_superblock(uint8_t *buffer) {
    print_string("[DEBUG] Reading ext2 superblock...\n");

    // Read the second sector (LBA 2) where the superblock resides
    ata_read_sector(2, buffer);

    // Cast the buffer to a superblock structure
    struct ext2_superblock *sb = (struct ext2_superblock *) buffer; // Directly use buffer

    // Check if it's a valid ext2 filesystem
    if (sb->s_magic != EXT2_MAGIC) {
        print_string("[ERROR] Invalid ext2 filesystem! Superblock magic number is incorrect.\n");
        return -1; // Invalid filesystem
    }

    print_string("[DEBUG] ext2 filesystem detected. Superblock valid.\n");

    char inodes_count[16];
    char blocks_count[16];
    itoa(sb->s_inodes_count, inodes_count, 10);
    itoa(sb->s_blocks_count, blocks_count, 10);

    print_string("[DEBUG] Inodes count: ");
    print_string(inodes_count);
    print_string("\n[DEBUG] Blocks count: ");
    print_string(blocks_count);
    print_string("\n");

    return 0;
}


// Read the block group descriptor table
void read_block_group_descriptor(uint8_t *buffer) {
    // Read the block group descriptor table from disk (starts right after the superblock)
    ata_read_sector(3, buffer);

    // Cast to block group descriptor structure
    struct ext2_block_group_descriptor *bgd = (struct ext2_block_group_descriptor *) buffer;

    // Access fields like bgd->bg_inode_table to find the inode table location
}

void read_inode(uint8_t *buffer, uint32_t inode_index, uint32_t inode_table_block) {
    char inode_index_str[16], inode_table_block_str[16];
    itoa(inode_index, inode_index_str, 10);
    itoa(inode_table_block, inode_table_block_str, 10);

    print_string("[DEBUG] Reading inode ");
    print_string(inode_index_str);
    print_string(" from inode table block ");
    print_string(inode_table_block_str);
    print_string("...\n");

    // Calculate the LBA of the inode block
    uint32_t inode_block = inode_table_block + (inode_index * sizeof(struct ext2_inode)) / 512;
    
    // Read the inode block into the buffer
    ata_read_sector(inode_block, buffer);

    // Calculate the offset of the specific inode
    struct ext2_inode *inode = (struct ext2_inode *)(buffer + ((inode_index * sizeof(struct ext2_inode)) % 512));

    // Debug output of inode details
    print_string("[DEBUG] Inode ");
    print_string(inode_index_str);
    print_string(" read successfully.\n");

    char inode_size[16], first_block[16];
    itoa(inode->i_size, inode_size, 10);
    itoa(inode->i_block[0], first_block, 10);

    print_string("[DEBUG] Inode size: ");
    print_string(inode_size);
    print_string(" bytes\n[DEBUG] First data block: ");
    print_string(first_block);
    print_string("\n");
}


void read_directory(uint8_t *buffer, uint32_t block) {
    char block_str[16];
    itoa(block, block_str, 10);

    print_string("[DEBUG] Reading directory block ");
    print_string(block_str);
    print_string("...\n");

    // Read the directory block from disk
    ata_read_sector(block, buffer);

    // Cast the buffer to a directory entry structure
    struct ext2_dir_entry *entry = (struct ext2_dir_entry *) buffer;

    // Loop through the directory entries
    while (entry->inode != 0) {
        char inode_str[16];
        itoa(entry->inode, inode_str, 10);

        print_string("[DEBUG] Directory entry: inode = ");
        print_string(inode_str);
        print_string(", name = ");
        
        for (int i = 0; i < entry->name_len; i++) {
            print_char(entry->name[i], get_cursor());  // Assuming you have a `print_char` function
        }
        print_string("\n");

        // Move to the next directory entry
        entry = (struct ext2_dir_entry *)((uint8_t *) entry + entry->rec_len);
    }

    print_string("[DEBUG] Finished reading directory block ");
    print_string(block_str);
    print_string(".\n");
}


// Function to read file data
void read_file(uint8_t *buffer, struct ext2_inode *inode) {
    // Read the first data block of the file
    ata_read_sector(inode->i_block[0], buffer);

    // Now buffer contains the file data (up to 512 bytes)
}