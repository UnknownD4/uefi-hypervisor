#pragma once
#include "msr.h"
#include "vmx.h"
#include "states.h"
#include "vmcall.h"
#define EXIT_REASON_EXCEPTION_NMI       0
#define EXIT_REASON_EXTERNAL_INTERRUPT  1
#define EXIT_REASON_TRIPLE_FAULT        2
#define EXIT_REASON_INIT                3
#define EXIT_REASON_SIPI                4
#define EXIT_REASON_IO_SMI              5
#define EXIT_REASON_OTHER_SMI           6
#define EXIT_REASON_PENDING_VIRT_INTR   7
#define EXIT_REASON_PENDING_VIRT_NMI    8
#define EXIT_REASON_TASK_SWITCH         9
#define EXIT_REASON_CPUID               10
#define EXIT_REASON_GETSEC              11
#define EXIT_REASON_HLT                 12
#define EXIT_REASON_INVD                13
#define EXIT_REASON_INVLPG              14
#define EXIT_REASON_RDPMC               15
#define EXIT_REASON_RDTSC               16
#define EXIT_REASON_RSM                 17
#define EXIT_REASON_VMCALL              18
#define EXIT_REASON_VMCLEAR             19
#define EXIT_REASON_VMLAUNCH            20
#define EXIT_REASON_VMPTRLD             21
#define EXIT_REASON_VMPTRST             22
#define EXIT_REASON_VMREAD              23
#define EXIT_REASON_VMRESUME            24
#define EXIT_REASON_VMWRITE             25
#define EXIT_REASON_VMXOFF              26
#define EXIT_REASON_VMXON               27
#define EXIT_REASON_CR_ACCESS           28
#define EXIT_REASON_DR_ACCESS           29
#define EXIT_REASON_IO_INSTRUCTION      30
#define EXIT_REASON_MSR_READ            31
#define EXIT_REASON_MSR_WRITE           32
#define EXIT_REASON_INVALID_GUEST_STATE 33
#define EXIT_REASON_MSR_LOADING         34
#define EXIT_REASON_MWAIT_INSTRUCTION   36
#define EXIT_REASON_MONITOR_TRAP_FLAG   37
#define EXIT_REASON_MONITOR_INSTRUCTION 39
#define EXIT_REASON_PAUSE_INSTRUCTION   40
#define EXIT_REASON_MCE_DURING_VMENTRY  41
#define EXIT_REASON_TPR_BELOW_THRESHOLD 43
#define EXIT_REASON_APIC_ACCESS         44
#define EXIT_REASON_ACCESS_GDTR_OR_IDTR 46
#define EXIT_REASON_ACCESS_LDTR_OR_TR   47
#define EXIT_REASON_EPT_VIOLATION       48
#define EXIT_REASON_EPT_MISCONFIG       49
#define EXIT_REASON_INVEPT              50
#define EXIT_REASON_RDTSCP              51
#define EXIT_REASON_VMX_PREEMPTION_TIMER_EXPIRED     52
#define EXIT_REASON_INVVPID             53
#define EXIT_REASON_WBINVD              54
#define EXIT_REASON_XSETBV              55
#define EXIT_REASON_APIC_WRITE          56
#define EXIT_REASON_RDRAND              57
#define EXIT_REASON_INVPCID             58
#define EXIT_REASON_RDSEED              61
#define EXIT_REASON_PML_FULL            62
#define EXIT_REASON_XSAVES              63
#define EXIT_REASON_XRSTORS             64
#define EXIT_REASON_PCOMMIT             65

#define DPL_USER         3
#define DPL_SYSTEM       0
#define RPL_MASK         3
#define TYPE_MOV_TO_CR   0
#define TYPE_MOV_FROM_CR 1
#define TYPE_CLTS        2
#define TYPE_LMSW        3

#define HYPERV_CPUID_INTERFACE                0x40000001
#define HYPERV_CPUID_VERSION                  0x40000002
#define HYPERV_HYPERVISOR_PRESENT_BIT         0x80000000

#define CPUID_VMX_EXIT_RAX 0xdeadbeef
#define CPUID_VMX_EXIT_RCX 0xcafebabe

void ResumeGuestExecution(void){
    uint64_t CurrentRIP = 0;
    uint64_t ExitInstructionLength = 0;
    __vmx_vmread(GUEST_RIP, &CurrentRIP);
    __vmx_vmread(VM_EXIT_INSTRUCTION_LEN, &ExitInstructionLength);
    __vmx_vmwrite(GUEST_RIP, CurrentRIP + ExitInstructionLength);
    __vmx_vmresume();
    __vmx_off();
}

