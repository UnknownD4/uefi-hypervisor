#include <efi.h>
#include <efilib.h>
#include "include/vmx.h"
#include "include/memory.h"
#include "include/states.h"
#include "include/msr.h"


/*
VMPTRLD - Will make the VMCS Active and current.
VMCLEAR - Change the state from current to Not current. Idealy should be used before every VMPTLDR so that any previous current VMCS become Non current.
VMLAUNCH - To launch the VMCS or VM defined by VMCS.
VMRESUME - Used to launch again the previously launched VMCS. Used to launch the VM which is exit (VMEXIT) due to some reason.
*/
void SetupVmcs(void){   
    UINTN physicalAddress;
    allocatePages((UINTN *)&physicalAddress, 3);
    zeroMemory((void *)&physicalAddress, PAGE_SIZE * 3);
    g_HostState->VmmStack = physicalAddress; 
    if(g_HostState->VmmStack == 0){return;}
    //g_GuestState-> = physicalAddress + PAGE_SIZE; 
    //if(g_GuestState->VmmStack == 0){return;}
    setHostState();
    setGuestState();
}
void LaunchVm(void){
    if(!__vmx_vmclear(g_GuestState)){__vmx_off(); return;}
    if(!__vmx_vmptrld(&g_GuestState->VmcsRegion)){return;}
    SetupVmcs();
    SaveGeneralRegistersAndVmlaunch(); // save current state and vmlaunch 
    __vmx_off();
    uint64_t ErrorCode = 0;
    __vmx_vmread(VM_INSTRUCTION_ERROR, &ErrorCode);
    
}
int allocateVmcsRegion(void){
    UINTN physicalAddress;
    allocatePages((void *)&physicalAddress, 1);
    IA32_VMX_BASIC_MSR basic = {0};
    basic.All = __readmsr(MSR_IA32_VMX_BASIC);
    *(UINTN *)physicalAddress = basic.Fields.RevisionIdentifier;
    g_GuestState->VmcsRegion = physicalAddress;
    return 0;
}

void fixedCR0CR4Bits(void){
    uint64_t fixed0, fixed1;
    fixed0 = __readmsr(MSR_IA32_VMX_CR0_FIXED0);
    fixed1 =__readmsr(MSR_IA32_VMX_CR0_FIXED1);
    SetCR0((GetCR0() | fixed0) & fixed1);
    fixed0 = __readmsr(MSR_IA32_VMX_CR4_FIXED0);
    fixed1 =__readmsr(MSR_IA32_VMX_CR4_FIXED1);
    SetCR4((GetCR4() | fixed0) & fixed1);
}
int isVmxFeatureEnabled(void){
    IA32_FEATURE_CONTROL_MSR Control = {0};
    Control.All = __readmsr(MSR_IA32_FEATURE_CONTROL);
    if(Control.Fields.Lock == 0){
        Control.Fields.Lock = TRUE;
        Control.Fields.EnableVmxon = TRUE;
        __writemsr(MSR_IA32_FEATURE_CONTROL, (MSR)Control.All);
    }
    else if (Control.Fields.EnableVmxon == 0){Print(L"[-] VMX locked off in BIOS"); return -1;}
    return 0;
}
int allocateVmxRegion(void){
    UINTN physicalAddress;
    allocatePages((UINTN *)&physicalAddress, 1);
    IA32_VMX_BASIC_MSR basic = {0};
    basic.All = __readmsr(MSR_IA32_VMX_BASIC);
    *(UINTN *)physicalAddress = basic.Fields.RevisionIdentifier;
    if(__vmx_on(&physicalAddress)){
        return -1;
    }
    g_GuestState->VmxonRegion = physicalAddress;
    return 0;

}
int InitVmx(void){
    if(!compareStrings(CpuVendor(), CPUID_INTEL)){return -1;}
    if(!VmxOperationSupport()){return -1;}
    if(!isVmxFeatureEnabled()){return -1;}
    fixedCR0CR4Bits();
    EnableVmxOperation();
    if(!allocateVmxRegion()){return -1;}
    return 0;
}
int TerminateVmx(void) {
    __vmx_off();
    DisableVmxOperation();
    if(!freePages((UINTN *)g_GuestState->VmxonRegion, 3)){return -1;}
    if(!freePages((UINTN *)g_GuestState->VmcsRegion, 1)){return -1;}
    if(!TerminateEpt(g_GuestState->VmEptp)){return -1;}
    return 0;
}
int InitVmcs(void){
    LaunchVm();
    return 0;
}





