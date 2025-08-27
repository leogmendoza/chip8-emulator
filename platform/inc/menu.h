#pragma once

#include "platform.h"
#include "chip8.h"

typedef enum {
    MENU_STARTUP,
    MENU_SELECT,
    MENU_IDLE
} MenuState;

void platform_menu_init(void);
MenuState platform_menu_update(Chip8* chip8, const uint8_t keypad[16]);
void platform_menu_draw(uint16_t *framebuffer);