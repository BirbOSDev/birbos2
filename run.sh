#!/bin/bash
make
dd if=/dev/zero of=initrd.fat bs=8M count=1
LOOP=`losetup -f`
sudo losetup $LOOP initrd.fat
sudo mkfs.vfat $LOOP
sudo mount $LOOP /mnt
cp -r userspace/bin/* /mnt/
umount /mnt
losetup -d $LOOP
sync
chown $SUDO_USER:$SUDO_USER initrd.fat
cp kernel.bin bootdisk-root/boot/
cp initrd.fat bootdisk-root/boot/
grub-mkrescue -o birb.iso bootdisk-root
qemu-system-i386 -cdrom birb.iso