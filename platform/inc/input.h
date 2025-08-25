#pragma once

#include <stdint.h>

#include "platform.h"

int platform_input_init(void);
void platform_input_destroy(void);
void platform_input_update(uint8_t keypad[16]);