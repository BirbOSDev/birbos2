#include "../common.h"
#include "../all_drivers.h"


bool handledLDwn = false;
bool handledRDwn = false;
uint16_t paintBuffer[512][90*50*2];

void PaintGame(){
    uint8_t key;
    int currentBuf = 0;
    int lclicktimer = 42069;
    int rclicktimer = 42069;
    int pressTimer = 42069;
    bool ldclick = false;
    bool rdclick = false;
    int primcolor = 0xF;
    int seccolor = 0x0;
    terminal_initialize();
    while(true){
        key = old_keyboard_read_key();
        if(key == 0x1){
            break;
        }
        // 4b 4d
        if(key == 0x4B){
            int _ms = stopTimer(pressTimer);
            if(_ms > 40){
                if(_ms > 1){
                    currentBuf--;
                        if(currentBuf < 0){
                            currentBuf = 0;
                        }
                    }
            }
            pressTimer = startTimer();
        }
        if(key == 0x4D){
            int _ms = stopTimer(pressTimer);
            if(_ms > 40){
                if(_ms > 1){
                    currentBuf++;
                        if(currentBuf > 512){
                            currentBuf = 512;
                        }
                    }
            }
            pressTimer = startTimer();
        }


        if(lmouseDown){
            if(mouseY < VGA_HEIGHT && mouseY > VGA_HEIGHT-3 && mouseX < 16*3){
                primcolor = mouseX/3;
            }

            int _ms = stopTimer(lclicktimer);
            
            buffer_putentryat(paintBuffer[currentBuf], 219, setUpperNibble(primcolor, primcolor), mouseX, mouseY);

            if(_ms < 75){
                if(_ms > 1 && !handledLDwn){
                    for(int i = 0; i<VGA_HEIGHT; i++){
                        for(int j = 0; j<VGA_WIDTH; j++){
                            buffer_putentryat(paintBuffer[currentBuf], 219, setUpperNibble(primcolor, primcolor), j, i);
                        }
                    }
                }
            }
            handledLDwn = true;
            lclicktimer = startTimer();
        } else {
            handledLDwn = false;
        }
        if(rmouseDown){
            if(mouseY < VGA_HEIGHT && mouseY > VGA_HEIGHT-3 && mouseX < 16*3){
                seccolor = mouseX/3;
            }

            int _ms = stopTimer(rclicktimer);
            

            buffer_putentryat(paintBuffer[currentBuf], 219, setUpperNibble(seccolor, seccolor), mouseX, mouseY);
            if(_ms < 75){
                if(_ms > 1 && !handledRDwn){
                    for(int i = 0; i<VGA_HEIGHT; i++){
                        for(int j = 0; j<VGA_WIDTH; j++){
                            buffer_putentryat(paintBuffer[currentBuf], 219, setUpperNibble(seccolor, seccolor), j, i);
                        }
                    }
                }
            }
            handledRDwn = true;
            rclicktimer = startTimer();
        } else {
            handledRDwn = false;
        }
        for(int j = 1; j<3; j++){
        for(int i = 0; i<16*3; i++){
            buffer_putentryat(paintBuffer[currentBuf], 219, setUpperNibble(i/3, i/3), i, VGA_HEIGHT-j);
        }
        for(int i = 16*3; i<VGA_WIDTH; i++){
            buffer_putentryat(paintBuffer[currentBuf], 219, 0x88, i, VGA_HEIGHT-j);
        }
        }
        char* name = "paint";
        for(int i = 0; i<strlen(name); i++){
            buffer_putentryat(paintBuffer[currentBuf], name[i], 0x87, i+(VGA_WIDTH-strlen(name)), VGA_HEIGHT-2);
        }

        char* no = itoa(currentBuf, 10);
        for(int i = 0; i<strlen(no); i++){
            buffer_putentryat(paintBuffer[currentBuf], no[i], 0x87, i+(VGA_WIDTH-strlen(no)), VGA_HEIGHT-1);
        }

        memcpy(terminal_buffer_layer, paintBuffer[currentBuf], 90*50*2);
    }
    terminal_initialize();

}