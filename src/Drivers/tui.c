#include "../all_drivers.h"
#include "../common.h"

char _mc = 30;

bool __set__ = false;
int clickTimer = 420;
bool showMenu = false;
bool showWeekday = false;
bool showTestWindow = false;
bool __drag = false;
int __prevX = 0;
int __prevY = 0;
int __grabX = 0;
int renderdelay = 5;
extern uint16_t* terminal_buffer;
extern bool barEnabled;
extern uint16_t terminal_buffer_main[90*60*2];
extern unsigned char second;
extern unsigned char minute;
extern unsigned char hour;
extern unsigned char day;
extern unsigned char month;
extern unsigned char weekday;
extern unsigned int year;
extern unsigned char century;
extern bool lmouseDown;
extern bool terminalmousecursor;
extern unsigned char dcolor;
extern int VGA_WIDTH;
extern int VGA_HEIGHT;
extern uint16_t terminal_buffer_layer[90*60*2];
extern int16_t mouseX;
extern int16_t mouseY;

void drawBox(uint8_t c, uint8_t col, int x, int x2, int y, int y2){
    for(int i = x; i<x2; i++){
        for(int j = y; j<y2; j++){
            _terminal_putentryat(c,col,i,j);
        }
    }
}

void textAt(char* str, uint8_t col, int x, int wrapx, int y){
    int wraps = 0;
    int c = x;
    int currx = x;
    
    for(int i = 0; i<strlen(str); i++){
        if(str[i] == '\n'){
            wraps++;
            continue;
        }
        if(c >= wrapx){
            c=x;
            currx=x;
            wraps++;
        }
        _terminal_putentryat(str[i], col, currx, y+wraps);
        c++;
        currx++;
    }
}

void barRender(){

    if(!barEnabled)
		return;
    
	char data[90] = " ";
    if(showWeekday){
	    switch(weekday){
            case 1: strcat(data, "Sunday "); break;
            case 2: strcat(data, "Monday "); break;
            case 3: strcat(data, "Tuesday "); break;
            case 4: strcat(data, "Wednesday "); break;
            case 5: strcat(data, "Thursday "); break;
            case 6: strcat(data, "Friday "); break;
            case 7: strcat(data, "Saturday "); break;
            default: break;
        }
    }
    strcat(data, itoa(day,10));
    strcat(data, "/");
    strcat(data, itoa(month,10));
    strcat(data, "/");
    strcat(data, itoa(year,10));
    strcat(data, " ");
    if(hour<10){
        strcat(data, "0");
    }
    strcat(data, itoa(hour,10));
    strcat(data, ":");
    if(minute<10){
        strcat(data, "0");
    }
    strcat(data, itoa(minute,10));
    strcat(data, ":");
    if(second<10){
        strcat(data, "0");
    }
    
    strcat(data, itoa(second,10));
    strcat(data, "               ");
    strcat(data, itoa(mouseX, 10));
    strcat(data, ", ");
    strcat(data, itoa(mouseY, 10));

	int len = strlen(data);
	for(int i = len; i < VGA_WIDTH - 6; i++){
		data[i] = ' ';
	}
    for (size_t i = 0; i < strlen(data); i++){
        _terminal_putentryat(data[i], 0x87, i, VGA_HEIGHT);
    }
    len = strlen(data);
	strcat(data, "birbOS");
    if(mouseX > VGA_WIDTH-7 && mouseX < VGA_WIDTH && mouseY == VGA_HEIGHT && terminalmousecursor){
        for (size_t i = len; i < strlen(data); i++){
            _terminal_putentryat(data[i], 0x87, i, VGA_HEIGHT);
        }
    } else {
        for (size_t i = len; i < strlen(data); i++){
            _terminal_putentryat(data[i], 0x78, i, VGA_HEIGHT);
        }
    }
	
}

void renderCursor(){
    if(terminalmousecursor){

        uint16_t num = _terminal_getentryat(mouseX, mouseY);
        num = (num >> 8)&0xFF;
        bool __e = false;
        char nibble = (char) ((num >> 4) & 0x0F);
        char inverted = nibble;
        //bgcolor = (bgcolor & 0xF0) | (bgcolor & 0xF);
        if(inverted == 0x8){
            inverted = 0x7;
            __e = true;
        }
        if(getBit(inverted, 4) && !__e){
            inverted &= ~(1 << 3);
        } else if (!__e) {
            inverted |= 1 << 3;
        }

        dcolor = setUpperNibble(dcolor, nibble);
        dcolor = setLowerNibble(dcolor, inverted);

        if(lmouseDown){
            _mc = 254;
        } else {
            _mc = 30;
        }
        _terminal_putentryat(_mc, dcolor, mouseX, mouseY);

        
    }
}

void menuDrawClickableText(char* text, int posx, int maxx, int posy){
    if(mouseX > posx - 1 && mouseX < maxx && mouseY == posy){
        textAt(text, 0x87, posx, maxx, posy);
    } else {
        textAt(text, 0x78, posx, maxx, posy);
    }
}


void renderErrorWindow(char* err){
    drawBox(219, 0x88, VGA_WIDTH/4, VGA_WIDTH/4+40, VGA_HEIGHT/5, VGA_HEIGHT/5+10);
    drawBox(219, 0x77, VGA_WIDTH/4, VGA_WIDTH/4+40, VGA_HEIGHT/5, VGA_HEIGHT/5+1);
    menuDrawClickableText("X", VGA_WIDTH/4+39, VGA_WIDTH/4+40, VGA_HEIGHT/5);
    textAt("   ERROR", 0x78, VGA_WIDTH/4, VGA_WIDTH/4+40, VGA_HEIGHT/5);
    textAt(err, 0x87, VGA_WIDTH/4+1, VGA_WIDTH/4+40, VGA_HEIGHT/5+2);
}


