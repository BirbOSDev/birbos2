#include "../common.h"
#include "../all_drivers.h"


bool handledLDwn = false;
bool handledRDwn = false;
uint16_t paintBuffer[512][90*50*2];
extern uint16_t* terminal_buffer;
extern bool lmouseDown;
extern bool rmouseDown;
extern int VGA_WIDTH;
extern int VGA_HEIGHT;
extern uint16_t terminal_buffer_layer[90*60*2];
extern int16_t mouseX;
extern int16_t mouseY;

void PaintGame(){
    uint8_t key;
    int currentBuf = 0;
    int lclicktimer = 42069;
    int rclicktimer = 42069;
    int pressTimer = 42069;
    char brush = 219;
    bool ldclick = false;
    bool rdclick = false;
    int lineX1 = -1;
    int lineX2 = -1;
    int lineY1 = -1;
    int lineY2 = -1;
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
        
        if(key != 0){
            continue;
        }


        if(lmouseDown){
            if(mouseY < VGA_HEIGHT && mouseY > VGA_HEIGHT-3 && mouseX < 16*3){
                primcolor = mouseX/3;
            }

            if(mouseY == VGA_HEIGHT-1 && mouseX > (16*3+1) && mouseX<(16*3+6)){
                if(mouseX<(16*3+5)){
                    brush = mouseX-16*3 + 174;
                } else {
                    brush = 219;
                }
            }

            int _ms = stopTimer(lclicktimer);
            
            buffer_putentryat(paintBuffer[currentBuf], brush, primcolor, mouseX, mouseY);

            if(_ms < 75){
                if(_ms > 1 && !handledLDwn){
                    for(int i = 0; i<VGA_HEIGHT; i++){
                        for(int j = 0; j<VGA_WIDTH; j++){
                            buffer_putentryat(paintBuffer[currentBuf], brush, primcolor, j, i);
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
            

            buffer_putentryat(paintBuffer[currentBuf], brush, seccolor, mouseX, mouseY);
            if(_ms < 75){
                if(_ms > 1 && !handledRDwn){
                    for(int i = 0; i<VGA_HEIGHT; i++){
                        for(int j = 0; j<VGA_WIDTH; j++){
                            buffer_putentryat(paintBuffer[currentBuf], brush, seccolor, j, i);
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
            buffer_putentryat(paintBuffer[currentBuf], brush, i/3, i, VGA_HEIGHT-j);
        }
        for(int i = 16*3; i<VGA_WIDTH; i++){
            buffer_putentryat(paintBuffer[currentBuf], brush, 0x88, i, VGA_HEIGHT-j);
        }
        }

        buffer_putentryat(paintBuffer[currentBuf], 176, 0x87, 16*3+2, VGA_HEIGHT-1);
        buffer_putentryat(paintBuffer[currentBuf], 177, 0x87, 16*3+3, VGA_HEIGHT-1);
        buffer_putentryat(paintBuffer[currentBuf], 178, 0x87, 16*3+4, VGA_HEIGHT-1);
        buffer_putentryat(paintBuffer[currentBuf], 219, 0x87, 16*3+5, VGA_HEIGHT-1);
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
