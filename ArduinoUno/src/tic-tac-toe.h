#ifndef TIC_TAC_TOE_H
#define	TIC_TAC_TOE_H

#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
#define F_CPU 20000000
#include <util/delay.h>
#include <stdlib.h>
#include "led_driver.h"
#include "config.h"

#define COL_PORT PORTC
#define COL_DIR DDRC
#define ROW_DIR DDRD
#define ROW_PORT PORTD

bool gameEnded(uint8_t board[3][3], int currPlayer);
void setupIO();
bool scanKey(uint8_t *row, uint8_t *col);
bool drawCheck(uint8_t board[3][3]);
void remove_old(uint8_t board[3][3], uint8_t moves[3][2], uint8_t *move_count, Strip *s);
void bot_move(uint8_t board[3][3], int *row, int *col, int banned_r, int banned_c, int move_count, uint8_t move[2]);
void check_end(uint8_t board[3][3], Strip *s, int player, uint8_t *p1_count, 
                uint8_t *p2_count, uint8_t *banned_r, uint8_t *banned_c);


#endif	/* TIC_TAC_TOE_H */

