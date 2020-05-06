
#include "../include/kb.h"
static unsigned long int next = 1;
int rand( void ){
    next = next * 1103515245 + 12345;
    return (unsigned int)(next / 65536) % 32768;
}

void srand( unsigned int seed ){
    next = seed;
}

string readStr(){
    char buff;
    string buffstr = (string) malloc(200);
    uint8 i = 0;
    uint8 reading = 1;
    while(reading)
    {
        if(inportb(0x64) & 0x1)                 
        {
            switch(inportb(0x60))
            { 
      /*case 1:
                printch('(char)27);           Escape button
                buffstr[i] = (char)27;
                i++;rand();
                break;*/
        case 2:
                printch('1');
                buffstr[i] = '1';
                i++;rand();
		rand();
                break;
        case 3:
                printch('2');
                buffstr[i] = '2';
                i++;rand();
		rand();
                break;
        case 4:
                printch('3');
                buffstr[i] = '3';
                i++;rand();
		rand();
                break;
        case 5:
                printch('4');
                buffstr[i] = '4';
                i++;rand();
		rand();
                break;
        case 6:
                printch('5');
                buffstr[i] = '5';
                i++;rand();
		rand();
                break;
        case 7:
                printch('6');
                buffstr[i] = '6';
                i++;rand();
		rand();
                break;
        case 8:
                printch('7');
                buffstr[i] = '7';
                i++;rand();
		rand();
                break;
        case 9:
                printch('8');
                buffstr[i] = '8';
                i++;rand();
                break;
        case 10:
                printch('9');
                buffstr[i] = '9';
                i++;rand();
                break;
        case 11:
                printch('0');
                buffstr[i] = '0';
                i++;rand();
                break;
        case 12:
                printch('-');
                buffstr[i] = '-';
                i++;rand();
                break;
        case 13:
                printch('=');
                buffstr[i] = '=';
                i++;rand();
                break;
        case 14:
                printch('\b');
                i--;
                if(i<0)
                {
                	i = 0;
                }
                buffstr[i+1] = 0;
                buffstr[i] = 0;
                break;
       /* case 15:
                printch('\t');          Tab button
                buffstr[i] = '\t';
                i++;rand();
                break;*/
        case 16:
                printch('q');
                buffstr[i] = 'q';
                i++;rand();
                break;
        case 17:
                printch('w');
                buffstr[i] = 'w';
                i++;rand();
                break;
        case 18:
                printch('e');
                buffstr[i] = 'e';
                i++;rand();
                break;
        case 19:
                printch('r');
                buffstr[i] = 'r';
                i++;rand();
                break;
        case 20:
                printch('t');
                buffstr[i] = 't';
                i++;rand();
                break;
        case 21:
                printch('y');
                buffstr[i] = 'y';
                i++;rand();
                break;
        case 22:
                printch('u');
                buffstr[i] = 'u';
                i++;rand();
                break;
        case 23:
                printch('i');
                buffstr[i] = 'i';
                i++;rand();
                break;
        case 24:
                printch('o');
                buffstr[i] = 'o';
                i++;rand();
                break;
        case 25:
                printch('p');
                buffstr[i] = 'p';
                i++;rand();
                break;
        case 26:
                printch('[');
                buffstr[i] = '[';
                i++;rand();
                break;
        case 27:
                printch(']');
                buffstr[i] = ']';
                i++;rand();
                break;
        case 28:
               // printch('\n');
               // buffstr[i] = '\n';
                  i++;rand();
               reading = 0;
                break;
      /*  case 29:
                printch('q');           Left Control
                buffstr[i] = 'q';
                i++;rand();
                break;*/
        case 30:
                printch('a');
                buffstr[i] = 'a';
                i++;rand();
                break;
        case 31:
                printch('s');
                buffstr[i] = 's';
                i++;rand();
                break;
        case 32:
                printch('d');
                buffstr[i] = 'd';
                i++;rand();
                break;
        case 33:
                printch('f');
                buffstr[i] = 'f';
                i++;rand();
                break;
        case 34:
                printch('g');
                buffstr[i] = 'g';
                i++;rand();
                break;
        case 35:
                printch('h');
                buffstr[i] = 'h';
                i++;rand();
                break;
        case 36:
                printch('j');
                buffstr[i] = 'j';
                i++;rand();
                break;
        case 37:
                printch('k');
                buffstr[i] = 'k';
                i++;rand();
                break;
        case 38:
                printch('l');
                buffstr[i] = 'l';
                i++;rand();
                break;
        case 39:
                printch(';');
                buffstr[i] = ';';
                i++;rand();
                break;
        case 40:
                printch((char)44);               //   Single quote (')
                buffstr[i] = (char)44;
                i++;rand();
                break;
        case 41:
                printch((char)44);               // Back tick (`)
                buffstr[i] = (char)44;
                i++;rand();
                break;
     /* case 42:                                 Left shift 
                printch('q');
                buffstr[i] = 'q';
                i++;rand();
                break;
        case 43:                                 \ (< for somekeyboards)   
                printch((char)92);
                buffstr[i] = 'q';
                i++;rand();
                break;*/
        case 44:
                printch('z');
                buffstr[i] = 'z';
                i++;rand();
                break;
        case 45:
                printch('x');
                buffstr[i] = 'x';
                i++;rand();
                break;
        case 46:
                printch('c');
                buffstr[i] = 'c';
                i++;rand();
                break;
        case 47:
                printch('v');
                buffstr[i] = 'v';
                i++;rand();
                break;                
        case 48:
                printch('b');
                buffstr[i] = 'b';
                i++;rand();
                break;               
        case 49:
                printch('n');
                buffstr[i] = 'n';
                i++;rand();
                break;                
        case 50:
                printch('m');
                buffstr[i] = 'm';
                i++;rand();
                break;               
        case 51:
                printch(',');
                buffstr[i] = ',';
                i++;rand();
                break;                
        case 52:
                printch('.');
                buffstr[i] = '.';
                i++;rand();
                break;            
        case 53:
                printch('/');
                buffstr[i] = '/';
                i++;rand();
                break;            
        case 54:
                printch('.');
                buffstr[i] = '.';
                i++;rand();
                break;            
        case 55:
                printch('/');
                buffstr[i] = '/';
                i++;rand();
                break;            
      /*case 56:
                printch(' ');           Right shift
                buffstr[i] = ' ';
                i++;rand();
                break;*/           
        case 57:
                printch(' ');
                buffstr[i] = ' ';
                i++;rand();
                break;
            }
        }
    }
    buffstr[i-1] = 0;                
    return buffstr;
}
