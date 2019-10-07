[org 0x7c00]
KERNEL_OFFSET equ 0x1000 ; The same one we used when linking the kernel

    mov [BOOT_DRIVE], dl ; Remember that the BIOS sets us the boot drive in 'dl' on boot
    call do_e820
    mov bp, 0x9000
    mov sp, bp


    mov bx, MSG_REAL_MODE
    call print
    call print_nl



    call load_kernel ; read the kernel from disk
    call switch_to_pm ; disable interrupts, load GDT,  etc. Finally jumps to 'BEGIN_PM'
    jmp $ ; Never executed

%include "bootsect/bootsect-memmap.asm"
%include "bootsect/bootsect-print.asm"
%include "bootsect/bootsect-printhex.asm"
%include "bootsect/bootsect-disk.asm"
%include "bootsect/bootsectgdt.asm"
%include "bootsect/vgaprint.asm"
%include "bootsect/protected-switch.asm"


[bits 16]
load_kernel:
    mov bx, MSG_LOAD_KERNEL
    call print
    call print_nl

    mov bx, KERNEL_OFFSET ; Read from disk and store in 0x1000
    mov dh, 50
    mov dl, [BOOT_DRIVE]
    call disk_load
    ret

[bits 32]
BEGIN_PM:
    mov ebx, MSG_PROT_MODE
    call print_string_pm
    call KERNEL_OFFSET ; Give control to the kernel
    jmp $ ; Stay here when the kernel returns control to us (if ever)


BOOT_DRIVE db 0 ; It is a good idea to store it in memory because 'dl' may get overwritten
MSG_REAL_MODE db "16-bit", 0
MSG_PROT_MODE db "32-bit", 0
MSG_LOAD_KERNEL db "Loading kernel", 0
; bootsector
times 510-($-$$) db 0
dw 0xaa55
