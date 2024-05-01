#include "tetris.h"
#include "tetris_backend.h"

void createField(GameInfo_t* game) {
  game->field = (int**)malloc(21 * sizeof(int*));
  for (int i = 0; i < 21; i++) {
    game->field[i] = (int*)malloc(BOARD_W * sizeof(int));
  }
}

void deleteField(GameInfo_t* game) {
  for (int i = 0; i < 21; i++) {
    free(game->field[i]);
  }
  free(game->field);
}


int readIntFromFile(const char* filename) {
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    return 0;
  }
  int value;
  if (fscanf(file, "%d", &value) != 1) {
    fprintf(stderr, "Error reading integer from file\n");
    fclose(file);
    return -1;
  }
  fclose(file);
  return value;
}


void writeNumberToFile(int number, const char* filename) {
  FILE* file = fopen(filename, "w");

  if (file == NULL) {
    printf("Ошибка при открытии файла.\n");
    return;
  }

  fprintf(file, "%d", number);
  fclose(file);
}


void randGen(GameInfo_t* game) {
  game->next = (int**)malloc(4 * sizeof(int*));
  for (int i = 0; i < 4; i++) game->next[i] = (int*)malloc(4 * sizeof(int));
  time_t t;
  srand((unsigned)time(&t));
  int secret = random_number(0, 100) % 7;
  switch (secret) {
    case 0:
      game->color = 1;
      game->transpose.type_figure = 0;
      for (int i = 0; i < 4; i++) game->next[0][i] = 1;
      break;
    case 1:
      game->color = 2;
      game->transpose.type_figure = 1;
      game->next[0][0] = 1;
      for (int i = 0; i < 3; i++) game->next[1][i] = 1;
      break;
    case 2:
      game->color = 3;
      game->transpose.type_figure = 2;
      game->next[0][2] = 1;
      for (int i = 0; i < 3; i++) game->next[1][i] = 1;
      break;
    case 3:
      game->color = 4;
      game->transpose.type_figure = 3;
      for (int i = 1; i < 3; i++) game->next[0][i] = 1;
      for (int i = 1; i < 3; i++) game->next[1][i] = 1;
      break;
    case 4:
      game->color = 5;
      game->transpose.type_figure = 4;
      for (int i = 1; i < 3; i++) game->next[0][i] = 1;
      for (int i = 0; i < 2; i++) game->next[1][i] = 1;
      break;
    case 5:
      game->color = 6;
      game->transpose.type_figure = 5;
      game->next[0][1] = 1;
      for (int i = 0; i < 3; i++) game->next[1][i] = 1;
      break;
    case 6:
      game->color = 7;
      game->transpose.type_figure = 6;
      for (int i = 0; i < 2; i++) game->next[0][i] = 1;
      for (int i = 1; i < 3; i++) game->next[1][i] = 1;
      break;
    default:
      break;
  }
}

void cleanGen(GameInfo_t* game) {
  for (int i = 0; i < 4; i++) free(game->next[i]);
  free(game->next);
}

GameInfo_t updateCurrentState() {
  GameInfo_t game;
  game.level = 1;
  game.score = 0;
  game.speed = 1;
  game.pause = 1;
  game.terminate = 0;
  createField(&game);
  for (int i = 0; i < BOARD_M; i++) {
    for (int j = 0; j < BOARD_W; j++) {
      game.field[i][j] = 0;
    }
  }
  const char* filename = "number.txt";
  game.high_score = readIntFromFile(filename);
  game.score = 0;
  writeNumberToFile(game.high_score, filename);
  randGen(&game);
  return game;
}


int reflection(GameInfo_t* game, GameInfo_t* game1, int* color) {
  int stop = 1;
  for (int i = 0; i < 2; i++) {
    for (int j = 3; j < 7; j++) {
      if (game->field[i][j] && game->next[i][j - 3]) stop = 0;
    }
  }
  if (stop) {
    for (int i = 0; i < 2; i++) {
      for (int j = 3; j < 7; j++) {
        if (game->next[i][j - 3]) game1->field[i][j] = game->next[i][j - 3];
      }
    }
  }
  game1->transpose.left_up_x = 3;
  game1->transpose.left_up_y = 0;
  game1->transpose.right_down_x = 6;
  game1->transpose.right_down_y = 3;
  game1->transpose.type_figure = game->transpose.type_figure;
  game1->transpose.type_rotate = 0;
  *color = game->color;
  game1->color = game->color;
  cleanGen(game);
  randGen(game);
  graphicsTetris(game, game1, 1);
  return stop;
}

