[bits 16]
[org 0x7c00]   
    
    jmp word 0x0000:start

start:
    
    ; Read sector into memory (0x2000:0x0000) (es:bx)
    mov ax, 0x2000
    mov es, ax
    xor bx, bx
    mov ah, 2
    mov al, 0
    dw 0x9066 ; magic value
    mov ch, 0
    mov cl, 2
    mov dh, 0 
    int 0x13

    ; Jump to stage2
    jmp word 0x2000:0x0000


%if ($ - $$) > 510
    %fatal "Bootloader code exceed 512 bytes"

%endif

times 510 - ($ - $$) db 0
db 0x55
db 0xaa