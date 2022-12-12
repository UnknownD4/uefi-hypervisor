#pragma once 
#include <stdint.h>
#include <efi.h>
#include <efilib.h>

#define i686_GDT_CODE_SEGMENT 56
#define i686_GDT_DATA_SEGMENT 0x10
#define GDT_ENTRY_LIMIT_LOW(limit)                      (limit & 0xffff)
#define GDT_ENTRY_BASE_LOW(base)                        (base & 0xffff)
#define GDT_ENTRY_BASE_MIDDLE(base)                     ((base >> 16) & 0xff)
#define GDT_ENTRY_LIMIT_HIGH_FLAGS(limit, flags)        ((limit >> 16) & 0xf) |  (flags & 0xf0)
#define GDT_ENTRY_BASE_HIGH(base)                       ((base >> 24) & 0xff)
#define SET_GDT_ENTRY(base, limit, access, flags) {      \
    GDT_ENTRY_LIMIT_LOW(limit),                          \
    GDT_ENTRY_BASE_LOW(base),                            \
    GDT_ENTRY_BASE_MIDDLE(base),                         \
    access,                                              \
    GDT_ENTRY_LIMIT_HIGH_FLAGS(limit, flags),            \
    GDT_ENTRY_BASE_HIGH(base)                            \
}
enum
{
    GDT_ACCESS_CODE_READABLE                = 0x02,
    GDT_ACCESS_DATA_WRITEABLE               = 0x02,
    GDT_ACCESS_CODE_CONFORMING              = 0x04,
    GDT_ACCESS_DATA_DIRECTION_NORMAL        = 0x00,
    GDT_ACCESS_DATA_DIRECTION_DOWN          = 0x04,
    GDT_ACCESS_DATA_SEGMENT                 = 0x10,
    GDT_ACCESS_CODE_SEGMENT                 = 0x18,
    GDT_ACCESS_DESCRIPTOR_TSS               = 0x00,
    GDT_ACCESS_RING0                        = 0x00,
    GDT_ACCESS_RING1                        = 0x20,
    GDT_ACCESS_RING2                        = 0x40,
    GDT_ACCESS_RING3                        = 0x60,
    GDT_ACCESS_PRESENT                      = 0x80,
};
enum 
{
    GDT_FLAG_64BIT                          = 0x20,
    GDT_FLAG_32BIT                          = 0x40,
    GDT_FLAG_16BIT                          = 0x00,
    GDT_FLAG_GRANULARITY_1B                 = 0x00,
    GDT_FLAG_GRANULARITY_4K                 = 0x80,
};

typedef union _AccessByte{
    uint8_t All;
    struct {
        uint8_t A : 1;
        uint8_t RW : 1;
        uint8_t DC : 1;
        uint8_t E : 1;
        uint8_t S : 1;
        uint8_t DPL : 2;
        uint8_t P : 1;
    } Fields;
} __attribute__((packed, aligned(1))) AccessByte;
typedef union _LimitHighFlagsByte { 
    uint8_t All;
    struct {
        uint8_t LimitHigh : 4;
        uint8_t Reserved : 1;
        uint8_t L : 1;
        uint8_t DB : 1;
        uint8_t G : 1;
    }  Fields;
}  __attribute__((packed, aligned(1))) LimitHighFlagsByte;
typedef struct _GDTEntry { // Segment Descriptor
    uint16_t limitLow;
    uint16_t baseLow;
    uint8_t baseMiddle;
    AccessByte access;
    LimitHighFlagsByte limitHighFlags;
    uint8_t baseHigh;
}__attribute__((packed, aligned(1))) GDTEntry, *PGDTEntry;
typedef struct  _GDTDescriptor {
    uint16_t limit;
    PGDTEntry segmentDescriptor;
}__attribute__((packed, aligned(1))) GDTDescriptor, *PGDTDescriptor;//__attribute__((packed, aligned(1))) 3CB4D120

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
    
extern void LoadGDT(PGDTDescriptor rGDT, uint16_t codeSegment, uint16_t dataSegment);
void initGDT();
