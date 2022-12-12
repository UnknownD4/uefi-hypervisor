global GetCR0
GetCR0:
    xor rax, rax
    mov rax, cr0
    ret
global SetCR0
SetCR0:
    mov cr0, rdi
    ret
global GetCR4
GetCR4:
    xor rax, rax
    mov rax, cr4
    ret
global SetCR4
SetCR4:
    mov cr4, rdi
    ret

global LoadGDT 
LoadGDT:
    push rbp
    mov rbp, rsp
    cli
    lgdt [rdi]
    push rsi
    xor rax, rax
    lea rax, [rel loadCS]  
    push rax
    retfq
loadCS:
    mov ds, dx
    mov es, dx
    mov fs, dx
    mov gs, dx
    mov ss, dx
    mov rsp, rbp
    pop rbp                                                                                                            
    ret

global LoadPaging
 LoadPaging:
    cli
    push rbp
    mov rbp, rsp
    ;int3
    mov cr3, rdi
    mov rsp, rbp
    pop rbp
    ret

global GetGDT
GetGDT:
    push rbp
    mov rbp, rsp
    sgdt [rdi]
    mov rsp, rbp
    pop rbp
    ret
global GetIDT
GetIDT:
    push rbp
    mov rbp, rsp
    sidt [rdi]
    mov rsp, rbp
    pop rbp
    ret

