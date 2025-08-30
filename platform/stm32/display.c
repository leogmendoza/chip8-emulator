#include "display.h"

#ifdef PLATFORM_STM32

#include <string.h>
#include "stm32f1xx_hal.h"
#include "chip8.h"

// SSD1306 command defines
#define SSD1306_I2C_ADDR (0x3C << 1)  // Default I2C address

#define SSD1306_DISPLAYOFF            0xAE
#define SSD1306_DISPLAYON             0xAF
#define SSD1306_SETMEMORYMODE         0x20
#define SSD1306_HORIZADDRMODE         0x00
#define SSD1306_SETSTARTPAGE          0xB0
#define SSD1306_SETSTARTLINE          0x40
#define SSD1306_SETCONTRAST           0x81
#define SSD1306_SEGREMAP              0xA1
#define SSD1306_NORMALDISPLAY         0xA6
#define SSD1306_SETMULTIPLEX          0xA8
#define SSD1306_SETOFFSET             0xD3
#define SSD1306_SETDISPLAYCLOCKDIV    0xD5
#define SSD1306_SETPRECHARGE          0xD9
#define SSD1306_SETCOMPINS            0xDA
#define SSD1306_SETVCOMDETECT         0xDB
#define SSD1306_CHARGEPUMP            0x8D

// Values for config
#define SSD1306_CONTRAST_MAX          0xFF
#define SSD1306_MULTIPLEX_64          0x3F
#define SSD1306_NO_OFFSET             0x00
#define SSD1306_CLOCK_DIV_DEFAULT     0xF0
#define SSD1306_PRECHARGE_DEFAULT     0x22
#define SSD1306_COMPINS_ALT           0x12
#define SSD1306_VCOMH_DEFAULT         0x20
#define SSD1306_CHARGEPUMP_ENABLE     0x14

// SSD1306 geometry
#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64
#define SSD1306_BUFFER_SIZE (SSD1306_WIDTH * SSD1306_HEIGHT / 8)

extern I2C_HandleTypeDef hi2c1;

static uint8_t SSD1306_Buffer[SSD1306_BUFFER_SIZE];

typedef enum {
    Black = 0x00,
    White = 0x01
} SSD1306_COLOR;

// -- I2C write helpers --
static void ssd1306_write_command(uint8_t byte) {
    HAL_I2C_Mem_Write(&hi2c1, SSD1306_I2C_ADDR, 0x00, 1, &byte, 1, HAL_MAX_DELAY);

    return;
}

static void ssd1306_write_data(uint8_t* buffer, size_t buff_size) {
    HAL_I2C_Mem_Write(&hi2c1, SSD1306_I2C_ADDR, 0x40, 1, buffer, buff_size, HAL_MAX_DELAY);

    return;
}

// -- SSD1306 Drivers --
static void ssd1306_init(void) {
    HAL_Delay(100);

    ssd1306_write_command(SSD1306_DISPLAYOFF);
    ssd1306_write_command(SSD1306_SETMEMORYMODE);
    ssd1306_write_command(SSD1306_HORIZADDRMODE);
    ssd1306_write_command(SSD1306_SETSTARTPAGE);
    ssd1306_write_command(0xC8); // COM scan dec
    ssd1306_write_command(0x00); // low col
    ssd1306_write_command(0x10); // high col
    ssd1306_write_command(SSD1306_SETSTARTLINE);
    ssd1306_write_command(SSD1306_SETCONTRAST);
    ssd1306_write_command(SSD1306_CONTRAST_MAX);
    ssd1306_write_command(SSD1306_SEGREMAP);
    ssd1306_write_command(SSD1306_NORMALDISPLAY);
    ssd1306_write_command(SSD1306_SETMULTIPLEX);
    ssd1306_write_command(SSD1306_MULTIPLEX_64);
    ssd1306_write_command(SSD1306_SETOFFSET);
    ssd1306_write_command(SSD1306_NO_OFFSET);
    ssd1306_write_command(SSD1306_SETDISPLAYCLOCKDIV);
    ssd1306_write_command(SSD1306_CLOCK_DIV_DEFAULT);
    ssd1306_write_command(SSD1306_SETPRECHARGE);
    ssd1306_write_command(SSD1306_PRECHARGE_DEFAULT);
    ssd1306_write_command(SSD1306_SETCOMPINS);
    ssd1306_write_command(SSD1306_COMPINS_ALT);
    ssd1306_write_command(SSD1306_SETVCOMDETECT);
    ssd1306_write_command(SSD1306_VCOMH_DEFAULT);
    ssd1306_write_command(SSD1306_CHARGEPUMP);
    ssd1306_write_command(SSD1306_CHARGEPUMP_ENABLE);
    ssd1306_write_command(SSD1306_DISPLAYON);

    memset(SSD1306_Buffer, 0, sizeof(SSD1306_Buffer));

    return;
}

static void ssd1306_fill(SSD1306_COLOR color) {
    memset(SSD1306_Buffer,
           (color == Black) ? 0x00 : 0xFF,
           sizeof(SSD1306_Buffer));

    return;
}

static void ssd1306_update_screen(void) {
    for (uint8_t i = 0; i < SSD1306_HEIGHT/8; i++) {
        ssd1306_write_command(SSD1306_SETSTARTPAGE + i);
        ssd1306_write_command(0x00);
        ssd1306_write_command(0x10);

        ssd1306_write_data(&SSD1306_Buffer[SSD1306_WIDTH * i], SSD1306_WIDTH);
    }

    return;
}

static void ssd1306_draw_pixel(uint8_t x, uint8_t y, SSD1306_COLOR color) {
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) {
        return;
    } 

    if (color == White) {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= (1 << (y % 8));
    } else {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }

    return;
}

// -- Display API --
int platform_display_init(int scale) {
    ssd1306_init();

    return 0;
}

void platform_display_destroy(void) {
    return;
}

/* Map 64x32 CHIP-8 framebuffer to 128x64 OLED (scale 2x2) */ 
void platform_display_draw(const uint8_t *framebuffer) {
    ssd1306_fill(Black);

    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            if (framebuffer[y * DISPLAY_WIDTH + x]) {
                ssd1306_draw_pixel(x*2,   y*2,   White);
                ssd1306_draw_pixel(x*2+1, y*2,   White);
                ssd1306_draw_pixel(x*2,   y*2+1, White);
                ssd1306_draw_pixel(x*2+1, y*2+1, White);
            }
        }
    }

    ssd1306_update_screen();
}

int platform_display_poll_events(void) {
    return 1;
}

#endif
