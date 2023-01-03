#include <efi.h>
#include <efilib.h>
#include "include/gdt.h"
#include "include/vmx.h"

GDTEntry __attribute__((aligned(16))) GDT_Table[10] = {
    SET_GDT_ENTRY(0x0, 0x0, 0x0, 0x0), // null descriptor
    SET_GDT_ENTRY(0x0, 0x0, GDT_ACCESS_CODE_SEGMENT | GDT_ACCESS_CODE_READABLE | GDT_ACCESS_RING0 | GDT_ACCESS_PRESENT, (GDT_FLAG_64BIT | GDT_FLAG_GRANULARITY_4K)),
    SET_GDT_ENTRY(0x0, 0xfffff, GDT_ACCESS_DATA_SEGMENT | GDT_ACCESS_DATA_WRITEABLE | GDT_ACCESS_RING0 | GDT_ACCESS_PRESENT,  (GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K)),
    SET_GDT_ENTRY(0x0, 0x0, 0x0, 0x0), // null descriptor
    SET_GDT_ENTRY(0x0, 0x0, 0x0, 0x0), // null descriptor
    SET_GDT_ENTRY(0x0, 0x0, 0x0, 0x0), // null descriptor
    SET_GDT_ENTRY(0x0, 0x0, 0x0, 0x0), // null descriptor
    SET_GDT_ENTRY(0x0, 0x0, GDT_ACCESS_CODE_SEGMENT | GDT_ACCESS_CODE_READABLE | GDT_ACCESS_RING0 | GDT_ACCESS_PRESENT, (GDT_FLAG_64BIT | GDT_FLAG_GRANULARITY_4K)),
};

GDTDescriptor g_GDT = {sizeof(GDT_Table) -1, (PGDTEntry)&GDT_Table};

void initGDT(){
    LoadGDT((PGDTDescriptor)&g_GDT, i686_GDT_CODE_SEGMENT, i686_GDT_DATA_SEGMENT); 
    Print(L"[+] GDT has been succesfully loaded!");
}