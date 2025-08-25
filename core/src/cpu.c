#include <stdio.h>

#include "cpu.h"
#include "instructions.h"

// "Primary" function pointer table for leading nibble (0x0 - 0xF)
static OpcodeHandler *table[0x10];

// "Secondary" function pointer table for repeated leading nibbles (0x0, 0x8, 0xE, 0xF)
static OpcodeHandler *table0[0x10];  // 0x0???
static OpcodeHandler *table8[0x10];  // 0x8xy?
static OpcodeHandler *tableE[0x10];  // 0xEx?? (only two instructions with unique third nibbles, so only last needs to be masked)
static OpcodeHandler *tableF[0x100];  // 0xFx?? (some third nibbles are repeated)

static void table0_dispatch(Chip8* chip8, uint16_t opcode) {
    table0[( opcode & 0x000F )](chip8, opcode);
}

static void table8_dispatch(Chip8* chip8, uint16_t opcode) {
    table8[( opcode & 0x000F )](chip8, opcode);
}

static void tableE_dispatch(Chip8* chip8, uint16_t opcode) {
    tableE[( opcode & 0x000F )](chip8, opcode);
}

static void tableF_dispatch(Chip8* chip8, uint16_t opcode) {
    tableF[( opcode & 0x00FF )](chip8, opcode);
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

    // Assign defined opcode handler functions to primary tables
    table[0x0] = table0_dispatch;
    table[0x1] = op_1nnn;
    table[0x2] = op_2nnn;
    table[0x3] = op_3xkk;
    table[0x4] = op_4xkk;
    table[0x5] = op_5xy0;
    table[0x6] = op_6xkk;
    table[0x7] = op_7xkk;
    table[0x8] = table8_dispatch;
    table[0x9] = op_9xy0;
    table[0xA] = op_Annn;
    table[0xB] = op_Bnnn;
    table[0xC] = op_Cxkk;
    table[0xD] = op_Dxyn;
    table[0xE] = tableE_dispatch;
    table[0xF] = tableF_dispatch;

    // Assign defined opcode handler functions to secondary tables
    table0[0x0] = op_00E0;
    table0[0xE] = op_00EE;

    table8[0x0] = op_8xy0;
    table8[0x1] = op_8xy1;
    table8[0x2] = op_8xy2;
    table8[0x3] = op_8xy3;
    table8[0x4] = op_8xy4;
    table8[0x5] = op_8xy5;
    table8[0x6] = op_8xy6;
    table8[0x7] = op_8xy7;
    table8[0xE] = op_8xyE;

    tableE[0x1] = op_ExA1;
    tableE[0xE] = op_Ex9E;

    tableF[0x07] = op_Fx07;
    tableF[0x0A] = op_Fx0A;
    tableF[0x15] = op_Fx15;
    tableF[0x18] = op_Fx18;
    tableF[0x1E] = op_Fx1E;
    tableF[0x29] = op_Fx29;
    tableF[0x33] = op_Fx33;
    tableF[0x55] = op_Fx55;
    tableF[0x65] = op_Fx65;

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
