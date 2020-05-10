#include "common.h"
#include "all_drivers.h"
#include <stdarg.h>
#include <stddef.h>
static unsigned long int next = 69420;
static unsigned long int random_seed = 42069;
int rand( void ){
    next = next * 1103515245 + 12345;
    return (unsigned int)(next / INT16_MAX * 2) % INT16_MAX;
}

int maxrand(int seed,int max)
{
	random_seed = random_seed+seed * 1103515245 +12345;
	return (unsigned int)(random_seed / 65536) % (max+1); 
}

unsigned int randomInt(unsigned int max){
    return maxrand(rtcGetUnixTimestamp(), max);
}

void srand( unsigned int seed ){
    next = seed;
}


char setLowerNibble(char orig, char nibble) {
    char res = orig;
    res &= 0xF0; // Clear out the lower nibble
    res |= (nibble & 0x0F); // OR in the desired mask
    return res;
}

char setUpperNibble(char orig, char nibble) {
    char res = orig;
    res &= 0x0F; // Clear out the upper nibble
    res |= ((nibble << 4) & 0xF0); // OR in the desired mask
    return res;
}



int getBit(int8_t byteFlag, int whichBit)
{
    if (whichBit > 0 && whichBit <= 8)
        return (byteFlag & (1<<(whichBit-1)));
    else
        return 0;
}

char* cpuidstring(int in, char* buf){
    int e = 0;
    asm("mov eax, %0\n"
                "cpuid"::"r" (in) :);
    register uint32_t ebx1 asm("ebx");
    register uint32_t edx1 asm("edx");
    register uint32_t ecx1 asm("ecx");
    uint32_t ebx = ebx1;
    uint32_t edx = edx1;
    uint32_t ecx = ecx1;
    uint32_t byt;
    for(int j = 0; j<4; j++){
        byt = ((unsigned char *)(&ebx))[j];
        buf[e++] = byt;
    }
    for(int j = 0; j<4; j++){
        byt = ((unsigned char *)(&edx))[j];
        buf[e++] = byt;
    }
    for(int j = 0; j<4; j++){
        byt = ((unsigned char *)(&ecx))[j];
        buf[e++] = byt;
    }
    buf[12] = '\0';
    return buf;

}

void outportb(uint16_t port, uint8_t value)
{
	asm("mov dx, %0"::"r" (port) : );
	asm("mov al, %0"::"r" (value) : );
	asm("outb dx, al");
}
uint8_t inportb(uint16_t port)
{
	asm("mov dx, %0"::"r" (port) : );
	asm("inb al, dx");
	register uint8_t r asm("al");
    return r;
}
void outportw(uint16_t port, uint16_t value)
{
	asm("mov dx, %0"::"r" (port) : );
	asm("mov ax, %0"::"r" (value) : );
	asm("outw dx, ax");
}
uint16_t inportw(uint16_t port)
{
	asm("mov dx, %0"::"r" (port) : );
	asm("inw ax, dx");
	register uint16_t r asm("ax");
    return r;
}
void outportdw(uint16_t port, uint32_t value)
{
	asm("mov dx, %0"::"r" (port) : );
	asm("mov eax, %0"::"r" (value) : );
	asm("out dx, eax");
}
uint32_t inportdw(uint16_t port)
{
	asm("mov dx, %0"::"r" (port) : );
	asm("in eax, dx");
	register uint32_t r asm("eax");
    return r;
}

