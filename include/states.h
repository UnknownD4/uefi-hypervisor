#pragma once
#include "ept.h"
#include "gdt.h"
#include "idt.h"

typedef struct _segment {
    uint64_t limit;
    uint64_t base;
    uint8_t access;
    uint16_t selector;
} segment, *psegmenet;
typedef struct _general_registers {
    uint64_t rax, rbx, rcx, rdx, rdi, rsi, rip, rsp, rbp, r8, r9, r10, r11, r12, r13, r14, r15, rflags;
} general_registers, *pgeneral_registers;
typedef struct _registers {
    GDTDescriptor gdt;
    IDTDescriptor idt;
    uint64_t sysenter_rip, sysenter_rsp, sysenter_cs;
    uint64_t cr0, cr3, cr4;
    general_registers general;
    segment cs, ds, es, fs, gs, ss, tr; 
} registers, *pregisters;

typedef struct _VIRTUAL_MACHINE_STATE {
    uint64_t VmxonRegion;
    uint64_t VmcsRegion;
    EPT *VmEptp;
    registers Regs;
    uint64_t VmmStack;
    uint64_t MsrBitmap;
   // uint64_t MsrBitmapPhysical;
} VIRTUAL_MACHINE_STATE, *PVIRTUAL_MACHINE_STATE;
VIRTUAL_MACHINE_STATE *g_HostState, *g_GuestState;

extern void SaveRegistersState(VIRTUAL_MACHINE_STATE *state);
extern void SaveGeneralRegistersAndVmlaunch(void);
extern void RestoreGeneralRegisterState(void);
extern void VmxExitHandler(void);

extern void GetGDT(GDTDescriptor *rgdt);
extern void GetIDT(IDTDescriptor *ridt);

void setGuestState(void);
void setHostState(void);
void setControlFields(void);


void SetGeneralRegistersState(VIRTUAL_MACHINE_STATE *state, general_registers general_regs){
    state->Regs.general = general_regs;
}

void SetRegistersState(VIRTUAL_MACHINE_STATE *state, registers regs){
    state->Regs = regs;
}