void logicTetris(GameInfo_t* game, GameInfo_t* game1) {
  int ch;
  int massive[BOARD_W];
  for (int i = 19; i > 0; i--) {
    for (int j = 0; j < BOARD_W; j++) {
      massive[j] = game1->field[i - 1][j];
    }
    for (int j = 0; j < BOARD_W; j++) {
      game1->field[i][j] = massive[j];
    }
    if (i == 1)
      for (int j = 0; j < BOARD_W; j++) game1->field[0][j] = 0;
  }
  game1->transpose.left_up_y++;
  game1->transpose.right_down_y++;
  graphicsTetris(game, game1, 1);
}

int checkSpawn(GameInfo_t* game, GameInfo_t* game1) {
  int stop = 1;
  for (int i = 0; i < 4; i++) {
    for (int j = 3; j < 7; j++) {
      if (game->next[i][j - 3] && game->field[i][j]) stop = 0;
    }
  }
  for (int i = 0; stop && i < 4; i++) {
    for (int j = 3; j < 7; j++) {
      if (game->next[i][j - 3]) game1->field[i][j] = game->next[i][j - 3];
    }
  }
  return stop;
}


void rewrite(GameInfo_t* game, GameInfo_t* game1) {
  for (int i = 0; i < BOARD_M; i++) {
    for (int j = 0; j < BOARD_W; j++) {
      if (game1->field[i][j]) game->field[i][j] = 1;
      game1->field[i][j] = 0;
    }
  }
}

int collision(GameInfo_t* game, GameInfo_t* game1) {
  int stop = 0;
  for (int i = 0; i < BOARD_M; i++) {
    for (int j = 0; j < BOARD_W; j++) {
      if (game1->field[i][j] && game->field[i + 1][j] || checkLine(game1, 19))
        stop = 1;
    }
  }

  return stop;
}

int checkLine(GameInfo_t* game, int ind) {
  int stop = 0;

  for (int i = 0; i < BOARD_W; i++) {
    if (game->field[ind][i]) stop = 1;
  }

  return stop;
}

int random_number(int minimum, int maximum) {
  // Установка seed для генератора случайных чисел
  srand(time(NULL));
  // Генерация случайного числа в заданном диапазоне
  return minimum + rand() % (maximum - minimum + 1);
}

int checkLine_LR(GameInfo_t* game, int ind) {
  int stop = 0;

  for (int i = 0; i < BOARD_M; i++) {
    if (game->field[i][ind]) stop = 1;
  }

  return stop;
}

void shift_right(GameInfo_t* game) {
  for (int i = 0; i < BOARD_M; i++) {
    for (int j = BOARD_W - 1; j > 0; j--) {
      game->field[i][j] = game->field[i][j - 1];
    }
  }
  for (int i = 0; i < BOARD_M; i++) game->field[i][0] = 0;
}

void shift_left(GameInfo_t* game) {
  for (int i = 0; i < BOARD_M; i++) {
    for (int j = 0; j < BOARD_W - 1; j++) {
      game->field[i][j] = game->field[i][j + 1];
    }
  }
  for (int i = 0; i < BOARD_M; i++) game->field[i][BOARD_W - 1] = 0;
}

void performAction(int action, GameInfo_t* game, GameInfo_t* game1,
                   bool* hold) {
  switch (action) {
    case Start:
      game->pause = 0;
      *hold = true;
      break;
    case Pause:
      game->pause = 1;
      *hold = true;
      break;
    case Terminate:
      game->terminate = 1;
      *hold = true;
      break;
    case KEY_LEFT:
      shiftTotheLeft(game, game1);
      *hold = true;
      break;
    case KEY_RIGHT:
      shiftTotheRight(game, game1);
      *hold = true;
      break;
    case KEY_DOWN:
      shift_down(game, game1);
      *hold = true;
      break;
    case Action:
      rotate(game, game1);
      *hold = true;
      break;
    default:
      *hold = false;
      break;
  }
}

void shift_down(GameInfo_t* game, GameInfo_t* game1) {
  if (!collision(game, game1)) {
    logicTetris(game, game1); 
  }
}

