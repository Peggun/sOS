#include "../drivers/vga/vga.h"

void kernel_main() {
    clear_screen();
    print_char('a', WHITE_ON_BLACK);
    print_string("Hello, OS!");
}