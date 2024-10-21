#ifndef DISK_H
#define DISK_H

#include <stdint.h>

#define EXT2_BLOCK_SIZE 1024 // Define the block size for ext2

#define ATA_PRIMARY_IO_BASE   0x1F0  // Base I/O address for the primary ATA bus
#define ATA_REG_DATA          0x00
#define ATA_REG_ERROR         0x01
#define ATA_REG_SECCOUNT      0x02
#define ATA_REG_LBA_LOW       0x03
#define ATA_REG_LBA_MID       0x04
#define ATA_REG_LBA_HIGH      0x05
#define ATA_REG_DRIVE         0x06
#define ATA_REG_STATUS        0x07
#define ATA_REG_COMMAND       0x07

#define ATA_SR_BSY            0x80   // Busy
#define ATA_SR_DRDY           0x40   // Drive ready
#define ATA_SR_DRQ            0x08   // Data request

#define ATA_CMD_IDENTIFY      0xEC   // Command to identify the drive
#define ATA_SR_ERR            0x01   // Error flag in the status register

#define ATA_CMD_READ_PIO      0x20   // Read command
#define ATA_CMD_WRITE_PIO     0x30   // Write command

#define SECTOR_SIZE           512    // Size of each sector (in bytes)

void disk_read(uint32_t sector, void *buffer, uint32_t size);
void disk_write(uint32_t sector, const void *buffer, uint32_t size);
int disk_wait_for_ready();
int disk_init();

#endif // DISK_H
