#include "../all_drivers.h"
#include "mouse.h"

uint8_t mouseCycle = 0;
uint8_t mouseByte[4];
uint8_t sensitivity = 6;
int16_t highmouseX = 79 * 6;
int16_t highmouseY = 0;
int16_t mouseX = 79;
int16_t mouseY = 0;
bool _mouseIRQ = false;
bool terminalmousecursor = false;
bool lmouseDown = false;
bool rmouseDown = false;
bool mmouseDown = false;
bool handledDown = false;
int mouseTimer = 420;

unsigned char lcolor = 0x01;
unsigned char rcolor = 0x04;
unsigned char mcolor = 0x02;
unsigned char dcolor = 0x07;


int oldmouseX = 0;
int oldmouseY = 0;
int oldentry = 0;
unsigned int oldscrolls = 0;

void handleMouseDown(uint8_t key) {
  if(key == 0){
    int ret = stopTimer(mouseTimer);
    if(ret < 300){
      if(ret != -3 && !handledDown){
        handleDoubleClick(key);
        handledDown = true;
      }
    }
    lmouseDown = true;
    mouseTimer = startTimer();
  } else if(key == 1) {
    rmouseDown = true;
  } else if(key == 2){
    mmouseDown = true;
  }
}
void handleMouseUp(uint8_t key) {
  if(key == 0){
    lmouseDown = false;
    handledDown = false;
  } else if(key == 1) {
    rmouseDown = false;
  } else if(key == 2){
    mmouseDown = false;
  }
}
void handleDoubleClick(uint8_t key) {
  return;
}

void handleMouse() {
  rand();
  if(!getBit(inportb(0x64), 1)){
    return;
  }
  
  _mouseIRQ = true;
	switch(mouseCycle) {
	case 0: {
		mouseByte[0] = mouse_read();
    
		if((mouseByte[0] & 0x08) != 0) {
			mouseCycle++; // Only accept this as the first byte if the "must be 1" bit is set
		}
		//mouseCycle++;

		break;
	};
	case 1: {
		mouseByte[1] = mouse_read();
		mouseCycle++;
		break;
	};
	case 2: {
		mouseByte[2] = mouse_read();
		int8_t mouseXd = mouseByte[1];
		int8_t mouseYd = mouseByte[2];
		mouseCycle = 0;
		highmouseX += mouseXd;
		highmouseY -= mouseYd;

		if ((getBit(mouseByte[0], 1) != 0))
      handleMouseDown(0);
		else
			handleMouseUp(0);

      if ((getBit(mouseByte[0], 2) != 0))
      handleMouseDown(1);
		else
			handleMouseUp(1);

      if ((getBit(mouseByte[0], 3) != 0))
      handleMouseDown(2);
		else
			handleMouseUp(2);
    
    if (highmouseX > (VGA_WIDTH - 1) * sensitivity)
      highmouseX = (VGA_WIDTH - 1) * sensitivity;
    else if (highmouseX < 0)
      highmouseX = 0;

    if (highmouseY > (VGA_HEIGHT/* - 1*/) * (sensitivity+1))
      highmouseY = (VGA_HEIGHT/* - 1*/) * (sensitivity+1);
    else if (highmouseY < 0)
      highmouseY = 0;

      mouseX = highmouseX / sensitivity;
      mouseY = highmouseY / (sensitivity+1);
  
      srand(highmouseX);
      rand();
      srand(highmouseY);
      rand();

    

    
    
		break;
	  };
  case 3: {
    mouseCycle = 0;
    mouseByte[4] = mouse_read();
    break;
   }
	}
}

void setMouseSensitivity(int s){
  
  if(s < 1){
    sensitivity = 1;
  } else
  if(s > 255){
    sensitivity = 255;
  } else {
    sensitivity = s;
  }

  
}

void mouseToggleTerminalCursor(){
  if(terminalmousecursor){
    terminal_putrawentryat(oldentry, oldmouseX, oldmouseY);
    terminalmousecursor = false;
  } else {
    terminalmousecursor = true;
  }
}

void waitForMouseIRQ(){
  while(!_mouseIRQ){itoa(1,2);}
  _mouseIRQ = false;
}

bool hasMouseIRQFired(){
  if(_mouseIRQ){
    _mouseIRQ = false;
    return true;
  }
  return false;
}

uint8_t mouse_read()
{
  mouse_wait(0);
  return inportb(0x60);
}

inline void mouse_wait(uint8_t a_type)
{
  uint32_t _time_out=100000;
  if(a_type==0)
  {
    while(_time_out--)
    {
      if((inportb(0x64) & 1))
      {
        return;
      }
    }
    return;
  }
  else
  {
    while(_time_out--)
    {
      if((inportb(0x64) & 2)==0)
      {
        return;
      }
    }
    return;
  }
}

inline void mouse_write(uint8_t a_write){
  mouse_wait(1);
  outportb(0x64, 0xD4);

  mouse_wait(1);
  outportb(0x60, a_write);
}

void mouse_install()
{
  uint8_t _status;
  mouse_wait(1);
  outportb(0x64, 0xA8);
 
  //mouse_wait(1);
  outportb(0x64, 0x20);
  mouse_wait(0);
  _status=(inportb(0x60) | 2);
  mouse_wait(1);
  outportb(0x64, 0x60);
  mouse_wait(1);
  outportb(0x60, _status);
 

  mouse_write(0xF6);
  mouse_read();
 
  mouse_write(0xF4);
  mouse_read(); 

  mouse_write(0xF3);
  outportb(0x60, 200);
  mouse_read(); 

  mouse_write(0xE7);
  mouse_read();

  irq_install_handler(12, handleMouse);
}