#include <Adafruit_NeoPixel.h> // Include the NeoPixel library

#define LED_PIN    4    // Digital pin connected to the NeoPixel data input
#define NUM_LEDS   9    // Number of NeoPixels in your 3x3 grid
#define ANIM_SPEED 200

Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

uint8_t GAME_MODE;
uint8_t BOT;
uint8_t PLAYER_1_COLOR[3] = {255, 0, 247};
uint8_t PLAYER_2_COLOR[3] = {0, 255, 8};
uint8_t DRAW_COLOR[3] = {255, 0, 0};
#define DIM_DIV 16 //Sets the brightness of the dim space (only GAMEMODE 1),
                   //Decides by what number will the values of color be dived by

const int rowPins[] = {8, 7, 6}; // Row 0 (top) -> A3, Row 1 (middle) -> A2, Row 2 (bottom) -> A1

const int colPins[] = {14, 16, 10}; // Col 0 (left) -> Pin 15, Col 1 (middle) -> Pin 16, Col 2 (right) -> Pin 14

int keypadToNeoPixelIndex[3][3] = {
  {0, 1, 2}, // If Key '1' -> LED 0, '2' -> LED 1, '3' -> LED 2
  {5, 4, 3}, // If Key '4' -> LED 3, '5' -> LED 4, '6' -> LED 5
  {6, 7, 8}  // If Key '7' -> LED 6, '8' -> LED 7, '9' -> LED 8
};

uint8_t board[3][3] = {{0}};
    
uint8_t player1_moves[3][2] = {{255}};
uint8_t player2_moves[3][2] = {{255}};
uint8_t p1_count = 0;
uint8_t p2_count = 0;

uint8_t turnCounter = 0;
uint8_t row = 0;
uint8_t col = 0;
uint8_t player;
uint8_t banned_c = -1;
uint8_t banned_r = -1;


void setup() {
  pinMode(A2, INPUT_PULLUP);
  pinMode(A0, INPUT_PULLUP);
  
  
  GAME_MODE = digitalRead(A0) == LOW;  //OR put 0, for traditional tic-tac-toe
  BOT = digitalRead(A2) == LOW;
  
  Serial.begin(9600);
  
  // Initialize NeoPixel strip
  pixels.begin();         // Initialize the NeoPixel library
  pixels.clear();         // Turn off all NeoPixels initially
  pixels.show();          // Send the updated pixel colors to the hardware.

  // Set row pins as outputs and ensure they are all LOW initially (inactive)
  for (int i = 0; i < 3; i++) {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], HIGH); // All rows are inactive (LOW) by default
  }

  // Set column pins as inputs and enable internal pull-up resistors
  for (int i = 0; i < 3; i++) {
    pinMode(colPins[i], INPUT_PULLUP);
  }
}

void loop() {
  Serial.println("BOT AND GAMEMODE");
  Serial.print(BOT);
  Serial.print(" ");
  Serial.println(GAME_MODE);
  printBoard(board);
  player = turnCounter % 2 == 0 ? 1 : 2;
  uint8_t (*moves)[2] = player == 1 ? player1_moves : player2_moves;
  uint8_t *move_count = player == 1 ? &p1_count : &p2_count;
  uint8_t *color = player == 1 ? PLAYER_1_COLOR : PLAYER_2_COLOR;

  if (GAME_MODE) {
      if (*move_count == 3) {
          pixels.setPixelColor(keypadToNeoPixelIndex[moves[0][0]][moves[0][1]], pixels.Color(color[0] / DIM_DIV, color[1] / DIM_DIV, color[2] / DIM_DIV));
          pixels.show();
      }
  }
  
  
  while (!scanKey(&row, &col)) {}
  if (board[row][col] != 0) {
      return;
  }

  if (GAME_MODE) {
      remove_old(board, moves, move_count);
      moves[*move_count - 1][0] = row;
      moves[*move_count - 1][1] = col;
  }
  
  board[row][col] = player;
  pixels.setPixelColor(keypadToNeoPixelIndex[row][col], pixels.Color(color[0], color[1], color[2]));
  pixels.show();
  
  check_end(board, player, &p1_count, &p2_count, &banned_c, &banned_r);
  
  turnCounter++;

  if (!BOT) {
      return;
  }

  delay(500);
  player = 2;
  color = PLAYER_2_COLOR;
  moves = player2_moves;
  move_count = &p2_count;

  int bot_c, bot_r;
  if (GAME_MODE) {
      if (*move_count == 3) {
          banned_c = moves[0][1];
          banned_r = moves[0][0];

          pixels.setPixelColor(keypadToNeoPixelIndex[moves[0][0]][moves[0][1]], pixels.Color(color[0] / DIM_DIV, color[1] / DIM_DIV, color[2] / DIM_DIV));
          pixels.show();
          _delay_ms(1000);
      }
      remove_old(board, moves, move_count);
  }

  bot_move(board, &bot_r, &bot_c, banned_r, banned_c, p1_count, player1_moves[0]);
  Serial.print("Bot moved to ");
  Serial.print(bot_r);
  Serial.print(" ");
  Serial.println(bot_c);
  if (GAME_MODE) {
      moves[*move_count - 1][0] = bot_r;
      moves[*move_count - 1][1] = bot_c;
  }

  pixels.setPixelColor(keypadToNeoPixelIndex[bot_r][bot_c], pixels.Color(color[0], color[1], color[2]));
  pixels.show();

  check_end(board, player, &p1_count, &p2_count, &banned_c, &banned_r);

  turnCounter++;
}

