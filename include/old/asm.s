global CpuVendor
CpuVendor: 
    VENDOR: DQ 13  
    PUSH RAX
    PUSH RBX
    PUSH RDI
    XOR RAX, RAX
    XOR RBX, RBX
    CPUID
    MOV EAX, EBX
    MOV RDI, VENDOR
    MOV [RDI+0], AL
    MOV [RDI+1], AH
    SHR EAX, 16
    MOV [RDI+2], AL
    MOV [RDI+3], AH
    SHR EAX, 16
    MOV [RDI+4], AL
    MOV [RDI+5], AH
    SHR EAX, 16
    MOV [RDI+6], AL
    MOV [RDI+7], AH
    SHR EAX, 16
    MOV [RDI+8], AL
    MOV [RDI+9], AH
    SHR EAX, 16
    MOV [RDI+10], AL
    MOV [RDI+11], AH
    SHR EAX, 16
    MOV [RDI+12], AL
    MOV [RDI+13], 0
    MOV RAX, RDI
    POP RDI
    POP RBX
    POP RAX

global VmxOperationSupport
VmxOperationSupport:
    PUSH RDX
    MOV RAX, 1
    CPUID 
    XOR RDX, RDX
    MOV RDX, RCX
    SHR RDX, 5
    MOV RAX, RDX
    POP RDX
    RET
    
global EnableVmxOperation
EnableVmxOperation:
    PUSH RAX
    XOR RAX, RAX
    MOV RAX, CR4
    OR RAX, 02000h
    MOV CR4, RAX
    POP RAX
    RET