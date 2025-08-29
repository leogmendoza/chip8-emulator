#include "menu.h"

#include <string.h>
#include "chip8.h"
#include "display.h"
#include "rom.h"
#include "sound.h"

static MenuState state;
static int startup_frames;
static int current_selection;
static uint8_t menu_fb[DISPLAY_SIZE];
static uint8_t prev_keypad[16];  // For rising-edge detection
static int click_frames = 0;

// List navigation indices
static int general_index = 0;  // selected item
static int screen_index  = 0;  // top-most item shown

// ROM list from rom_catalog
extern const char *rom_list[];
extern const int rom_count;

// 4x6 font
static const uint8_t font[] = {
    0x04, // width
    0x06, // height
    0x20, // first = SPACE
    0x5f, // Char count

    0x00,0x00,0x00,0x00,  // <space>
    0x00,0x17,0x00,0x00,  // !
    0x03,0x00,0x03,0x00,  // "
    0x1f,0x0a,0x1f,0x00,  // #
    0x16,0x1f,0x0d,0x00,  // $
    0x19,0x04,0x13,0x00,  // %
    0x0a,0x15,0x1a,0x00,  // &
    0x00,0x03,0x00,0x00,  // '
    0x00,0x0e,0x11,0x00,  // (
    0x11,0x0e,0x00,0x00,  // )
    0x0a,0x04,0x0a,0x00,  // *
    0x04,0x0e,0x04,0x00,  // +
    0x10,0x08,0x00,0x00,  // ,
    0x04,0x04,0x04,0x00,  // -
    0x00,0x10,0x00,0x00,  // .
    0x18,0x04,0x03,0x00,  // /

    0x1f,0x11,0x1f,0x00,  // 0
    0x12,0x1f,0x10,0x00,  // 1
    0x1a,0x19,0x16,0x00,  // 2
    0x11,0x15,0x0a,0x00,  // 3
    0x07,0x04,0x1f,0x00,  // 4
    0x17,0x15,0x0d,0x00,  // 5
    0x0e,0x15,0x09,0x00,  // 6
    0x19,0x05,0x03,0x00,  // 7
    0x1f,0x15,0x1f,0x00,  // 8
    0x12,0x15,0x0e,0x00,  // 9
    0x00,0x0a,0x00,0x00,  // :
    0x10,0x0a,0x00,0x00,  // ;
    0x04,0x0a,0x11,0x00,  // <
    0x0a,0x0a,0x0a,0x00,  // =
    0x11,0x0a,0x04,0x00,  // >
    0x02,0x19,0x06,0x00,  // ?

    0x0e,0x15,0x16,0x00,  // @
    0x1e,0x05,0x1e,0x00,  // A
    0x1f,0x15,0x0a,0x00,  // B
    0x0e,0x11,0x0a,0x00,  // C
    0x1f,0x11,0x0e,0x00,  // D
    0x1f,0x15,0x11,0x00,  // E
    0x1f,0x05,0x01,0x00,  // F
    0x0e,0x11,0x0d,0x00,  // G
    0x1f,0x04,0x1f,0x00,  // H
    0x11,0x1f,0x11,0x00,  // I
    0x11,0x1f,0x01,0x00,  // J
    0x1f,0x04,0x1b,0x00,  // K
    0x1f,0x10,0x10,0x00,  // L
    0x1f,0x02,0x1f,0x00,  // M
    0x1f,0x0e,0x1f,0x00,  // N
    0x0e,0x11,0x0e,0x00,  // O

    0x1f,0x05,0x02,0x00,  // P
    0x0e,0x11,0x2e,0x00,  // Q
    0x1f,0x05,0x1a,0x00,  // R
    0x12,0x15,0x09,0x00,  // S
    0x01,0x1f,0x01,0x00,  // T
    0x0f,0x10,0x0f,0x00,  // U
    0x07,0x18,0x07,0x00,  // V
    0x0f,0x1c,0x0f,0x00,  // W
    0x1b,0x04,0x1b,0x00,  // X
    0x03,0x1c,0x03,0x00,  // Y
    0x19,0x15,0x13,0x00,  // Z
    0x00,0x1f,0x11,0x00,  // [
    0x03,0x04,0x18,0x00,  // <backslash>
    0x11,0x1f,0x00,0x00,  // ]
    0x02,0x01,0x02,0x00,  // ^
    0x10,0x10,0x10,0x00,  // _

    0x00,0x01,0x02,0x00,  // `
    0x0c,0x12,0x1c,0x00,  // a
    0x1f,0x12,0x0c,0x00,  // b
    0x0c,0x12,0x12,0x00,  // c
    0x0c,0x12,0x1f,0x00,  // d
    0x0c,0x1a,0x14,0x00,  // e
    0x04,0x1f,0x05,0x00,  // f
    0x24,0x2a,0x1c,0x00,  // g
    0x1f,0x02,0x1c,0x00,  // h
    0x00,0x1d,0x00,0x00,  // i
    0x20,0x1d,0x00,0x00,  // j
    0x1f,0x08,0x14,0x00,  // k
    0x00,0x1f,0x00,0x00,  // l
    0x1e,0x06,0x1e,0x00,  // m
    0x1c,0x02,0x1c,0x00,  // n
    0x0c,0x12,0x0c,0x00,  // o

    0x3e,0x12,0x0c,0x00,  // p
    0x0c,0x12,0x3e,0x00,  // q
    0x1e,0x04,0x02,0x00,  // r
    0x14,0x0a,0x00,0x00,  // s
    0x02,0x1f,0x02,0x00,  // t
    0x0e,0x10,0x0e,0x00,  // u
    0x06,0x18,0x06,0x00,  // v
    0x0e,0x18,0x0e,0x00,  // w
    0x12,0x0c,0x12,0x00,  // x
    0x2e,0x28,0x1e,0x00,  // y
    0x1a,0x1e,0x16,0x00,  // z
    0x04,0x1f,0x11,0x00,  // {
    0x00,0x1b,0x00,0x00,  // |
    0x11,0x1f,0x04,0x00,  // }
    0x06,0x02,0x03,0x00   // ~
};

