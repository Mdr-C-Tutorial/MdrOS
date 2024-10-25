section .multiboot

dd 0x1badb002 ; 文件魔术头, 内核识别码
dd 7 ; FLAGS
dd -464367625 ;-(MAGIC + FLAGS)
dd 0 ;HEADER_ADDR
dd 0 ;LOAD_ADDR
dd 0 ;LOAD_END_ADDR
dd 0 ;BSS_END_ADDR
dd 0 ;ENTRY_ADDR
dd 0 ;MODE_TYPE
dd 1280 ;WIDTH
dd 768  ;HEIGHT
dd 32   ;DEPTH
resb 4 * 13

section .text
global _start
extern kernel_main ; 内核主函数 main.c

_start:
    mov esp,stack_top
    push esp
    push ebx
    call kernel_main
L1:
    hlt
    jmp L1


section .bss
stack_bottom:
resb 16384 ; 内核栈大小 16KB
stack_top: