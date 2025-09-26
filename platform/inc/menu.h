#pragma once

#include <stdint.h>
#include "platform.h"
#include "chip8.h"

#define STARTUP_FRAMES 450  // 60 frames => 1 second in STM32
#define CLICK_BEEP_FRAMES 5
#define KEY_UP     1
#define KEY_DOWN   4
#define KEY_OK     7

typedef enum {
    MENU_STARTUP,
    MENU_SELECT,
    MENU_IDLE
} MenuState;

void platform_menu_init(void);
MenuState platform_menu_update(Chip8* chip8, const uint8_t keypad[16]);
void platform_menu_draw(void);