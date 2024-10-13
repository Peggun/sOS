#include "vga.h"

static uint16_t cursor_position = 0;

void print_char(char c, uint8_t colour) {
    if (cursor_position < VGA_WIDTH * VGA_HEIGHT) { // Ensure within bounds
        VGA_BUFFER[cursor_position] = ((uint16_t)colour << 8) | c; // Write character and color
        cursor_position++; // Move to the next position
    }
}

// ok. time to fix you now.
void print_string(const char* str) {
    while (*str) {
        print_char(*str, WHITE_ON_BLACK);
        str++;
    }
}

void clear_screen(void) {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        VGA_BUFFER[i] = ((uint16_t)WHITE_ON_BLACK << 8) | ' ';
    }
}