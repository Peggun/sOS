#include "stdint.h"

#define ATA_PRIMARY_IO  0x1F0
#define ATA_PRIMARY_CTRL 0x3F6

void ata_wait_for_ready();
void ata_initialize();
void ata_read_sector(uint32_t lba, uint8_t *buffer);
void read_directory(uint8_t *buffer, uint32_t block);
