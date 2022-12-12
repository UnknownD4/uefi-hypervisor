#pragma once
#include <efi.h>
#include <efilib.h>
#define ALIGNMENT_PAGE_SIZE 4096
#define VMXON_SIZE          4096
#define VMM_STACK_SIZE      4096
#define VMCS_SIZE           4096
#define MSR_SIZE            4096   
#define EPT_SIZE            4096
#define MAXIMUM_ADDRESS     0xffffffffffffffff
#define POOLTAG             0xdeadbeef
#define PAGE_SIZE 4096
#define _2MB 2097152
#define _1GB 1073741824
#define _512GB 549755813888

typedef struct _EFI_MEMORY_DESCRIPTOR {
    uint32_t type;
    void* physicalAddress;
    void* linearAddress;
    uint64_t numPages;
    uint64_t attributes;
} EFI_MEMORY_DESCRIPTOR, *PEFI_MEMORY_DESCRIPTOR;

const char* EFI_MEMORY_TYPE_STRINGS[] {
   "EfiReservedMemoryType",
    "EfiLoaderCode",
    "EfiLoaderData",
    "EfiBootServicesCode",
    "EfiBootServicesData",
    "EfiRuntimeServicesCode",
    "EfiRuntimeServicesData",
    "EfiConventionalMemory",
    "EfiUnusableMemory",
    "EfiACPIReclaimMemory",
    "EfiACPIMemoryNVS",
    "EfiMemoryMappedIO",
    "EfiMemoryMappedIOPortSpace",
    "EfiPalCode",
};

void SetSystemTable(EFI_SYSTEM_TABLE *SysTable);
EFI_STATUS allocatePages(UINTN *buffer, UINTN numPages);
EFI_STATUS freePages(UINTN *buffer, UINTN numPages);
void zeroMemory(void * buffer, UINTN size);

int MathPower(int base, int exponent);