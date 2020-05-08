#pragma once
#include <stdint.h>
#include "../common.h"


bool _mouseIRQ;
bool mouseDown;
bool terminalmousecursor;
uint8_t mouseCycle;
uint8_t mouseByte[3];
int16_t mouseX;
int16_t mouseY;
int oldmouseX ;
int oldmouseY;
int oldentry;
unsigned int oldscrolls;

uint8_t mouse_read();
void handleMouseDown(uint8_t key);
void handleMouseUp(uint8_t key);