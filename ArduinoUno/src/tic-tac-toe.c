#include "tic-tac-toe.h"

const uint8_t rows[3] = {PIN0, PIN1, PIN2};
bool prev_states[3][3] = {{true}};

bool gameEnded(uint8_t board[3][3], int currPlayer) {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] == currPlayer){
            return true;
        }
    }
    
    for (int i = 0; i < 3; i++) {
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] == currPlayer){
            return true;
        }
    }
    
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] == currPlayer) {
        return true;
    }
    
    if (board[2][0] == board[1][1] && board[1][1] == board[0][2] && board[2][0] == currPlayer) {
        return true;
    }
    
    return false;
}

void setupIO() {
    ROW_DIR = 0xff;
    COL_DIR = 0x00;
    COL_PORT |= (1 << PIN0);
    COL_PORT |= (1 << PIN1);
    COL_PORT |= (1 << PIN2);
}

bool scanKey(uint8_t* row, uint8_t* col) {
    bool keyPressed = false;
    for (int r = 0; r < 3; r++) {
        ROW_PORT |= (1 << rows[r]);
        _delay_ms(5);

        for(int c = 0; c < 3; c++) {
            bool pressed = !(COL_PORT & 1 << c);
                
            if (pressed && !prev_states[r][c]) {
                *col = c;
                *row = r;
                keyPressed = true;
            }
            
            prev_states[r][c] = pressed;
        }

        ROW_PORT &= ~(1 << rows[r]);
        _delay_ms(20);
    }
    
    return keyPressed;
}

bool drawCheck(uint8_t board[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 0) {
                return false;
            }
        }
    }
    
    return true;
}

void remove_old(uint8_t board[3][3], uint8_t moves[3][2], uint8_t *move_count, Strip *s) {
    if (*move_count == 3) {
        uint8_t *move = moves[0];

        board[move[0]][move[1]] = 0;

        uint8_t led = move[0] * 3 + move[1];
        if (move[0] == 1) {
            led = 3 + (2 - move[1]);
        }
        set_color(s, (Color){{0, 0, 0}}, move[0], move[1]);

        for (int i = 0; i < 2; i++) {
            moves[i][0] = moves[i + 1][0];
            moves[i][1] = moves[i + 1][1];
        }
    } else {
        (*move_count)++;
    }

    
}

bool is_empty(uint8_t board[3][3], int row, int col) {
    return board[row][col] == 0;
}

void randomize(int indices[4]) {
    for (int i = 3; i >= 0; --i){
        //generate a random number [0, n-1]
        int j = rand() % 4;

        //swap the last element with element at random index
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }
}

void bot_move(uint8_t board[3][3], int *row, int *col, int b_r, int b_c, int moves_count, uint8_t move[2]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (b_c != -1 && b_r == i && b_c == j) {
                continue;
            }
            if (is_empty(board, i, j)) {

                board[i][j] = 2;
                if (gameEnded(board, 2)) {
                    //ZAPISE A VYHRA
                    *col = j;
                    *row = i;
                    return;
                }
                board[i][j] = 0;

            }
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (b_c != -1 && b_c == i && b_r == j) {
                continue;
            }
            if (is_empty(board, i, j)) {
                board[i][j] = 1;
                if (moves_count == 3) {
                    board[move[0]][move[1]] = 0;
                }
                if (gameEnded(board, 1)) {
                    //ZAPISE A BLOKUJE
                    board[i][j] = 2;
                    *col = j;
                    *row = i;
                    return;
                }
                board[i][j] = 0;
                if (moves_count == 3) {
                    board[move[0]][move[1]] = 1;
                }
            }
        }
    }

    if (!(b_c != -1 && b_c == 1 && b_r == 1) && is_empty(board, 1, 1)) {
        *col = 1;
        *row = 1;
        board[1][1] = 2;
        return;
    }

    int indices[4] = {0, 1, 2, 3};
    randomize(indices);

    int corners[4][2] = { {0,0}, {0,2}, {2,0}, {2,2} };
    for (int i = 0; i < 4; i++) {
        int c = corners[indices[i]][1];
        int r = corners[indices[i]][0];
        if (b_c != -1 && b_c == c && b_r == r) {
            continue;
        }
        if (is_empty(board, c, r)) {
            board[c][r] = 2;
            *col = c;
            *row = r;
            return;
        }
    }

    randomize(indices);

    int sides[4][2] = { {0,1}, {1,0}, {2,1}, {1,2} };
    for (int i = 0; i < 4; i++) {
        int c = sides[indices[i]][1];
        int r = sides[indices[i]][0];
        if (b_c != -1 && b_c == c && b_r == r) {
            continue;
        }
        if (is_empty(board, c, r)) {
            board[c][r] = 2;
            *col = c;
            *row = r;
            return;
        }
    }
}

void check_end(uint8_t board[3][3], Strip *s, int player, uint8_t *p1_count, 
    uint8_t *p2_count, uint8_t *banned_c, uint8_t *banned_r) {
    bool game_ended = gameEnded(board, player);
    if (game_ended || drawCheck(board)) {
        ending_animation(s, game_ended ? (player == 1 ? PLAYER_1_COLOR : PLAYER_2_COLOR) : DRAW_COLOR);
        
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                board[i][j] = 0;
            }
        }
        
        *p1_count = 0;
        *p2_count = 0;
        *banned_c = -1;
        *banned_r = -1;
    }
}