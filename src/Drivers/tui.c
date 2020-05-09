#include "../all_drivers.h"
#include "../common.h"

char _mc = 30;

bool __set__ = false;
int clickTimer = 420;
bool showMenu = false;
bool showWeekday = false;

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
        if(c >= wrapx){
            c=0;
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
        char nibble = (char) ((num >> 4) & 0x0F);
        //bgcolor = (bgcolor & 0xF0) | (bgcolor & 0xF);
      

        lcolor = setUpperNibble(lcolor, nibble);
        rcolor = setUpperNibble(rcolor, nibble);
        mcolor = setUpperNibble(mcolor, nibble);
        dcolor = setUpperNibble(dcolor, nibble);

        if(lmouseDown){
            _terminal_putentryat(_mc, lcolor, mouseX, mouseY);
        }else if (rmouseDown){
            _terminal_putentryat(_mc, rcolor, mouseX, mouseY);
        } else if (mmouseDown) {
            _terminal_putentryat(_mc, mcolor, mouseX, mouseY);
        } else {
            _terminal_putentryat(_mc, dcolor, mouseX, mouseY);
        }
    }
}

void menuDrawClickableText(char* text, int pos){
    if(mouseX > 63 && mouseX < 80 && mouseY == pos){
        textAt(text, 0x87, 63, 80, pos);
    } else {
        textAt(text, 0x78, 63, 80, pos);
    }
}


void terminalRenderTask(int taskno){
    memcpy(terminal_buffer, terminal_buffer_layer, 2048*2);
    
    barRender();

    if(lmouseDown){
        if(mouseX > 73 && mouseX < 80 && mouseY == 24 && terminalmousecursor){
            int ret = stopTimer(clickTimer);
            if(ret > 50 || ret == -2){
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
        menuDrawClickableText("--[Placeholder]--", 14);
        menuDrawClickableText("--[Placeholder]--", 15);
        menuDrawClickableText("--[Placeholder]--", 16);
        menuDrawClickableText("--[Placeholder]--", 17);
        menuDrawClickableText("--[Placeholder]--", 18);
        menuDrawClickableText("--[Placeholder]--", 19);
        menuDrawClickableText("--[Placeholder]--", 20);
        menuDrawClickableText("--[Placeholder]--", 21);
        menuDrawClickableText("--[Placeholder]--", 22);

        
        
    }

    renderCursor();
}