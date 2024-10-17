// vga.h

#ifndef VGA_H
#define VGA_H

#include <stdint.h>

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

/* please dont ask. after hours of debugging, 
the code only works if defined either here, or in every function locally. 
c is confusing... */
#define VGA_BUFFER ((uint16_t *)0xB8000) 

#define WHITE_ON_BLACK 0x0F

void print_char(char c, int offset);
void print_string(const char *str);
void clear_screen(void);

int get_offset(int col, int row);
int get_row_from_offset(int offset);
int move_offset_to_new_line(int offset);
int scroll_ln(int offset);
void print_backspace();
void print_nl();
void memory_copy(char *source, char *dest, int nbytes);

#endif