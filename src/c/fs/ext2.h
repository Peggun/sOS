#ifndef EXT2_H
#define EXT2_H

#include "ext2_superblock.h"
#include "ext2_inode.h"
#include "ext2_directory.h"

void ext2_mount(void);
void ext2_umount(void);

#endif // EXT2_H