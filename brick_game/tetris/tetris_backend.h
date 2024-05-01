#ifndef TETRIS_BACKEND_H
#define TETRIS_BACKEND_H

#include "tetris.h"


GameInfo_t updateCurrentState();

void createField(GameInfo_t* game);
void deleteField(GameInfo_t* game);
void writeNumberToFile(int number, const char* filename);
void randGen(GameInfo_t* game);
void cleanGen(GameInfo_t* game);
void logicTetris(GameInfo_t* game, GameInfo_t* game1);
void rewrite(GameInfo_t* game, GameInfo_t* game1);
void performAction(int action, GameInfo_t* game, GameInfo_t* game1, bool* hold);
void shiftTotheLeft(GameInfo_t* game, GameInfo_t* game1);
void shiftTotheRight(GameInfo_t* game, GameInfo_t* game1);
void shift_down(GameInfo_t* game, GameInfo_t* game1);
void shift_left(GameInfo_t* game);
void shift_right(GameInfo_t* game);
void active_game(GameInfo_t* game, GameInfo_t* game1);
void check_field(GameInfo_t* game);
void decrease_field(GameInfo_t* game, int k);


int readIntFromFile(const char* filename);
int reflection(GameInfo_t* game, GameInfo_t* game1, int* color);
int checkSpawn(GameInfo_t* game, GameInfo_t* game1);
int checkLine(GameInfo_t* game, int ind);
int collision(GameInfo_t* game, GameInfo_t* game1);
int random_number(int minimum, int maximum);
int checkLine_LR(GameInfo_t* game, int ind);
int collision_left(GameInfo_t* game, GameInfo_t* game1);
int collision_right(GameInfo_t* game, GameInfo_t* game1);

void rotate(GameInfo_t* game, GameInfo_t* game1);

int can_rotate_zero(GameInfo_t* game, GameInfo_t* game1);
void rotate_zero(GameInfo_t* game);

int can_rotate_one(GameInfo_t* game, GameInfo_t* game1);
void rotate_one(GameInfo_t* game);

int can_rotate_two(GameInfo_t* game, GameInfo_t* game1);
void rotate_two(GameInfo_t* game);

int can_rotate_four(GameInfo_t* game, GameInfo_t* game1);
void rotate_four(GameInfo_t* game);

int can_rotate_five(GameInfo_t* game, GameInfo_t* game1);
void rotate_five(GameInfo_t* game);

int can_rotate_six(GameInfo_t* game, GameInfo_t* game1);
void rotate_six(GameInfo_t* game);

void inf_loop();

// у нас пустое поле на него я отражаю след фигуру, чел нажимает старт, потом
// начинается процесс сдвига фигур вниз, пока не соприкосновение, как только
// соприкасаются, проверка на стирание строки, потом переприсвоение элементу i
// элемента i-1, потом снова создается фигура и начинается отражение

#endif