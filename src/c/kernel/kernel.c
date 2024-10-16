#include "../drivers/vga/vga.h"
#include "../drivers/vga/cursor.h"
#include "isr.h"
#include "../drivers/keyboard/keyboard.h"

void kernel_main() {
    clear_screen();
    print_string("Installing interrupt service routines (ISRs).");
    isr_install();

    print_string("Enabling external interrupts.\n");
    asm volatile("sti");

    print_string("Initializing keyboard (IRQ 1).\n");
    init_keyboard();
}