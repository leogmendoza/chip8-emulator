#include "rom_loader.h"

#ifdef PLATFORM_STM32

#include <string.h>
#include <stdint.h>
#include "rom_catalog.h"

extern const uint8_t rom_tetris[];          
extern const int rom_tetris_len;

extern const uint8_t rom_space_invaders[];  
extern const int rom_space_invaders_len;

extern const uint8_t rom_tic_tac_toe[];     
extern const int rom_tic_tac_toe_len;

extern const uint8_t rom_brix[];            
extern const int rom_brix_len;

extern const uint8_t rom_pong[];            
extern const int rom_pong_len;

extern const uint8_t rom_connect_4[];       
extern const int rom_connect_4_len;

extern const uint8_t rom_puzzle[];          
extern const int rom_puzzle_len;

extern const uint8_t rom_blinky[];          
extern const int rom_blinky_len;

extern const uint8_t rom_test_opcode[];     
extern const int rom_test_opcode_len;

/* Map ROM names to each corresponding ROM hex dump (and length duh) */
static int load_named_rom(const char *name, const uint8_t **out_data, int *out_len) {
    if (strcmp(name, "tetris") == 0) { 
        *out_data = rom_tetris;          
        *out_len = rom_tetris_len;          
        
        return 0; 
    }

    if (strcmp(name, "space_invaders") == 0){ 
        *out_data = rom_space_invaders;  
        *out_len = rom_space_invaders_len;  

        return 0; 
    }

    if (strcmp(name, "tic_tac_toe") == 0) { 
        *out_data = rom_tic_tac_toe;     
        *out_len = rom_tic_tac_toe_len;     
        
        return 0; 
    }

    if (strcmp(name, "brix") == 0) { 
        *out_data = rom_brix;            
        *out_len = rom_brix_len;            
        
        return 0; 
    }

    if (strcmp(name, "pong") == 0) { 
        *out_data = rom_pong;            
        *out_len = rom_pong_len;            
        
        return 0; 
    }

    if (strcmp(name, "connect_4") == 0) { 
        *out_data = rom_connect_4;       
        *out_len = rom_connect_4_len;       
        
        return 0; 
    }

    if (strcmp(name, "puzzle") == 0) { 
        *out_data = rom_puzzle;          
        *out_len = rom_puzzle_len;          
        
        return 0; 
    }

    if (strcmp(name, "blinky") == 0) { 
        *out_data = rom_blinky;          
        *out_len = rom_blinky_len;          
        
        return 0; 
    }

    if (strcmp(name, "test_opcode") == 0) { 
        *out_data = rom_test_opcode;     
        *out_len = rom_test_opcode_len;     
        
        return 0; 
    }

    return -1;
}

uint8_t platform_rom_loader_read_mem(Chip8* chip8, uint16_t address) {
    return chip8->memory[address];
}

void platform_rom_loader_write_mem(Chip8* chip8, uint16_t address, uint8_t value) {
    chip8->memory[address] = value;

    return;
}

int platform_rom_loader_load_rom(uint8_t *buffer, size_t max_size, const char *name) {
    if (!buffer || !name) {
        return -1;
    }

    const uint8_t *src = NULL;
    int len = 0;

    if (load_named_rom(name, &src, &len) != 0) {
        return -1;
    }

    if ((size_t)len > max_size) {
        return -1;
    }

    memcpy(buffer, src, (size_t)len);

    return len; 
}

#endif