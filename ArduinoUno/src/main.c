#include <avr/io.h>
#include <stdbool.h>
#define F_CPU 20000000
#include <util/delay.h>
#include <assert.h>

#include "led_driver.h"
#include "tic-tac-toe.h"
#include "config.h"

int main(void) {

    setupIO();
    uint8_t board[3][3] = {{0}};
    
    uint8_t player1_moves[3][2] = {{255}};
    uint8_t player2_moves[3][2] = {{255}};
    uint8_t p1_count = 0;
    uint8_t p2_count = 0;
    
    uint8_t turnCounter = 0;
    uint8_t row = 0;
    uint8_t col = 0;
    uint8_t player;
    DDRD = 0xff;
    uint8_t banned_c = -1;
    uint8_t banned_r = -1;
    
    Strip s = {0};
    flush_strip(s, 9);

    if (check_color(PLAYER_1_COLOR, PLAYER_2_COLOR) || 
        check_color(PLAYER_1_COLOR, DRAW_COLOR) ||
        check_color(PLAYER_2_COLOR, DRAW_COLOR)) {
            error_anim(&s);
    }
    
    while (1) {
        player = turnCounter % 2 == 0 ? 1 : 2;
        uint8_t (*moves)[2] = player == 1 ? player1_moves : player2_moves;
        uint8_t *move_count = player == 1 ? &p1_count : &p2_count;
        Color color = player == 1 ? PLAYER_1_COLOR : PLAYER_2_COLOR;

        if (GAME_MODE) {
            if (*move_count == 3) {
                set_color(&s, get_dim(color, DIM_DIV), moves[0][1], moves[0][0]);
                flush_strip(s, 9);
            }
        }
        
        
        while (!scanKey(&row, &col)) {}
        if (board[row][col] != 0) {
            continue;
        }

        if (GAME_MODE) {
            remove_old(board, moves, move_count, &s);
            moves[*move_count - 1][0] = row;
            moves[*move_count - 1][1] = col;
        }
        
        board[row][col] = player;
        set_color(&s, color, row, col);
        flush_strip(s, 9);
        
        check_end(board, &s, player, &p1_count, &p2_count, &banned_c, &banned_r);
        
        turnCounter++;

        if (!BOT) {
            continue;
        }

        _delay_ms(500);
        player = 2;
        color = PLAYER_2_COLOR;
        moves = player2_moves;
        move_count = &p2_count;

        int bot_c, bot_r;
        if (GAME_MODE) {
            if (*move_count == 3) {
                banned_c = moves[0][1];
                banned_r = moves[0][0];

                set_color(&s, get_dim(color, DIM_DIV), moves[0][1], moves[0][0]);
                flush_strip(s, 9);
                _delay_ms(1000);
            }
            remove_old(board, moves, move_count, &s);
        }

        bot_move(board, &bot_r, &bot_c, banned_c, banned_r, p1_count, player1_moves[0]);
        if (GAME_MODE) {
            moves[*move_count - 1][0] = bot_r;
            moves[*move_count - 1][1] = bot_c;
        }

        set_color(&s, color, bot_r, bot_c);
        flush_strip(s, 9);

        check_end(board, &s, player, &p1_count, &p2_count, &banned_c, &banned_r);

        turnCounter++;
    }
}