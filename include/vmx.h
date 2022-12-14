#pragma once
#include "states.h"
#define CPUID_INTEL "GenuineIntel"
#define GUEST_ES_SELECTOR  0x00000800
#define GUEST_CS_SELECTOR  0x00000802
#define GUEST_SS_SELECTOR  0x00000804
#define GUEST_DS_SELECTOR  0x00000806
#define GUEST_FS_SELECTOR  0x00000808
#define GUEST_GS_SELECTOR  0x0000080a
#define GUEST_LDTR_SELECTOR 0x0000080c
#define GUEST_TR_SELECTOR  0x0000080e
#define GUEST_ES_LIMIT  0x00004800
#define GUEST_CS_LIMIT  0x00004802
#define GUEST_SS_LIMIT  0x00004804
#define GUEST_DS_LIMIT  0x00004806
#define GUEST_FS_LIMIT  0x00004808
#define GUEST_GS_LIMIT  0x0000480a
#define GUEST_LDTR_LIMIT  0x0000480c
#define GUEST_TR_LIMIT  0x0000480e
#define GUEST_GDTR_LIMIT  0x00004810
#define GUEST_IDTR_LIMIT  0x00004812
#define GUEST_ES_AR_BYTES  0x00004814
#define GUEST_CS_AR_BYTES  0x00004816
#define GUEST_SS_AR_BYTES  0x00004818
#define GUEST_DS_AR_BYTES  0x0000481a
#define GUEST_FS_AR_BYTES  0x0000481c
#define GUEST_GS_AR_BYTES  0x0000481e
#define GUEST_LDTR_AR_BYTES  0x00004820
#define GUEST_TR_AR_BYTES  0x00004822
#define GUEST_INTERRUPTIBILITY_INFO  0x00004824
#define GUEST_ACTIVITY_STATE  0x00004826
#define GUEST_SM_BASE  0x00004828
#define GUEST_SYSENTER_CS  0x0000482A
#define GUEST_PHYSICAL_ADDRESS  0x2400
#define GUEST_PHYSICAL_ADDRESS_HIGH  0x2401
#define GUEST_IA32_DEBUGCTL  0x00002802
#define GUEST_IA32_DEBUGCTL_HIGH  0x00002803
#define GUEST_LINEAR_ADDRESS  0x0000640a
#define GUEST_CR0  0x00006800
#define GUEST_CR3  0x00006802
#define GUEST_CR4  0x00006804
#define GUEST_ES_BASE  0x00006806
#define GUEST_CS_BASE  0x00006808
#define GUEST_SS_BASE  0x0000680a
#define GUEST_DS_BASE  0x0000680c
#define GUEST_FS_BASE  0x0000680e
#define GUEST_GS_BASE  0x00006810
#define GUEST_LDTR_BASE  0x00006812
#define GUEST_TR_BASE  0x00006814
#define GUEST_GDTR_BASE  0x00006816
#define GUEST_IDTR_BASE  0x00006818
#define GUEST_DR7  0x0000681a
#define GUEST_RSP  0x0000681c
#define GUEST_RIP  0x0000681e
#define GUEST_RFLAGS  0x00006820
#define GUEST_PENDING_DBG_EXCEPTIONS  0x00006822
#define GUEST_SYSENTER_ESP  0x00006824
#define GUEST_SYSENTER_EIP  0x00006826
#define GUEST_IA32_EFER			0x00002806
#define GUEST_IA32_PAT 0x00002804

#define IO_BITMAP_A  0x00002000
#define IO_BITMAP_A_HIGH  0x00002001
#define IO_BITMAP_B  0x00002002
#define IO_BITMAP_B_HIGH  0x00002003
#define MSR_BITMAP  0x00002004
#define MSR_BITMAP_HIGH  0x00002005
#define TSC_OFFSET  0x00002010
#define TSC_OFFSET_HIGH  0x00002011
#define VIRTUAL_APIC_PAGE_ADDR  0x00002012
#define VIRTUAL_APIC_PAGE_ADDR_HIGH  0x00002013
#define VMFUNC_CONTROLS  0x00002018
#define VMFUNC_CONTROLS_HIGH  0x00002019
#define EPT_POINTER  0x0000201A
#define EPT_POINTER_HIGH  0x0000201B
#define EPTP_LIST  0x00002024
#define EPTP_LIST_HIGH  0x00002025
#define VMCS_LINK_POINTER  0x00002800
#define VMCS_LINK_POINTER_HIGH  0x00002801
#define PIN_BASED_VM_EXEC_CONTROL  0x00004000
#define CPU_BASED_VM_EXEC_CONTROL  0x00004002
#define EXCEPTION_BITMAP  0x00004004
#define PAGE_FAULT_ERROR_CODE_MASK  0x00004006
#define PAGE_FAULT_ERROR_CODE_MATCH  0x00004008
#define VM_EXIT_CONTROLS  0x0000400c
#define VM_EXIT_MSR_STORE_COUNT  0x0000400e
#define VM_EXIT_MSR_LOAD_COUNT  0x00004010
#define VM_ENTRY_CONTROLS  0x00004012
#define VM_ENTRY_MSR_LOAD_COUNT  0x00004014
#define VM_ENTRY_INTR_INFO_FIELD  0x00004016
#define VM_ENTRY_EXCEPTION_ERROR_CODE  0x00004018
#define VM_ENTRY_INSTRUCTION_LEN  0x0000401a
#define VM_INSTRUCTION_ERROR  0x00004400
#define VM_EXIT_REASON  0x00004402
#define VM_EXIT_INTR_INFO  0x00004404
#define VM_EXIT_INTR_ERROR_CODE  0x00004406
#define VM_EXIT_MSR_STORE_ADDR  0x00002006
#define VM_EXIT_MSR_STORE_ADDR_HIGH  0x00002007
#define VM_EXIT_MSR_LOAD_ADDR  0x00002008
#define VM_EXIT_MSR_LOAD_ADDR_HIGH  0x00002009
#define VM_ENTRY_MSR_LOAD_ADDR  0x0000200a
#define VM_ENTRY_MSR_LOAD_ADDR_HIGH  0x0000200b
#define TPR_THRESHOLD  0x0000401c
#define CPU_BASED_VM_EXEC_CONTROL2  0x0000401e
#define CPU_BASED_CTL2_ENABLE_XSAVE_XRSTORS       0x100000
#define VM_EXIT_INSTRUCTION_LEN  0x0000440c
#define IDT_VECTORING_INFO_FIELD  0x00004408
#define IDT_VECTORING_ERROR_CODE  0x0000440a
#define VMX_INSTRUCTION_INFO  0x0000440e

