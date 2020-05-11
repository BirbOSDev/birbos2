mkdir -p debug
nasm -f elf32 src/boot.asm -o debug/boot.o

i686-elf-gcc -w -c src/kernel.c -o debug/kernel.o -std=gnu99 -m32 -ffreestanding -masm=intel -O2 -Wall -Wextra
i686-elf-gcc -w -c src/common.c -o debug/common.o -std=gnu99 -m32 -ffreestanding -masm=intel -O2 -Wall -Wextra
i686-elf-gcc -w -c src/Drivers/keyboard.c -o debug/keyboard.o -m32 -std=gnu99 -ffreestanding -masm=intel -O2 -Wall -Wextra
i686-elf-gcc -w -c src/Drivers/VGA.c -o debug/VGA.o -std=gnu99 -m32 -ffreestanding -masm=intel -O2 -Wall -Wextra
i686-elf-gcc -w -c src/Drivers/irq.c -o debug/irq.o -std=gnu99 -m32 -ffreestanding -masm=intel -O2 -Wall -Wextra
i686-elf-gcc -w -c src/Drivers/isrs.c -o debug/isrs.o -std=gnu99 -m32 -ffreestanding -masm=intel -O2 -Wall -Wextra
i686-elf-gcc -w -c src/Drivers/gdt.c -o debug/gdt.o -std=gnu99 -m32 -ffreestanding -masm=intel -O2 -Wall -Wextra
i686-elf-gcc -w -c src/Drivers/timer.c -o debug/timer.o -std=gnu99 -m32 -ffreestanding -masm=intel -O2 -Wall -Wextra
i686-elf-gcc -w -c src/Drivers/idt.c -o debug/idt.o -std=gnu99 -m32 -ffreestanding -masm=intel -O2 -Wall -Wextra
i686-elf-gcc -w -c src/Drivers/sound.c -o debug/sound.o -std=gnu99 -m32 -ffreestanding -masm=intel -O2 -Wall -Wextra
i686-elf-gcc -w -c src/Drivers/acpi.c -o debug/acpi.o -std=gnu99 -m32 -ffreestanding -masm=intel -O2 -Wall -Wextra
i686-elf-gcc -w -c src/Drivers/mouse.c -o debug/mouse.o -std=gnu99 -m32 -ffreestanding -masm=intel -O2 -Wall -Wextra
i686-elf-gcc -w -c src/Drivers/rtc.c -o debug/rtc.o -std=gnu99 -m32 -ffreestanding -masm=intel -O2 -Wall -Wextra
i686-elf-gcc -w -c src/Drivers/tui.c -o debug/tui.o -std=gnu99 -m32 -ffreestanding -masm=intel -O2 -Wall -Wextra


i686-elf-ld -T src/linker.ld -o debug/kernel.elf debug/boot.o debug/kernel.o debug/irq.o debug/idt.o debug/timer.o debug/gdt.o debug/isrs.o debug/common.o debug/keyboard.o debug/VGA.o debug/sound.o debug/acpi.o debug/mouse.o debug/rtc.o debug/tui.o

sh makegrub.sh
qemu-system-i386 -boot menu=on -kernel ./debug/kernel.elf -serial file:serial.log

