#include <efi.h>
#include <efilib.h>
#include "include/memory.h"

EFI_SYSTEM_TABLE *SystemTable;
void SetSystemTable(EFI_SYSTEM_TABLE *SysTable){
    SystemTable = SysTable;
}
void zeroMemory(UINTN *buffer, UINTN size){
    for(UINTN i = 0; i < size; i++){
        buffer[i] = 0;
    }
}
int compareStrings(char *str1, char *str2){
    for(uint64_t i = 0; i < sizeof(str1) / sizeof(*str1); i++){
        if(str1[i] != str2[i]){return -1;} 
    }
    return 0;
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