#include "../all_drivers.h"
#include "mouse.h"
#define BIT_SET(a,b) ((a) |= (1ULL<<(b)))


uint8_t* screen = (uint8_t*)0xA0000;
bool terminalScrolling = false;
unsigned int terminalScrolls = 0;

void putpixel(int x,int y, int color) {
    unsigned where = x*3 + y*2400;
    screen[where] = color & 255;              // BLUE
    screen[where + 1] = (color >> 8) & 255;   // GREEN
    screen[where + 2] = (color >> 16) & 255;  // RED
}

bool barEnabled = true;

void fillrect(int px, int py, int sx, int sy, int color) {
 
    for (int i = 0; i < sy; i++) {
        for (int j = 0; j < sx; j++) {
            putpixel(j+px, i+py, color);
        }
    }
}




inline uint8_t vga_entry_color(uint8_t fg, uint8_t bg)
{
	return fg | bg << 4;
}

inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
	return (uint16_t)uc | (uint16_t)color << 8;
}


unsigned int terminal_row;
unsigned int terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void update_cursor(uint16_t x, uint16_t y)
{
	terminal_putentryat('\0', 0x07, x, y);
	uint16_t pos = y * VGA_WIDTH + x;
	outportb(0x3D4, 0x0F);
	outportb(0x3D5, (uint8_t) (pos & 0xFF));
	outportb(0x3D4, 0x0E);
	outportb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void terminal_initialize(void)
{
	terminal_row = 0;
	terminal_column = 0;
	update_cursor(terminal_column, terminal_row);
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*)0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putrawentryat(uint16_t entry, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = entry;
}

uint16_t terminal_getentryat(size_t x, size_t y){
	const size_t index = y * VGA_WIDTH + x;
	uint16_t data = terminal_buffer[index];
	return data;
}

void terminal_scroll(){
	bool wasCursorEnabled = terminalmousecursor;
	if(terminalmousecursor)mouseToggleTerminalCursor();
	terminalScrolling = true;
	barEnabled = false;
	char* data = "                                                                                ";
    for (size_t i = 0; i < strlen(data); i++){
        terminal_putentryat(data[i], 0x00, i, 24);
    }
    for(int i = 0; i < VGA_HEIGHT; i++){
        for (int m = 0; m < VGA_WIDTH; m++){
            terminal_buffer[i * VGA_WIDTH + m] = terminal_buffer[(i + 1) * VGA_WIDTH + m];
        }
    }
	barEnabled = true;
	terminalScrolling = false;
	terminalScrolls++;
	if(wasCursorEnabled)mouseToggleTerminalCursor();
}


void barTask(){
	char data[80] = "";

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
	strcat(data, "birbOS");

	
	if(!barEnabled)
		return;
    for (size_t i = 0; i < strlen(data); i++){
        terminal_putentryat(data[i], 0x87, i, 24);
    }
	
}




void terminal_putchar(char c)
{
	if (c == '\n') {
        terminal_row++;
        terminal_column = -1;
    } else {
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    }
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT){
			terminal_scroll();
			terminal_row = VGA_HEIGHT-1;
			
		}
	}
	if(terminal_row == VGA_HEIGHT){
		terminal_scroll();
        terminal_row = VGA_HEIGHT-1;
        
	}
	update_cursor(terminal_column, terminal_row);
	
}

void terminal_putcharat(char c, int tx, int ty)
{
	
    terminal_putentryat(c, terminal_color, tx, ty);
    
}

void terminal_putcharnoex(char c)
{

	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT){
			terminal_row = 0;
			terminal_scroll();
		}
	}
	if(terminal_row == VGA_HEIGHT){
        terminal_row = 0;
        terminal_scroll();
	}
	update_cursor(terminal_column, terminal_row);
}
void terminal_putcharbehind(char c)
{
    if (c == '\n') {
        terminal_row++;
        terminal_column = -1;
    } else {
        terminal_putentryat(c, terminal_color, terminal_column - 1, terminal_row);
    }
	if (--terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (--terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}
	update_cursor(terminal_column, terminal_row);
}

void print(const char* data)
{
	for (size_t i = 0; i < strlen(data); i++)
        terminal_putchar(data[i]);
}

void print_at(const char* data, int tx, int ty)
{
	int oldr = terminal_row;
	int oldc = terminal_column;
	terminal_row = ty;
	terminal_column = tx;
	print(data);
	terminal_row = oldr;
	terminal_column = oldc;
	update_cursor(terminal_column, terminal_row);
}

void print_c(const char* data, uint8_t color)
{
	terminal_color = color;
	print(data);
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	update_cursor(terminal_column, terminal_row);
}

void drawSprite(int px, int py, int sy, int sx, int sprite[][sx], int scale){
    int _ = 0;
    for(int y = 0; y<sy*scale; y++){
        for(int x = 0; x<sx*scale; x++){
            if (sprite[y/scale][x/scale] == -1) {_++; continue;}
			if (sprite[y/scale][x/scale] == -2) continue;
            putpixel(x + px - _, y + py - _, sprite[y/scale][x/scale]);
        }
        _ = 0;
    }
}

void drawChar(int px, int py, int sprite[8][8], int color, int scale){
    int _ = 0;
    for(int y = 0; y<8*scale; y++){
        for(int x = 0; x<8*scale; x++){
            if (sprite[y/scale][x/scale] == 0) {_++; continue;}
			if (sprite[y/scale][x/scale] == 1) continue;
            putpixel(x + px - _, y + py - _, color);
        }
        _ = 0;
    }
}

