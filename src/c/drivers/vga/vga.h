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

void print_char(char c, uint8_t colour);
void print_string(const char *str);
void clear_screen(void);

#endif