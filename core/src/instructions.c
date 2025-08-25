#include <string.h>
#include <stdio.h>

#include "instructions.h"
#include "chip8.h"

// Non-zero RNG state
static uint32_t rng_state = 1285334002;  // Chosen by yoms >_<

/* RNG Helper */
static uint8_t random_byte(void) {
    // xorshift32 algorithm
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;

    return (uint8_t)(rng_state & 0xFF);
}

/* No-operation (NOP) */
void op_null(Chip8 *chip8, uint16_t opcode) {
    return;
}

/* CLS: Clear screen */
void op_00E0(Chip8 *chip8, uint16_t opcode) {
    memset(chip8->display, 0, (DISPLAY_WIDTH * DISPLAY_HEIGHT));

    return;
}

/* RET: Return from subroutine */
void op_00EE(Chip8* chip8, uint16_t opcode) {
    // Handle underflow
    if (chip8->SP == 0) {
        return;
    }

    --chip8->SP;
    chip8->PC = chip8->stack[chip8->SP];

    return;
}

/* JP addr: Jump to memory location nnn */
void op_1nnn(Chip8 *chip8, uint16_t opcode) {
    chip8->PC = (opcode & 0x0FFF);

    return;
}

/* CALL addr: Call subroutine at nnn */
void op_2nnn(Chip8* chip8, uint16_t opcode) {
    // Handle overflow
    if (chip8->SP >= STACK_DEPTH) {
        return;
    }

    chip8->stack[chip8->SP] = chip8->PC;
    chip8->SP++;

    chip8->PC = (opcode & 0x0FFF);

    return;
}

/* SE Vx, byte: Skip next instruction if Vx = kk */
void op_3xkk(Chip8* chip8, uint16_t opcode) {
    if ( chip8->V[ (opcode & 0x0F00) >> 8 ] == (opcode & 0x00FF) ) {
        chip8->PC += 2; 
    }

    return;
}

/* SNE Vx, byte: Skip next instruction if Vx != kk */
void op_4xkk(Chip8* chip8, uint16_t opcode) {
    if ( chip8->V[ (opcode & 0x0F00) >> 8 ] != (opcode & 0x00FF) ) {
        chip8->PC += 2; 
    }

    return;
}

/* SE Vx, Vy: Skip next instruction if Vx = Vy */
void op_5xy0(Chip8* chip8, uint16_t opcode) {
    if ( chip8->V[ (opcode & 0x0F00) >> 8 ] == chip8->V[ (opcode & 0x00F0) >> 4 ] ) {
        chip8->PC += 2; 
    }

    return;
}

/* LD Vx, byte: Set register Vx to kk */
void op_6xkk(Chip8 *chip8, uint16_t opcode) {
    chip8->V[( (opcode & 0x0F00) >> 8 )] = ( opcode & 0x00FF );

    return;
}

/* ADD Vx, byte: Increment register Vx by kk */
void op_7xkk(Chip8 *chip8, uint16_t opcode) {
    chip8->V[( (opcode & 0x0F00) >> 8 )] += ( opcode & 0x00FF );

    return;
}

/* LD Vx, Vy: Set Vx = Vy */
void op_8xy0(Chip8* chip8, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    chip8->V[x] = chip8->V[y];

    return;
}

/* OR Vx, Vy: Set Vx = Vx OR Vy */
void op_8xy1(Chip8* chip8, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    chip8->V[x] |= chip8->V[y];

    return;
}

/* AND Vx, Vy: Set Vx = Vx AND Vy */
void op_8xy2(Chip8* chip8, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    chip8->V[x] &= chip8->V[y];

    return;
}

/* XOR Vx, Vy: Set Vx = Vx XOR Vy */
void op_8xy3(Chip8* chip8, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    chip8->V[x] ^= chip8->V[y];

    return;
}

/* ADD Vx, Vy: Set Vx = Vx + Vy, set VF = carry (overflow flag) */
void op_8xy4(Chip8* chip8, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    uint16_t sum = chip8->V[x] + chip8->V[y]; 

    // Check for overflow
    if (sum > 255) {
        chip8->V[0xF] = 1;

    } else {
        chip8->V[0xF] = 0;
    }

    chip8->V[x] = sum & 0x00FF;

    return;
}

/* SUB Vx, Vy: Set Vx = Vx - Vy, set VF = NOT borrow */
void op_8xy5(Chip8* chip8, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    if ( chip8->V[x] >= chip8->V[y] ) {
        chip8->V[0xF] = 1;
    } else {
        chip8->V[0xF] = 0;
    }
    
    chip8->V[x] -= chip8->V[y];

    return;
}

/* SHR Vx {, Vy}: Set Vx = Vx shifted right by 1 */
void op_8xy6(Chip8* chip8, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;

    // Save LSB in VF
    chip8->V[0xF] = chip8->V[x] & 0x01;

    chip8->V[x] >>= 1;

    return;
}

/* SUBN Vx, Vy: Set Vx = Vy - Vx, set VF = NOT borrow */
void op_8xy7(Chip8* chip8, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    if ( chip8->V[y] >= chip8->V[x] ) {
        chip8->V[0xF] = 1;
    } else {
        chip8->V[0xF] = 0;
    }
    
    chip8->V[x] = chip8->V[y] - chip8->V[x];

    return;
}

/* SHL Vx {, Vy}: Set Vx = Vx SHL 1 */
void op_8xyE(Chip8* chip8, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;

    // Save MSB in VF
    chip8->V[0xF] = chip8->V[x] >> 7;

    chip8->V[x] <<= 1;

    return;
}

