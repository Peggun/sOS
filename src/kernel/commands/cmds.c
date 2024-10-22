#include "../../include/vga.h"
#include "../../include/cmds.h"
#include "../../include/kernel.h"

void exit_command(int argc, char *argv[]) {
    print_string("Stopping sOS...\n");
    asm volatile("hlt");
}

void echo_command(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) { // Start from 1 to skip the command itself
        print_string(argv[i]); // Print each argument
        if (i < argc - 1) {
            print_string(" "); // Add space between arguments
        }
    }
    print_nl(); // Print newline at the end
}

void help_command(int argc, char *argv[]) {
    print_string("Available commands:\n");
    print_string(" - exit: Stops the CPU.\n");
    print_string(" - help: Displays this help message.\n");
    print_string(" - clear: Clears the screen.\n");
    print_string(" - echo: Writes to the screen.\n");
    //print_string(" - DT: Disk test.\n");
    print_nl();
}

void clear_command(int argc, char *argv[]) {
    clear_screen();
}

Command command_table[] = {
    {"exit", exit_command},
    {"help", help_command},
    {"echo", echo_command},
    {"clear", clear_command},
    //{"DT", dt_command},
    {NULL, NULL}  // Null terminator
};