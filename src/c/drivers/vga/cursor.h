#ifndef CURSOR_H
#define CURSOR_H

#define VGA_CTRL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e

void set_cursor(int offset);
int get_cursor();

#endif