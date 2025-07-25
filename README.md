# TicTacToeGamepad

## TBD 
Test new version of code, assemble the integrated arduino version, upload edited case

## 1 Introduction
This project is an implementation of a Tic Tac Toe game using both hardware and software. The
purpose of the project was to practice working with an 8-bit mcu on register level, but I worked on it more and created a standalone version.
## 2 Materials and technologies
* MCU (I used Avr curiosity nano for school and an Arduino pro micro for standalone)  
* 9x Keyboard switches  
* 9x SK6812MINI-E LEDs  
* 3x 100uF capacitors  
* 1x 330r resistor  
  
The game itself is played on a 3x3 playing field made from keyboard switches, which are soldered onto
a PCB I designed. The PCB sits in a 3D printed case.

## 2 Functions
The game uses addressable LEDs to show the board state. Each player has their own color, which is
shown on the fields the player has occupied. After the game is over, whether it was a draw or a player
won, there is an animation and 5 second delay, after which the game restarts. The first player to act
is decided by which player won the last game, or which player went last. There are 2 possible game
modes:  

* Classic  
* 3-mark rule  

Both game modes can be played in single player, which replaces the second player with a bot.
### 2.1 Classic
This game mode uses the classic rules of a 3x3 tic tac toe game. The player wins by putting 3 marks
in a row, column or diagonal. After the board is filled and no player has won, the game is declared as
a draw.
1
### 2.2 3-mark rule
Here each player only gets 3 marks. After that they must move their oldest mark to a new position.
In this game mode there is no draw, a player must win to end the game. The moving mark is shown
by being dimmer than the rest.
### 2.3 Bot
The bot uses a simple algorithm to decide which space it wants to occupy.
1. Checks if a win is possible, then it places the mark there.  
2. Checks if the player can win, then it blocks the player.  
3. If the center is free, it puts the mark there.  
4. Checks every corner in a random order and occupies it if possible.  
5. Checks every side field in a random order and occupies it if possible.

In the 3-mark rule, when checking if the player can win, it also remembers which mark the player is
moving. This ensures that the bot blocks the correct field.
### 3.4 Ending animation
After the game ends, either by a player winning or a draw, there is an animation played. The animation
lights up the LEDs in a “2” shape with the color of the winning player, or in the case of a draw with
the “draw” color.
## 3 Costumizations
There are a few customizations in the game. First, the game mode and if the bot is active can be
set. The colors of each player and draw are also customizable, using RGB format. The dimness of the
moving mark is also possible to change.
To change these settings these macros can be modified in the “config.h” file:

* GAME_MODE – 1 (3-mark rule) or 0 (Classic).  
* BOT – 1 (Bot is turned on) or 0 (Bot is off).  
* PLAYER_N_COLOR and DRAW_COLOR – has this syntax (Color) R, G, B, you replace the R, G, B
with the corresponding colors of the value. Player colors and draw color all have to be different
otherwise an error occurs.  
* DIM_DIV – the amount by which the values of the color are divided, e.g. DIM_DIV 16 makes the
LEDs be 1
16 of their brightness.  

If there were changes to any of the settings the game can be simply re-flashed onto the mcu by using
the Makefile in the project folder.
### 3.1 Error
If an error occurs, the board starts blinking red. In case this happens check your color values, because
some of them have the same values.
## Schematic and PCB
![image](https://github.com/user-attachments/assets/093761e4-1026-42ff-8dac-c76c713c707c)
![image](https://github.com/user-attachments/assets/21b14925-c09e-4a63-81e3-8c0233fa0579)
The gerber files for the PCB are present in the repository.
## PINOUT
 ![20250605_113647](https://github.com/user-attachments/assets/8168c5ad-e2a1-4dab-aa40-1cf3175d9f8d)



