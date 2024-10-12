; Set up for 32-bit addressing mode
bits 32

section .multiboot_header
header_start:
    dd 0xe85250d6                ; magic number
    dd 0                         ; protected mode code
    dd header_end - header_start ; header length

    ; checksum
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

    ; required end tag
    dw 0    ; type
    dw 0    ; flags
    dd 8    ; size
header_end:

section .text
global _start

_start:
    ; Set up the stack
    cli                                  ; Clear interrupts
    xor eax, eax                         ; Zero out eax (32-bit version)
    mov ds, ax                           ; Initialize data segment
    mov es, ax                           ; Initialize extra segment
    mov fs, ax                           ; Initialize fs segment
    mov gs, ax                           ; Initialize gs segment

    ; Set up the stack pointer
    mov esp, 0x9FC00                     ; Set up stack pointer (adjust as needed)

    ; Print Initial Message
    mov esi, welcome_msg                 ; Use esi for 32-bit addressing
    call print_string

    ; Print the Kernel Loading Message
    mov esi, loading_msg                 ; Use esi for 32-bit addressing
    call print_string

    ; Load Kernel
    mov ah, 0x02                         ; BIOS read sectors function
    mov al, 0x01                         ; Read 1 sector
    mov cx, 0x0001                       ; Cylinder: 0, Sector: 1
    mov dx, 0x0000                       ; Head: 0
    mov ebx, 0x7E00                      ; Kernel load address (32-bit register)
    mov dl, 0x00                         ; Drive (0 = floppy)
    int 0x13                             ; BIOS Interrupt Call

    ; Check for errors
    jc load_error                        ; Jump if there's an error
    mov esi, loaded_msg
    call print_string                    ; Print success message

    ; Jump to Kernel
    jmp ebx

load_error:
    mov esi, error_msg
    call print_string                    ; Print error message
    jmp load_error                       ; Infinite loop on error

print_string:
    mov ah, 0x0E                         ; BIOS teletype output
.next_char:
    lodsb                                ; Load next character into al from esi
    cmp al, 0                            ; Check for null terminator
    je .done
    int 0x10                             ; Print character
    jmp .next_char
.done:
    ret

section .data
welcome_msg db "Bootloader Started...", 0
loading_msg db "Loading Kernel...", 0
loaded_msg db "Kernel Loaded Successfully!", 0
error_msg db "Error loading kernel!", 0

times 510 - ($ - $$) db 0
dw 0xAA55                                ; Boot sector magic number