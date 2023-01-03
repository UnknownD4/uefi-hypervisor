#include "include/idt.h"
ISRHandler g_ISRHandler[256];
IDTEntry __attribute__((aligned(16))) IDT_Gates[256];
IDTDescriptor g_IDT = {(uint16_t)(sizeof(IDT_Gates) - 1), (PIDTEntry)IDT_Gates};