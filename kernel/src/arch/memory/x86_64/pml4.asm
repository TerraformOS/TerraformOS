bits 64
global load_pml4


load_pml4:
    mov rax, rdi
    and rax, 0x000ffffffffff000
    mov cr3, rax
    retq
