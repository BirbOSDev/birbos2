#include "../common.h"
#include "../all_drivers.h"


bool handledLDwn = false;
bool handledRDwn = false;
uint16_t paintBuffer[90*50*2];

void PaintGame(){
    uint8_t key;
    int lclicktimer = 42069;
    int rclicktimer = 42069;
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
        if(lmouseDown){
            if(mouseY == VGA_HEIGHT-1 && mouseX < 16*2){
                primcolor = mouseX/2;
            }

            int _ms = stopTimer(lclicktimer);
            
            buffer_putentryat(paintBuffer, 219, setUpperNibble(primcolor, primcolor), mouseX, mouseY);

            if(_ms < 100){
                if(_ms > 1 && !handledLDwn){
                    for(int i = 0; i<VGA_HEIGHT; i++){
                        for(int j = 0; j<VGA_WIDTH; j++){
                            buffer_putentryat(paintBuffer, 219, setUpperNibble(primcolor, primcolor), j, i);
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
            if(mouseY == VGA_HEIGHT-1 && mouseX < 16*2){
                seccolor = mouseX/2;
            }

            int _ms = stopTimer(rclicktimer);
            

            buffer_putentryat(paintBuffer, 219, setUpperNibble(seccolor, seccolor), mouseX, mouseY);
            if(_ms < 100){
                if(_ms > 1 && !handledRDwn){
                    for(int i = 0; i<VGA_HEIGHT; i++){
                        for(int j = 0; j<VGA_WIDTH; j++){
                            buffer_putentryat(paintBuffer, 219, setUpperNibble(seccolor, seccolor), j, i);
                        }
                    }
                }
            }
            handledRDwn = true;
            rclicktimer = startTimer();
        } else {
            handledRDwn = false;
        }
        for(int i = 0; i<16*2; i++){
            buffer_putentryat(paintBuffer, 219, setUpperNibble(i/2, i/2), i, VGA_HEIGHT-1);
        }
        for(int i = 16*2; i<VGA_WIDTH; i++){
            buffer_putentryat(paintBuffer, 219, 0x88, i, VGA_HEIGHT-1);
        }

        memcpy(terminal_buffer_layer, paintBuffer, 90*50*2);
    }
    terminal_initialize();

}