void terminalRenderTask(int taskno){
    removeTask(taskno);
    memcpy(terminal_buffer_main, terminal_buffer_layer, VGA_WIDTH*(VGA_HEIGHT+1)*2);
    
    

    if(lmouseDown){
        if(mouseX > VGA_WIDTH-7 && mouseX < VGA_WIDTH && mouseY == VGA_HEIGHT && terminalmousecursor){
            int ret = stopTimer(clickTimer);
            if(ret > 50 + renderdelay || ret == -2){
                if(showMenu){
                    showMenu = false;
                }else
                if(!showMenu){
                    showMenu = true;
                }
            }
            clickTimer = startTimer();
        }
        if((mouseY < VGA_HEIGHT-13 || (mouseX < VGA_WIDTH-17 && mouseY <  VGA_HEIGHT))){
            if(showMenu){
                showMenu = false;
            }
        }
    }
    if(showMenu){
        drawBox(219, 0x88, VGA_WIDTH-17, VGA_WIDTH, VGA_HEIGHT-13, VGA_HEIGHT);
        textAt("   Start Menu", 0x87, VGA_WIDTH-17, VGA_WIDTH, VGA_HEIGHT-13);
        menuDrawClickableText("   Test window   ", VGA_WIDTH-17, VGA_WIDTH, VGA_HEIGHT-11);
        menuDrawClickableText("--[Placeholder]--", VGA_WIDTH-17, VGA_WIDTH, VGA_HEIGHT-10);
        menuDrawClickableText("--[Placeholder]--", VGA_WIDTH-17, VGA_WIDTH, VGA_HEIGHT-9);
        menuDrawClickableText("--[Placeholder]--", VGA_WIDTH-17, VGA_WIDTH, VGA_HEIGHT-8);
        menuDrawClickableText("--[Placeholder]--", VGA_WIDTH-17, VGA_WIDTH, VGA_HEIGHT-7);
        menuDrawClickableText("--[Placeholder]--", VGA_WIDTH-17, VGA_WIDTH, VGA_HEIGHT-6);
        menuDrawClickableText("--[Placeholder]--", VGA_WIDTH-17, VGA_WIDTH, VGA_HEIGHT-5);
        menuDrawClickableText("     Reboot      ", VGA_WIDTH-17, VGA_WIDTH, VGA_HEIGHT-4);
        menuDrawClickableText("    Shutdown     ", VGA_WIDTH-17, VGA_WIDTH, VGA_HEIGHT-3);
        if(lmouseDown){
            int ret = stopTimer(clickTimer);
            if(ret > 50 + renderdelay || ret == -2){
                if(mouseX > VGA_WIDTH-17 && mouseX < VGA_WIDTH && mouseY == VGA_HEIGHT-11 && terminalmousecursor){
                    showMenu = false;
                    showTestWindow = true;
                }
                if(mouseX > VGA_WIDTH-17 && mouseX < VGA_WIDTH && mouseY == VGA_HEIGHT-4 && terminalmousecursor){
                    showMenu = false;
                    uint8_t good = 0x02;
                    while (good & 0x02)
                        good = inportb(0x64);
                    outportb(0x64, 0xFE);
                    asm("hlt");
                }
                if(mouseX > VGA_WIDTH-17 && mouseX < VGA_WIDTH && mouseY == VGA_HEIGHT-3 && terminalmousecursor){
                    showMenu = false;
                    acpiPowerOff();
                    
                }
            }
            clickTimer = startTimer();
            
        }
        
        
    }

    if(showTestWindow){
        drawBox(219, 0x88, VGA_WIDTH/4, VGA_WIDTH/4+40, VGA_HEIGHT/5, VGA_HEIGHT/5+10);
        drawBox(219, 0x77, VGA_WIDTH/4, VGA_WIDTH/4+40, VGA_HEIGHT/5, VGA_HEIGHT/5+1);
        menuDrawClickableText("X", VGA_WIDTH/4+39, VGA_WIDTH/4+40, VGA_HEIGHT/5);
        textAt("   Title", 0x78, VGA_WIDTH/4, VGA_WIDTH/4+40, VGA_HEIGHT/5);
        textAt(" xandrei was here", 0x87, VGA_WIDTH/4, VGA_WIDTH/4+40, VGA_HEIGHT/5+2);
        textAt(" mori in foc", 0x87, VGA_WIDTH/4, VGA_WIDTH/4+40, VGA_HEIGHT/5+4);
        if(lmouseDown){
            int ret = stopTimer(clickTimer);
            if(ret > 50 + renderdelay || ret == -2){
                if(mouseX > VGA_WIDTH/4+38 && mouseX < VGA_WIDTH/4+40 && mouseY == VGA_HEIGHT/5 && terminalmousecursor)
                    showTestWindow = false;
            }
            clickTimer = startTimer();
            
            
            
            
            
            
        }
    }

    barRender();

    renderCursor();
    memcpy(terminal_buffer, terminal_buffer_main, VGA_WIDTH*(VGA_HEIGHT+1)*2);
    newTask(terminalRenderTask, renderdelay);
}


