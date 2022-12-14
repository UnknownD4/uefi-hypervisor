global CpuVendor
CpuVendor: 
    vendor: dq 13  
    push rax
    push rbx
    push rdi
    xor rax, rax
    xor rbx, rbx
    cpuid
    mov eax, ebx
    mov rdi, vendor
    mov [rdi+0], al
    mov [rdi+1], ah
    shr eax, 16
    mov [rdi+2], al
    mov [rdi+3], ah
    shr eax, 16
    mov [rdi+4], al
    mov [rdi+5], ah
    shr eax, 16
    mov [rdi+6], al
    mov [rdi+7], ah
    shr eax, 16
    mov [rdi+8], al
    mov [rdi+9], ah
    shr eax, 16
    mov [rdi+10], al
    mov [rdi+11], ah
    shr eax, 16
    mov [rdi+12], al
    mov byte [rdi+13], 0h
    mov rax, rdi
    pop rdi
    pop rbx
    pop rax

global VmxOperationSupport
VmxOperationSupport:
    push rdx
    mov rax, 1
    cpuid 
    xor rdx, rdx
    mov rdx, rcx
    shr rdx, 5
    mov rax, rdx
    pop rdx
    ret

global EnableVmxOperation
EnableVmxOperation:
    push rax
    xor rax, rax
    mov rax, cr4
    or rax, 02000h
    mov cr4, rax
    pop rax
    ret

global __vmx_on
__vmx_on:
    vmxon [rdi]
    ret
global __vmx_off
__vmx_off:
    vmxoff
    ret
global __vmx_vmptrld
__vmx_vmptrld:
    vmptrld [rdi]
    ret
global __vmx_vmclear
__vmx_vmclear:
    vmclear [rdi]
    ret
global __vmx_vmlaunch
__vmx_vmlaunch:
    vmlaunch 
    ret
global __vmx_vmresume
__vmx_vmresume:
    vmresume
    ret
global __vmx_vmwread
__vmx_vmread:
    vmread rsi, rdi
    ret
global __vmx_vmwrite
__vmx_vmwrite:
    vmwrite rdi, rsi
    ret
global VmxVmcall
VmxVmcall: 
    vmcall 
    ret
global __cpuidex
__cpuidex:
    mov rax, rsi
    mov rcx, rdx
    cpuid
    push rdx
    push rcx
    push rbx
    push rax
    mov rdi, rsp
    add rsp, 0x20
    ret