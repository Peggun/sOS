// NOTE: after some testing, it turns out with the welcomeMsg[] you can only print it out 13 times before it breaks. Fun fact. Before I increase the storage allowed.

#include "../drivers/vga/vga.h"
#include "../drivers/vga/cursor.h"

void kernel_main() {
    clear_screen();
    char welcomeMsg[] = "Welcome to sOS!\n";
    print_string(welcomeMsg);
}