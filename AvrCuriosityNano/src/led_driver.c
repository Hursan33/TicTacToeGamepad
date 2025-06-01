/*
 * File:   led_driver.c
 * Author: hursa
 *
 * Created on May 12, 2025, 11:40 PM
 */


#include <avr/io.h>
#define F_CPU 20000000
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "led_driver.h"

#define ANIM_SPEED 200

// Send one byte to WS2812
static void send_byte(uint8_t byte) {
    for (uint8_t i = 0; i < 8; i++) {
        if (byte & (1 << (7 - i))) {
            // Send '1' bit
            PORTD.OUTSET = PIN0_bm;       // HIGH
            __builtin_avr_delay_cycles(13); // ~0.8�s
            PORTD.OUTCLR = PIN0_bm;       // LOW
            __builtin_avr_delay_cycles(6);  // ~0.45�s
        } else {
            // Send '0' bit
            PORTD.OUTSET = PIN0_bm;       // HIGH
            __builtin_avr_delay_cycles(4);  // ~0.35�s
            PORTD.OUTCLR = PIN0_bm;       // LOW
            __builtin_avr_delay_cycles(14); // ~0.9�s
        }
    }
}

void send_rgb(uint8_t *rgb_data, uint16_t len) {
    cli();
    for (uint16_t i = 0; i < len; i++) {
        send_byte(rgb_data[i]);
    }

    sei();
}

void flush_strip(Strip strip, uint8_t led_count) {
    for (int i = 0; i < led_count; i++) {
        send_rgb(strip.led[i].rgb, 3);
    }
    
    _delay_us(50);
}

void set_color(Strip *strip, Color color, int col, int row) {
    uint8_t led = col * 3 + row;
    if (col == 1) {
        led = 3 + (2 - row);
    }
    strip->led[led].rgb[1] = color.rgb[0];
    strip->led[led].rgb[0] = color.rgb[1];
    strip->led[led].rgb[2] = color.rgb[2];
}

void set_color_true_pos(Strip *strip, int led_count, Color color) {
    strip->led[led_count].rgb[1] = color.rgb[0];
    strip->led[led_count].rgb[0] = color.rgb[1];
    strip->led[led_count].rgb[2] = color.rgb[2];
}

void clear_strip(Strip *strip, uint8_t led_count) {
    for (int i = 0; i < led_count; i++) {
        strip->led[i].rgb[0] = 0;
        strip->led[i].rgb[1] = 0;
        strip->led[i].rgb[2] = 0;
    }
}

void ending_animation(Strip *s, Color color) {

    _delay_ms(700);
    clear_strip(s, 9);
    flush_strip(*s, 9);
    _delay_ms(ANIM_SPEED);
    for (int i = 0; i < 9; i++) {
        set_color_true_pos(s, i, color);
        flush_strip(*s, 9);
        _delay_ms(ANIM_SPEED);
    }

    for (int i = 0; i < 3; i++) {
        _delay_ms(400);
        clear_strip(s, 9);
        flush_strip(*s, 9);
        _delay_ms(400);
        for (int j = 0; j < 9; j++) {
            set_color_true_pos(s, j, color);
        }
        flush_strip(*s, 9);
    }

    _delay_ms(5000);
    clear_strip(s, 9);
    flush_strip(*s, 9);
}

Color get_dim(Color color, int DIM_DIV) {
    return (Color) {{(int) (color.rgb[0] / DIM_DIV), (int) (color.rgb[1] / DIM_DIV), (int) (color.rgb[2] / DIM_DIV)}};
}

bool check_color(Color color1, Color color2) {
    return color1.rgb[0] == color2.rgb[0] &&
            color1.rgb[1] == color2.rgb[1] &&
            color1.rgb[2] == color2.rgb[2];
}

void error_anim(Strip *s) {
    while (1) {
        for (int j = 0; j < 9; j++) {
            set_color_true_pos(s, j, (Color) {{255, 0, 0}});
        }
        flush_strip(*s, 9);
        _delay_ms(400);
        clear_strip(s, 9);
        flush_strip(*s, 9);
        _delay_ms(400);
    }
}