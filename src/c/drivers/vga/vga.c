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