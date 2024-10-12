ENTRY(_start)

SECTIONS
{
    . = 0x00100000; /* Load at 1MB */

    .multiboot ALIGN(4) : {
        *(.multiboot)
    }

    .text ALIGN(4) : {
        *(.text)
    }

    .data ALIGN(4) : {
        *(.data)
    }
}