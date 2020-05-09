mkdir -p debug
nasm -f elf32 src/boot.asm -o debug/boot.o

i686-elf-gcc -c src/kernel.c -o debug/kernel.o -std=gnu99 -m32 -ffreestanding -masm=intel -O -Wall -Wextra
i686-elf-gcc -c src/common.c -o debug/common.o -std=gnu99 -m32 -ffreestanding -masm=intel -O -Wall -Wextra
i686-elf-gcc -c src/Drivers/keyboard.c -o debug/keyboard.o -m32 -std=gnu99 -ffreestanding -masm=intel -O -Wall -Wextra
i686-elf-gcc -c src/Drivers/VGA.c -o debug/VGA.o -std=gnu99 -m32 -ffreestanding -masm=intel -O -Wall -Wextra
i686-elf-gcc -c src/Drivers/irq.c -o debug/irq.o -std=gnu99 -m32 -ffreestanding -masm=intel -O -Wall -Wextra
i686-elf-gcc -c src/Drivers/isrs.c -o debug/isrs.o -std=gnu99 -m32 -ffreestanding -masm=intel -O -Wall -Wextra
i686-elf-gcc -c src/Drivers/gdt.c -o debug/gdt.o -std=gnu99 -m32 -ffreestanding -masm=intel -O -Wall -Wextra
i686-elf-gcc -c src/Drivers/timer.c -o debug/timer.o -std=gnu99 -m32 -ffreestanding -masm=intel -O -Wall -Wextra
i686-elf-gcc -c src/Drivers/idt.c -o debug/idt.o -std=gnu99 -m32 -ffreestanding -masm=intel -O -Wall -Wextra
i686-elf-gcc -c src/Drivers/sound.c -o debug/sound.o -std=gnu99 -m32 -ffreestanding -masm=intel -O -Wall -Wextra
i686-elf-gcc -c src/Drivers/acpi.c -o debug/acpi.o -std=gnu99 -m32 -ffreestanding -masm=intel -O -Wall -Wextra
i686-elf-gcc -c src/Drivers/mouse.c -o debug/mouse.o -std=gnu99 -m32 -ffreestanding -masm=intel -O -Wall -Wextra
i686-elf-gcc -c src/Drivers/rtc.c -o debug/rtc.o -std=gnu99 -m32 -ffreestanding -masm=intel -O -Wall -Wextra

i686-elf-ld -T src/linker.ld -o debug/kernel.elf debug/boot.o debug/kernel.o debug/irq.o debug/idt.o debug/timer.o debug/gdt.o debug/isrs.o debug/common.o debug/keyboard.o debug/VGA.o debug/sound.o debug/acpi.o debug/mouse.o debug/rtc.o 

sh makegrub.sh
qemu-system-i386 BirbOS.iso -serial file:serial.log 

