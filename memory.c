#include <efi.h>
#include <efilib.h>
#include "include/memory.h"

EFI_SYSTEM_TABLE *SystemTable;
void SetSystemTable(EFI_SYSTEM_TABLE *SysTable){
    SystemTable = SysTable;
}
void zeroMemory(void * buffer, UINTN size){
    for(int i = 0; i < size; i++){
        buffer[i] = 0;
    }
}
int allocatePages(UINTN *buffer, UINTN numPages){
    EFI_STATUS status = uefi_call_wrapper(SystemTable->BootServices->AllocatePages, 4, AllocateAnyPages, EfiRuntimeServicesData, numPages, buffer);
    if (EFI_ERROR(status)){return -1;}
    return 0;
}
int freePages(UINTN *buffer, UINTN numPages){
    EFI_STATUS status = uefi_call_wrapper(SystemTable->BootServices->FreePages, 2, (VOID *)(UINTN)buffer, numPages);
    if (EFI_ERROR(status)){return -1;}
    return 0;
}