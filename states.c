#include "include/states.h"
#include "include/vmx.h"
#include "include/gdt.h"
#include "include/idt.h"
#include "include/paging.h"
#include "include/msr.h"
//#include <stdint.h>
void setControlFields(void){
    /* Not important for now */
    __vmx_vmwrite(TSC_OFFSET, 0);
    __vmx_vmwrite(TSC_OFFSET_HIGH, 0);
    
    __vmx_vmwrite(PAGE_FAULT_ERROR_CODE_MASK, 0);
    __vmx_vmwrite(PAGE_FAULT_ERROR_CODE_MATCH, 0);
    
    __vmx_vmwrite(VM_EXIT_MSR_LOAD_COUNT, 0);
    __vmx_vmwrite(VM_EXIT_MSR_STORE_COUNT, 0);

    __vmx_vmwrite(VM_ENTRY_MSR_LOAD_COUNT, 0);
    __vmx_vmwrite(VM_ENTRY_INTR_INFO_FIELD, 0);

    __vmx_vmwrite(CPU_BASED_VM_EXEC_CONTROL, AdjustControls(MSR_IA32_VMX_PROCBASED_CTLS, CPU_BASED_ACTIVATE_MSR_BITMAP | CPU_BASED_ACTIVATE_SECONDARY_CONTROLS)); // As you can see, for the CPU_BASED_VM_EXEC_CONTROL, we set CPU_BASED_ACTIVATE_MSR_BITMAP; this way, we can enable the MSR BITMAP filter (described later in this part). Setting this field is somehow mandatory. As you might guess, Windows accesses lots of MSRs during a simple kernel execution, so if we don’t set this bit, then we’ll exit on each MSR access, and of course, our VMX Exit-Handler is called, hence clearing this bit to zero makes the system substantially slower.
    __vmx_vmwrite(CPU_BASED_VM_EXEC_CONTROL2, AdjustControls(MSR_IA32_VMX_PROCBASED_CTLS2, CPU_BASED_CTL2_RDTSCP | CPU_BASED_CTL2_ENABLE_VPID | CPU_BASED_CTL2_ENABLE_XSAVE_XRSTORS | CPU_BASED_CTL2_ENABLE_EPT)); // features used by Windows 10(RDTSCP ->  reads the current value of the processor’s time-stamp counter, INVPCID -> Invalidates mappings in the translation lookaside buffers (TLBs) and paging-structure caches based on process-context identifier (PCID), XSAVE -> Performs a full or partial save of processor state components)
    __vmx_vmwrite(PIN_BASED_VM_EXEC_CONTROL, AdjustControls(MSR_IA32_VMX_PINBASED_CTLS, 0));

    __vmx_vmwrite(VM_ENTRY_CONTROLS, AdjustControls(MSR_IA32_VMX_ENTRY_CTLS, VM_ENTRY_IA32E_MODE));
    __vmx_vmwrite(VM_EXIT_CONTROLS, AdjustControls(MSR_IA32_VMX_EXIT_CTLS, VM_EXIT_IA32E_MODE | VM_EXIT_ACK_INTR_ON_EXIT));

    __vmx_vmwrite(EPT_POINTER, (uint64_t)g_GuestState->VmEptp);
    __vmx_vmwrite(EXCEPTION_BITMAP, 0); 
}
void setGuestState(void){  
    uint64_t value = 0;
    setControlFields();  
    g_GuestState->Regs.cr0 =  (g_GuestState->Regs.cr0 | __readmsr(MSR_IA32_VMX_CR0_FIXED0)) & __readmsr(MSR_IA32_VMX_CR0_FIXED1);
    g_GuestState->Regs.cr4 =  (g_GuestState->Regs.cr4 | __readmsr(MSR_IA32_VMX_CR4_FIXED0)) & __readmsr(MSR_IA32_VMX_CR4_FIXED1);
    
    __vmx_vmwrite(GUEST_CR0, g_GuestState->Regs.cr0);
    __vmx_vmwrite(GUEST_CR3, g_GuestState->Regs.cr3);
    __vmx_vmwrite(GUEST_CR4, g_GuestState->Regs.cr4);
    __vmx_vmwrite(GUEST_RFLAGS, g_GuestState->Regs.general.rflags);
    __vmx_vmwrite(GUEST_CS_SELECTOR, g_GuestState->Regs.cs.selector & 0xF8);
    __vmx_vmwrite(GUEST_DS_SELECTOR, g_GuestState->Regs.ds.selector & 0xF8);
    __vmx_vmwrite(GUEST_ES_SELECTOR, g_GuestState->Regs.es.selector & 0xF8);
    __vmx_vmwrite(GUEST_SS_SELECTOR, g_GuestState->Regs.ss.selector & 0xF8);
    __vmx_vmwrite(GUEST_FS_SELECTOR, g_GuestState->Regs.fs.selector & 0xF8);
    __vmx_vmwrite(GUEST_GS_SELECTOR, g_GuestState->Regs.gs.selector & 0xF8);
    __vmx_vmwrite(GUEST_TR_SELECTOR, g_GuestState->Regs.tr.selector & 0xF8);
    __vmx_vmwrite(GUEST_LDTR_SELECTOR, 0);

    __vmx_vmwrite(GUEST_CS_LIMIT, g_GuestState->Regs.cs.limit);
    __vmx_vmwrite(GUEST_DS_LIMIT, g_GuestState->Regs.ds.limit);
    __vmx_vmwrite(GUEST_ES_LIMIT, g_GuestState->Regs.es.limit);
    __vmx_vmwrite(GUEST_SS_LIMIT, g_GuestState->Regs.ss.limit);
    __vmx_vmwrite(GUEST_FS_LIMIT, g_GuestState->Regs.fs.limit);
    __vmx_vmwrite(GUEST_GS_LIMIT, g_GuestState->Regs.gs.limit);
    __vmx_vmwrite(GUEST_TR_LIMIT, g_GuestState->Regs.tr.limit);
    __vmx_vmwrite(GUEST_GDTR_LIMIT, g_GuestState->Regs.idt.limit);
    __vmx_vmwrite(GUEST_IDTR_LIMIT, g_GuestState->Regs.gdt.limit);
    __vmx_vmwrite(GUEST_LDTR_LIMIT, 0);
    
    __vmx_vmwrite(GUEST_CS_BASE, g_GuestState->Regs.cs.base);
    __vmx_vmwrite(GUEST_DS_BASE, g_GuestState->Regs.ds.base);
    __vmx_vmwrite(GUEST_ES_BASE, g_GuestState->Regs.es.base);
    __vmx_vmwrite(GUEST_SS_BASE, g_GuestState->Regs.ss.base);
    __vmx_vmwrite(GUEST_FS_BASE, g_GuestState->Regs.fs.base);
    __vmx_vmwrite(GUEST_GS_BASE, g_GuestState->Regs.gs.base);
    __vmx_vmwrite(GUEST_TR_BASE, g_GuestState->Regs.tr.base);
    __vmx_vmwrite(GUEST_IDTR_BASE, (uint64_t)g_GuestState->Regs.gdt.segmentDescriptor);
    __vmx_vmwrite(GUEST_GDTR_BASE, (uint64_t)g_GuestState->Regs.idt.isr);
    __vmx_vmwrite(GUEST_LDTR_BASE, 0);

    __vmx_vmwrite(GUEST_CS_AR_BYTES, g_GuestState->Regs.cs.access);
    __vmx_vmwrite(GUEST_DS_AR_BYTES, g_GuestState->Regs.ds.access);
    __vmx_vmwrite(GUEST_ES_AR_BYTES, g_GuestState->Regs.es.access);
    __vmx_vmwrite(GUEST_SS_AR_BYTES, g_GuestState->Regs.ss.access);
    __vmx_vmwrite(GUEST_FS_AR_BYTES, g_GuestState->Regs.fs.access);
    __vmx_vmwrite(GUEST_GS_AR_BYTES, g_GuestState->Regs.gs.access);
    __vmx_vmwrite(GUEST_TR_AR_BYTES, g_GuestState->Regs.tr.access);
    __vmx_vmwrite(GUEST_LDTR_BASE, 0);
    
    __vmx_vmwrite(GUEST_SYSENTER_ESP, g_GuestState->Regs.sysenter_rsp);
    __vmx_vmwrite(GUEST_SYSENTER_EIP, g_GuestState->Regs.sysenter_rip); 
    __vmx_vmwrite(GUEST_SYSENTER_CS, g_GuestState->Regs.sysenter_cs);
    
    __vmx_vmread(HOST_IA32_EFER, &value);
    __vmx_vmwrite(GUEST_IA32_EFER, value);
    __vmx_vmread(HOST_IA32_PAT, &value);
    __vmx_vmwrite(GUEST_IA32_PAT, value);
    __vmx_vmwrite(VMCS_LINK_POINTER, ~0ULL); // Next, we set the VMCS_LINK_POINTER, which should be ‘0xffffffffffffffff’. As we don’t have an additional VMCS. This field is mainly used for hypervisors that want to implement a nested-virtualization behavior (like VMware Nested Virtualization or KVM’s nVMX).
    __vmx_vmwrite(GUEST_IA32_DEBUGCTL, __readmsr(MSR_IA32_DEBUGCTL) & 0xffffffff);
    __vmx_vmwrite(GUEST_IA32_DEBUGCTL_HIGH, __readmsr(MSR_IA32_DEBUGCTL) >> 32);
    
    __vmx_vmwrite(GUEST_RSP, g_GuestState->VmmStack + VMM_STACK_SIZE -1);
    __vmx_vmwrite(GUEST_RIP, (uint64_t)RestoreGeneralRegisterState);
    __vmx_vmwrite(GUEST_ACTIVITY_STATE, 0); // Logical processor's activity state - 0:Active
    __vmx_vmwrite(GUEST_INTERRUPTIBILITY_INFO, 0);   
}
void setHostState(void){

    SaveRegistersState(&g_GuestState->Regs); // save the guest starting state, before modifying it 
    GetGDT(&g_GuestState->Regs.gdt);
    GetIDT(&g_GuestState->Regs.idt);
    g_GuestState->Regs.sysenter_rsp = __readmsr(HOST_IA32_SYSENTER_ESP);
    g_GuestState->Regs.sysenter_rip = __readmsr(HOST_IA32_SYSENTER_EIP);
    g_GuestState->Regs.sysenter_cs = __readmsr(HOST_IA32_SYSENTER_CS);

  //  initGDT();
   // initIDT(); // TODO
   // initPaging();
    SaveRegistersState(&g_HostState->Regs);
    g_HostState->Regs.gdt = g_GDT;
    g_HostState->Regs.idt = g_IDT;
    g_HostState->Regs.sysenter_rsp = __readmsr(HOST_IA32_SYSENTER_ESP);
    g_HostState->Regs.sysenter_rip = __readmsr(HOST_IA32_SYSENTER_EIP);
    g_HostState->Regs.sysenter_cs = __readmsr(HOST_IA32_SYSENTER_CS);
    
    __vmx_vmwrite(HOST_CR0, g_HostState->Regs.cr0);
    __vmx_vmwrite(HOST_CR3, g_HostState->Regs.cr3);
    __vmx_vmwrite(HOST_CR4, g_HostState->Regs.cr4);

    __vmx_vmwrite(HOST_CS_SELECTOR, g_HostState->Regs.cs.selector & 0xf8); 
    __vmx_vmwrite(HOST_DS_SELECTOR, g_HostState->Regs.ds.selector & 0xf8); 
    __vmx_vmwrite(HOST_ES_SELECTOR, g_HostState->Regs.es.selector & 0xf8); 
    __vmx_vmwrite(HOST_SS_SELECTOR, g_HostState->Regs.ss.selector & 0xf8); 
    __vmx_vmwrite(HOST_FS_SELECTOR, g_HostState->Regs.fs.selector & 0xf8); 
    __vmx_vmwrite(HOST_GS_SELECTOR, g_HostState->Regs.gs.selector & 0xf8); 
    __vmx_vmwrite(HOST_TR_SELECTOR, g_HostState->Regs.tr.selector & 0xf8); 
    __vmx_vmwrite(HOST_FS_BASE, g_HostState->Regs.fs.base);
    __vmx_vmwrite(HOST_GS_BASE, g_HostState->Regs.gs.base);
    __vmx_vmwrite(HOST_TR_BASE, g_HostState->Regs.tr.base);
    __vmx_vmwrite(HOST_GDTR_BASE, (uint64_t)g_HostState->Regs.gdt.segmentDescriptor);
    __vmx_vmwrite(HOST_IDTR_BASE, (uint64_t)g_HostState->Regs.idt.isr);

    __vmx_vmwrite(HOST_IA32_SYSENTER_ESP, g_HostState->Regs.sysenter_rsp);
    __vmx_vmwrite(HOST_IA32_SYSENTER_EIP, g_HostState->Regs.sysenter_rip);
    __vmx_vmwrite(HOST_IA32_SYSENTER_CS, g_HostState->Regs.sysenter_cs);
    __vmx_vmwrite(HOST_IA32_EFER, __readmsr(HOST_IA32_EFER));
    __vmx_vmwrite(HOST_IA32_PAT, __readmsr(HOST_IA32_PAT)); // https://en.wikipedia.org/wiki/Page_attribute_table

    __vmx_vmwrite(HOST_RSP, g_HostState->VmmStack + VMM_STACK_SIZE -1);
    __vmx_vmwrite(HOST_RIP, (uint64_t)VmxExitHandler); 
}
