#include <string.h>
#include <stdio.h>

#include "instructions.h"

// No-operation (NOP)
void op_null(Chip8 *chip8, uint16_t opcode) {
    return;
}

// CLS: Clear screen
void op_00E0(Chip8 *chip8, uint16_t opcode) {
    memset(chip8->display, 0, sizeof(chip8->display));

    return;
}

// JP addr: Jump to memory location nnn
void op_1nnn(Chip8 *chip8, uint16_t opcode) {
    chip8->PC = (opcode & 0x0FFF);

    return;
}

// LD Vx, byte: Set register Vx to kk
void op_6xkk(Chip8 *chip8, uint16_t opcode) {
    chip8->V[( (opcode & 0x0F00) >> 8 )] = ( opcode & 0x00FF );

    return;
}

// ADD Vx, byte: Increment register Vx by kk
void op_7xkk(Chip8 *chip8, uint16_t opcode) {
    chip8->V[( (opcode & 0x0F00) >> 8 )] += ( opcode & 0x00FF );

    return;
}

// LD I, addr: Set index register to nnn
void op_Annn(Chip8 *chip8, uint16_t opcode) {
    chip8->I = ( opcode & 0x0FFF );

    return;
}

// DRW Vx, Vy, nibble: Display n-byte sprite from memory location I at (Vx, Vy), set VF = collision
void op_Dxyn(Chip8 *chip8, uint16_t opcode) {
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