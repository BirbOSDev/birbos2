SOURCES_C=$(patsubst %.c,%.o,$(wildcard ./*.c))


CC=cc
LD=ld
CFLAGS=-I../usr/i686-birb/include -nostdlib -fno-builtin -m32 -Werror -c -O3
LDFLAGS=-Tlink.ld -m elf_i386 ../usr/i686-birb/lib/crt0.o -L../usr/i686-birb/lib  -lc -lg -lm -lnosys -lsosousdk

OBJ = $(SOURCES_C)

all: $(OBJ)

clean:
	-rm *.o
	-rm -rf bin

%.o:%.c
	mkdir -p bin
	$(CC) $(CFLAGS) $< -o $@
	$(LD) $@ $(LDFLAGS) -o bin/$*
