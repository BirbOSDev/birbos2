#include "../all_drivers.h"
#include "../common.h"

char _mc = 30;

bool __set__ = false;
int clickTimer = 420;
bool showMenu = false;
bool showWeekday = false;
bool showTestWindow = false;
int testWindowX = 20;
int testWindowY = 5;
bool __drag = false;
int __prevX = 0;
int __prevY = 0;
int __grabX = 0;
int renderdelay = 5;

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
    
	char data[80] = " ";
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
	for(int i = len; i < 80 - 6; i++){
		data[i] = ' ';
	}
    for (size_t i = 0; i < strlen(data); i++){
        terminal_putentryat(data[i], 0x87, i, 24);
    }
    len = strlen(data);
	strcat(data, "birbOS");
    if(mouseX > 73 && mouseX < 80 && mouseY == 24 && terminalmousecursor){
        for (size_t i = len; i < strlen(data); i++){
            terminal_putentryat(data[i], 0x87, i, 24);
        }
    } else {
        for (size_t i = len; i < strlen(data); i++){
            terminal_putentryat(data[i], 0x78, i, 24);
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
    drawBox(219, 0x88, 20, 20+40, 5, 5+10);
    drawBox(219, 0x77, 20, 20+40, 5, 5+1);
    menuDrawClickableText("X", 20+39, 20+40, 5);
    textAt("   ERROR", 0x78, 20, 20+40, 5);
    textAt(err, 0x87, 20+1, 20+40, 5+2);
}


void terminalRenderTask(int taskno){
    removeTask(taskno);
    memcpy(terminal_buffer_main, terminal_buffer_layer, 80*25*2);
    
    

    if(lmouseDown){
        if(mouseX > 73 && mouseX < 80 && mouseY == 24 && terminalmousecursor){
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
        if((mouseY < 12 || (mouseX < 63 && mouseY < 24))){
            if(showMenu){
                showMenu = false;
            }
        }
    }
    if(showMenu){
        drawBox(219, 0x88, 63, 80, 12, 24);
        textAt("   Start Menu", 0x87, 63, 80, 12);
        menuDrawClickableText("   Test window   ", 63, 80, 14);
        menuDrawClickableText("--[Placeholder]--", 63, 80, 15);
        menuDrawClickableText("--[Placeholder]--", 63, 80, 16);
        menuDrawClickableText("--[Placeholder]--", 63, 80, 17);
        menuDrawClickableText("--[Placeholder]--", 63, 80, 18);
        menuDrawClickableText("--[Placeholder]--", 63, 80, 19);
        menuDrawClickableText("--[Placeholder]--", 63, 80, 20);
        menuDrawClickableText("     Reboot      ", 63, 80, 21);
        menuDrawClickableText("    Shutdown     ", 63, 80, 22);
        if(lmouseDown){
            int ret = stopTimer(clickTimer);
            if(ret > 50 + renderdelay || ret == -2){
                if(mouseX > 63 && mouseX < 80 && mouseY == 14 && terminalmousecursor){
                    showMenu = false;
                    showTestWindow = true;
                    testWindowX = 20;
                    testWindowY = 5;
                }
                if(mouseX > 63 && mouseX < 80 && mouseY == 21 && terminalmousecursor){
                    showMenu = false;
                    uint8_t good = 0x02;
                    while (good & 0x02)
                        good = inportb(0x64);
                    outportb(0x64, 0xFE);
                    asm("hlt");
                }
                if(mouseX > 63 && mouseX < 80 && mouseY == 22 && terminalmousecursor){
                    showMenu = false;
                    acpiPowerOff();
                    
                }
            }
            clickTimer = startTimer();
            
        }
        
        
    }

    if(showTestWindow){
        drawBox(219, 0x88, testWindowX, testWindowX+40, testWindowY, testWindowY+10);
        drawBox(219, 0x77, testWindowX, testWindowX+40, testWindowY, testWindowY+1);
        menuDrawClickableText("X", testWindowX+39, testWindowX+40, testWindowY);
        textAt("   Title", 0x78, testWindowX, testWindowX+40, testWindowY);
        textAt(" xandrei was here", 0x87, testWindowX, testWindowX+40, testWindowY+2);
        textAt(" mori in foc", 0x87, testWindowX, testWindowX+40, testWindowY+4);
        if(lmouseDown){
            int ret = stopTimer(clickTimer);
            if(ret > 50 + renderdelay || ret == -2){
                if(mouseX > testWindowX+38 && mouseX < testWindowX+40 && mouseY == testWindowY && terminalmousecursor)
                    showTestWindow = false;
            }
            clickTimer = startTimer();
            
            /*
            if(mouseX >= testWindowX && mouseX <= testWindowX+37 && mouseY == testWindowY && terminalmousecursor){
                __drag = true;
                __prevX = mouseX;
                __prevY = mouseY;
                __grabX = mouseX - testWindowX;
            }
            if((__prevX != mouseX || __prevY != mouseY) && __drag){
                __drag = false;
                __prevX = mouseX;
                __prevY = mouseY;
                testWindowY = mouseY;
                print(itoa(__grabX, 10));
                print("  ");

                testWindowX = mouseX - __grabX;
                testWindowX = mouseX;
                if(testWindowX+40 > 80){
                    testWindowX = 40;
                }
                
            }
            */
            
            
            
        }
    }

    barRender();

    renderCursor();
    memcpy(terminal_buffer, terminal_buffer_main, 80*25*2);
    newTask(terminalRenderTask, renderdelay);
}


