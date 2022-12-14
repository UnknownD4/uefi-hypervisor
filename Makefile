CC = gcc
LD = ld
OBJCOPY = objcopy
CFLAGS = -g -fpic -Ignu-efi/inc -ffreestanding -fno-stack-protector -fno-stack-check -fshort-wchar -mno-red-zone -maccumulate-outgoing-args
LDFLAGS = -g -shared -Bsymbolic -Lgnu-efi/x86_64/lib -Lgnu-efi/x86_64/gnuefi -Tgnu-efi/gnuefi/elf_x86_64_efi.lds -lgnuefi -lefi
OBJCOPYFLAGS = -j .text -j .sdata -j .data -j .dynamic -j .dynsym  -j .rel -j .rela -j .rel.* -j .rela.* -j .reloc --target efi-app-x86_64 --subsystem=10
QEMUFLAGS = --bios-path /usr/share/OVMF/OVMF_CODE.fd --size 1000 -- -m 1000 -serial mon:stdio -cpu kvm64 -enable-kvm  

TARGETS = $(wildcard *.c)
HEADERS = $(wildcard include/*.h)
ASM = $(wildcard include/asm/*.s)

all: BOOTX64.EFI
BOOTX64.EFI : main.o
	objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym  -j .rel -j .rela -j .rel.* -j .rela.* -j .reloc --target efi-app-x86_64 --subsystem=10 hypervisor.o BOOTX64.EFI

build/hypervisor.o : $(TARGETS).o $(ASM).o
	$(LD) $(LDFLAGS) gnu-efi/x86_64/gnuefi/crt0-efi-x86_64.o  $< -o $@ -lgnuefi -lefi

build/$(TARGETS).o : $(TARGETS)
	$(CC) $(CFLAGS) -c $< -o $@
$(ASM).o : $(ASM)
	nasm $< -f elf64 -o $@