int VmExitDispatcher(general_registers general_regs){
    g_GuestState->Regs.general = general_regs;
    uint64_t ExitReason = 0;
    __vmx_vmread(VM_EXIT_REASON, &ExitReason);
    uint64_t ExitQualification = 0;
    __vmx_vmread(EXIT_QUALIFICATION, &ExitQualification);
    switch(ExitReason){
        case EXIT_REASON_VMCLEAR:
        case EXIT_REASON_VMPTRLD:
        case EXIT_REASON_VMPTRST:
        case EXIT_REASON_VMREAD:
        case EXIT_REASON_VMRESUME:
        case EXIT_REASON_VMWRITE:
        case EXIT_REASON_VMXOFF:
        case EXIT_REASON_VMXON:
        case EXIT_REASON_VMLAUNCH: 
        {
            __vmx_vmread(GUEST_RFLAGS, &g_GuestState->Regs.general.rflags);
            __vmx_vmwrite(GUEST_RFLAGS, g_GuestState->Regs.general.rflags | 0x1); // cf=1 indicate vm instructions fail
            break;
        }
        case EXIT_REASON_HLT: 
        {
            break;
        }
        case EXIT_REASON_EXCEPTION_NMI: {break;}
        case EXIT_REASON_CPUID: 
        {   
            uibt64_t mode = 0;
            __vmx_vmread(GUEST_CS_SELECTOR, &mode);
            mode = mode & RPL_MASK;
            if((g_GuestState->Regs.general.rax == CPUID_VMX_EXIT_RAX) && (g_GuestState->Regs.general.rcx == CPUID_VMX_EXIT_RCX) && (mode == DPL_SYSTEM)){ // CPUID POC for vmxoff
                uint64_t ExitInstructionLength = 0;
                __vmx_vmread(GUEST_RSP, &g_GuestState->Regs.general.rsp);
                __vmx_vmread(GUEST_RIP, &g_GuestState->Regs.general.rip);
                __vmx_vmread(VM_EXIT_INSTRUCTION_LEN, &ExitInstructionLength);
                g_GuestState->Regs.general.rip += ExitInstructionLength;
                return 1;
            }
            else { 
                int32_t cpu_info[4]; 
                __cpuidex(cpu_info, (int32_t)g_GuestState->Regs.general.rax, (int32_t)g_GuestState->Regs.general.rcx);
                if((int32_t)g_GuestState->Regs.general.rax == 1){
                    cpu_info[2] |= HYPERV_HYPERVISOR_PRESENT_BIT; // set the cpu present bit in RCX 
                }
                else if((int32_t)g_GuestState->Regs.general.rax == HYPERV_CPUID_INTERFACE){
                    cpu_info[0] = 'HYPT'; // [H][Y]per[P]a[T]ch 
                }
                g_GuestState->Regs.general.rax = cpu_info[0];
                g_GuestState->Regs.general.rbx = cpu_info[1];
                g_GuestState->Regs.general.rcx = cpu_info[2];
                g_GuestState->Regs.general.rdx = cpu_info[3];
            }   
            break;
        }
        case EXIT_REASON_INVD: {break;}
        case EXIT_REASON_VMCALL: {
                g_GuestState->Regs.general.rax = VmxVmcallHandler(g_GuestState->Regs.general.rdi, g_GuestState->Regs.general.rdi, g_GuestState->Regs.general.rdx, g_GuestState->Regs.general.rcx);
                break;
            }
        case EXIT_REASON_CR_ACCESS: {

            MOV_CR_EXIT_QUALIFICATION CrExitQualification = (MOV_CR_EXIT_QUALIFICATION)ExitQualification;
            if(CrExitQualification.Fields.Register == 4){
                __vmx_vmread(GUEST_RSP, g_GuestState->Regs.general.rsp);
            }
            uint64_t *RegPtr = (&g_GuestState->Regs.general.rax-CrExitQualification.Fields.Register); // calculate the address of the used register   
            switch (CrExitQualification.Fields.AccessType){
                case TYPE_MOV_TO_CR:
                    switch(CrExitQualification.Fields.ControlRegister){
                        case 0:{
                            __vmx_vmwrite(GUEST_CR0, *RegPtr);
                            break;
                        }
                        case 3:{
                            __vmx_vmwrite(GUEST_CR3, *RegPtr);
                            break;
                        }
                        case 4:{
                            __vmx_vmwrite(GUEST_CR4, *RegPtr);
                            break;
                        }
                        default:{break;}
                    }
                case TYPE_MOV_FROM_CR:
                    switch(CrExitQualification.Fields.ControlRegister){
                        case 0:{
                            __vmx_vmread(GUEST_CR0, RegPtr);
                            break;
                        }
                        case 3:{
                            __vmx_vmread(GUEST_CR3, RegPtr);
                            break;
                        }
                        case 4:{
                            __vmx_vmread(GUEST_CR4, RegPtr);
                            break;
                        }
                        default:{break;}
                    }
            }
            break;
        }
        case EXIT_REASON_MSR_READ: 
        {
            MSR msr = {0};
            msr.All = __readmsr(g_GuestState->Regs.general.rcx & 0xffffffff);
            g_GuestState->Regs.general.rax = msr.Low;
            g_GuestState->Regs.general.rdx = msr.High;
            break;
        }
        case EXIT_REASON_MSR_WRITE: {
            MSR msr = {0};
            msr.Low = g_GuestState->Regs.general.rax;
            msr.High = g_GuestState->Regs.general.rdx;    
            __writemsr(g_GuestState->Regs.general.rcx & 0xffffffff, msr.All);
            break;
        }
        case EXIT_REASON_EPT_VIOLATION: {
            uint64_t ViolatedAddress = 0;
            __vmx_vmread(GUEST_PHYSICAL_ADDRESS, &ViolatedAddress);
            EPT_VIOLATION_EXIT_QUALIFICATION EptViolationExitQualification;
            EptViolationExitQualification.All = (EPT_VIOLATION_EXIT_QUALIFICATION)ExitQualification;
            
            break;
        }
        default:{break;}
        return 0;
    }

}
