#include "Drivers/multiboot.h"
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
void kernel_main(multiboot_info_t* mbi, unsigned int magic){
    terminal_initialize();
    gdt_install();
    idt_install();
    irq_install();
    __asm__ __volatile__ ("sti");
    isrs_install();
    timer_install(1000);
    print("\n");
    sleep(1000);
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
		          print("\nBirb OS 0.3 (New Drivers Version)\n");

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
            print("\nclear     : Clears the screen");
            print("\necho      : Takes input text reprint a given text");
            print("\nabout     : Prints outs detail of the os");
            print("\ncalc      : Calculator");
            print("\ndie       : Halts the system");
            print("\n\n");
        }
        else if(strequ(cmd,"clear")){
           terminal_initialize();
        }
        else if(strequ(cmd,"die")){
            terminal_initialize();
            print_c("Your PC Just halted\nPlease restart to continue using your pc",VGA_COLOR_RED);
            //asm("hlt"); what it was new implemetaion starts
            _asm{
                hlt
            }//new implementation ends
            //old halt
            //while(true){
            //    //worst implemention i know but asm("hlt") has no mood to work
               // return 0;
            //}
            
        }
        else if(strequ(cmd, "")){
            print("");
        }
       else{
            print("\nBad Command\n");   
        }

    }
  

}
