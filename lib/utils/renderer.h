#ifndef RENDERER_H
#define RENDERER_H

#include <fxcg/display.h>

void init_renderer();

void clear(unsigned short bg = 0xffff, int start_row = 0, int end_row = LCD_HEIGHT_PX);

void put_pixel(unsigned short pixel, int x, int y);
void draw_sprite(const unsigned short* sprite, int x, int y, int w, int h);

void write_frame();

#endif // RENDERER_H
