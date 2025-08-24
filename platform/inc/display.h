#pragma once

#include "platform.h"

#define SCALE 10

int display_init(void);
void display_destroy(void);
void display_draw(const Chip8* chip8);