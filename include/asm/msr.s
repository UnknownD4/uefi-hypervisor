global __readmsr
__readmsr:
    mov ecx, edi 
    rdmsr
    mov rax, edx << 32
    ret
global __writemsr
__readmsr:
    mov eax, esi
    mov edx, (rsi >> 32) & 0xffffffff
    mov ecx, edi 
    wrmsr
    ret