#define FONT_WIDTH  (font[0])
#define FONT_HEIGHT (font[1])
#define FONT_FIRST  (font[2])
#define FONT_COUNT  (font[3])

// --Utilities
static inline void fb_clear(void) {
    memset(menu_fb, 0, sizeof(menu_fb));

    return;
}

static inline void set_px(int x, int y) {
    if (((unsigned)x < DISPLAY_WIDTH) && (unsigned)y < DISPLAY_HEIGHT) {
        menu_fb[y * DISPLAY_WIDTH + x] = 1;
    }

    return;
}

static void draw_char(int px, int py, char c) {
    unsigned char uc = (unsigned char)c;
    if (uc < FONT_FIRST || uc >= (FONT_FIRST + FONT_COUNT)) {
        uc = ' ';
    }
    unsigned idx = (unsigned)(uc - FONT_FIRST);
    unsigned base_off = 4u + idx * (unsigned)FONT_WIDTH;

    for (int col = 0; col < (int)FONT_WIDTH; ++col) {
        uint8_t bits = font[base_off + (unsigned)col];

        for (int row = 0; row < (int)FONT_HEIGHT; ++row) {

            if (bits & 0x01) {
                set_px(px + col, py + row);
            }

            bits >>= 1;
        }
    }

    return;
}

static int text_width_px(const char *s, int max_chars) {
    int n = 0;

    while (*s && n < max_chars) { 
        ++n; ++s; 
    }

    return ( n * (int)FONT_WIDTH );
}

static void draw_text(int x, int y, const char *s, int max_chars) {
    int i = 0;

    while (*s && i < max_chars) {
        draw_char(x + i * (int)FONT_WIDTH, y, *s++);
        ++i;
    }

    return;
}

static void to_display_name(const char *path, char *out, int out_cap) {
    // Extract base name, strip extension, uppercase, replace '_' with SPACE
    const char *base = path;
    const char *p = path;
    const char *dot = 0;
    int n = 0;

    while (*p) {
        if (*p == '/' || *p == '\\') base = p + 1;
        if (*p == '.') dot = p;
        ++p;
    }
    if (!dot || dot <= base) dot = p; // no extension

    // Copy and normalize 
    for (const char *q = base; q < dot && n < out_cap - 1; ++q) {
        char c = *q;
        if (c >= 'a' && c <= 'z') c = (char)(c - 'a' + 'A');
        if (c == '_') c = ' ';
        out[n++] = c;
    }

    out[n] = '\0';

    return;
}

