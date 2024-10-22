#include "../include/vga.h"
#include "../include/cursor.h"
#include "../include/isr.h"
#include "../include/keyboard.h"
#include "../include/util.h"
#include "../include/cmds.h"
#include "../include/kernel.h"
#include "../include/ctype.h"

void kernel_main() {
    clear_screen();
    print_string("Installing interrupt service routines (ISRs).\n");
    isr_install();

    print_string("Enabling external interrupts.\n");
    asm volatile("sti");

    print_string("Initializing keyboard (IRQ 1).\n");
    init_keyboard();

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

    // Convert the command to lowercase
    strToLower(argv[0]);

    // Iterate over the command table to find the matching command
    for (int i = 0; command_table[i].name != NULL; i++) {
        // Use compare_string to check for a match (commands are already lowercase)
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