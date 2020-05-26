# Birnix
This is birnix which is basically xv6 recreated
Some of the code has been taken from the xv6 repository but mostly it is written
# compiling
So you thought you need a cross compiler 
nah you just need gcc and nasm
just type 
`make qemu` for gui
`make qemu-nox` for using it in bash
# writing applications for it
well create the application like hello world
```
include "types.h"
include "stat.h"
include "user.h"
#include "user.h"
int main(){
  printf(1, "Hello, world!\n");
  return 0;
}
```
save it as hello.c or something else 
and then open up the makefile
and find
these parts
```
EXTRA=\
 mkfs.c ulib.c user.h cat.c echo.c forktest.c grep.c kill.c\
 ln.c ls.c mkdir.c rm.c stressfs.c usertests.c wc.c zombie.c\
hello.c\
 printf.c umalloc.c\
 README dot-bochsrc *.pl toc.* runoff runoff1 runoff.list\
 .gdbinit.tmpl gdbutil\
```
`<program name>.c\`
add the file here
and
```
UPROGS=\
_cat\
_crash\
_echo\
_factor\
_forktest\
_grep\
_hello\
_init\
_kill\
_ln\
_ls\
_mkdir\
_null\
_rm\
_sh\
_share\
_stressfs\
_usertests\
_wc\
_zombie\
_hello\
```
here `_<program name>`


done
