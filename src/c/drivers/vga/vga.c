#include <stdint.h>
#include <stdbool.h>
#include "vga.h"

#define VGA_MEMORY 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

volatile uint16_t *vga_buffer_pointer;

uint8_t tty_row = 0;
uint8_t tty_col = 0;
uint8_t color = 0x0F;  // White on black

// VGA Functions
void vga_initialise() {
    vga_buffer_pointer = (uint16_t *)VGA_MEMORY;
    vga_clear_screen();
}

void vga_clear_screen() {
    for (uint8_t row = 0; row < VGA_HEIGHT; row++) {
        for (uint8_t col = 0; col < VGA_WIDTH; col++) {
            uint16_t index = (VGA_WIDTH * row) + col;
            vga_buffer_pointer[index] = ((uint16_t)color << 8) | ' ';
        }
    }
}

void vga_put_char(char c) {
    uint16_t index = (VGA_WIDTH * tty_row) + tty_col;

    switch (c) {
        case '\n':
            tty_row++;
            tty_col = 0;
            break;
        case '\t':
            tty_col += 4; // Adjust tab space
            break;
        default:
            vga_buffer_pointer[index] = ((uint16_t)color << 8) | c;
            tty_col++;

            // Handle character overflow
            if (tty_col == VGA_WIDTH) {
                tty_row++;
                tty_col = 0;
            }
            if (tty_row == VGA_HEIGHT) {
                tty_row = 0; // Add scrolling if needed
            }
            break;
    }
}
