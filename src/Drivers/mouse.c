#include "../all_drivers.h"
#include "mouse.h"

uint8_t mouseCycle = 0;
uint8_t mouseByte[3];
uint8_t sensitivity = 4;
int16_t highmouseX = 256 * 4;
int16_t highmouseY = 256 * 4;
int16_t mouseX = 256;
int16_t mouseY = 256;
bool _mouseIRQ = false;
bool terminalmousecursor = false;
bool mouseDown = false;
int mouseTimer = 420;

int oldmouseX = 255;
int oldmouseY = 255;
int oldentry = 0;
unsigned int oldscrolls = 0;

void handleMouseDown(uint8_t key) {
  int ret = stopTimer(mouseTimer);
  if(ret < 300){
    if(ret != -2)
      handleDoubleClick(key);
  }
  mouseDown = true;
  mouseTimer = startTimer();
}
void handleMouseUp(uint8_t key) {
  mouseDown = false;
}
void handleDoubleClick(uint8_t key) {
  return;
}

void handleMouse() {
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

		if ((getBit(mouseByte[0], 0) != 0) || (getBit(mouseByte[0], 1) != 0))
      handleMouseDown(0);
		else
			handleMouseUp(0);
    
    if (highmouseX > (VGA_WIDTH - 1) * sensitivity)
      highmouseX = (VGA_WIDTH - 1) * sensitivity;
    else if (highmouseX < 0)
      highmouseX = 0;

    if (highmouseY > (VGA_HEIGHT - 1) * sensitivity)
      highmouseY = (VGA_HEIGHT - 1) * sensitivity;
    else if (highmouseY < 0)
      highmouseY = 0;

      mouseX = highmouseX / sensitivity;
      mouseY = highmouseY / sensitivity;
  
    


    

    
    
		break;
	  };
	}
}

void setMouseSensitivity(int s){
  sensitivity = s;
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
 
  mouse_wait(1);
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

  irq_install_handler(12, handleMouse);
}