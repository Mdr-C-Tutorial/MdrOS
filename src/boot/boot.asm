section .multiboot

dd 0x1badb002 ; 文件魔术头, 内核识别码
dd 3
dd -464367621

section .text
global _start
extern kernel_main ; 内核主函数

_start:
    mov esp,stack_top
    push ebx
    call kernel_main
    hlt
    jmp $

section .bss
stack_bottom:
resb 16384
stack_top: