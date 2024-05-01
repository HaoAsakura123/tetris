#ifndef TETRIS_FRONTEND_H
#define TETRIS_FRONTEND_H

#include "../../brick_game/tetris/tetris.h"

void print_stats(GameInfo_t* stats);
void print_next(GameInfo_t* game);
void graphicsTetris(GameInfo_t* game, GameInfo_t* game1, int color);
void print_stats(GameInfo_t* stats);
void graphicsTetrisv2();
void print_pos(GameInfo_t* game);

#endif