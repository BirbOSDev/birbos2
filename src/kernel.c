#include "Drivers/multiboot.h"
#include "Drivers/mouse.h"
#include "common.h"
#include "all_drivers.h"
#include "types.h"
unsigned int terminal_start;
//int to string conversion from old code
void * malloc(int nbytes){
	char variable[nbytes];
	return &variable;
}

void int_to_ascii(int n, char str[]) {          
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';         
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

    /* TODO: implement "reverse" */
    return str;
}

string int_to_string(int n)
{
    string ch = malloc(50);
    int_to_ascii(n,ch);
    int len = strlen(ch);
    int i = 0;
    int j = len - 1;
    while(i<(len/2 + len%2))
    {
        char tmp = ch[i];
        ch[i] = ch[j];
        ch[j] = tmp;
        i++;
        j--;
    }
    return ch;
}
int str_to_int(string ch)
{
	int n = 0;
	int p = 1;
	int strlenx = strlen(ch);
	int i;
	for (i = strlenx-1;i>=0;i--)
	{
		n += ((int)(ch[i] - '0')) * p;
		p *= 10;
	}
	return n;
}

void loadingTask(){
    print("| loading"); for(int i = 0; i < 65535*255; i++) {} terminal_initialize();
    print("/ loading"); for(int i = 0; i < 65535*255; i++) {} terminal_initialize();
    print("- loading"); for(int i = 0; i < 65535*255; i++) {} terminal_initialize();
    print("\\ loading");for(int i = 0; i < 65535*255; i++) {} terminal_initialize();
    print("| loading"); for(int i = 0; i < 65535*255; i++) {} terminal_initialize();
}