/* SNE Vx, Vy: Skip next instruction if Vx != Vy */
void op_9xy0(Chip8* chip8, uint16_t opcode) {
    if ( chip8->V[ (opcode & 0x0F00) >> 8 ] != chip8->V[ (opcode & 0x00F0) >> 4 ] ) {
        chip8->PC += 2; 
    }

    return;
}

/* LD I, addr: Set index register to nnn */
void op_Annn(Chip8 *chip8, uint16_t opcode) {
    chip8->I = ( opcode & 0x0FFF );

    return;
}

/* JP V0, addr: Jump to location nnn + V0 */
void op_Bnnn(Chip8* chip8, uint16_t opcode) {
    chip8->PC = (opcode & 0x0FFF) + (chip8->V[0x0]);

    return;
}

/* RND Vx, byte */
void op_Cxkk(Chip8* chip8, uint16_t opcode) {
    chip8->V[ (opcode & 0x0F00) >> 8 ] = random_byte() & (opcode & 0x00FF);

    return;
}

/* DRW Vx, Vy, nibble: Display n-byte sprite from memory location I at (Vx, Vy), set VF = collision */
void op_Dxyn(Chip8 *chip8, uint16_t opcode) {
    uint8_t starting_x_pos = chip8->V[( (opcode & 0x0F00) >> 8 )] % DISPLAY_WIDTH;
    uint8_t starting_y_pos = chip8->V[( (opcode & 0x00F0) >> 4 )] % DISPLAY_HEIGHT;

    chip8->V[0xF] = 0;

    for (size_t row = 0; row < ( opcode & 0x000F ); row++) {
        // Protect against index out-of-range
        if (chip8->I + row >= MEMORY_SIZE) {
            break;
        }

        uint8_t current_y_pos = starting_y_pos + row;
        uint8_t sprite_byte = chip8->memory[chip8->I + row];

        // Stop drawing sprite when bottom edge is reached
        if (current_y_pos >= DISPLAY_HEIGHT) {
            break;
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

/* SKP Vx: Skip next instruction if key Vx is pressed */
void op_Ex9E(Chip8* chip8, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;

    if ( chip8->keypad[ chip8->V[x] ] ) {
        chip8->PC += 2;
    }

    return;
}

/* SKNP Vx: Skip next instruction if key Vx is not pressed */
void op_ExA1(Chip8* chip8, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;

    if ( !chip8->keypad[ chip8->V[x] ] ) {
        chip8->PC += 2;
    }

    return;
}

/* LD Vx, DT: Set Vx = delay timer value */
void op_Fx07(Chip8* chip8, uint16_t opcode) {
    chip8->V[ (opcode & 0x0F00) >> 8 ] = chip8->delay_timer;

    return;
} 

/* LD Vx, K: Pause execution until a key is pressed, then store key in Vx */
void op_Fx0A(Chip8* chip8, uint16_t opcode) {
    for (size_t i = 0; i < INPUT_KEY_COUNT; ++i) {
        // Find a pressed key
        if (chip8->keypad[i]) {
            chip8->V[ (opcode & 0x0F00) >> 8 ] = i;

            return;
        }
    }

    // Essentially, run instruction again if no key is pressed
    chip8->PC -= 2;

    return;
} 

/* LD DT, Vx: Set delay timer = Vx */
void op_Fx15(Chip8* chip8, uint16_t opcode) {
    chip8->delay_timer = chip8->V[ (opcode & 0x0F00) >> 8 ];

    return;
} 

/* LD ST, Vx: Set sound timer = Vx */
void op_Fx18(Chip8* chip8, uint16_t opcode) {
    chip8->sound_timer = chip8->V[ (opcode & 0x0F00) >> 8 ];    

    return;
} 

/* ADD I, Vx: Set I = I + Vx */
void op_Fx1E(Chip8* chip8, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;

    // Flag overflow
    chip8->V[0xF] = ( (chip8->I + chip8->V[x]) > 0x0FFF );

    chip8->I += chip8->V[x];

    return;
} 

/* LD F, Vx: Set I = memory location of sprite (in font set) for digit Vx */
void op_Fx29(Chip8* chip8, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;

    // Extract last nibble
    uint8_t digit = chip8->V[x] & 0x0F;  

    chip8->I = FONT_SET_START_ADDRESS + ( 5 * digit );

    return;
} 

/* LD B, Vx: Store BCD rep. of Vx in memory locations I, I+1, and I+2 */
void op_Fx33(Chip8* chip8, uint16_t opcode) {
    if ( (chip8->I + 2) >= MEMORY_SIZE) {
        return;
    }

    uint8_t num = chip8->V[ (opcode & 0x0F00) >> 8 ];

    // Store ones place
    chip8->memory[chip8->I + 2] = num % 10;
    num /= 10;

    // Store tens place
    chip8->memory[chip8->I + 1] = num % 10;
    num /= 10;

    // Store hundreds place
    chip8->memory[chip8->I] = num % 10;

    return;
} 

/* LD [I], Vx: Store V0 through Vx (inclusive) in memory starting at location I */
void op_Fx55(Chip8* chip8, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;

    // Prevent writing past end of memory
    if ( (chip8->I + x) >= MEMORY_SIZE) {
        return;
    }

    for (size_t i = 0; i <= x; ++i) {
        chip8->memory[chip8->I + i] = chip8->V[i];
    }

    return;
} 

/* LD Vx, [I]: Load V0 through Vx (inclusive) from memory starting at location I */
void op_Fx65(Chip8* chip8, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;

    // Prevent writing past end of memory
    if ( (chip8->I + x) >= MEMORY_SIZE) {
        return;
    }

    for (size_t i = 0; i <= x; ++i) {
        chip8->V[i] = chip8->memory[chip8->I + i];
    }

    return;
} 
