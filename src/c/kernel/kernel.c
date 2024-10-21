#include "../drivers/vga/vga.h"
#include "../drivers/vga/cursor.h"
#include "isr.h"
#include "../drivers/keyboard/keyboard.h"
#include "util.h"
#include "cmds.h"
#include "kernel.h"
#include "../drivers/disk/disk.h"
#include "../fs/ext2.h"

void kernel_main() {
    clear_screen();
    print_string("Installing interrupt service routines (ISRs).\n");
    isr_install();

    print_string("Enabling external interrupts.\n");
    asm volatile("sti");

    print_string("Initializing keyboard (IRQ 1).\n");
    init_keyboard();

    print_string("Initializing disk.\n");
    if (!disk_init()) {
        print_string("Disk initialization failed!\n");
        return;
    }
    else
    {
        print_string("Disk initialized successfully.\n");
    }

    print_string("Mounting ext2 filesystem.\n");
    ext2_mount();  // Mount the ext2 filesystem

    clear_screen();
    print_string("test@sOS: > ");
}

void parse_input(char *input, char *argv[], int *argc) {
    *argc = 0;
    while (*input) {
        while (*input == ' ') {
            *input++ = '\0';  // Replace spaces with null terminator
        }
        if (*input) {
            argv[(*argc)++] = input;  // Store argument
        }
        while (*input && *input != ' ') {
            input++;
        }
    }
}

void execute_command(char *input) {
    char *argv[10];  // Array to store command arguments
    int argc = 0;

    // Parse the input to separate arguments
    parse_input(input, argv, &argc);

    // If no command is entered, simply return
    if (argc == 0) {
        print_string("test@sOS: > ");
        return;
    }

    // Iterate over the command table to find the matching command
    for (int i = 0; command_table[i].name != NULL; i++) {
        // Use compare_string to check for a match
        if (compare_string(argv[0], command_table[i].name) == 0) {
            // Execute the command function with arguments
            command_table[i].func(argc, argv);
            print_string("test@sOS: > ");
            return;
        }
    }

    // If no command was found, print an error message
    print_string("Unknown command: ");
    print_string(argv[0]);
    print_string("\ntest@sOS: > ");
}