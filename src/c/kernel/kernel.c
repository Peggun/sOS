#include "kernel.h"
#include "../drivers/vga/vga.h"

void kernel_main() {
    //vga_initialise();     // Initialize VGA
    //vga_clear_screen();   // Clear the screen

    // Print "Hello World!" to the screen
    //const char *msg = "Hello World!";
    //while (*msg) {
        //vga_put_char(*msg++);  // Print each character
    //}
    char* video_memory = (char*) 0xb8000;
    *video_memory = 'X';
}