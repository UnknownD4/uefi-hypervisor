extern ResumeGuestExecution
extern SetGeneralRegistersState
extern VmExitDispatcher
extern SetRegistersState
global SaveGeneralRegistersAndVMLaunch
SaveGeneralRegistersAndVMLaunch:
    pushfq
    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8
    push rbp
    push rsp
  ;  push rip
    push rsi
    push rdi
    push rdx
    push rcx
    push rbx
    push rax
    mov rsi, rsp
    vmlaunch
    int3
    jmp RestoreGeneralRegisterState
global RestoreGeneralRegisterState
RestoreGeneralRegisterState:
    popfq
    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rdi
    pop rsi
   ; pop rip
    pop rsp
    pop rbp
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15
    popfq
    ret



global VmxExitHandler
VmxExitHandler:
    sub rsp, 200
    pushfq
    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8
    push rbp
    push rsp
   ; push rip
    push rsi
    push rdi
    push rdx
    push rcx
    push rbx
    push rax
    mov rdi, rsp
    sub rsp, 150
    call VmExitDispatcher
    mov al, 1
    je VmxoffHandler    
    call ResumeGuestExecution
    add rsp, 150
    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rdi
    pop rsi
  ;  pop rip
    pop rsp
    pop rbp
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15
    add rsp, 200
    ret 

VmxoffHandler:
    vmxoff
    sub rsp, 150
    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rdi
    pop rsi
  ;  pop rip
    pop rsp
    pop rbp
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15
    popfq
    add rsp, 200
    ret
    

global SaveRegistersState
SaveRegistersState:
    sub rsp, 300
    xor rax, rax
    str rax
    mov rdi, rax
    call SaveSegment
    mov rdi, gs
    call SaveSegment
    mov rdi, fs
    call SaveSegment
    mov rdi, es
    call SaveSegment
    mov rdi, ds
    call SaveSegment
    mov rdi, cs
    call SaveSegment
    
    pushfq
    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8
    push rbp
    push rsp
  ;  push rip
    push rsi
    push rdi
    push rdx
    push rcx
    push rbx
    push rax
    mov rax, cr4
    push rax
    mov rax, cr3
    push rax
    mov rax, cr0
    push rax
    
    push 0
    push 0 
    push 0
    push 0 
    push 0 
    mov rsi, rsp 
    call SetRegistersState
    add rsp, 300
    
    ret 


SaveSegment:
    xor rax, rax
    xor rbx, rbx
    xor rcx, rcx
    
    push rdi 
    sgdt [rax] 
    mov rax, [rax]
    mov rax, [rax+2+rdi] ; segment descriptor (limit low)
    
    mov rbx, [rax+6] ; access
    push rbx 
    xor rbx, rbx

    mov rbx, [rax+8] ; base high
    or rbx, 0xff
    shl rbx, 24
    
    mov rcx, [rax+3] ; base low
    or rbx, 0xffff
    or rbx, rcx

    push rbx
    xor rbx, rbx
    xor rcx, rcx

    
    mov rbx, [rax+7] ; limit high
    or rbx, 0xf
    shl rbx, 16
    
    mov rcx, [rax] ; limit low
    or rcx, 0xffff
    or rbx, rcx 

    push rbx
    ret