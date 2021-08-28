# BirbBuilder
Basically a fat script for everything BirbOS. Cleaning, running, compiling, etc.

# Prerequisites
To set up and use BirbOS 2.0 you need:
1. an i686-elf crosscompile toolchain
2. GRUB2 (to make the disk image)
3. xorriso (to make the disk image)
4. mtools (to make the disk image)
5. qemu-system-i386 (supported way of running BirbOS)

# How to use

## Windows
Unsupported. Seriously, where will you get GRUB tools? Figure it out yourself if you want.

## MacOS
Unsupported. Seriously, where will you get GRUB tools? Figure it out yourself if you want.

## Linux (non-Gentoo)
Install deps, run `make`, to run BirbOS run `make run`.

## Gentoo Linux
Install my overlay ( https://github.com/seal331/gentoo-overlay ) and emerge app-misc/birbbuilder.