void shiftTotheRight(GameInfo_t* game, GameInfo_t* game1) {
  if (!collision_right(game, game1)) {
    shift_right(game1);
    game1->transpose.left_up_x++;
    game1->transpose.right_down_x++;
  }
}

void shiftTotheLeft(GameInfo_t* game, GameInfo_t* game1) {
  if (!collision_left(game, game1)) {
    shift_left(game1);
    game1->transpose.left_up_x--;
    game1->transpose.right_down_x--;
  }
}

int collision_left(GameInfo_t* game, GameInfo_t* game1) {
  int stop = 0;
  for (int i = 0; i < BOARD_M; i++) {
    for (int j = 1; j < BOARD_W - 1; j++) {
      if (game1->field[i][j] && game->field[i][j - 1] || checkLine_LR(game1, 0))
        stop = 1;
    }
  }
  return stop;
}

int collision_right(GameInfo_t* game, GameInfo_t* game1) {
  int stop = 0;
  for (int i = 0; i < BOARD_M; i++) {
    for (int j = 0; j < 8; j++) {
      if (game1->field[i][j] && game->field[i][j + 1] || checkLine_LR(game1, BOARD_W - 1))
        stop = 1;
    }
  }

  return stop;
}


void active_game(GameInfo_t* game, GameInfo_t* game1) {
  int ch;
  int stop;
  bool hold;
  while (game->terminate != 1 && game->pause != 2) {
    if (game->pause == 0) {
      graphicsTetris(game, game1, 1);
      if (collision(game, game1)) {
        break;
      } else
        logicTetris(game, game1);
    }
    ch = getch();
    performAction(ch, game, game1, &hold);
  }
}

void rotate(GameInfo_t* game, GameInfo_t* game1) {
  switch (game1->transpose.type_figure) {
    case 0:
      if (can_rotate_zero(game, game1)) rotate_zero(game1);
      break;
    case 1:
      if (can_rotate_one(game, game1)) rotate_one(game1);
      break;
    case 2:
      if (can_rotate_two(game, game1)) rotate_two(game1);
      break;
    case 4:
      if (can_rotate_four(game, game1)) rotate_four(game1);
      break;
    case 5:
      if (can_rotate_five(game, game1)) rotate_five(game1);
      break;
    case 6:
      if (can_rotate_six(game, game1)) rotate_six(game1);
      break;
    default:
      break;
  }
}

void rotate_zero(GameInfo_t* game) {
  switch (game->transpose.type_rotate) {
    case 0:
      for (int i = game->transpose.left_up_y; i <= game->transpose.right_down_y;
           i++)
        game->field[i][game->transpose.left_up_x + 1] = 0;

      for (int i = game->transpose.left_up_x; i <= game->transpose.right_down_x;
           i++)
        game->field[game->transpose.left_up_y][i] = 1;
      break;
    case 1:
      for (int i = game->transpose.left_up_x; i <= game->transpose.right_down_x;
           i++)
        game->field[game->transpose.left_up_y][i] = 0;
      for (int i = game->transpose.left_up_y; i <= game->transpose.right_down_y;
           i++)
        game->field[i][game->transpose.left_up_x + 1] = 1;
      break;
    default:
      break;
  }
}




int can_rotate_zero(GameInfo_t* game, GameInfo_t* game1) {
  int rotate = 1;
  if (game1->transpose.type_rotate == 0) {
    if (game1->transpose.right_down_y > 19)
      rotate = 0;
    else
      for (int i = game1->transpose.left_up_x;
           i <= game1->transpose.right_down_x; i++)
        if (game->field[game1->transpose.left_up_y][i] == 1) rotate = 0;
  } else if (game1->transpose.type_rotate == 1) {
    if (game1->transpose.left_up_x < 0 || game1->transpose.left_up_x >= 7)
      rotate = 0;
    else
      for (int i = game1->transpose.left_up_y;
           i <= game1->transpose.right_down_y; i++)
        if (game->field[i][game1->transpose.left_up_x + 1] == 1) rotate = 0;
  }
  if (rotate)
    game1->transpose.type_rotate = (game1->transpose.type_rotate + 1) % 2;
  return rotate;
}

void rotate_one(GameInfo_t* game) {
  switch (game->transpose.type_rotate) {
    case 0:
      game->field[game->transpose.left_up_y + 2][game->transpose.left_up_x] = 0;
      for (int i = game->transpose.left_up_y; i < game->transpose.left_up_y + 3;
           i++)
        game->field[i][game->transpose.left_up_x + 1] = 0;
      game->field[game->transpose.left_up_y][game->transpose.left_up_x] = 1;
      for (int i = game->transpose.left_up_x; i < game->transpose.left_up_x + 3;
           i++)
        game->field[game->transpose.left_up_y + 1][i] = 1;
      break;
    case 1:
      game->field[game->transpose.left_up_y][game->transpose.left_up_x] = 0;
      for (int i = game->transpose.left_up_x; i < game->transpose.left_up_x + 3;
           i++)
        game->field[game->transpose.left_up_y + 1][i] = 0;
      game->field[game->transpose.left_up_y][game->transpose.left_up_x + 2] = 1;
      for (int i = game->transpose.left_up_y; i < game->transpose.left_up_y + 3;
           i++)
        game->field[i][game->transpose.left_up_x + 1] = 1;
      break;
    case 2:
      game->field[game->transpose.left_up_y][game->transpose.left_up_x + 2] = 0;
      for (int i = game->transpose.left_up_y; i < game->transpose.left_up_y + 3;
           i++)
        game->field[i][game->transpose.left_up_x + 1] = 0;
      game->field[game->transpose.left_up_y + 2]
                 [game->transpose.left_up_x + 2] = 1;
      for (int i = game->transpose.left_up_x; i < game->transpose.left_up_x + 3;
           i++)
        game->field[game->transpose.left_up_y + 1][i] = 1;
      break;
    case 3:
      game->field[game->transpose.left_up_y + 2]
                 [game->transpose.left_up_x + 2] = 0;
      for (int i = game->transpose.left_up_x; i < game->transpose.left_up_x + 3;
           i++)
        game->field[game->transpose.left_up_y + 1][i] = 0;
      game->field[game->transpose.left_up_y + 2][game->transpose.left_up_x] = 1;
      for (int i = game->transpose.left_up_y; i < game->transpose.left_up_y + 3;
           i++)
        game->field[i][game->transpose.left_up_x + 1] = 1;
      break;
    default:
      break;
  }
}

int can_rotate_one(GameInfo_t* game, GameInfo_t* game1) {
  int rotate = 1;
  if (game1->transpose.type_rotate == 0) {
    if (game1->transpose.right_down_y > 19)
      rotate = 0;
    else {
      for (int i = game->transpose.left_up_y; i < game->transpose.left_up_y + 3;
           i++)
        if (game->field[i][game1->transpose.left_up_x + 1] == 1) rotate = 0;
      if (game->field[game1->transpose.left_up_y]
                     [game1->transpose.left_up_x + 2] == 1)
        rotate = 0;
    }
  } else if (game1->transpose.type_rotate == 1) {
    if (game1->transpose.left_up_x < 0)
      rotate = 0;
    else {
      for (int i = game1->transpose.left_up_x;
           i < game1->transpose.left_up_x + 3; i++)
        if (game->field[game1->transpose.left_up_y + 1][i] == 1) rotate = 0;
      if (game->field[game->transpose.left_up_y + 2]
                     [game->transpose.left_up_x + 2] == 1)
        rotate = 0;
    }
  } else if (game1->transpose.type_rotate == 2) {
    if (game1->transpose.right_down_y > 19)
      rotate = 0;
    else {
      for (int i = game->transpose.left_up_y; i < game->transpose.left_up_y + 3;
           i++)
        if (game->field[i][game->transpose.left_up_x + 1] == 1) rotate = 0;
      if (game->field[game->transpose.left_up_y + 2]
                     [game->transpose.left_up_x] == 1)
        rotate = 0;
    }
  } else if (game1->transpose.type_rotate == 3) {
    if (game1->transpose.left_up_x > 7)
      rotate = 0;
    else {
      for (int i = game->transpose.left_up_x; i < game->transpose.left_up_x + 3;
           i++)
        if (game->field[game1->transpose.left_up_y + 1][i] == 1) rotate = 0;
      if (game->field[game->transpose.left_up_y][game->transpose.left_up_x] ==
          1)
        rotate = 0;
    }
  }
  if (rotate)
    game1->transpose.type_rotate = (game1->transpose.type_rotate + 1) % 4;
  return rotate;
}