void kernel_main(multiboot_info_t* mbi, unsigned int magic){
    timer_install(1000);
    int _boot_timer_ = startTimer();
    terminal_initialize();
    gdt_install();
    idt_install();
    irq_install();
    __asm__ __volatile__ ("sti");
    keyboard_install();
    isrs_install();
    mouse_install();
    outportb(0x70, inportb(0x70) & 0x7F);
    int _time_boot_ = stopTimer(_boot_timer_);
    //newTask(barTask, 5);
    int __ret = -1;
    while(__ret == -1)
        __ret = newTask(terminalRenderTask, 5);
    mouseToggleTerminalCursor();
    keyboard_send_key(0xe1);
    print("render failed. please restart your computer.");
    memcpy(terminal_buffer, terminal_buffer_layer, 2048*2);
    terminal_initialize();
    read_rtc();
    initAcpi();
    
   
    //print(itoa(mbi->mem_upper, 10));

    //while(1) print(itoa(keyboard_read_key(), 16));
    

    print("boot took ");
    print(itoa(_time_boot_, 10));
    print("ms");
    print("\n");
    //newTask(taskTest, 1000);
    print_c("Welcome to BirbOS!\n",VGA_COLOR_LIGHT_GREEN);
    while(true){
        print("birb>");
        char* cmd = input();
        print("\n");
        if(strequ(cmd, "echo")){
            print("Enter string:\n");
            char* string = input();
            print("\n");
            print(string);
            print("\n");
            
        }
        else if(strequ(cmd,"about")){
        
            print("\n");
			print("oooooo____oo__________oo__________oooo_____ooooo__\n");
		    print("oo____oo______oo_ooo__oooooo____oo____oo__oo___oo_\n");
		    print("oooooooo__oo__ooo___o_oo___oo__oo______oo__oo_____\n");
		    print("oo____oo__oo__oo______oo___oo__oo______oo____oo___\n");
        	print("oo____oo__oo__oo______oo___oo___oo____oo__oo___oo_\n");
        	print("ooooooo__oooo_oo______oooooo______oooo_____ooooo__\n");
        	print("____________________________________________________\n");
                  
               
        }
        else if(strequ(cmd,"calc")){
            print("Enter + to add,- to subtract,* to multiply,/ to divide\n");
            string opcode = input();
            if(strequ(opcode,"+")){
            print("\nNum1:\n");
            int x = str_to_int(input());
            print("\nNum2:\n");
            int y = str_to_int(input());
            print("\n");
            print(int_to_string(x+y));
            print("\n");
            print("\n");   
            }
            else if(strequ(opcode,"-")){
                print("\nNum1:\n");
                int x = str_to_int(input());
                print("\nNum2:\n");
                int y = str_to_int(input());
                print("\n");
                print(int_to_string(x-y));
                print("\n");   
            }
            else if(strequ(opcode,"*")){
                print("\nNum1:\n");
                int x = str_to_int(input());
                print("\nNum2:\n");
                int y = str_to_int(input());
                print("\n");
                print(int_to_string(x*y));
                print("\n"); 
            }
            else if(strequ(opcode,"/")){
                print("\nNum1:\n");
                int x = str_to_int(input());
                print("\nNum2:\n");
                int y = str_to_int(input());
                print("\n");
                print(int_to_string(x/y));
                print("\n"); 
            }
            else{
                print("\nenter a valid operation rerun the program\n");
            }
        }
        else if(strequ(cmd,"help")){
            print("\nclear       : Clears the screen");
            print("\necho        : Takes input text reprint a given text");
            print("\nabout       : Shows details of the OS");
            print("\ncalc        : Calculator");
            print("\ndie         : Halts the system");
            print("\ntime        : Shows the time from RTC");
            print("\nrand        : Returns a random number");
            print("\ncursor      : Toggles the mouse cursor (note that it can break while scrolling)");
            print("\ncpuvendor   : Get the 12 character vendor string from CPUID");
            print("\nsensitivity : Set mouse sensitivity (higher number - lower sensitivity)");
            print("\ncursorcolor : Change mouse cursor color");
            print("\n\n");
        }
        else if(strequ(cmd,"clear")){
            terminal_initialize();
        }
        else if(strequ(cmd,"sensitivity")){
            setMouseSensitivity(atoi(input(), 10));
            print("\n");
        }
        else if(strequ(cmd,"cursorcolor")){
            print("\n0 BLACK");
            print("\n1 BLUE");
            print("\n2 GREEN");
            print("\n3 CYAN");
            print("\n4 RED");
            print("\n5 MAGENTA");
            print("\n6 BROWN");
            print("\n7 LIGHT GREY");
            print("\n8 DARK GREY");
            print("\n9 LIGHT BLUE");
            print("\nA LIGHT GREEN");
            print("\nB LIGHT CYAN");
            print("\nC LIGHT RED");
            print("\nD LIGHT MAGENTA");
            print("\nE LIGHT BROWN");
            print("\nF WHITE\n");
            print("Left click color (1): ");
            lcolor = atoi(input(), 16);
            print("\nRight click color (4): ");
            rcolor = atoi(input(), 16);
            print("\nMiddle click color (2): ");
            mcolor = atoi(input(), 16);
            print("\nDefault color (7): ");
            dcolor = atoi(input(), 16);
            print("\n");


        }
        else if(strequ(cmd, "memory")){
            if((mbi->flags >> 6) & 1){
                print(itoa(mbi->mem_upper, 10));
                
            } else {
                __asm__ __volatile__ (

                    "probeRAM:\n" 
                "    push eax\n"
                "    push ebx\n"
                "    push edx\n"
                "    push ebp\n" 
                "    mov esi, 0x10000\n" 
                "    mov edx, 2147483647\n" 
                "    mov ebp,esi\n" 
                "    add esi,0x00000FFF\n"
                "    and esi, 0x00000FFF\n"
                "    push esi\n"
                "    mov eax, esi\n"
                "    sub eax, ebp\n"
                "    xor ecx,ecx\n"
                "    sub edx,eax\n"
                "    jc .done\n"
                "    or esi,0x00000FFC\n"
                "    shr edx,12\n"
                "    je .done\n"
                " \n"
                ".testAddress:\n"
                "    mov eax,[esi]\n"
                "    mov ebx,eax\n"
                "    not eax\n"
                "    mov [esi],eax\n"
                "    mov [0x2000],ebx\n"
                "    wbinvd\n"
                "    mov ebp,[esi]\n"
                "    mov [esi],ebx\n"
                "    cmp ebp,eax\n"
                "    jne .done\n"
                " \n" 
                " \n"
                "    add ecx,0x00001000\n"
                "    add esi,0x00001000\n"
                "    dec edx\n" 
                "    jne .testAddress\n"
                " \n"
                " \n"
                ".done:\n"
                "    pop esi\n"
                "    pop ebp\n"
                "    pop edx\n"
                "    pop ebx\n"
                "    pop eax");
                register uint32_t r asm("ecx");
                print(itoa(r/1024, 10));
            }
            print("KiB");
            print("\n");
            
        }
        else if(strequ(cmd,"cursor")){
            mouseToggleTerminalCursor();
        }
        else if(strequ(cmd,"cpuvendor")){
            char str[13];
            print(cpuidstring(0, str));
            print("\n");

        }
	    else if(strequ(cmd,"time")){
            print(itoa(rtcGetUnixTimestamp(), 10));
	        print("\n");
	        rtc_print_formatted_time();
	        print("\n");
        }
        else if(strequ(cmd,"rand")){
	        print(itoa(rand(), 10));
            print("\n");
        }
        else if(strequ(cmd,"die")){
            terminal_initialize();
            char* death[7] = {"string1","string2","string3","string4","string5","string6","string7"};
            print_c(death[randomInt(6)],VGA_COLOR_RED);
            memcpy(terminal_buffer, terminal_buffer_layer, 2048*2);
            __asm__ __volatile__("cli\nhlt");
            
        }
        else if(strequ(cmd, "")){
            print("");
        }
       else{
            print("\nBad Command\n");   
        }

    }
  

}
