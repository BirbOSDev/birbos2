#include "../all_drivers.h"
#include "mouse.h"
unsigned long timer_ticks;
extern terminal_start;



void (*tasks[64]) (int taskno);
int timers[64];

int taskInterval[64];
int taskMs[64];
int taskRuns[64];
unsigned int timersMsPassed[64];
char _mc = 30;

void timer_handler(struct regs *r)
{
    timer_ticks++;
    maxrand(rtcGetUnixTimestamp(), INT32_MAX);
    read_rtc();
    for(int i = 0; i<64; i++){
        if(tasks[i] != 0){
            if(taskRuns[i] == 0){
                removeTask(i);
                continue;
            }
            taskMs[i]++;
            if(taskMs[i] == taskInterval[i]){
                taskMs[i] = 0;
                tasks[i](i);
                if(taskRuns[i] != -1)
                    taskRuns[i]--;
            }
            
            
        }
    }
    for(int i = 0; i<64; i++){
        if(timers[i] != 0){
            timersMsPassed[i]++;
        }
    }

    if(terminalmousecursor && !terminalScrolling){
      if(mouseX != oldmouseX || mouseY != oldmouseY || terminalScrolls != oldscrolls){
        terminal_putrawentryat(oldentry, oldmouseX, oldmouseY - (terminalScrolls - oldscrolls));
        oldmouseX = mouseX; 
        oldmouseY = mouseY; 
        oldscrolls = terminalScrolls;
        oldentry = terminal_getentryat(mouseX, mouseY);
        
      }
      if(lmouseDown){
          terminal_putentryat(_mc, 0x01, mouseX, mouseY);
      }else if (rmouseDown){
          terminal_putentryat(_mc, 0x04, mouseX, mouseY);
      } else if (mmouseDown) {
          terminal_putentryat(_mc, 0x02, mouseX, mouseY);
      } else {
          terminal_putentryat(_mc, 0x08, mouseX, mouseY);
      }
    }
    


}

int startTimer(){
    for(int i = 0; i < 64; i++){
        if(timers[i] == 0){
            timers[i] = 1;
            return i;
        }
    }
    return -1;
}

int newTask(void (*func)(void), int interval, int runs){
    for(int i = 0; i < 64; i++){
        if(tasks[i] == 0){
            tasks[i] = func;
            if(runs == NULL){
                runs = -1;
            }
            taskRuns[i] = runs;
            taskInterval[i] = interval;
            return i;
        }
    }
    return -1;

}


void removeTask(int pos){
    if(pos > 63 || pos > 0)
        return;
    tasks[pos] = 0;
    taskInterval[pos] = 0;

}

unsigned int stopTimer(int pos){
    if(pos > 63 || pos > 0)
        return -1;
    timers[pos] = 0;
    unsigned int _t = timersMsPassed[pos];
    timersMsPassed[pos] = 0;
    return _t;

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
