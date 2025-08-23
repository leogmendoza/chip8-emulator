#include <string.h>

#include "cpu.h"

// Primary function pointer table for leading nibble (0x0 - 0xF)
static OpcodeHandler *table[0x10];

// Secondary function pointer table for repeated leading nibbles (0x0, 0x8, 0xE, 0xF)
static OpcodeHandler *table0[0x10];  // 0x0???
static OpcodeHandler *table8[0x10];  // 0x8xy?
static OpcodeHandler *tableE[0x10];  // 0xEx?? (only two instructions with unique third nibbles, so only last needs to be masked)
static OpcodeHandler *tableF[0x100];  // 0xFx?? (some third nibbles are repeated)

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
    printf(
        "Executed: %04X | PC: %04X | I: %04X | V[0]=%02X V[1]=%02X ...\n",
        opcode, chip8->PC, chip8->I, chip8->V[0], chip8->V[1]
    );
    
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

// No-operation (NOP)
static void op_null(Chip8 *chip8, uint16_t opcode) {
    return;
}

// CLS: Clear screen
static void op_00E0(Chip8 *chip8, uint16_t opcode) {
    memset(chip8->display, 0, sizeof(chip8->display));

    return;
}

// JP addr: Jump to memory location nnn
static void op_1nnn(Chip8 *chip8, uint16_t opcode) {
    chip8->PC = (opcode & 0x0FFF);

    return;
}

// LD Vx, byte: Set register Vx to kk
static void op_6xkk(Chip8 *chip8, uint16_t opcode) {
    chip8->V[( opcode & 0x0F00 >> 8 )] = ( opcode & 0x00FF );

    return;
}

// ADD Vx, byte: Increment register Vx by kk
static void op_7xkk(Chip8 *chip8, uint16_t opcode) {
    chip8->V[( opcode & 0x0F00 >> 8 )] += ( opcode & 0x00FF );

    return;
}

// LD I, addr: Set index register to nnn
static void op_Annn(Chip8 *chip8, uint16_t opcode) {
    chip8->I = ( opcode & 0x0FFF );

    return;
}

// DRW Vx, Vy, nibble: Display n-byte sprite from memory location I at (Vx, Vy), set VF = collision
static void op_Dxyn(Chip8 *chip8, uint16_t opcode) {
    uint8_t starting_x_pos = chip8->V[( (opcode & 0x0F00) >> 8 )] % DISPLAY_WIDTH;
    uint8_t starting_y_pos = chip8->V[( (opcode & 0x00F0) >> 4 )] % DISPLAY_HEIGHT;

    chip8->V[0xF] = 0;

    for (size_t row = 0; row < ( opcode & 0x000F ); row++) {
        uint8_t current_y_pos = starting_y_pos + row;
        uint8_t sprite_byte = chip8->memory[chip8->I + row];

        // Stop drawing sprite when bottom edge is reached
        if (current_y_pos >= DISPLAY_HEIGHT) {
            return;
        }

        for (size_t col = 0; col < 8; col++) {
            uint8_t current_x_pos = starting_x_pos + col;

            // Stop drawing row when right edge is reached
            if (current_x_pos >= DISPLAY_WIDTH) {
                break;
            }

            uint8_t sprite_pixel = ( (sprite_byte >> (7 - col)) & 0b1 );

            if (sprite_pixel) {
                uint8_t *display_pixel_ptr = &chip8->display[ (current_y_pos * DISPLAY_WIDTH) + current_x_pos ];

                // Flag a collision
                if (*display_pixel_ptr) {
                    chip8->V[0xF] = 0b1;
                } 
                
                // Flip pixel
                *display_pixel_ptr ^= 0b1;
            }
        }
    }
    return;
}