// --Drawing Helpers--

/* Print two lines: "Leopoldo Mendoza" and "CHIP-8 EMU, 2025" */
static void draw_startup_banner(void) {
    const char *l1 = "Leo Mendoza";
    const char *l2 = "2025";
    const int max_chars = DISPLAY_WIDTH / (int)FONT_WIDTH;

    int w1 = text_width_px(l1, max_chars);
    int w2 = text_width_px(l2, max_chars);

    int x1 = (DISPLAY_WIDTH - w1) / 2;
    int x2 = (DISPLAY_WIDTH - w2) / 2;

    int y = (DISPLAY_HEIGHT - (int)(2*FONT_HEIGHT)) / 2; // Two rows of font height

    draw_text(x1, y, l1, max_chars);
    draw_text(x2, y + (int)FONT_HEIGHT + 1, l2, max_chars);

    return;
}

/* List renderer that shows up to VISIBLE_ROWS items */
static void draw_menu_list(void) {
    const int VISIBLE_ROWS = (int)(DISPLAY_HEIGHT / (int)FONT_HEIGHT); // 32/8 = 4
    for (int i = 0; i < VISIBLE_ROWS; ++i) {
        int idx = screen_index + i;
        if (idx >= rom_count) break;
        char name[32];
        to_display_name(rom_list[idx], name, (int)sizeof(name));

        // leave a column for '>' + one space; clip to fit
        int max_chars = (DISPLAY_WIDTH / (int)FONT_WIDTH) - 2; // e.g., 10 -> 8
        int y = i * (int)FONT_HEIGHT;

        if (idx == general_index) {
            draw_char(0, y, '>');
        }

        draw_text(2 * (int)FONT_WIDTH, y, name, max_chars);
    }

    return;
}

// -- Menu API
void platform_menu_init(void) {
    state = MENU_STARTUP;
    startup_frames = STARTUP_FRAMES;
    general_index = 0;
    screen_index  = 0;

    memset(prev_keypad, 0, sizeof(prev_keypad));
    fb_clear();

    return;
}

/* Rising edge helper */
static int pressed_edge(const uint8_t keypad[16], int idx) {
    int edge = (keypad[idx] && !prev_keypad[idx]);
    prev_keypad[idx] = keypad[idx];

    if (edge) {
        click_frames = CLICK_BEEP_FRAMES; 
    }

    return edge;
}

MenuState platform_menu_update(Chip8* chip8, const uint8_t keypad[16]) {
    switch (state) {
        case MENU_STARTUP:
            if (--startup_frames <= 0) {
                state = MENU_SELECT;
            }

            break;

        case MENU_SELECT: {
            const int VISIBLE_ROWS = (int)(DISPLAY_HEIGHT / (int)FONT_HEIGHT); // 4

            if (pressed_edge(keypad, KEY_UP)) {
                if (general_index > 0) {
                    --general_index;
                    if (general_index < screen_index) {
                        --screen_index;
                    }
                }
            }

            if (pressed_edge(keypad, KEY_DOWN)) {
                if (general_index < rom_count - 1) {
                    ++general_index;
                    if (general_index > screen_index + (VISIBLE_ROWS - 1)) {
                        ++screen_index;
                    }
                }
            }

            if (pressed_edge(keypad, KEY_OK)) {
                rom_load_rom(chip8, rom_list[general_index]);
                state = MENU_IDLE;
            }

            if (screen_index < 0) {
                screen_index = 0;
            }

            int max_top = rom_count - VISIBLE_ROWS; 
            
            if (max_top < 0) {
                max_top = 0;
            }

            if (screen_index > max_top) {
                screen_index = max_top;
            }

            break; 
        }

        case MENU_IDLE:
            break;
    }

    return state;
}

void platform_menu_draw(void) {
    fb_clear();

    if (state == MENU_STARTUP) {
        draw_startup_banner();
    } else if (state == MENU_SELECT) {
        draw_menu_list();
    }

    platform_display_draw(menu_fb);

    // TODO: UNCOMMENT
    // Drive a short click sound while timer is active
    // if (click_frames > 0) {
    //     platform_sound_update(1);
    //     --click_frames;
    // } else {
    //     platform_sound_update(0);
    // }

    return;
}
