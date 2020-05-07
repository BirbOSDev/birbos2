//#include "all_drivers.h"
#include "common.h"
#include "Drivers/VGA.c"
#include "Drivers/keyboard.c"
#include "types.h"
//#include "Drivers/rtc.c"
//#include "Drivers/irq.h"

void * malloc(int nbytes)
{
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

void kernel_main(void) {
  //  terminal_initialize();
  //  terminal_writestring("Hello, world!\n");
    terminal_initialize();
      print("\n");
			  print("oooooo____oo__________oo__________oooo_____ooooo__\n");
		          print("oo____oo______oo_ooo__oooooo____oo____oo__oo___oo_\n");
		          print("oooooooo__oo__ooo___o_oo___oo__oo______oo__oo_____\n");
		          print("oo____oo__oo__oo______oo___oo__oo______oo____oo___\n");
        		  print("oo____oo__oo__oo______oo___oo___oo____oo__oo___oo_\n");
        		  print("ooooooo__oooo_oo______oooooo______oooo_____ooooo__\n");
        		  print("____________________________________________________\n");
    print("Welcome to BirbOS !\nType in 'help' to view all the commands availaible\n");
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
            /*char x[16];
            char y[16];
            print("Num1:\n");
            strcpy(input(), &x);
            print("\nNum2:\n");
            strcpy(input(), &y);
            int n1 = atoi(x,10);
            int n2 = atoi(y,10);
            print("\n");
            print(itoa(n1+n2,10));
            print("\n");
            */
        }
        else if(strequ(cmd,"help")){
            print("\nclear     : Clears the screen");
            print("\necho      : Reprint a given text");
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
            print("Good Job you halted the cpu go now power it off or restart it");
            asm("hlt");
        }
        else if(strequ(cmd, "")){
            print("");
        }
       else{
            print("\nBad Command\n");   
        }

    }
    
}
