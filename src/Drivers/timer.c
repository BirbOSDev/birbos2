#include "../all_drivers.h"
unsigned long timer_ticks;
extern terminal_start;

void timer_handler(struct regs *r)
{
    timer_ticks++;
    rand();
    read_rtc();
}


void timer_wait(int ms)
{
    unsigned long eticks;
    eticks = timer_ticks + ms;
    while(timer_ticks <= eticks){
        itoa(20, 10); // dummy function call. needed for some reason
    }
}

unsigned long get_timer_ticks(){
    return timer_ticks;
}


void timer_install(uint32_t frequency)
{
   // Firstly, register our timer callback.
   irq_install_handler(0, timer_handler);

   // The value we send to the PIT is the value to divide it's input clock
   // (1193180 Hz) by, to get our required frequency. Important to note is
   // that the divisor must be small enough to fit into 16-bits.
   uint32_t divisor = 1193180 / frequency;

   // Send the command byte.
   outportb(0x43, 0x36);

   // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
   uint8_t l = (uint8_t)(divisor & 0xFF);
   uint8_t h = (uint8_t)( (divisor>>8) & 0xFF );

   // Send the frequency divisor.
   outportb(0x40, l);
   outportb(0x40, h);
}