void rotate_two(GameInfo_t* game) {
  switch (game->transpose.type_rotate) {
    case 0:
      game->field[game->transpose.left_up_y][game->transpose.left_up_x] = 0;
      for (int i = game->transpose.left_up_y; i < game->transpose.left_up_y + 3;
           i++)
        game->field[i][game->transpose.left_up_x + 1] = 0;
      game->field[game->transpose.left_up_y][game->transpose.left_up_x + 2] = 1;
      for (int i = game->transpose.left_up_x; i < game->transpose.left_up_x + 3;
           i++)
        game->field[game->transpose.left_up_y + 1][i] = 1;
      break;
    case 1:
      game->field[game->transpose.left_up_y][game->transpose.left_up_x + 2] = 0;
      for (int i = game->transpose.left_up_x; i < game->transpose.left_up_x + 3;
           i++)
        game->field[game->transpose.left_up_y + 1][i] = 0;
      game->field[game->transpose.left_up_y + 2]
                 [game->transpose.left_up_x + 2] = 1;
      for (int i = game->transpose.left_up_y; i < game->transpose.left_up_y + 3;
           i++)
        game->field[i][game->transpose.left_up_x + 1] = 1;
      break;
    case 2:
      game->field[game->transpose.left_up_y + 2]
                 [game->transpose.left_up_x + 2] = 0;
      for (int i = game->transpose.left_up_y; i < game->transpose.left_up_y + 3;
           i++)
        game->field[i][game->transpose.left_up_x + 1] = 0;
      game->field[game->transpose.left_up_y + 2][game->transpose.left_up_x] = 1;
      for (int i = game->transpose.left_up_x; i < game->transpose.left_up_x + 3;
           i++)
        game->field[game->transpose.left_up_y + 1][i] = 1;
      break;
    case 3:
      game->field[game->transpose.left_up_y + 2][game->transpose.left_up_x] = 0;
      for (int i = game->transpose.left_up_x; i < game->transpose.left_up_x + 3;
           i++)
        game->field[game->transpose.left_up_y + 1][i] = 0;
      game->field[game->transpose.left_up_y][game->transpose.left_up_x] = 1;
      for (int i = game->transpose.left_up_y; i < game->transpose.left_up_y + 3;
           i++)
        game->field[i][game->transpose.left_up_x + 1] = 1;
      break;
    default:
      break;
  }
}

int can_rotate_two(GameInfo_t* game, GameInfo_t* game1) {
  int rotate = 1;
  if (game1->transpose.type_rotate == 0) {
    if (game1->transpose.right_down_y > 19)
      rotate = 0;
    else {
      for (int i = game->transpose.left_up_y; i < game->transpose.left_up_y + 3;
           i++)
        if (game->field[i][game1->transpose.left_up_x + 1] == 1) rotate = 0;
      if (game->field[game1->transpose.left_up_y + 2]
                     [game1->transpose.left_up_x + 2] == 1)
        rotate = 0;
    }
  } else if (game1->transpose.type_rotate == 1) {
    if (game1->transpose.left_up_x < 0)
      rotate = 0;
    else {
      for (int i = game1->transpose.left_up_x;
           i < game1->transpose.left_up_x + 3; i++)
        if (game->field[game1->transpose.left_up_y + 1][i] == 1) rotate = 0;
      if (game->field[game->transpose.left_up_y + 2]
                     [game->transpose.left_up_x] == 1)
        rotate = 0;
    }
  } else if (game1->transpose.type_rotate == 2) {
    if (game1->transpose.right_down_y > 19)
      rotate = 0;
    else {
      for (int i = game->transpose.left_up_y; i < game->transpose.left_up_y + 3;
           i++)
        if (game->field[i][game->transpose.left_up_x + 1] == 1) rotate = 0;
      if (game->field[game->transpose.left_up_y][game->transpose.left_up_x] ==
          1)
        rotate = 0;
    }
  } else if (game1->transpose.type_rotate == 3) {
    if (game1->transpose.left_up_x > 7)
      rotate = 0;
    else {
      for (int i = game->transpose.left_up_x; i < game->transpose.left_up_x + 3;
           i++)
        if (game->field[game1->transpose.left_up_y + 1][i] == 1) rotate = 0;
      if (game->field[game->transpose.left_up_y]
                     [game->transpose.left_up_x + 2] == 1)
        rotate = 0;
    }
  }
  if (rotate)
    game1->transpose.type_rotate = (game1->transpose.type_rotate + 1) % 4;
  return rotate;
}

