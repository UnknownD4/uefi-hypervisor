#pragma once
#include <stdint.h>
#define EPT_TABLE_SIZE 4096
#define EPT_TABLE_ENTRIES 512
typedef union _EPT {
    uint64_t All;
    struct {
        uint64_t MemoryType : 3;
        uint64_t PageWalkLength : 3;
        uint64_t DirtyAndAccessEnabled : 1;
        uint64_t Reserved1 : 5;
        uint64_t PML4Address : 36;
        uint64_t Reserved2 : 16;
    } Fields;
} EPT, *PEPT;
typedef union _EPT_PML4E {
    uint64_t All;
    struct {
        uint64_t Read : 1;
        uint64_t Write : 1;
        uint64_t UserSupervisor : 1;
        uint64_t Reserved1 : 5;
        uint64_t Accessed : 1;
        uint64_t Ignored1 : 1;
        uint64_t ExecuteForUserMode : 1;
        uint64_t Ignored2 : 1;
        uint64_t PageFrameNumber : 20;
        uint64_t Reserved2 : 20;
        uint64_t Ignored3 : 12;
    } Fields;
} __attribute__((packed, aligned(1))) EPT_PML4E, *PEPT_PML4E;

typedef union _EPT_PDPTE {
    uint64_t All;
    struct {
        uint64_t Read : 1;
        uint64_t Write : 1;
        uint64_t UserSupervisor : 1;
        uint64_t Reserved1 : 5;
        uint64_t Accessed : 1;
        uint64_t Ignored1 : 1;
        uint64_t ExecuteForUserMode : 1;
        uint64_t Ignored2 : 1;
        uint64_t PageFrameNumber : 20;
        uint64_t Reserved2 : 20;
        uint64_t Ignored3 : 12; 
    } Fields;
} __attribute__((packed, aligned(1)))EPT_PDPTE, *PEPT_PDPTE;


typedef union _EPT_PDE_2M {
    uint64_t All;
    struct {
        uint64_t Read : 1;
        uint64_t Write : 1;
        uint64_t UserSupervisor : 1;
        uint64_t EPTMemoryType : 3;
        uint64_t IgnorePAT : 1;
        uint64_t NotEPTPageTable : 1;
        uint64_t Accessed : 1;
        uint64_t Dirty : 1;
        uint64_t ExecuteForUserMode : 1;
        uint64_t Ignored1 : 1;
        uint64_t Reserved1 : 9;
        uint64_t PageFrameNumber : 20;
        uint64_t Reserved2 : 11;
        uint64_t Ignored2 : 5;
        uint64_t VerifyGuestPaging : 1;
        uint64_t PagingWriteAccess : 1;
        uint64_t Ignored3 : 1;
        uint64_t SupervisorShadowStack : 1;
        uint64_t Ignored4 : 2;
        uint64_t SuppressVE : 1;
    } Fields;
} __attribute__((packed, aligned(1)))EPT_PDE_2M, *PEPT_PDE_2M;
EPT *EPTP;
EPT_PML4E *EPTP_PML4;
EPT_PDPTE *EPTP_PDPT;
EPT_PDE_2M *EPTP_PD;
int initEpt(void);
int TerminateEpt(EPT *ept_pointer);