inline void manualCopy( uint8_t *pDest, uint8_t *pSrc, uint32_t len )
{
    uint32_t i;
 
    // Manually copy the data
    for ( i = 0; i < len; i++ )
    {
        // Copy data from source to destination
        *pDest++ = *pSrc++;
    }
}

 
void memcpy( void *pDest, void *pSrc, uint32_t len )
{
    uint32_t srcCnt;
    uint32_t destCnt;
    uint32_t newLen;
    uint32_t endLen;
    uint32_t longLen;
    uint32_t *pLongSrc;
    uint32_t *pLongDest;
    uint32_t longWord1;
    uint32_t longWord2;
    uint32_t methodSelect;
     
    // Determine the number of bytes in the first word of src and dest
    srcCnt = 4 - ( (uint32_t) pSrc & 0x03 );
    destCnt = 4 - ( (uint32_t) pDest & 0x03 );
     
    // Copy the initial bytes to the destination
    manualCopy( pDest, pSrc, destCnt );
     
    // Determine the number of bytes remaining
    newLen = len - destCnt;
     
    // Determine how many full long words to copy to the destination
    longLen = newLen / 4;
     
    // Determine number of lingering bytes to copy at the end
    endLen = newLen & 0x03;
     
    // Pick the initial long destination word to copy to
    pLongDest = (uint32_t*) ( pDest + destCnt );
     
    // Pick the initial source word to start our algorithm at
    if ( srcCnt <= destCnt )
    {
        // Advance to pSrc at the start of the next full word
        pLongSrc = (uint32_t*) ( pSrc + srcCnt );
    }
    else // There are still source bytes remaining in the first word
    {
        // Set pSrc to the start of the first full word
        pLongSrc = (uint32_t*) ( pSrc + srcCnt - 4 );
    }
     
    // There are 4 different longWord copy methods
    methodSelect = ( srcCnt - destCnt ) & 0x03;
     
    // Just copy one-to-one
    if ( methodSelect == 0 )
    {
        // Just copy the specified number of long words
        while ( longLen-- > 0 )
        {
            *pLongDest++ = *pLongSrc++;
        }
    }
    else if ( methodSelect == 1 )
    {
        // Get the first long word
        longWord1 = *pLongSrc++;
         
        // Copy words created by combining 2 adjacent long words
        while ( longLen-- > 0 )
        {
            // Get the next 32-bit word
            longWord2 = *pLongSrc++;
             
            // Write to the destination
            *pLongDest++ = ( longWord1 >> 24 ) | ( longWord2 << 8 );
             
            // Re-use the word just retrieved
            longWord1 = longWord2;
        }
    }
    else if ( methodSelect == 2 )
    {
        // Get the first long word
        longWord1 = *pLongSrc++;
         
        // Copy words created by combining 2 adjacent long words
        while ( longLen-- > 0 )
        {
            // Get the next 32-bit word
            longWord2 = *pLongSrc++;
             
            // Write to the destination
            *pLongDest++ = ( longWord1 >> 16 ) | ( longWord2 << 16 );
             
            // Re-use the word just retrieved
            longWord1 = longWord2;
        }
    }
    else // ( methodSelect == 3 )
    {
        // Get the first long word
        longWord1 = *pLongSrc++;
         
        // Copy words created by combining 2 adjacent long words
        while ( longLen-- > 0 )
        {
            // Get the next 32-bit word
            longWord2 = *pLongSrc++;
 
            // Write to the destination
            *pLongDest++ = ( longWord1 >> 8 ) | ( longWord2 << 24 );
 
            // Re-use the word just retrieved
            longWord1 = longWord2;
        }
    }
     
    // Copy any remaining bytes
    if ( endLen != 0 )
    {
        // The trailing bytes will be copied next
        pDest = (uint8_t*) pLongDest;
         
        // Determine where the trailing source bytes are located
        pSrc += len - endLen;
         
        // Copy the remaining bytes
        manualCopy( pDest, pSrc, endLen );
    }
}
 



void *memset(void *dest, char val, size_t count)
{
    char *temp = (char *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count)
{
    unsigned short *temp = (unsigned short *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

int memcmp (unsigned char* str1, unsigned char* str2, size_t count)
{
  register unsigned char *s1 = (unsigned char*)str1;
  register unsigned char *s2 = (unsigned char*)str2;

  while (count-- > 0)
    {
      if (*s1++ != *s2++)
	  return s1[-1] < s2[-1] ? -1 : 1;
    }
  return 0;
}



size_t strlen(const char* str)
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

int16_t charInStr(char val, char *arr){
	int16_t size = sizeof(*arr) / sizeof(char);
    for (int16_t i=0; i < size; i++) {
        if (arr[i] == val)
            return i;
    }
    return -1;
}

void swap(char *x, char *y){
    char temp = *x;
    *x = *y;
    *y = temp;
}

void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        swap(str+start, str+end);
        start++;
        end--;
    }
}

// Implementation of itoa()
char* itoa(int num, int base)
{

    char* str = NULL;
    int i = 0;
    bool isNegative = false;
    if (num == 0)
    {
        str = "0";
        return str;
    }



    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }

    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }

    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str, i);

    return str;
}

