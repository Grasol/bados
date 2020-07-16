[bits 16]
[org 0x0000] ; 0x2000:0x0000
    
stage2:
    mov ax, 0x2000
    mov ds, ax
    mov es, ax

    ; fix stack
    mov ax, 0x1f00
    mov ss, ax
    xor sp, sp

    ; enter to 32bit protected mode
    cli
    lgdt [gdt_addr]
    mov eax, cr0
    or ax, 1
    mov cr0, eax
    jmp dword 0x08:(start32bit + 0x20000)

start32bit:
    ; -> 32-bit protected mode <-
    [bits 32]

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax

    ; enable paging (4MB)
    mov eax, PDE - $$ + 0x20000
    mov cr3, eax ; address of page directory

    mov eax, cr4
    bts eax, 4   ; set PSE flag
    mov cr4, eax

    mov eax, cr0
    bts eax, 31  ; set PAGE flag
    mov cr0, eax

    ; -> 32-bit pm with paging enable <-

    ; jump to kernel
    cld
    ; search PE Header
    mov edi, "PE" ; PE signature
    mov eax, KERNEL + 0x20000 - 4
.search_loop:
    add eax, 4
    cmp edi, [eax]
    jne .search_loop
    ; EAX is pointer to start of PE header

    mov ebp, [eax + 13 * 4] ; load ImageBase to EBP 
    add eax, 62 * 4 ; pointer to section array



    ; load section array and store all section into place indicated by pointers
    xor edx, edx ; EDX - virtual pointer to .text section

.copy_loop:
    ; clear before copies
    mov esi, eax
    xor eax, eax
    mov ecx, [esi + 2 * 4] ; numbers bytes to cleares
    mov edi, [esi + 3 * 4] 
    add edi, ebp  ; destination pointer (there section saves)
    rep stosb ; clear

    mov eax, esi
    mov edi, [eax + 3 * 4] 
    add edi, ebp  ; destination pointer (there section saves)
    mov ecx, [eax + 4 * 4] ; number bytes to copies 
    mov esi, [eax + 5 * 4]
    add esi, KERNEL + 0x20000 ; source pointer (physical address in PE file)

    test edx, edx
    jne .skip
        mov edx, edi
.skip:
    rep movsb ; copy

    add eax, 10 * 4
    cmp byte [eax], "."
    je .copy_loop ; if found next section, jump to copy

    ; jump to _start function
.jump_loop:
    add eax, 1
    cmp dword [eax], "__st"
    jne .jump_loop
    cmp dword [eax + 4], "art"
    jne .jump_loop ; if it found "__start" signature, load address of
.address_defines:  ;                           _start function and jump there
    add eax, 1
    cmp byte [eax], 1
    jne .address_defines ; search address

    mov eax, [eax - 4] ; load address
    add eax, edx

    mov esp, 0x003f0000 ; fix stack

    jmp eax

gdt_addr:
dw GDT_END - GDT - 1
dd GDT + 0x20000

times (32 - ($ - $$)) % 32 db 0xcc

; =======================
; Global Descriptor Table

GDT:
    ; Null Segment
    dq 0

    ; Code Segment
    ; Base Address (0000) | Segment Limit (ffff)
    dd 0x0000ffff
    ;    |-base-|FFFFseglFplFtype|-base-|
    dd 0b00000000110011111001101000000000
    ;   31      87      0F      87      0

    ; Data Segment
    ; Base Address (0000) | Segment Limit (ffff)
    dd 0x0000ffff
    ;    |-base-|FFFFseglFplFtype|-base-|
    dd 0b00000000110011111001001000000000
    ;   31      87      0F      87      0

GDT_END:

times (4096 - ($ - $$)) % 4096 db 0x00

; =====================
; Page Directory (4 MB)

PDE:
    ;    address4mb     addrF   F FFFFFF 
    dd 0b00000000000000000000000011100011
    dd 0b00000000010000000000000011100011
    ;   31      87      0F      87      0
    times 1022 dd 0x00


times (512 - ($ - $$)) % 512 db 0x00

KERNEL:
