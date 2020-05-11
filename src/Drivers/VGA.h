#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Hardware text mode color constants. */

#define VGA_COLOR_BLACK 0
#define VGA_COLOR_BLUE 1
#define VGA_COLOR_GREEN 2
#define VGA_COLOR_CYAN 3
#define VGA_COLOR_RED 4
#define VGA_COLOR_MAGENTA 5
#define VGA_COLOR_BROWN 6
#define VGA_COLOR_LIGHT_GREY 7
#define VGA_COLOR_DARK_GREY 8
#define VGA_COLOR_LIGHT_BLUE 9
#define VGA_COLOR_LIGHT_GREEN 10
#define VGA_COLOR_LIGHT_CYAN 11
#define VGA_COLOR_LIGHT_RED 12
#define VGA_COLOR_LIGHT_MAGENTA 13
#define VGA_COLOR_LIGHT_BROWN 14
#define VGA_COLOR_WHITE 15

int VGA_WIDTH;
int VGA_HEIGHT;

uint8_t* screen;
unsigned int terminal_row;
unsigned int terminal_column;
uint16_t* terminal_buffer;
bool terminalScrolling;
unsigned int terminalScrolls;
bool barEnabled;
uint16_t terminal_buffer_layer[90*60*2];
uint16_t terminal_buffer_main[90*60*2];

void barTask();
void update_cursor(uint16_t x, uint16_t y);
uint8_t vga_entry_color(uint8_t fg, uint8_t bg);
uint16_t vga_entry(unsigned char uc, uint8_t color);
void terminal_initialize(void);
void terminal_setcolor(uint8_t color);
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
void terminal_putchar(char c);
void terminal_putcharbehind(char c);
void terminal_write(const char* data, size_t size);
void print(const char* data);
void print_c(const char* data, uint8_t color);
