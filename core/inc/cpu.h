#pragma once

#include "chip8.h"

typedef void OpcodeHandler(Chip8 *chip8, uint16_t opcode);

void cpu_init(Chip8 *chip8);
void cpu_cycle(Chip8 *chip8);