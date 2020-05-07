#include "../common.h"
#include "../all_drivers.h"


uint8_t mouse_cycle=0;    
char mouse_byte[3];    
char delta_x=0;         
char delta_y=0;       

int mouse_x=0;         
int mouse_y=0;    
     

uint8_t mouse_read()
{
  mouse_wait(0);
  return inportb(0x60);
}


void mouse_handler(struct regs *r) 
{
  uint8_t __status = mouse_read();
    if (!(__status & 0x20))
        return;
  switch(mouse_cycle)
  {
    case 0:
      mouse_byte[0]=inportb(0x60);
      mouse_cycle++;
      break;
    case 1:
      mouse_byte[1]=inportb(0x60);
      mouse_cycle++;
      break;
    case 2:
      mouse_byte[2]=inportb(0x60);
      delta_x=mouse_byte[1];
      delta_y=mouse_byte[2];
      mouse_cycle=0;
      break;
  }
    mouse_x+=delta_x;
    mouse_y+=delta_y;
    
    if(mouse_x < 1){
        mouse_x = 1;
    } else if(mouse_x > 79){
        mouse_x = 79;
    }
    if(mouse_y < 1){
        mouse_y = 1;
    } else if(mouse_y > 24){
        mouse_y = 24;
    }
    
    
    //update_cursor(mouse_x, mouse_y);
    
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

  irq_install_handler(12, mouse_handler);
}