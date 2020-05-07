#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct regs
{
    unsigned int gs, fs, es, ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags, useresp, ss;
};
void beep();
int getBit(int8_t byteFlag, int whichBit);
void outportb(uint16_t port, uint8_t value);
uint8_t inportb(uint16_t port);
uint16_t inportw(uint16_t port);
void outportw(uint16_t port, uint16_t value);
void outportdw(uint16_t port, uint32_t value);
uint32_t inportdw(uint16_t port);
void *memcpy(void *dest, const void *src, size_t count);
void *memset(void *dest, char val, size_t count);
unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);
int16_t charInStr(char val, char *arr);
void swap(char *x, char *y);
void reverse(char str[], int length);
int atoi(const char* str, int b);
char* itoa(int num, int base);
char ktoc(uint8_t key);
char ktocSHIFT(uint8_t key);
char* strcat(char* dest, const char *src);
char** strsplit(char* str, char delim);
bool strequ(char* s1, char* s2);
bool strcontains(char* s, char* key);
void strcpy(char *dest, char *src);
void sleep(int seconds);
