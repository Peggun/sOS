#include "../../../include/ports.h"
#include "../../../include/ata.h"
#include "../../../include/vga.h"
#include "../../../include/util.h"

void ata_wait_for_ready() {
    print_string("Waiting for ATA drive to be ready...\n");
    while (port_byte_in(ATA_PRIMARY_IO + 7) & 0x80);  // Wait for BSY flag to clear
    print_string("ATA drive is ready!\n");
}

void ata_initialize() {
    print_string("Initializing ATA disk...\n");
    port_byte_out(ATA_PRIMARY_CTRL, 0);  // Disable IRQs
    print_string("ATA disk initialized!\n");
}

void ata_read_sector(uint32_t lba, uint8_t *buffer) {
    char lba_str[16];
    itoa(lba, lba_str, 10);
    print_hex(lba);

    print_string("Reading sector ");
    print_string(lba_str);
    print_string(" from ATA disk...\n");

    // Select the drive and LBA mode
    port_byte_out(ATA_PRIMARY_IO + 6, 0xE0 | ((lba >> 24) & 0xF));

    // Send the LBA (Logical Block Address) to the drive
    port_byte_out(ATA_PRIMARY_IO + 1, 0x00);           // Null byte for features
    port_byte_out(ATA_PRIMARY_IO + 2, 1);              // Number of sectors to read (1 sector)
    port_byte_out(ATA_PRIMARY_IO + 3, (uint8_t) lba);  // LBA low byte
    port_byte_out(ATA_PRIMARY_IO + 4, (uint8_t)(lba >> 8));   // LBA mid byte
    port_byte_out(ATA_PRIMARY_IO + 5, (uint8_t)(lba >> 16));  // LBA high byte
    port_byte_out(ATA_PRIMARY_IO + 7, 0x20);           // Command: Read sectors with retry

    // Wait for the drive to be ready
    ata_wait_for_ready();

    // Check if there was an error after issuing the read command
    if (port_byte_in(ATA_PRIMARY_IO + 7) & 0x01) {
        print_string("[ERROR] Read command failed.\n");
        return;
    }

    // Read data into the buffer (512 bytes / 256 words)
    for (int i = 0; i < 256; i++) {
        uint16_t data = port_word_in(ATA_PRIMARY_IO);   // Read 16-bit word
        buffer[i * 2] = (uint8_t)data;
        buffer[i * 2 + 1] = (uint8_t)(data >> 8);
    }

    print_string("Finished reading sector ");
    print_string(lba_str);
    print_string(" from ATA disk.\n");
}