void rotate_four(GameInfo_t* game) {
  switch (game->transpose.type_rotate) {
    case 0:
      for (int i = game->transpose.left_up_y; i < game->transpose.left_up_y + 2;
           i++)
        game->field[i][game->transpose.left_up_x] = 0;
      for (int i = game->transpose.left_up_y + 1;
           i < game->transpose.left_up_y + 3; i++)
        game->field[i][game->transpose.left_up_x + 1] = 0;
      for (int i = game->transpose.left_up_x + 1;
           i < game->transpose.left_up_x + 3; i++)
        game->field[game->transpose.left_up_y][i] = 1;
      for (int i = game->transpose.left_up_x; i < game->transpose.left_up_x + 2;
           i++)
        game->field[game->transpose.left_up_y + 1][i] = 1;
      break;
    case 1:
      for (int i = game->transpose.left_up_x + 1;
           i < game->transpose.left_up_x + 3; i++)
        game->field[game->transpose.left_up_y][i] = 0;
      for (int i = game->transpose.left_up_x; i < game->transpose.left_up_x + 2;
           i++)
        game->field[game->transpose.left_up_y + 1][i] = 0;
      for (int i = game->transpose.left_up_y; i < game->transpose.left_up_y + 2;
           i++)
        game->field[i][game->transpose.left_up_x] = 1;
      for (int i = game->transpose.left_up_y + 1;
           i < game->transpose.left_up_y + 3; i++)
        game->field[i][game->transpose.left_up_x + 1] = 1;
      break;
    default:
      break;
  }
}

int can_rotate_four(GameInfo_t* game, GameInfo_t* game1) {
  int rotate = 1;
  if (game1->transpose.type_rotate == 0) {
    if(game1->transpose.left_up_y>17) rotate = 0;
    else{
    for (int i = game1->transpose.left_up_y; i < game1->transpose.left_up_y + 2;
         i++)
      if (game->field[i][game1->transpose.left_up_x] == 1) rotate = 0;
    for (int i = game1->transpose.left_up_y + 1;
         i < game1->transpose.left_up_y + 3; i++)
      if (game->field[i][game1->transpose.left_up_x + 1] == 1) rotate = 0;
    }
  } else if (game1->transpose.type_rotate == 1) {
    if(game1->transpose.left_up_x > 7) rotate = 0;
    else{
    for (int i = game1->transpose.left_up_x + 1;
         i < game1->transpose.left_up_x + 3; i++)
      if (game->field[game1->transpose.left_up_y][i] == 1) rotate = 0;
    for (int i = game->transpose.left_up_x; i < game->transpose.left_up_x + 2;
         i++)
      if (game->field[game->transpose.left_up_y + 1][i] == 1) rotate = 0;
    }
  }
  if (rotate)
    game1->transpose.type_rotate = (game1->transpose.type_rotate + 1) % 2;
  return rotate;
}

void rotate_five(GameInfo_t* game) {
  switch (game->transpose.type_rotate) {
    case 0:
      game->field[game->transpose.left_up_y + 1][game->transpose.left_up_x] = 0;
      for (int i = game->transpose.left_up_y; i < game->transpose.left_up_y + 3;
           i++)
        game->field[i][game->transpose.left_up_x + 1] = 0;
      game->field[game->transpose.left_up_y][game->transpose.left_up_x + 1] = 1;
      for (int i = game->transpose.left_up_x; i < game->transpose.left_up_x + 3;
           i++)
        game->field[game->transpose.left_up_y + 1][i] = 1;
      break;
    case 1:
      game->field[game->transpose.left_up_y][game->transpose.left_up_x + 1] = 0;
      for (int i = game->transpose.left_up_x; i < game->transpose.left_up_x + 3;
           i++)
        game->field[game->transpose.left_up_y + 1][i] = 0;
      game->field[game->transpose.left_up_y + 1]
                 [game->transpose.left_up_x + 2] = 1;
      for (int i = game->transpose.left_up_y; i < game->transpose.left_up_y + 3;
           i++)
        game->field[i][game->transpose.left_up_x + 1] = 1;
      break;
    case 2:
      game->field[game->transpose.left_up_y + 1]
                 [game->transpose.left_up_x + 2] = 0;
      for (int i = game->transpose.left_up_y; i < game->transpose.left_up_y + 3;
           i++)
        game->field[i][game->transpose.left_up_x + 1] = 0;
      game->field[game->transpose.left_up_y + 2]
                 [game->transpose.left_up_x + 1] = 1;
      for (int i = game->transpose.left_up_x; i < game->transpose.left_up_x + 3;
           i++)
        game->field[game->transpose.left_up_y + 1][i] = 1;
      break;
    case 3:
      game->field[game->transpose.left_up_y + 2]
                 [game->transpose.left_up_x + 1] = 0;
      for (int i = game->transpose.left_up_x; i < game->transpose.left_up_x + 3;
           i++)
        game->field[game->transpose.left_up_y + 1][i] = 0;
      game->field[game->transpose.left_up_y + 1][game->transpose.left_up_x] = 1;
      for (int i = game->transpose.left_up_y; i < game->transpose.left_up_y + 3;
           i++)
        game->field[i][game->transpose.left_up_x + 1] = 1;
      break;
    default:
      break;
  }
}

