#ifndef LED_DRIVER
#define LED_DRIVER

#include <stdint.h>
#include <stdbool.h>
#include <avr/interrupt.h>

struct Color {
    uint8_t rgb[3];
} typedef Color;

struct Strip {
    Color led[9];
} typedef Strip;

void flush_strip(Strip strip, uint8_t led_count);
void set_color(Strip *strip, Color color, int col, int row);
void clear_strip(Strip *strip, uint8_t led_count);
void ending_animation(Strip *s, Color color);
Color get_dim(Color color, int DIM_DIV);
bool check_color(Color color1, Color color2);
void error_anim(Strip *s);

#endif