global __readmsr
__readmsr:
    mov ecx, edi 
    rdmsr
    shl edx, 32
    mov ebx, eax
    mov rax, rdx
    mov eax, ebx
    ret
global __writemsr
__writemsr:
    mov eax, esi
    shr rsi, 32
    and esi, 0xffffffff
    mov rdx, rsi ;rsi & 0xffffffff

    mov ecx, edi 
    wrmsr
    ret
