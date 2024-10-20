global gdt_flush,tss_flush,idt_flush,switch_to

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

switch_to: ;void switch_to(struct context *prev, struct context *next);
        mov eax, [esp+4]

        mov [eax+0],  esp
        mov [eax+4],  ebp
        mov [eax+8],  ebx
        mov [eax+12], esi
        mov [eax+16], edi
        mov [eax+20], ecx
        mov [eax+24], edx

        pushf        ;保存eflags
        pop ecx
        mov [eax+28], ecx

        mov eax, [esp+8]

        mov esp, [eax+0]
        mov ebp, [eax+4]
        mov ebx, [eax+8]
        mov esi, [eax+12]
        mov edi, [eax+16]
        mov ecx, [eax+20]
        mov edx, [eax+24]

        mov eax, [eax+28] ;加载eflags
        push eax
        popf

        ret