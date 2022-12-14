#include <efi.h>
#include <efilib.h>
#include "include/paging.h"
#include "include/memory.h"

void infoPaging(){
    uint64_t cr3;
    __asm__ __volatile__("mov %%cr3, %[cr3]"
			     :  [cr3]"=rm"(cr3));
    Print(L"CR3: %x\n", cr3);
    Print(L"PML4: %x\n", PML4);

}

int initPaging(){
    UINTN buffer; 
    if(!allocatePages(&buffer, 600)){return -1;};
    PML4 = (void *)buffer;       
    PDPT = (void *)buffer + TABLE_SIZE;
    PD = (void *)buffer + TABLE_SIZE * 2;
    Print(L"PML4 address: 0x%x\nPDPT address: 0x%x\nPD address: 0x%x\n", PML4, PDPT, PD);
    PML4[0].Fields.Present = 1;
    PML4[0].Fields.ReadWrite = 1;
    PML4[0].Fields.UserSupervisor = 0;
    PML4[0].Fields.PageWriteThrough = 0;
    PML4[0].Fields.PageCacheDisbale = 0;
    PML4[0].Fields.ExecuteDisable = 0;
    PML4[0].Fields.Accessed = 0;
    PML4[0].Fields.R = 0;
    PML4[0].Fields.PageFrameNumber = (uint64_t)PDPT / 0x1000; 
    PML4[0].Fields.Reserved1 = 0;
    PML4[0].Fields.Ignored1 = 0;
    PML4[0].Fields.Ignored2 = 0;
    PML4[0].Fields.Ignored3 = 0;
     for(int i = 0; i < TABLE_ENTRIES; i++){
        PDPT[i].Fields.Present = 1;
        PDPT[i].Fields.ReadWrite = 1;
        PDPT[i].Fields.UserSupervisor = 0;
        PDPT[i].Fields.PageWriteThrough = 0;
        PDPT[i].Fields.PageCacheDisbale = 0;
        PDPT[i].Fields.PageSize = 0;
        PDPT[i].Fields.ExecuteDisable = 0;
        PDPT[i].Fields.Accessed = 0;
        PDPT[i].Fields.R = 0;
        PDPT[i].Fields.PageFrameNumber = (uint64_t)(PD + i * TABLE_SIZE) / 0x1000; // each PDPTE points to a different PD that has 512 PDEs
        PDPT[i].Fields.Ignored1 = 0;
        PDPT[i].Fields.Ignored2 = 0;
        PDPT[i].Fields.Ignored3 = 0;
        PDE_2M *next_pd = (PDE_2M *)((char *)PD + i * TABLE_SIZE);
        for(int j = 0; j < TABLE_ENTRIES; j++){
            next_pd[j].Fields.Present = 1;
            next_pd[j].Fields.UserSupervisor = 0;
            next_pd[j].Fields.PageWriteThrough = 0;
            next_pd[j].Fields.PageCacheDisbale = 0;
            next_pd[j].Fields.ExecuteDisable = 0;
            next_pd[j].Fields.Accessed = 0;
            next_pd[j].Fields.ReadWrite = 0;
            next_pd[j].Fields.Dirty = 0;
            next_pd[j].Fields.PAT = 1;
            next_pd[j].Fields.G = 0;
            next_pd[j].Fields.R = 0;
            next_pd[j].Fields.ProtectionKey = 0;
            next_pd[j].Fields.PageFrameNumber = (uint64_t) (i * _1GB + j * _2MB) / 0x1000; 
            next_pd[j].Fields.Ignored1 = 0;
            next_pd[j].Fields.Ignored2 = 0;
        }
     }
    LoadPaging((uint64_t)PML4);   
    return 0; 
}