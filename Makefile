AS = nasm
CC = i686-elf-gcc
LD = i686-elf-ld

C_SOURCES = $(wildcard *.c)
ASM_SOURCES = $(wildcard *.s)

OBJ = ${C_SOURCES:.c=.o} ${ASM_SOURCES:.s=.o}

all: myos.iso

%.o: %.c
	${CC} -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra -masm=intel

%.o: %.s
	${AS} $< -o $@ -f elf32 

myos.bin: ${OBJ}
	${LD} -T linker.ld -o $@ $^

check-multiboot: myos.bin
	grub-file --is-x86-multiboot myos.bin

myos.iso: check-multiboot
	rm -rf isodir/
	mkdir -p isodir/boot/grub
	cp myos.bin isodir/boot/myos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir
	qemu-system-i386 -kernel myos.bin


clean:
	rm -rf isodir/
	rm -rf *.o
