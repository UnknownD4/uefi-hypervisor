#pragma once
#include <stdint.h> 

#define MSR_IA32_FEATURE_CONTROL         0x03A
#define MSR_IA32_VMX_BASIC               0x480
#define MSR_IA32_DEBUGCTL                0x1D9
#define MSR_IA32_VMX_PROCBASED_CTLS      0x482
#define MSR_IA32_VMX_PROCBASED_CTLS2     0x48B
#define MSR_IA32_VMX_PINBASED_CTLS       0x481
#define MSR_IA32_VMX_EXIT_CTLS           0x483
#define MSR_IA32_VMX_ENTRY_CTLS          0x484

#define MSR_IA32_VMX_CR0_FIXED0 0x00000486
#define MSR_IA32_VMX_CR0_FIXED1 0x00000487
#define MSR_IA32_VMX_CR4_FIXED0 0x00000488
#define MSR_IA32_VMX_CR4_FIXED1 0x00000489

#define CPU_BASED_VIRTUAL_INTR_PENDING        0x00000004
#define CPU_BASED_USE_TSC_OFFSETING           0x00000008
#define CPU_BASED_HLT_EXITING                 0x00000080
#define CPU_BASED_INVLPG_EXITING              0x00000200
#define CPU_BASED_MWAIT_EXITING               0x00000400
#define CPU_BASED_RDPMC_EXITING               0x00000800
#define CPU_BASED_RDTSC_EXITING               0x00001000
#define CPU_BASED_CR3_LOAD_EXITING            0x00008000
#define CPU_BASED_CR3_STORE_EXITING           0x00010000
#define CPU_BASED_CR8_LOAD_EXITING            0x00080000
#define CPU_BASED_CR8_STORE_EXITING           0x00100000
#define CPU_BASED_TPR_SHADOW                  0x00200000
#define CPU_BASED_VIRTUAL_NMI_PENDING         0x00400000
#define CPU_BASED_MOV_DR_EXITING              0x00800000
#define CPU_BASED_UNCOND_IO_EXITING           0x01000000
#define CPU_BASED_ACTIVATE_IO_BITMAP          0x02000000
#define CPU_BASED_MONITOR_TRAP_FLAG           0x08000000
#define CPU_BASED_ACTIVATE_MSR_BITMAP         0x10000000
#define CPU_BASED_MONITOR_EXITING             0x20000000
#define CPU_BASED_PAUSE_EXITING               0x40000000
#define CPU_BASED_ACTIVATE_SECONDARY_CONTROLS 0x80000000
#define CPU_BASED_CTL2_ENABLE_EPT             0x2
#define CPU_BASED_CTL2_RDTSCP                 0x8
#define CPU_BASED_CTL2_ENABLE_VPID            0x20
#define CPU_BASED_CTL2_UNRESTRICTED_GUEST     0x80
#define CPU_BASED_CTL2_ENABLE_VMFUNC          0x200

#define VM_ENTRY_IA32E_MODE                   0x00000200
#define VM_ENTRY_SMM                          0x00000400
#define VM_ENTRY_DEACT_DUAL_MONITOR           0x00000800
#define VM_ENTRY_LOAD_GUEST_PAT               0x00004000

#define VM_EXIT_IA32E_MODE                    0x00000200
#define VM_EXIT_ACK_INTR_ON_EXIT              0x00008000
#define VM_EXIT_SAVE_GUEST_PAT                0x00040000
#define VM_EXIT_LOAD_HOST_PAT                 0x00080000

#define PIN_BASED_VM_EXECUTION_CONTROLS_EXTERNAL_INTERRUPT        0x00000001
#define PIN_BASED_VM_EXECUTION_CONTROLS_NMI_EXITING               0x00000008
#define PIN_BASED_VM_EXECUTION_CONTROLS_VIRTUAL_NMI               0x00000020
#define PIN_BASED_VM_EXECUTION_CONTROLS_ACTIVE_VMX_TIMER          0x00000040
#define PIN_BASED_VM_EXECUTION_CONTROLS_PROCESS_POSTED_INTERRUPTS 0x00000080

typedef union _IA32_FEATURE_CONTROL_MSR {
    uint64_t All;
    struct {
        uint64_t Lock : 1;
        uint64_t EnableSMX : 1;
        uint64_t EnableVmxon : 1;
        uint64_t Reserved2 : 5;
        uint64_t EnableLocalSENTER : 7;
        uint64_t EnableGlobalSENTER : 1;
        uint64_t Reserved3a : 16;
        uint64_t Reserved3b : 32;
    } Fields;
} IA32_FEATURE_CONTROL_MSR, *PIA32_EATURE_CONTROL_MSR;
typedef union _IA32_VMX_BASIC_MSR{
    uint64_t All;
    struct {
        uint32_t RevisionIdentifier : 31;
        uint32_t Reserved1 : 1;
        uint32_t RegionSize : 12;
        uint32_t RegionClear : 1;
        uint32_t Reserved2 : 3;
        uint32_t SupportedIA64 : 1;
        uint32_t SupportedDualMonitor : 1;
        uint32_t MemoryType : 4;
        uint32_t VmExitReport : 1;
        uint32_t VmxCapabilityHint : 1;
        uint32_t Reserved3 : 8;
    } Fields;
} IA32_VMX_BASIC_MSR, *PIA32_VMX_BASIC_MSR;

typedef union _MSR{
    uint64_t All;
    struct {
        uint32_t High;
        uint32_t Low;
    } Fields;
} MSR, *PMSR;

extern uint64_t __readmsr(uint32_t msr);
extern void __writemsr(uint32_t msr, MSR content);
uint64_t AdjustControls(uint64_t msr, uint64_t control);


