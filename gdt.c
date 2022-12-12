#include "include/gdt.h"
#include "include/vmx.h"
void initGDT(){
    LoadGDT((PGDTDescriptor)&g_GDT, i686_GDT_CODE_SEGMENT, i686_GDT_DATA_SEGMENT); 
    Print(L"[+] GDT has been succesfully loaded!");
}
