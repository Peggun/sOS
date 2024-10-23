#include "stdint.h"

void initialize_ata_disk();
int initialize_ext2(uint8_t *buffer);
void kernel_disk_init();