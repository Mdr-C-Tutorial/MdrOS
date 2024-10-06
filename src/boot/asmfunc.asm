global gdt_flush,tss_flush,idt_flush

gdt_flush: ; void gdt_flush(uint32_t gdtr);
    mov eax, [esp + 4] ; [esp+4]按规定是第一个参数，即gdtr
    lgdt [eax] ; 加载新gdt指针，传入的是地址

    mov ax, 0x10 ; 新数据段
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax ; 各数据段全部划给这一数据段
    jmp 0x08:.flush ; cs没法mov，只能用farjmp/farcall，这里用farjmp刷新cs
.flush:
    ret ; 刷新完毕，返回

tss_flush:
    mov ax, 0x2B
    ltr ax
    ret

idt_flush: ; void idt_flush(uint32_t);
    mov eax, [esp + 4]
    lidt [eax]
    ret