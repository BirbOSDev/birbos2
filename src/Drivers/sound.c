#include "../all_drivers.h"
#include "../common.h"

  static void play_sound(uint32_t nFrequence) {
 	uint32_t Div;
 	uint8_t tmp;
 
        //Set the PIT to the desired frequency
 	Div = 1193180 / nFrequence;
 	outportb(0x43, 0xb6);
 	outportb(0x42, (uint8_t) (Div) );
 	outportb(0x42, (uint8_t) (Div >> 8));
 
        //And play the sound using the PC speaker
 	tmp = inportb(0x61);
  	if (tmp != (tmp | 3)) {
 		outportb(0x61, tmp | 3);
 	}
 }
 
 //make it shutup
 static void nosound() {
 	uint8_t tmp = inportb(0x61) & 0xFC;
 	outportb(0x61, tmp);
 }
 
 //Make a beep
 void beep(uint32_t freq, uint32_t ms) {
 	 play_sound(freq);
 	 timer_wait(ms);
 	 nosound();
 }

int asyncBeep(uint32_t freq, uint32_t ms) {
 	 play_sound(freq);
 	 return newTask(nosound, ms, 1);
 }

 void bootbeep(){
	beep(500, 100);
    beep(600, 100);
    beep(700, 100);
    beep(800, 100);
    beep(900, 150);
    beep(800, 75);
    beep(900, 75);
 }