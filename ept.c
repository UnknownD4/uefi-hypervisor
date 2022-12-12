#include "include/ept.h"
#include "include/memory.h"
#include "include/states.h"

int initEpt(void){
    UINTN buffer; 
    if(!allocatePages(&buffer, 600)){return -1;};
    EPTP = (void *)buffer;
    EPTP_PML4 = (void *)buffer + EPT_TABLE_SIZE;
    EPTP_PDPT = (void *)buffer + EPT_TABLE_SIZE * 2;
    EPTP_PD = (void *)buffer + EPT_TABLE_SIZE * 3;

    EPTP->Fields.MemoryType = 6; // Write-back (WB)
    EPTP->PageWalkLength = 2;
    EPTP->DirtyAndAccessEnabled = 1;
    EPTP->PML4Address = EPTP_PML4 / 0x1000;
    EPTP_PML4[0].Fields.Present = 1;
    EPTP_PML4[0].Fields.ReadWrite = 1;
    EPTP_PML4[0].Fields.UserSupervisor = 0;
    EPTP_PML4[0].Fields.PageWriteThrough = 0;
    EPTP_PML4[0].Fields.PageCacheDisbale = 0;
    EPTP_PML4[0].Fields.ExecuteDisable = 0;
    EPTP_PML4[0].Fields.Accessed = 0;
    EPTP_PML4[0].Fields.R = 0;
    EPTP_PML4[0].Fields.PageFrameNumber = (uint64_t)EPTP_PDPT / 0x1000; 
    EPTP_PML4[0].Fields.Ignored1 = 0;
    EPTP_PML4[0].Fields.Ignored2 = 0;
    EPTP_PML4[0].Fields.Ignored3 = 0;
    EPTP_PML4[0].Fields.Reserved1 = 0;
    EPTP_PML4[0].Fields.Reserved2 = 0;
    for(int i = 0; i < EPT_TABLE_ENTRIES; i++){
        EPTP_PDPT[i].Fields.Present = 1;
        EPTP_PDPT[i].Fields.ReadWrite = 1;
        EPTP_PDPT[i].Fields.UserSupervisor = 0;
        EPTP_PDPT[i].Fields.PageWriteThrough = 0;
        EPTP_PDPT[i].Fields.PageCacheDisbale = 0;
        EPTP_PDPT[i].Fields.PageSize = 0;
        EPTP_PDPT[i].Fields.ExecuteDisable = 0;
        EPTP_PDPT[i].Fields.Accessed = 0;
        EPTP_PDPT[i].Fields.R = 0;
        EPTP_PDPT[i].Fields.PageFrameNumber = (uint64_t)(EPTP_PD + i * EPT_TABLE_SIZE) / 0x1000; 
        EPTP_PDPT[i].Fields.Ignored1 = 0;
        EPTP_PDPT[i].Fields.Ignored2 = 0;
        EPTP_PDPT[i].Fields.Ignored3 = 0;
        EPTP_PDPT[i].Fields.Reserved1 = 0;
        EPT_PDE_2M *next_eptp_pd = (EPT_PDE_2M *)((char *)EPTP_PD + i * EPT_TABLE_SIZE);
        for(int j = 0; j < EPT_TABLE_ENTRIES; j++){
            next_eptp_pd[j].Fields.Present = 1;
            next_eptp_pd[j].Fields.ReadWrite = 1;
            next_eptp_pd[j].Fields.UserSupervisor = 0;
            next_eptp_pd[j].Fields.PageWriteThrough = 0;
            next_eptp_pd[j].Fields.PageCacheDisbale = 0;
            next_eptp_pd[j].Fields.ExecuteDisable = 0;
            next_eptp_pd[j].Fields.Accessed = 0;
            next_eptp_pd[j].Fields.Dirty = 0;
            next_eptp_pd[j].Fields.PAT = 1;
            next_eptp_pd[j].Fields.G = 0;
            next_eptp_pd[j].Fields.R = 0;
            next_eptp_pd[j].Fields.ProtectionKey = 0;
            next_eptp_pd[j].Fields.PageFrameNumber = (uint64_t) (i * _1GB + j * _2MB) / 0x1000; 
            next_eptp_pd[j].Fields.Ignored1 = 0;
            next_eptp_pd[j].Fields.Ignored2 = 0;
            next_eptp_pd[j].Fields.Reserved1 = 0;
        }
    }
    g_GuestState->VmEptp = EPTP;
    return 0;
}
int TerminateEpt(EPT *ept_pointer){
    if(!freePages(ept_pointer, 600)){return -1;}
    return 0;
}

