mkdir -p isodir/boot/grub
cp debug/kernel.elf isodir/boot/kernel.elf
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o BirbOS.iso isodir
