#pragma once

#include "platform.h"

// Used by menu.c
extern const char *rom_list[];
extern const int   rom_count;

typedef struct {
    const char *name;
    
    int instructions_per_frame;
    int cpu_cycle_delay_ms;
} RomConfig;

extern const RomConfig rom_configs[];
extern const int rom_config_count;