int can_rotate_five(GameInfo_t* game, GameInfo_t* game1) {
  int rotate = 1;
  if (game1->transpose.type_rotate == 0) {
    if(game1->transpose.left_up_y > 17) rotate = 0;
    else{
    for (int i = game->transpose.left_up_y; i < game->transpose.left_up_y + 3;
         i++)
      if (game->field[i][game1->transpose.left_up_x + 1] == 1) rotate = 0;
    if (game->field[game1->transpose.left_up_y + 1]
                   [game1->transpose.left_up_x + 2] == 1)
      rotate = 0;
    }
  } else if (game1->transpose.type_rotate == 1) {
    if(game1->transpose.left_up_x < 0) rotate = 0;
    else{
    for (int i = game1->transpose.left_up_x; i < game1->transpose.left_up_x + 3;
         i++)
      if (game->field[game1->transpose.left_up_y + 1][i] == 1) rotate = 0;
    if (game->field[game->transpose.left_up_y + 2]
                   [game->transpose.left_up_x + 1] == 1)
      rotate = 0;
    }
  } else if (game1->transpose.type_rotate == 2) {
    for (int i = game->transpose.left_up_y; i < game->transpose.left_up_y + 3;
         i++)
      if (game->field[i][game->transpose.left_up_x + 1] == 1) rotate = 0;
    if (game->field[game->transpose.left_up_y + 1][game->transpose.left_up_x] ==
        1)
      rotate = 0;
  } else if (game1->transpose.type_rotate == 3) {
    if(game1->transpose.left_up_x > 7) rotate = 0;
    else{
    for (int i = game->transpose.left_up_x; i < game->transpose.left_up_x + 3;
         i++)
      if (game->field[game1->transpose.left_up_y + 1][i] == 1) rotate = 0;
    if (game->field[game->transpose.left_up_y][game->transpose.left_up_x + 1] ==
        1)
      rotate = 0;
    }
  }
  if (rotate)
    game1->transpose.type_rotate = (game1->transpose.type_rotate + 1) % 4;
  return rotate;
}

void rotate_six(GameInfo_t* game) {
  switch (game->transpose.type_rotate) {
    case 0:
      for (int i = game->transpose.left_up_y + 1;
           i < game->transpose.left_up_y + 3; i++)
        game->field[i][game->transpose.left_up_x] = 0;
      for (int i = game->transpose.left_up_y; i < game->transpose.left_up_y + 2;
           i++)
        game->field[i][game->transpose.left_up_x + 1] = 0;
      for (int i = game->transpose.left_up_x; i < game->transpose.left_up_x + 2;
           i++)
        game->field[game->transpose.left_up_y][i] = 1;
      for (int i = game->transpose.left_up_x + 1;
           i < game->transpose.left_up_x + 3; i++)
        game->field[game->transpose.left_up_y + 1][i] = 1;
      break;
    case 1:
      for (int i = game->transpose.left_up_x; i < game->transpose.left_up_x + 2;
           i++)
        game->field[game->transpose.left_up_y][i] = 0;
      for (int i = game->transpose.left_up_x + 1;
           i < game->transpose.left_up_x + 3; i++)
        game->field[game->transpose.left_up_y + 1][i] = 0;
      for (int i = game->transpose.left_up_y + 1;
           i < game->transpose.left_up_y + 3; i++)
        game->field[i][game->transpose.left_up_x] = 1;
      for (int i = game->transpose.left_up_y; i < game->transpose.left_up_y + 2;
           i++)
        game->field[i][game->transpose.left_up_x + 1] = 1;
      break;
    default:
      break;
  }
}

