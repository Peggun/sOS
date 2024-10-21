#include "disk.h"
#include "../vga/vga.h"
#include "../vga/ports.h"

#define SECTOR_SIZE 512  // Typical size of a sector
#define ATA_TIMEOUT 10000  // Timeout for waiting on the disk

// Helper function to wait for the disk to be ready for data transfer (DRQ flag)
static int disk_wait_for_data_ready() {
    int timeout = ATA_TIMEOUT;
    unsigned char status;

    print_string("Waiting for disk to be ready for data transfer...\n");
    while (--timeout) {
        status = port_byte_in(ATA_PRIMARY_IO_BASE + ATA_REG_STATUS);
        if ((status & ATA_SR_BSY) == 0 && (status & ATA_SR_DRQ)) {
            print_string("Disk is ready for data transfer.\n");
            return 1;  // Disk is ready to transfer data
        }
    }
    print_string("Disk wait for data timed out.\n");
    return 0;  // Timed out, disk not ready
}

// Reads a sector from the disk into a buffer
void disk_read(uint32_t sector, void *buffer, uint32_t size) {
    if (size > SECTOR_SIZE) {
        print_string("Error: Attempt to read more than a sector size.\n");
        return;
    }

    print_string("Reading sector ");
    print_hex(sector);
    print_string(" from the disk...\n");

    // Select the drive (Master)
    port_byte_out(ATA_PRIMARY_IO_BASE + ATA_REG_DRIVE, 0xE0 | ((sector >> 24) & 0x0F));
    print_string("Drive selected.\n");

    // Send the sector number (LBA) to read from
    port_byte_out(ATA_PRIMARY_IO_BASE + ATA_REG_SECCOUNT, 1);                      // Read one sector
    port_byte_out(ATA_PRIMARY_IO_BASE + ATA_REG_LBA_LOW, (sector & 0xFF));
    port_byte_out(ATA_PRIMARY_IO_BASE + ATA_REG_LBA_MID, (sector >> 8) & 0xFF);
    port_byte_out(ATA_PRIMARY_IO_BASE + ATA_REG_LBA_HIGH, (sector >> 16) & 0xFF);
    print_string("Sector number sent.\n");

    // Send the read command
    port_byte_out(ATA_PRIMARY_IO_BASE + ATA_REG_COMMAND, ATA_CMD_READ_PIO);
    print_string("Read command sent to the disk.\n");

    // Wait for the disk to be ready to transfer data
    if (!disk_wait_for_data_ready()) {
        print_string("Error: Disk read timed out.\n");
        return;
    }

    // Read the data from the disk's data register (16 bits at a time)
    uint16_t *data = (uint16_t *)buffer;
    print_string("Reading data...\n");
    for (int i = 0; i < SECTOR_SIZE / 2; i++) {
        data[i] = port_word_in(ATA_PRIMARY_IO_BASE + ATA_REG_DATA);
    }

    print_string("Disk read completed.\n");
}

// Writes a sector to the disk from a buffer
void disk_write(uint32_t sector, const void *buffer, uint32_t size) {
    if (size > SECTOR_SIZE) {
        print_string("Error: Attempt to write more than a sector size.\n");
        return;
    }

    print_string("Writing sector ");
    print_hex(sector);
    print_string(" to the disk...\n");

    // Select the drive (Master)
    port_byte_out(ATA_PRIMARY_IO_BASE + ATA_REG_DRIVE, 0xE0 | ((sector >> 24) & 0x0F));
    print_string("Drive selected.\n");

    // Send the sector number (LBA) to write to
    port_byte_out(ATA_PRIMARY_IO_BASE + ATA_REG_SECCOUNT, 1);                       // Write one sector
    port_byte_out(ATA_PRIMARY_IO_BASE + ATA_REG_LBA_LOW, (sector & 0xFF));
    port_byte_out(ATA_PRIMARY_IO_BASE + ATA_REG_LBA_MID, (sector >> 8) & 0xFF);
    port_byte_out(ATA_PRIMARY_IO_BASE + ATA_REG_LBA_HIGH, (sector >> 16) & 0xFF);
    print_string("Sector number sent.\n");

    // Send the write command
    port_byte_out(ATA_PRIMARY_IO_BASE + ATA_REG_COMMAND, ATA_CMD_WRITE_PIO);
    print_string("Write command sent to the disk.\n");

    // Wait for the disk to be ready to receive data
    if (!disk_wait_for_data_ready()) {
        print_string("Error: Disk write timed out.\n");
        return;
    }

    // Write the data to the disk's data register (16 bits at a time)
    const uint16_t *data = (const uint16_t *)buffer;
    print_string("Writing data...\n");
    for (int i = 0; i < SECTOR_SIZE / 2; i++) {
        port_word_out(ATA_PRIMARY_IO_BASE + ATA_REG_DATA, data[i]);
    }

    print_string("Disk write completed.\n");
}

// Helper function to wait until the disk is not busy
int disk_wait_for_ready() {
    int timeout = ATA_TIMEOUT; // Define a timeout period

    print_string("Waiting for disk to be ready...\n");
    
    while (timeout--) {
        unsigned char status = port_byte_in(ATA_PRIMARY_IO_BASE + ATA_REG_STATUS);
        
        // Debugging output for status flags
        print_string("Current status: ");
        print_hex(status);  // Show current status register value

        // Check BSY flag
        if ((status & ATA_SR_BSY) == 0) { 
            print_string("Disk is not busy.\n"); // Debug message
            return (status & ATA_SR_DRDY); // Return whether the disk is ready
        }
    }
    
    print_string("Disk initialization timed out.\n");
    return 0; // Disk is not ready
}

int disk_init() {
    print_string("Initializing disk...\n");
    
    // Wait for the drive to be ready
    if (!disk_wait_for_ready()) {
        print_string("Disk not ready (BSY flag still set).\n");
        return 0;  // Disk is not ready
    }

    // Select the drive (Master) and send IDENTIFY command
    port_byte_out(ATA_PRIMARY_IO_BASE + ATA_REG_DRIVE, 0xA0); // Select master drive
    port_byte_out(ATA_PRIMARY_IO_BASE + ATA_REG_COMMAND, ATA_CMD_IDENTIFY);

    // Wait for the response from the disk
    unsigned char status;
    int timeout = ATA_TIMEOUT;

    // Poll the status register until BSY is cleared
    while (timeout--) {
        status = port_byte_in(ATA_PRIMARY_IO_BASE + ATA_REG_STATUS);
        print_string("Waiting for disk response, current status: ");
        print_hex(status);  // Show current status register value

        // Check if BSY is cleared
        if ((status & ATA_SR_BSY) == 0) {
            break;  // BSY cleared, exit loop
        }
    }

    if (timeout <= 0) {
        print_string("Disk initialization timed out while waiting for BSY to clear.\n");
        return 0;  // Timeout during initialization
    }

    // After BSY is cleared, check the DRDY status
    if (!(status & ATA_SR_DRDY)) {
        print_string("Drive not ready (DRDY flag not set).\n");
        return 0;  // Drive not ready
    }

    // Now read the sector count register to ensure drive is ready for data transfer
    unsigned char sector_count = port_byte_in(ATA_PRIMARY_IO_BASE + ATA_REG_SECCOUNT);
    print_string("Sector count: ");
    print_hex(sector_count);

    // Disk initialized successfully
    print_string("Disk initialized successfully.\n");
    return 1;
}
