#include "../include/kb.h"
#include "../include/isr.h"
#include "../include/idt.h"
#include "../include/util.h"
#include "../include/shell.h"
kmain()
{
	isr_install();
	clearScreen();
	print("Booting Up Birb\nSucessfully Booted up\n");
	print("\n\n");
	print("oooooo____oo__________oo____________oooo_____ooooo__\n");
	print("oo____oo______oo_ooo__oooooo______oo____oo__oo___oo_\n");
	print("oooooooo__oo__ooo___o_oo___oo____oo______oo__oo_____\n");
	print("oo____oo__oo__oo______oo___oo____oo______oo____oo___\n");
	print("oo____oo__oo__oo______oo___oo_____oo____oo__oo___oo_\n");
	print("ooooooo__oooo_oo______oooooo________oooo_____ooooo__\n");
	print("____________________________________________________\n");
	print("\n\nWelcome to Birb\n");
    launch_shell(0);    
}
