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
            LED_PORT |= (1 << PIN0);       // HIGH
            _delay_loop_1(3); // ~0.8�s
            asm volatile("nop");
            //TBD or just use NOPS
            LED_PORT &= ~(1 << PIN0);        // LOW
           _delay_loop_1(1);
           asm volatile("nop\n\t"
                        "nop\n\t");  // ~0.45�s
        } else {
            // Send '0' bit
            LED_PORT |= (1 << PIN0);        // HIGH
            _delay_loop_1(1);  // ~0.35�s
            LED_PORT &= ~(1 << PIN0);        // LOW
            _delay_loop_1(3); // ~0.9�s
            asm volatile("nop\n\t"
                        "nop\n\t");
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

void set_color(Strip *strip, Color color, int row, int col) {
    uint8_t led = row * 3 + col;
    if (row == 1) {
        led = 3 + (2 - col);
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