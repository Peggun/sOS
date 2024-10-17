
#define NUM_COMMANDS (sizeof(command_table) / sizeof(command_table[0]) - 1)

// Define the structure for a command
typedef struct {
    const char *name;
    void (*func)(int argc, char *argv[]);  // Pointer to a function that takes no arguments and returns void
} Command;

// Create the lookup table
extern Command command_table[];