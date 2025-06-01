#ifndef CONFIG
#define CONFIG

#define GAME_MODE 1 //OR put 0, for traditional tic-tac-toe
#define BOT 1
#define PLAYER_1_COLOR (Color) {{255, 0, 247}}
#define PLAYER_2_COLOR (Color) {{0, 255, 8}}
#define DRAW_COLOR (Color) {{255, 0, 0}}
#define DIM_DIV 16 //Sets the brightness of the dim space (only GAMEMODE 1),
                   //Decides by what number will the values of color be dived by

#endif