int atoi(const char* str, int b) 
{ 
    int sign = 1, base = 0, i = 0; 
    if(strequ(str, "0")){
        return 0;
    }
    // if whitespaces then ignore. 
    while (str[i] == ' ') { 
        i++; 
    } 
    // sign of number 
    if (str[i] == '-' || str[i] == '+') { 
        sign = 1 - 2 * (str[i++] == '-'); 
    } 
    // checking for valid input 
    while (str[i] >= '0' && str[i] <= '9') { 
        // handling overflow test case 
        if (base > 2147483646 / 10 || (base == 2147483646 / 10 && str[i] - '0' > 7)) { 
            if (sign == 1) 
                return 2147483646; 
            else
                return 0; 
        } 
        base = b * base + (str[i++] - '0'); 
    } 
    return base * sign; 
} 


bool strequ(char* s1, char* s2){
    if(strlen(s1) != strlen(s2))
        return false;
    for(size_t i = 0; i < strlen(s1); i++){
        if(s1[i] != s2[i])
            return false;
    }
    return true;
}
bool strcontains(char* s, char* key){
    if(strlen(key) > strlen(s))
        return false;

    for (int i = 0; i < strlen(s); i++){
        int j = 0;
        // If the first characters match
        if(s[i] == key[j]){
            int k = i;
            while (s[i] == key[j] && j < strlen(key)){
                j++;
                i++;
            }
            if (j == strlen(key))
                return true;
            else // Re-initialize i to its original value
                i = k;
        }
    }
    return false;
}
void strcpy(char *src, char *dest) {
    int size = strlen(src);
    if (size > 0) {
        int i;
        for (i = 0; i < size - 1 && src[i]; i++) {
             dest[i] = src[i];
        }
        dest[i+1] = '\0';
    }
}


char ktoc(uint8_t key){
    char c = 0;
    uint8_t dict[2][94] = {
        {57,40,51,12,52,53,11,2 ,3, 4, 5, 6, 7, 8, 9, 10,39,13,26,43,27,41,30,48,46,32, 18, 33, 34, 35, 23, 36, 37, 38, 50, 49, 24, 25, 16, 19, 31, 20, 22, 47, 17, 45, 21, 44},
        {32,39,44,45,46,47,48,49,50,51,52,53,54,55,56,57,59,61,91,92,93,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122}
    };
    for(int i = 0; i < 94; i++){
        if(dict[0][i]==key){
            c = (char)dict[1][i];
        }
    }
    return c;
}

char ktocSHIFT(uint8_t key){
    char c = 0;
    uint8_t dict[2][94] = {
        {41, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,26, 27, 43, 39,40,51,52,53,30,48,46,32,18,33,34,35,23,36,37,38,50,49,24,25,16,19,31,20,22,47,17,45,21,44,57},
        {126,33,64,35,36,37,94,38,42,40,41,95,43,123,125,124,58,34,60,62,63,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,32}
    };
    for(int i = 0; i < 94; i++){
        if(dict[0][i]==key){
            c = (char)dict[1][i];
        }
    }
    return c;
}

char* strcat(char* dest, const char *src)
{
    size_t i,j;
    char* d = dest;
    for (i = 0; d[i] != '\0'; i++)
        ;
    for (j = 0; src[j] != '\0'; j++)
        d[i+j] = src[j];
    d[i+j] = '\0';
    return d;
}
void qemudebug(char* s){
    for (size_t i = 0; i < strlen(s); i++){
        outportb(0x3F8, s[i]);
        itoa(2,10);
    }
}

void sleep(int ms){
    timer_wait(ms);
}

