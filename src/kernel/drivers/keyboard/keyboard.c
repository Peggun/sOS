#include "../../../include/stdbool.h"
#include "../../../include/stdint.h"
#include "../../../include/keyboard.h"
#include "../../../include/ports.h"
#include "../../../include/isr.h"
#include "../../../include/vga.h"
#include "../../../include/util.h"
#include "../../../include/kernel.h"
#include "../../../include/stdbool.h"

#define BACKSPACE 0x0E
#define ENTER 0x1C

#define LSHIFT_PRESSED  0x2A
#define LSHIFT_RELEASED 0xAA

#define RSHIFT_PRESSED  0x36
#define RSHIFT_RELEASED 0xB6

static char key_buffer[256];
bool shift_pressed = false;  // Initialize shift_pressed to false

#define SC_MAX 57

const char *sc_name[] = {"ERROR", "Esc", "1", "2", "3", "4", "5", "6",
                         "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E",
                         "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl",
                         "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`",
                         "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".",
                         "/", "RShift", "Keypad *", "LAlt", "Spacebar"};

const char sc_ascii_lower[] = {'?', '?', '1', '2', '3', '4', '5', '6',
                         '7', '8', '9', '0', '-', '=', '?', '?', 'q', 'w', 'e', 'r', 't', 'y',
                         'u', 'i', 'o', 'p', '[', ']', '?', '?', 'a', 's', 'd', 'f', 'g',
                         'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\', 'z', 'x', 'c', 'v',
                         'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' '};

const char sc_ascii_caps[] = {'?', '?', '!', '@', '#', '$', '%', '^',
                               '&', '*', '(', ')', '_', '+', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y',
                               'U', 'I', 'O', 'P', '{', '}', '?', '?', 'A', 'S', 'D', 'F', 'G',
                               'H', 'J', 'K', 'L', ':', '"', '~', '?', '|', 'Z', 'X', 'C', 'V',
                               'B', 'N', 'M', '<', '>', '?', '?', '?', '?', ' '};


static void keyboard_callback(registers_t *regs) {
    uint8_t scancode = port_byte_in(0x60);

    // Handle shift press and release
    if (scancode == LSHIFT_PRESSED || scancode == RSHIFT_PRESSED) {
        shift_pressed = true;
        return;
    } else if (scancode == LSHIFT_RELEASED || scancode == RSHIFT_RELEASED) {
        shift_pressed = false;
        return;
    }

    // Ignore any scancodes greater than SC_MAX, except for special keys
    if (scancode > SC_MAX) return;

    // Handle backspace
    if (scancode == BACKSPACE) {
        if (backspace(key_buffer)) {
            print_backspace();
        }
    } 
    // Handle enter
    else if (scancode == ENTER) {
        print_nl();
        execute_command(key_buffer);  // Process the entered command
        key_buffer[0] = '\0';         // Reset the buffer after command
    } 
    // Handle character output
    else {
        char letter;
        if (shift_pressed) {
            letter = sc_ascii_caps[(int)scancode];  // Use caps character map when Shift is pressed
        } else {
            letter = sc_ascii_lower[(int)scancode];  // Use lowercase character map otherwise
        }
        
        append(key_buffer, letter);     // Add the character to the buffer
        char str[2] = {letter, '\0'};   // Create a string to print
        print_string(str);              // Output the character
    }
}

void init_keyboard() {
    register_interrupt_handler(IRQ1, keyboard_callback);  // Register the keyboard interrupt
}