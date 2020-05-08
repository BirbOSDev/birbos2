#include "../all_drivers.h"
#include "../common.h"


_keyboardirq = 0;


void keyboard_send_key(uint8_t b){
    outportb(0x64, b);
}

uint8_t keyboard_read_key()
{
    uint8_t key_code = 0;
    key_code = inportb(0x60);
    while(key_code==0){}
    return key_code;
}

void keyboardIRQHandler(struct regs *r){
    _keyboardirq = 1;
    //print("key");
}

void keyboard_install(){
    irq_install_handler(1, keyboardIRQHandler);
}

void keyboardWaitForIRQ(){
    while(!_keyboardirq) { itoa(1, 2); }

    _keyboardirq = 0;
}
uint8_t input_key(){
    uint8_t key;
    keyboardWaitForIRQ();
    key = keyboard_read_key();
    return key;
}
char input_char(){
    uint8_t key;
    keyboardWaitForIRQ();
    key = keyboard_read_key();
    return ktoc(key);
}
char input_char_shift(){
    uint8_t key;
    keyboardWaitForIRQ();
    key = keyboard_read_key();
    if(key == 0x2A){
        while(true){
            keyboardWaitForIRQ();
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
        keyboardWaitForIRQ();
        key = keyboard_read_key();
        if(key == 0xE && c > 0){
            inp[c--] = '\0';
            terminal_putcharbehind('\0');
            continue;
        }
        if(c == 1023){
            continue;
        }
        if(key == 0x2A){
            while(true){
                keyboardWaitForIRQ();
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


char* inputs(char* buf){
    uint8_t key = 0;
    int c = 0;
    while(key != 0x1C){
        keyboardWaitForIRQ();
        key = keyboard_read_key();
        if(key == 0xE && c > 0){
            buf[c--] = '\0';
            terminal_putcharbehind('\0');
            continue;
        }
        if(key == 0x2A){
            while(true){
                keyboardWaitForIRQ();
                key = keyboard_read_key();
                if(key == 0xAA)
                    break;
                if(key == 0xE && c > 0){
                    buf[c--] = '\0';
                    terminal_putcharbehind('\0');
                    continue;
                }
                if(ktocSHIFT(key) == 0){
                    continue;
                }
                buf[c++] = ktocSHIFT(key);
                terminal_putchar(ktocSHIFT(key));
            }
        }
        if(ktoc(key) == 0){
            continue;
        }
        buf[c++] = ktoc(key);
        terminal_putchar(ktoc(key));


    }
    buf[c] = '\0';
    return buf;
}

char* input_br(){
    uint8_t key = 0;
    int c = 0;
    while(ktoc(key) != ' ' || key != 0x1C){
        keyboardWaitForIRQ();
        key = keyboard_read_key();
        if(key == 0xE && c > 0){
            inp[c--] = '\0';
            terminal_putcharbehind('\0');
            continue;
        }
        if(key == 0x2A){
            while(true){
                keyboardWaitForIRQ();
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
<<<<<<< Updated upstream:Drivers/keyboard.c
=======

>>>>>>> Stashed changes:src/Drivers/keyboard.c
