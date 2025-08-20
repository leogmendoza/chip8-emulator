#include "cpu.h"

// Primary function pointer table for leading nibble (0x0 - 0xF)
OpcodeHandler *table[0x10];

// Secondary function pointer table for repeated leading nibbles (0x0, 0x8, 0xE, 0xF)
OpcodeHandler *table0[0x10];  // 0x0???
OpcodeHandler *table8[0x10];  // 0x8xy?
OpcodeHandler *tableE[0x10];  // 0xEx?? (only two instructions with unique third nibbles, so only last needs to be masked)
OpcodeHandler *tableF[0x100];  // 0xFx?? (some third nibbles are repeated)

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

    // Assign opcode handler functions to tables








    return;
}

void cpu_cycle(Chip8 *chip8) {
    // FETCH next instruction from program counter as a two-byte opcode
    uint16_t opcode = ( chip8->memory[chip8->PC] << 8u | chip8->memory[chip8->PC + 1] );

    // Increment program counter before executing
    chip8->PC += 2;

    // DECODE and EXECUTE instruction by first extracting leading nibble
    (table[ (opcode & 0xF000) >> 12u ])(chip8, opcode);
    
    // // Decrement delay and sound timers if set
	// if (delay_timer > 0)
	// {
	// 	--delay_timer;
	// }

	// // Decrement the sound timer if it's been set
	// if (soundTimer > 0)
	// {
	// 	--sound_timer;
	// }
}