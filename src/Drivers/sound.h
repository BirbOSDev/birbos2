#pragma once

#include "../all_drivers.h"
#include "../common.h"

void beep(uint32_t freq, uint32_t ms);
static void nosound();
static void play_sound(uint32_t nFrequence);
void bootbeep();