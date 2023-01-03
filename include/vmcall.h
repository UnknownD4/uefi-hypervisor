#pragma once
#include <efi.h>
#include <efilib.h>
#include <stdint.h>

#define VMCALL_TEST	0x1
extern void VmxVmcall(uint64_t VmcallNumber, uint64_t arg1, uint64_t arg2, uint64_t arg3); 


static int VmxVmcallHandler(uint64_t VmcallNumber, uint64_t arg1, uint64_t arg2, uint64_t arg3){
    switch (VmcallNumber)
    {
        case VMCALL_TEST:
        {
            Print("This is a test VMCALL:\nVMCALL Number: %llx\nFirst Argument: %llx\nSeccond Argument: %llx\nThird Argument: %llx\n", VmcallNumber, arg1, arg2, arg3);            
            return 1;
            break;
        }
        default:
        {
            Print(L"[-]Error: VMCALL is not supported!\n");
            return -1;
            break;
        }
    }
}