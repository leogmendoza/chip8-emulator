#include <stdio.h>

#include "cpu.h"
#include "instructions.h"

// Primary function pointer table for leading nibble (0x0 - 0xF)
static OpcodeHandler *table[0x10];

// Secondary function pointer table for repeated leading nibbles (0x0, 0x8, 0xE, 0xF)
static OpcodeHandler *table0[0x10];  // 0x0???
static OpcodeHandler *table8[0x10];  // 0x8xy?
static OpcodeHandler *tableE[0x10];  // 0xEx?? (only two instructions with unique third nibbles, so only last needs to be masked)
static OpcodeHandler *tableF[0x100];  // 0xFx?? (some third nibbles are repeated)

static void table0_dispatch(Chip8* chip8, uint16_t opcode) {
    table0[( opcode & 0x000F )](chip8, opcode);
}

void cpu_init(Chip8 *chip8) {
    // Zero out tables with NOPs
    for (size_t i = 0; i < 0x10 ; i++) {
        table[i] = op_null;
        table0[i] = op_null;
        table8[i] = op_null;
        tableE[i] = op_null;
    }

    for (size_t i = 0; i < 0x100 ; i++) {
        tableF[i] = op_null;
    }

    // TODO: AFTER defining opcode handlers
    // Assign defined opcode handler functions to tables
    table[0x0] = table0_dispatch;
    table[0x1] = op_1nnn;
    table[0x2] = op_2nnn;
    table[0x6] = op_6xkk;
    table[0x7] = op_7xkk;
    table[0xA] = op_Annn;
    table[0xD] = op_Dxyn;

    table0[0x0] = op_00E0;
    table0[0xE] = op_00EE;

    return;
}

void cpu_cycle(Chip8 *chip8) {
    // FETCH next instruction from program counter as a two-byte opcode
    uint16_t opcode = ( chip8->memory[chip8->PC] << 8 | chip8->memory[chip8->PC + 1] );

    // Increment program counter before executing
    chip8->PC += 2;

    // DECODE and EXECUTE instruction by first extracting leading nibble
    (table[ (opcode & 0xF000) >> 12 ])(chip8, opcode);

    // Debug instruction executed
    // printf(
    //     "Executed: %04X | PC: %04X | I: %04X | V[0]=%02X V[1]=%02X ...\n",
    //     opcode, chip8->PC, chip8->I, chip8->V[0], chip8->V[1]
    // );
    
    // Decrement delay and sound timers if set
	if (chip8->delay_timer > 0)
	{
		--chip8->delay_timer;
	}

	// Decrement the sound timer if it's been set
	if (chip8->sound_timer > 0)
	{
		--chip8->sound_timer;
	}
}
