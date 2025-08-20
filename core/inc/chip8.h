#pragma once

#include <stdint.h>

#include "memory.h"
#include "rom.h"

#define REGISTER_COUNT 16
#define MEMORY_SIZE 4096
#define STACK_DEPTH 16
#define DISPLAY_HEIGHT 32
#define DISPLAY_WIDTH 64
#define DISPLAY_SIZE (DISPLAY_HEIGHT * DISPLAY_WIDTH)
#define INPUT_KEY_COUNT 16

typedef struct {
    // CPU
    uint8_t V[REGISTER_COUNT];      // General-purpose Registers (V0-VF)
    uint16_t I;                     // Index Register
    uint16_t PC;                    // Program Counter

    //  Memory
    uint8_t memory[MEMORY_SIZE];

    // Stack
    uint16_t stack[STACK_DEPTH];
    uint8_t SP;                     // Stack Pointer

    // Timers
    uint8_t delay_timer;
    uint8_t sound_timer;

    // Display
    uint8_t display[DISPLAY_SIZE];

    // Input
    uint8_t keypad[INPUT_KEY_COUNT];
} Chip8;

void chip8_init(Chip8* chip8);