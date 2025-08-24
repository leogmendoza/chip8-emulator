#include "chip8.h"

typedef void OpcodeHandler(Chip8 *chip8, uint16_t opcode);

void op_null(Chip8* chip8, uint16_t opcode);
void op_00E0(Chip8* chip8, uint16_t opcode);
void op_00EE(Chip8* chip8, uint16_t opcode);
void op_1nnn(Chip8* chip8, uint16_t opcode);
void op_2nnn(Chip8* chip8, uint16_t opcode);
void op_3xkk(Chip8* chip8, uint16_t opcode);
void op_4xkk(Chip8* chip8, uint16_t opcode);
void op_6xkk(Chip8* chip8, uint16_t opcode);
void op_7xkk(Chip8* chip8, uint16_t opcode);
void op_Annn(Chip8* chip8, uint16_t opcode);
void op_Dxyn(Chip8* chip8, uint16_t opcode);