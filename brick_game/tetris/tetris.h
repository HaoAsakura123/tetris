#ifndef TETRIS_H
#define TETRIS_H


#include <math.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tetris_backend.h"
#include "../../gui/cli/tetris_frontend.h"

#define MVPRINTW(y, x, ...) \
  mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__)
#define BOARD_M 20
#define BOARD_W 10
#define BOARDS_BEGIN 2

typedef enum {
  Pause = 112,
  Terminate = 113, 
  Action,
  Start = 115,
  Left,  
  Right,  
  Up,     
  Down    
} UserAction_t;

typedef struct {
  int right;
  int left;
  int left_up_x;
  int left_up_y;
  int right_down_x;
  int right_down_y;
  int type_figure;
  int type_rotate;
} PosFigure_t;

typedef struct {
  int** field;
  int** next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
  int terminate;
  int color;
  PosFigure_t transpose;
} GameInfo_t;


#endif