bool prev_states[3][3] = {{true}};

bool gameEnded(uint8_t board[3][3], int currPlayer) {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] == currPlayer){
            Serial.println("End by ROW");
            return true;
        }
    }
    
    for (int i = 0; i < 3; i++) {
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] == currPlayer){
            Serial.println("End by COL");
            return true;
        }
    }
    
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] == currPlayer) {
        Serial.println("End by DIAG");
        return true;
    }
    
    if (board[2][0] == board[1][1] && board[1][1] == board[0][2] && board[2][0] == currPlayer) {
        Serial.println("End by SEC_DIAG");
        return true;
    }
    
    return false;
}

bool scanKey(uint8_t* row, uint8_t* col) {
    bool keyPressed = false;
    for (int r = 0; r < 3; r++) {
        digitalWrite(rowPins[r], LOW);
        _delay_ms(5);

        for(int c = 0; c < 3; c++) {
            bool pressed = !(digitalRead(colPins[c]));
                
            if (pressed && !prev_states[r][c]) {
                *col = c;
                *row = r;
                Serial.print("found ");
                Serial.print(c);
                Serial.print(" ");
                Serial.println(r);
                keyPressed = true;
            }
            
            prev_states[r][c] = pressed;
        }

        digitalWrite(rowPins[r], HIGH);
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

void remove_old(uint8_t board[3][3], uint8_t moves[3][2], uint8_t *move_count) {
    if (*move_count == 3) {
        uint8_t *move = moves[0];

        board[move[0]][move[1]] = 0;

        uint8_t led = move[0] * 3 + move[1];
        if (move[0] == 1) {
            led = 3 + (2 - move[1]);
        }
        pixels.setPixelColor(keypadToNeoPixelIndex[move[0]][move[1]], pixels.Color(0, 0, 0));

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
            if (b_c != -1 && b_r == i && b_c == j) {
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
        if (is_empty(board, r, c)) {
            Serial.print(c);
            Serial.println(r);
            board[r][c] = 2;
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
        if (is_empty(board, r, c)) {
            board[r][c] = 2;
            *col = c;
            *row = r;
            return;
        }
    }
}

void check_end(uint8_t board[3][3], int player, uint8_t *p1_count, 
    uint8_t *p2_count, uint8_t *banned_c, uint8_t *banned_r) {
    bool game_ended = gameEnded(board, player);
    if (game_ended || drawCheck(board)) {
        ending_animation(game_ended ? (player == 1 ? PLAYER_1_COLOR : PLAYER_2_COLOR) : DRAW_COLOR);
        
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

void ending_animation(uint8_t* rgb) {

    delay(700);
    pixels.clear();
    pixels.show();
    delay(ANIM_SPEED);
    for (int i = 0; i < 9; i++) {
        pixels.setPixelColor(i, pixels.Color(rgb[0], rgb[1], rgb[2]));
        pixels.show();
        delay(ANIM_SPEED);
    }

    for (int i = 0; i < 3; i++) {
        delay(400);
        pixels.clear();
        pixels.show();
        delay(400);
        for (int j = 0; j < 9; j++) {
            pixels.setPixelColor(j, pixels.Color(rgb[0], rgb[1], rgb[2]));
        }
        pixels.show();
    }

    delay(5000);
    pixels.clear();
    pixels.show();
}
//
//bool check_color(Color color1, Color color2) {
//    return color1.rgb[0] == color2.rgb[0] &&
//            color1.rgb[1] == color2.rgb[1] &&
//            color1.rgb[2] == color2.rgb[2];
//}

void error_anim() {
    while (1) {
        for (int j = 0; j < 9; j++) {
            pixels.setPixelColor(j, pixels.Color(255, 0, 0));
        }
        pixels.show();
        delay(400);
        pixels.clear();
        pixels.show();
        delay(400);
    }
}

void printBoard(uint8_t board[3][3]) {
  Serial.println("\nCurrent Board:");
  Serial.println("-------------");
  
  for (int row = 0; row < 3; row++) {
    Serial.print("| ");
    for (int col = 0; col < 3; col++) {
      switch (board[row][col]) {
        case 1: Serial.print("X"); break;
        case 2: Serial.print("O"); break;
        default: Serial.print(" "); break;
      }
      Serial.print(" | ");
    }
    Serial.println("\n-------------");
  }
}
