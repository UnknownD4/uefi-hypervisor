#include <efi.h>
#include <efilib.h>
#include "include/memory.h"
#include "include/paging.h"

EFI_STATUS EFIAPI efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
  InitializeLib(ImageHandle, SystemTable);
  Print(L"Hello, world!\n");
  SetSystemTable(SystemTable);
  if(!InitVmx()){return EFI_ABORTED;};
  InitVmcs();
  return EFI_SUCCESS;
}