#pragma once
#include <stdint.h>
#include "../common.h"


/* bool _mouseIRQ; */
/* bool lmouseDown;
bool rmouseDown;
bool mmouseDown; */
/* bool terminalmousecursor; */
/* uint8_t mouseCycle; */
/* uint8_t mouseByte[4]; 
int16_t mouseX;
int16_t mouseY;
int16_t mouseX;
int16_t mouseY;int16_t mouseX;
int16_t mouseY; int oldmouseX ;
int oldmouseY;
int oldentry; */
/* unsigned int oldscrolls;
uint8_t sensitivity;
unsigned char lcolor;
unsigned char rcolor;
unsigned char mcolor;
unsigned char dcolor; */

uint8_t mouse_read();
void handleMouseDown(uint8_t key);
void handleMouseUp(uint8_t key);
