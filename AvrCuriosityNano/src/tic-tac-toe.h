/* 
 * File:   tic-tac-toe.h
 * Author: hursa
 *
 * Created on May 13, 2025, 4:14 PM
 */

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

#define ROW_PORT PORTC
#define COL_PORT PORTF

bool gameEnded(uint8_t board[3][3], int currPlayer);
void setupIO();
bool scanKey(uint8_t *col, uint8_t *row);
bool drawCheck(uint8_t board[3][3]);
void remove_old(uint8_t board[3][3], uint8_t moves[3][2], uint8_t *move_count, Strip *s);
void bot_move(uint8_t board[3][3], int *col, int *row, int banned_c, int banned_r, int move_count, uint8_t move[2]);
void check_end(uint8_t board[3][3], Strip *s, int player, uint8_t *p1_count, 
                uint8_t *p2_count, uint8_t *banned_c, uint8_t *banned_r);


#endif	/* TIC_TAC_TOE_H */

