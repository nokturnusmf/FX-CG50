#include "renderer.h"

static unsigned short* vram;

void init_renderer() {
    Bdisp_EnableColor(1);
    vram = (unsigned short*)GetVRAMAddress();
}

void clear(unsigned short bg, int start_row, int end_row) {
    for (int i = start_row * LCD_WIDTH_PX; i < end_row * LCD_WIDTH_PX; ++i) {
        vram[i] = bg;
    }
}

void put_pixel(unsigned short pixel, int x, int y) {
    vram[y * LCD_WIDTH_PX + x] = pixel;
}

static inline int max(int a, int b) {
    return a > b ? a : b;
}

void draw_sprite(const unsigned short* sprite, int x, int y, int w, int h) {
    for (int i = max(0, -y); i < h; ++i) {
        if (y + i >= LCD_HEIGHT_PX) {
            break;
        }

        for (int j = max(0, -x); j < w; ++j) {
            if (x + j >= LCD_WIDTH_PX) {
                break;
            }

            if (sprite[i * w + j]) { // filter out all black as transparent, 0x0821 is "black" instead
                vram[(y + i) * LCD_WIDTH_PX + x + j] = sprite[i * w + j];
            }
        }
    }
}

void write_frame() {
    Bdisp_PutDisp_DD();
}