#define CR0_GUEST_HOST_MASK  0x00006000
#define CR4_GUEST_HOST_MASK  0x00006002
#define CR0_READ_SHADOW  0x00006004
#define CR4_READ_SHADOW  0x00006006
#define CR3_TARGET_VALUE0  0x00006008
#define CR3_TARGET_VALUE1  0x0000600a
#define CR3_TARGET_VALUE2  0x0000600c
#define CR3_TARGET_VALUE3  0x0000600e
#define CR3_TARGET_COUNT  0x0000400a
#define EXIT_QUALIFICATION  0x00006400


#define HOST_ES_SELECTOR  0x00000c00
#define HOST_CS_SELECTOR  0x00000c02
#define HOST_SS_SELECTOR  0x00000c04
#define HOST_DS_SELECTOR  0x00000c06
#define HOST_FS_SELECTOR  0x00000c08
#define HOST_GS_SELECTOR  0x00000c0a
#define HOST_TR_SELECTOR  0x00000c0c
#define HOST_CR0  0x00006c00
#define HOST_CR3  0x00006c02
#define HOST_CR4  0x00006c04
#define HOST_FS_BASE  0x00006c06
#define HOST_GS_BASE  0x00006c08
#define HOST_TR_BASE  0x00006c0a
#define HOST_GDTR_BASE  0x00006c0c
#define HOST_IDTR_BASE  0x00006c0e
#define HOST_IA32_SYSENTER_ESP  0x00006c10
#define HOST_IA32_SYSENTER_EIP  0x00006c12
#define HOST_IA32_SYSENTER_CS  0x00004c00
#define HOST_RSP  0x00006c14
#define HOST_RIP  0x00006c16
#define HOST_IA32_EFER 0x00002C02
#define HOST_IA32_PAT 0x00002C00
#define VMM_STACK_SIZE 4096



typedef struct _CPUID {
    int eax, ebx, ecx, edx;
} CPUID, *PCPUID;
typedef union _MOV_CR_EXIT_QUALIFICATION {
    uint64_t All;
    struct {
        uint8_t ControlRegister : 4;
        uint8_t AccessType : 2;
        uint8_t LMSWOperandType : 1;
        uint8_t Reserved1 : 1;
        uint8_t Register : 4;
        uint8_t Reserved2 : 4;
        uint16_t LMSWSourceData; 
        uint64_t reserved3;
    } Fields;
} MOV_CR_EXIT_QUALIFICATION, *PMOV_CR_EXIT_QUALIFICATION;
typedef union _EPT_VIOLATION_EXIT_QUALIFICATION {
    uint64_t All;
    struct {
        uint8_t ReadViolation : 1;
        uint8_t WriteViolation : 1;
        uint8_t ExecuteViolation : 1;
        uint8_t Readable : 1;
        uint8_t Writable : 1;
        uint8_t ExecutableForSupervisor : 1;
        uint8_t ExecutableForUser : 1;
        uint8_t ValidLinearAddress : 1;
        uint8_t TranslationViolation : 1;
        uint8_t UserLinearAddress : 1;
        uint8_t ReadableWritable : 1;
        uint8_t ExecuteDisable : 1;
        uint8_t NMIUnblocking : 1;
        uint64_t reserved1 : 51;
    } Feilds;
} EPT_VIOLATION_EXIT_QUALIFICATION, *PEPT_VIOLATION_EXIT_QUALIFICATION;
extern int __vmx_vmread(uint64_t vmcsOffset, uint64_t *value);
extern int __vmx_vmwrite(uint64_t vmcsOffset, uint64_t value);
extern int __vmx_on(uint64_t *vmxAddress);
extern int __vmx_off(void);
extern int __vmx_vmptrld(uint64_t *vmcsAddress);
extern int __vmx_vmclear(VIRTUAL_MACHINE_STATE *state);
extern int __vmx_vmlaunch(void);
extern int __vmx_vmresume(void);

extern char* CpuVendor(void);
extern int VmxOperationSupport(void);
extern void EnableVmxOperation(void);
extern void DisableVmxOperation(void);
extern void VmxExitHandler(void);
extern void GuestZone(void);


extern int InitVmx(void);
extern int TerminateVmx(void);
extern int InitVmcs(void);

extern void __cpuidex(uint64_t cpu_info[4], uint64_t rax, uint64_t rcx);
extern uint64_t GetCR0(void);
extern void SetCR0(uint64_t cr0);
extern uint64_t GetCR4(void);
extern void SetCR4(uint64_t cr4);    
