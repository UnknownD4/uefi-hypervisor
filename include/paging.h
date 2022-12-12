#pragma once
#include <efi.h>
#include <efilib.h>

#define TABLE_ENTRIES 512
#define TABLE_SIZE 4096

typedef union _PML4E {
    uint64_t All;
    struct {
        uint64_t Present : 1;
        uint64_t ReadWrite : 1;
        uint64_t UserSupervisor : 1;
        uint64_t PageWriteThrough : 1;
        uint64_t PageCacheDisbale : 1;
        uint64_t Accessed : 1;
        uint64_t Ignored1 : 1;
        uint64_t Reserved1 : 1; // page size
        uint64_t Ignored2 : 3;
        uint64_t R : 1;
        uint64_t PageFrameNumber : 40;
        uint64_t Ignored3 : 11;
        uint64_t ExecuteDisable : 1;  
    } Fields;
} __attribute__((packed, aligned(1))) PML4E, *PPML4E;

typedef union _PDPTE {
    uint64_t All;
    struct {
        uint64_t Present : 1;
        uint64_t ReadWrite : 1;
        uint64_t UserSupervisor : 1;
        uint64_t PageWriteThrough : 1;
        uint64_t PageCacheDisbale : 1;
        uint64_t Accessed : 1;
        uint64_t Ignored1 : 1;
        uint64_t PageSize : 1; // page size
        uint64_t Ignored2 : 3;
        uint64_t R : 1;
        uint64_t PageFrameNumber : 40;
        uint64_t Ignored3 : 11;
        uint64_t ExecuteDisable : 1;   
    } Fields;
} __attribute__((packed, aligned(1)))PDPTE, *PPDPTE;

typedef union _PDE {
    uint64_t All;
    struct {
        uint64_t Present : 1;
        uint64_t ReadWrite : 1;
        uint64_t UserSupervisor : 1;
        uint64_t PageWriteThrough : 1;
        uint64_t PageCacheDisbale : 1;
        uint64_t Accessed : 1;
        uint64_t Ignored1 : 1;
        uint64_t PageSize : 1; // page size
        uint64_t Ignored2 : 3;
        uint64_t R : 1;
        uint64_t PageFrameNumber : 40;
        uint64_t Ignored3 : 11;
        uint64_t ExecuteDisable : 1; 
    } Fields;
} __attribute__((packed, aligned(1)))PDE, *PPDE;

typedef union _PDE_2M {
    uint64_t All;
    struct {
        uint64_t Present : 1;
        uint64_t ReadWrite : 1;
        uint64_t UserSupervisor : 1;
        uint64_t PageWriteThrough : 1;
        uint64_t PageCacheDisbale : 1;
        uint64_t Accessed : 1;
        uint64_t Dirty : 1;
        uint64_t PAT : 1; 
        uint64_t G : 1;
        uint64_t Ignored1 : 2;
        uint64_t R : 1;
        uint64_t PageFrameNumber : 40;
        uint64_t Ignored2 : 7;
        uint64_t ProtectionKey : 4;
        uint64_t ExecuteDisable : 1; 
    } Fields;
} __attribute__((packed, aligned(1)))PDE_2M, *PPDE_2M;

typedef union _PTE {
    uint64_t All;
    struct {
        uint64_t Present : 1;
        uint64_t ReadWrite : 1;
        uint64_t UserSupervisor : 1;
        uint64_t PageWriteThrough : 1;
        uint64_t PageCacheDisbale : 1;
        uint64_t Accessed : 1;
        uint64_t Dirty : 1;
        uint64_t PAT : 1; 
        uint64_t G : 1; 
        uint64_t Ignored1 : 2;
        uint64_t R : 1;
        uint64_t PageFrameNumber : 40;
        uint64_t Ignored2 : 7;
        uint64_t ProtectionKey : 4;
        uint64_t ExecuteDisable : 1; 
    } Fields;
} __attribute__((packed, aligned(1)))PTE, *PPTE;
PML4E *PML4;
PDPTE *PDPT;
PDE_2M **PD;

extern void LoadPaging(uint64_t cr3);
void infoPaging();
void initPaging();
