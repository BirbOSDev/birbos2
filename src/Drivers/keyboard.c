#include "../all_drivers.h"
#include "../common.h"


bool _keyboardIRQ = false;

void waitForKeyboardIRQ(){
    while(!_keyboardIRQ){itoa(1, 2);}
    _keyboardIRQ = false;
}

void keyboardIRQHandler(struct regs *r){
    _keyboardIRQ = true;
}

void keyboard_install(){
    irq_install_handler(1, keyboardIRQHandler);
}

void keyboard_send_key(uint8_t b){
    outportb(0x64, b);
}

uint8_t keyboard_read_key()
{
    uint8_t key_code = 0;
	if (inportb(0x64) & 1){
        key_code = inportb(0x60);
        
        while(key_code==0){}
        return key_code;
	}
    return 0;
}
uint8_t input_key(){
    uint8_t key;
    waitForKeyboardIRQ();
    key = keyboard_read_key();
    return key;
}
char input_char(){
    uint8_t key;
    waitForKeyboardIRQ();
    key = keyboard_read_key();
    return ktoc(key);
}
char input_char_shift(){
    uint8_t key;
    waitForKeyboardIRQ();
    key = keyboard_read_key();
    if(key == 0x2A){
        while(true){
            waitForKeyboardIRQ();
    	    key = keyboard_read_key();
            if(key == 0xAA)
                break;
            if(ktocSHIFT(key) == 0){
                continue;
            }
            
            return ktocSHIFT(key);
        }
    }
    return ktoc(key);
}

char* input(){
    uint8_t key = 0;
    int c = 0;
    while(key != 0x1C){
        waitForKeyboardIRQ();
        key = keyboard_read_key();
        if(key == 0xE && c > 0){
            inp[c--] = '\0';
            terminal_putcharbehind('\0');
            continue;
        }
        if(key == 0x2A){
            while(true){
                waitForKeyboardIRQ();
                key = keyboard_read_key();
                if(key == 0xAA)
                    break;
                if(key == 0xE && c > 0){
                    inp[c--] = '\0';
                    terminal_putcharbehind('\0');
                    continue;
                }
                if(ktocSHIFT(key) == 0){
                    continue;
                }
                inp[c++] = ktocSHIFT(key);
                terminal_putchar(ktocSHIFT(key));
            }
        }
        if(ktoc(key) == 0){
            continue;
        }
        inp[c++] = ktoc(key);
        terminal_putchar(ktoc(key));


    }
    inp[c] = '\0';
    return inp;
}

char* input_br(){
    uint8_t key = 0;
    int c = 0;
    while(ktoc(key) != ' ' || key != 0x1C){
        waitForKeyboardIRQ();
        key = keyboard_read_key();
        if(key == 0xE && c > 0){
            inp[c--] = '\0';
            terminal_putcharbehind('\0');
            continue;
        }
        if(key == 0x2A){
            while(true){
                waitForKeyboardIRQ();
        	key = keyboard_read_key();
                if(key == 0xAA)
                    break;
                if(key == 0xE && c > 0){
                    inp[c--] = '\0';
                    terminal_putcharbehind('\0');
                    continue;
                }
                if(ktocSHIFT(key) == 0){
                    continue;
                }
                inp[c++] = ktocSHIFT(key);
                terminal_putchar(ktocSHIFT(key));
            }
        }
        if(ktoc(key) == 0){
            continue;
        }
        inp[c++] = ktoc(key);
        terminal_putchar(ktoc(key));


    }
    inp[c] = '\0';
    return inp;
}