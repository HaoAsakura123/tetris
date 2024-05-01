#include "tetris_frontend.h"
#include "../../brick_game/tetris/tetris.h"

void print_stats(GameInfo_t* stats) {
  MVPRINTW(0, BOARD_M + BOARD_W, "High Score %d", stats->high_score);
  MVPRINTW(3, BOARD_M + BOARD_W, "Score %d", stats->score);
  MVPRINTW(6, BOARD_M + BOARD_W, "Level %d", stats->level);
  MVPRINTW(BOARD_W - 1, BOARD_M + BOARD_W, "Speed %d", stats->speed);
  print_next(stats);
}

void print_next(GameInfo_t* game) {
  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_WHITE, COLOR_BLACK);
  init_pair(5, COLOR_GREEN, COLOR_BLACK);
  init_pair(6, COLOR_BLUE, COLOR_BLACK);
  init_pair(7, COLOR_CYAN, COLOR_BLACK);
  MVPRINTW(12, BOARD_M + BOARD_W, " Next ");
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if ((game->next[i][j])) {
        attron(COLOR_PAIR(game->color));
        mvprintw(i + 15, j * 2 + 1 + BOARD_M + 15, "[]");
        attroff(COLOR_PAIR(game->color));
      } else
        mvprintw(i + 15, j * 2 + 1 + BOARD_M + 15, "  ");
    }
  }
}

void graphicsTetris(GameInfo_t* game, GameInfo_t* game1, int color) {
  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_WHITE, COLOR_BLACK);
  init_pair(5, COLOR_GREEN, COLOR_BLACK);
  init_pair(6, COLOR_BLUE, COLOR_BLACK);
  init_pair(7, COLOR_CYAN, COLOR_BLACK);
  clear(); 
  for (int i = 0; i < 24; ++i) {
    for (int j = 0; j < 11; ++j) {
      if (i == 0 || i == 1)
        mvprintw(i, j * 2, "--");
      else if (i == 22 || i == 23)
        mvprintw(i, j * 2, "--");
      else if (j == 0)
        mvprintw(i, j * 2, "||");
      else if (j == BOARD_W)
        mvprintw(i, j * 2, "||");
    }
  }
  for (int i = 0; i < BOARD_M; ++i) {
    for (int j = 0; j < BOARD_W; ++j) {
      if (game->field[i][j]) { 
        attron(COLOR_PAIR(1));
        mvprintw(i + 2, j * 2 + 1, "[]");
        attroff(COLOR_PAIR(1));
      } else if (game1->field[i][j]) {
        attron(COLOR_PAIR(game1->color));
        mvprintw(i + 2, j * 2 + 1, "[]");
        attroff(COLOR_PAIR(game1->color));
      } else
        mvprintw(i + 2, j * 2 + 1, "  ");
    }
  }
  print_stats(game);
  print_pos(game1);
}

void graphicsTetrisv2() {
  start_color();
  init_pair(5, COLOR_GREEN, COLOR_BLACK);
  attron(COLOR_PAIR(5));
  mvprintw(11, 7, "Y");
  mvprintw(11, BOARD_W - 1, "O");
  mvprintw(11, 11, "U");
  mvprintw(13, 7, "L");
  mvprintw(13, BOARD_W - 1, "O");
  mvprintw(13, 11, "S");
  mvprintw(13, 13, "E");
  attroff(COLOR_PAIR(5));
}

void print_pos(GameInfo_t* game) {
  mvprintw(BOARD_M, 30, "%d", game->transpose.left_up_x);
  mvprintw(BOARD_M, 34, "%d", game->transpose.left_up_y);
  mvprintw(22, 30, "%d", game->transpose.right_down_x);
  mvprintw(22, 34, "%d", game->transpose.right_down_y);
  mvprintw(24, 34, "%d", game->transpose.type_figure);
  mvprintw(24, 30, "%d", game->transpose.type_rotate);
}
