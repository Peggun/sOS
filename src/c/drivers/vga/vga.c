#include "vga.h"
#include "cursor.h"

void print_char(char c, int offset) {
    unsigned char *vidmem = (unsigned char *)VGA_ADDRESS;
    vidmem[offset] = c;
    vidmem[offset + 1] = WHITE_ON_BLACK;
}

// ok. time to fix you now.
void print_string(const char *str) {
    int offset = get_cursor();
    int i = 0;
    while (str[i] != 0)
    {
        if (str[i] == '\n') {
            offset = move_offset_to_new_line(offset);
        } else {
            print_char(str[i], offset);
            offset += 2;
        }
        i++;
    }
    set_cursor(offset);
}

// Function to print a single hexadecimal digit
void print_hex_digit(unsigned char digit) {
    char hex_digit;
    if (digit < 10) {
        hex_digit = '0' + digit;  // Print digits 0-9
    } else {
        hex_digit = 'A' + (digit - 10);  // Print letters A-F
    }
    print_char(hex_digit, get_cursor());  // Print the hex digit at the current cursor position
}

// Function to print a hexadecimal number (up to 32 bits)
void print_hex(uint32_t num) {
    print_string("0x");  // Prefix for hex representation
    int leading_zero = 1; // Flag to skip leading zeros
    for (int i = 28; i >= 0; i -= 4) {  // Print each hex digit
        unsigned char digit = (num >> i) & 0x0F;
        
        // Check if we have started printing significant digits
        if (digit != 0) {
            leading_zero = 0; // We have seen a non-zero digit
        }
        
        // Only print if we're past leading zeros
        if (!leading_zero) {
            print_hex_digit(digit);
        }
    }
    
    // If the number is zero, print a '0'
    if (leading_zero) {
        print_hex_digit(0);
    }
    
    print_nl();  // Newline for better formatting
}

void clear_screen(void) {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i) {
        print_char(' ', i * 2);
    }
    set_cursor(get_offset(0, 0));
}

int get_row_from_offset(int offset) {
    return offset / (2 * VGA_WIDTH);
}

int get_offset(int col, int row) {
    return 2 * (row * VGA_WIDTH + col);
}

int move_offset_to_new_line(int offset) {
    return get_offset(0, get_row_from_offset(offset) + 1);
}

int scroll_ln(int offset) {
    memory_copy(
            (char *) (get_offset(0, 1) + VGA_ADDRESS),
            (char *) (get_offset(0, 0) + VGA_ADDRESS),
            VGA_WIDTH * (VGA_HEIGHT - 1) * 2
    );

    for (int col = 0; col < VGA_WIDTH; col++) {
        print_char(' ', get_offset(col, VGA_HEIGHT - 1));
    }

    return offset - 2 * VGA_WIDTH;
}

void print_backspace() {
    int newCursor = get_cursor() - 2;
    print_char(' ', newCursor);
    set_cursor(newCursor);
}

void memory_copy(char *source, char *dest, int nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}

void print_nl() {
    int newOffset = move_offset_to_new_line(get_cursor());
    if (newOffset >= VGA_HEIGHT * VGA_WIDTH * 2) {
        newOffset = scroll_ln(newOffset);
    }
    set_cursor(newOffset);
}