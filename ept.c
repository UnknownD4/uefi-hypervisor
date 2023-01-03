#include <stdint.h>
#include "include/ept.h"
#include "include/memory.h"
#include "include/states.h"

EPT *EPTP;
EPT_PML4E *EPTP_PML4;
EPT_PDPTE *EPTP_PDPT;
EPT_PDE_2M *EPTP_PD;

int initEpt(void){
    UINTN buffer; 
    if(!allocatePages(&buffer, 600)){return -1;};
    EPTP = (void *)buffer;
    EPTP_PML4 = (void *)buffer + EPT_TABLE_SIZE;
    EPTP_PDPT = (void *)buffer + EPT_TABLE_SIZE * 2;
    EPTP_PD = (void *)buffer + EPT_TABLE_SIZE * 3;

    EPTP->Fields.MemoryType = 6; // Write-back (WB)
    EPTP->Fields.PageWalkLength = 2;
    EPTP->Fields.DirtyAndAccessEnabled = 1;
    EPTP->Fields.PML4Address = (uint64_t)EPTP_PML4 / 0x1000;
    EPTP_PML4[0].Fields.Read = 1;
    EPTP_PML4[0].Fields.Write = 1;
    EPTP_PML4[0].Fields.UserSupervisor = 0;
    EPTP_PML4[0].Fields.ExecuteForUserMode = 0;
    EPTP_PML4[0].Fields.Accessed = 0;
    EPTP_PML4[0].Fields.PageFrameNumber = (uint64_t)EPTP_PDPT / 0x1000; 
    EPTP_PML4[0].Fields.Ignored1 = 0;
    EPTP_PML4[0].Fields.Ignored2 = 0;
    EPTP_PML4[0].Fields.Ignored3 = 0;
    EPTP_PML4[0].Fields.Reserved1 = 0;
    EPTP_PML4[0].Fields.Reserved2 = 0;
    for(int i = 0; i < EPT_TABLE_ENTRIES; i++){
        EPTP_PDPT[i].Fields.Read = 1;
        EPTP_PDPT[i].Fields.Write = 1;
        EPTP_PDPT[i].Fields.UserSupervisor = 0;
        EPTP_PDPT[i].Fields.ExecuteForUserMode = 0;
        EPTP_PDPT[i].Fields.Accessed = 0;
        EPTP_PDPT[i].Fields.PageFrameNumber = (uint64_t)(EPTP_PD + i * EPT_TABLE_SIZE) / 0x1000; 
        EPTP_PDPT[i].Fields.Ignored1 = 0;
        EPTP_PDPT[i].Fields.Ignored2 = 0;
        EPTP_PDPT[i].Fields.Ignored3 = 0;
        EPTP_PDPT[i].Fields.Reserved1 = 0;
        EPTP_PDPT[i].Fields.Reserved2 = 0;
        EPT_PDE_2M *next_eptp_pd = (EPT_PDE_2M *)((char *)EPTP_PD + i * EPT_TABLE_SIZE);
        for(int j = 0; j < EPT_TABLE_ENTRIES; j++){
            next_eptp_pd[j].Fields.Read = 1;
            next_eptp_pd[j].Fields.Write = 1;
            next_eptp_pd[j].Fields.UserSupervisor = 0;
            next_eptp_pd[j].Fields.EPTMemoryType = 6;
            next_eptp_pd[j].Fields.VerifyGuestPaging = 0;
            next_eptp_pd[j].Fields.PagingWriteAccess = 0;
            next_eptp_pd[j].Fields.Accessed = 0;
            next_eptp_pd[j].Fields.Dirty = 0;
            next_eptp_pd[j].Fields.IgnorePAT = 1;
            next_eptp_pd[j].Fields.NotEPTPageTable = 0;
            next_eptp_pd[j].Fields.ExecuteForUserMode = 0;
            next_eptp_pd[j].Fields.SupervisorShadowStack = 0;
            next_eptp_pd[j].Fields.SuppressVE = 0;
            next_eptp_pd[j].Fields.PageFrameNumber = (uint64_t) (i * _1GB + j * _2MB) / 0x1000; 
            next_eptp_pd[j].Fields.Ignored1 = 0;
            next_eptp_pd[j].Fields.Ignored2 = 0;
            next_eptp_pd[j].Fields.Ignored3 = 0;
            next_eptp_pd[j].Fields.Ignored4 = 0;
            next_eptp_pd[j].Fields.Reserved1 = 0;
            next_eptp_pd[j].Fields.Reserved2 = 0;
        }
    }
    g_GuestState->VmEptp = EPTP;
    return 0;
}
int TerminateEpt(EPT *ept_pointer){
    if(!freePages((void *)ept_pointer, 600)){return -1;}
    return 0;
}

