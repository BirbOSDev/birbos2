#!/bin/bash
cp kernel.bin bootdisk-root/boot/
cp initrd.fat bootdisk-root/boot/
grub-mkrescue -o birb.iso bootdisk-root
