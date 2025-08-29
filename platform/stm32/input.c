#include "input.h"

#ifdef PLATFORM_STM32

#include "stm32f1xx_hal.h"
#include "chip8.h"

// Map CHIP-8 keypad indices to STM32 ports and pins
static GPIO_TypeDef *key_ports[16] = {
    GPIOA, // 0
    GPIOA, // 1 = KEY_UP
    GPIOA, // 2
    GPIOA, // 3
    GPIOA, // 4 = KEY_DOWN
    GPIOA, // 5
    GPIOA, // 6
    GPIOA, // 7 = KEY_OK
    GPIOB, // 8
    GPIOB, // 9
    GPIOB, // A
    GPIOB, // B
    GPIOB, // C
    GPIOB, // D
    GPIOB, // E
    GPIOB  // F
};

static const uint16_t key_pins[16] = {
    GPIO_PIN_0,   // 0
    GPIO_PIN_1,   // 1 = KEY_UP
    GPIO_PIN_2,   // 2
    GPIO_PIN_3,   // 3
    GPIO_PIN_4,   // 4 = KEY_DOWN
    GPIO_PIN_5,   // 5
    GPIO_PIN_6,   // 6
    GPIO_PIN_7,   // 7 = KEY_OK
    GPIO_PIN_8,   // 8
    GPIO_PIN_9,   // 9
    GPIO_PIN_10,  // A
    GPIO_PIN_11,  // B
    GPIO_PIN_12,  // C
    GPIO_PIN_13,  // D
    GPIO_PIN_14,  // E
    GPIO_PIN_15   // F
};

int platform_input_init(void) {
    // No setup needed since CubeMX configs pins
    
    return 0;
}

void platform_input_destroy(void) {
    // No teardown needed for STM keeb

    return;
}

/* Update CHIP-8 keypad by polling GPIO inputs */
void platform_input_update(uint8_t keypad[16]) {
    for (size_t i = 0; i < INPUT_KEY_COUNT; i++) {
        GPIO_PinState state = HAL_GPIO_ReadPin(key_ports[i], key_pins[i]);
        
        keypad[i] = (state == GPIO_PIN_RESET) ? 1 : 0;  // Note: Active-low
    }

    return;
}

#endif