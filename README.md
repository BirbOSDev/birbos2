# Birnix
BirbOS should be unix like
-NSG650
# Compile
You need only gcc cross compiler and nasm to compile
# Programs
Go to the userspace folder and then type 
make
to compile
# Run
in the root directory type in
sudo sh ./run.sh
and this script will do everthing to compile the kernel to initialise the ramdisk
then create the iso and finally  run it with qemu