int can_rotate_six(GameInfo_t* game, GameInfo_t* game1) {
  int rotate = 1;
  if (game1->transpose.type_rotate ==0) {
    if(game1->transpose.left_up_y>17) rotate = 0;
    else{
    for (int i = game1->transpose.left_up_y + 1;
         i < game1->transpose.left_up_y + 3; i++)
      if (game->field[i][game1->transpose.left_up_x] == 1) rotate = 0;
    for (int i = game1->transpose.left_up_y; i < game1->transpose.left_up_y + 2;
         i++)
      if (game->field[i][game1->transpose.left_up_x + 1] == 1) rotate = 0;
    }
  } else if (game1->transpose.type_rotate ==1) {
    if(game1->transpose.left_up_x > 7) rotate = 0;
    else{
    for (int i = game1->transpose.left_up_x; i < game1->transpose.left_up_x + 2;
         i++)
      if (game->field[game1->transpose.left_up_y][i] == 1) rotate = 0;
    for (int i = game->transpose.left_up_x + 1;
         i < game->transpose.left_up_x + 3; i++)
      if (game->field[game->transpose.left_up_y + 1][i] == 1) rotate = 0;
    }  
  }
  if (rotate)
    game1->transpose.type_rotate = (game1->transpose.type_rotate + 1) % 2;
  return rotate;
}

void check_field(GameInfo_t* game) {
  int k = 0;
  int score = 0;
  for (int i = 0; i < BOARD_M; i++) {
    int stop = 1;
    for (int j = 0; j < BOARD_W; j++) {
      if (game->field[i][j] == 0) stop = 0;
    }
    if (stop) {
      k++;
      decrease_field(game, i);
    }
  }
  for (int i = 1; i <= k; i++) score += pow(2, k - 1) * 100;
  game->score += score;
}

void decrease_field(GameInfo_t* game, int k) {
  for (int i = k; i > 0; i--) {
    for (int j = 0; j < BOARD_W; j++) {
      game->field[i][j] = game->field[i - 1][j];
    }
  }
  for (int i = 0; i < BOARD_W; i++) game->field[0][i] = 0;
}

void inf_loop(){
  GameInfo_t game, game1;
  game = updateCurrentState();
  createField(&game1);
  initscr();
  keypad(stdscr, TRUE);
  noecho();
  int color, ch, stop = 1;
  bool hold;
  while (game.terminate != 1) {
    if (game.pause == 0) reflection(&game, &game1, &color);
    graphicsTetris(&game, &game1, color);
    ch = getch();
    if (ch == Terminate || ch == Start) performAction(ch, &game, &game1, &hold);
    graphicsTetris(&game, &game1, color);
    if(hold == true) sleep(100);
    while (game.pause == 0 && game.terminate != 1) {
      game.level = ((int)(game.score / 600)) + 1;
      if(game.level > 10) game.level = 10;
      timeout(600 - game.level * 50);
      stop = checkSpawn(&game, &game1);
      if (!stop)
        game.pause = 2;
      else {
        reflection(&game, &game1, &color);
        ch = getch();
        performAction(ch, &game, &game1, &hold);
        graphicsTetris(&game, &game);
        active_game(&game, &game1);  // цикл падающей фигуры
        rewrite(&game, &game1);
        check_field(&game);
        if (game.score >= game.high_score) game.high_score = game.score;
      }
    }
    while (game.terminate != 1 && game.pause == 2) {
      graphicsTetrisv2();
      ch = getch();
      if (ch == Terminate || ch == Start)
        performAction(ch, &game, &game1, &hold);
    }
    if (game.score >= game.high_score) game.high_score = game.score;
    writeNumberToFile(game.high_score, "number.txt");
    game.score = 0;
    if (game.terminate != 1 && game.pause == 0) {
      deleteField(&game);
      cleanGen(&game);
      game = updateCurrentState();
    }
  }
  endwin();
  cleanGen(&game);
  deleteField(&game1);
  deleteField(&game);
}