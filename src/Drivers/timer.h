#pragma once

extern unsigned long timer_ticks;

extern void timer_wait(int ms);
extern void timer_install(uint32_t frequency);
unsigned long get_timer_ticks();