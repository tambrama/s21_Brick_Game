#include "backend.h"

GameInfo_t *getGameInfo() {
  static GameInfo_t info = {0};
  return &info;
}

Figure_t *getFigure() {
  static Figure_t figure = {0};
  return &figure;
}

GameStatus_t *getGameStatus() {
  static GameStatus_t status = {0};
  return &status;
}

UserAction_t *getAction() {
  static UserAction_t action = {0};
  return &action;
}

void initGame() {
  GameInfo_t *info = getGameInfo();
  Figure_t *figure = getFigure();
  GameStatus_t *status = getGameStatus();
  *status = START;
  info->pause = 2;
  info->level = 1;
  info->high_score = readHighScore();
  info->score = 0;
  info->speed = 0;

  info->field = fillingMatrix(HEIGHT_PLAY_WIN, WIDTH_PLAY_WIN);
  figure->figure = createMatrix(FIGURE_SIZE, FIGURE_SIZE);
  info->next = createMatrix(FIGURE_SIZE, FIGURE_SIZE);
  srand(time(NULL));
  figure->x = START_X;
  figure->y = START_Y;
  randomFigure(info->next);

  // функция для создания нового потока
  pthread_create(&figure->drop_thread, NULL, autoDropFigure, NULL);
}

GameInfo_t updateCurrentState() {
  GameInfo_t *info = getGameInfo();
  GameStatus_t *status = getGameStatus();
  // Проверка на завершение игры (верхняя строка)
  for (int j = 0; j < WIDTH_PLAY_WIN; j++) {
    if (info->field[0][j] == 2) {
      *status = GAME_OVER;
      return *info;
    }
  }

  if (info->level == 10) {
    *status = WIN;
  }

  if (info->score > info->high_score) {
    info->high_score = info->score;
  }

  return *info;
}


void *autoDropFigure() {
  GameInfo_t *info = getGameInfo();
  const GameStatus_t *status = getGameStatus();
  while (1) {
    if (*status == MOVING && info->pause == 0) {
      if (!canMoving()) {
        attachFigure();
      } else {
        UserAction_t action = Down;
        moveFigure(&action);
      }
    }
    int speed = 700000 / info->level;
    usleep(speed); // 700,000 микросекунд = 0.5 секунды

     info->speed = speed;
    if (*status == EXIT || *status == GAME_OVER) {
      break;
    }
  }

  return NULL;
}

int **newFigire() {
  GameInfo_t *info = getGameInfo();
  Figure_t *figure = getFigure();
  copyMatrix(info->next, figure->figure, FIGURE_SIZE, FIGURE_SIZE);
  randomFigure(info->next);
  figure->x = START_X;
  figure->y = START_Y;
  normalizeFigure(FIGURE_SIZE, figure->figure);
  setFigureSize(figure);
  return figure->figure;
}

int moveFigure(UserAction_t *action) {
  GameInfo_t *info = getGameInfo();
  Figure_t *figure = getFigure();
  GameStatus_t *status = getGameStatus();

  int new_x = figure->x;
  int new_y = figure->y;

  switch (*action) {
  case Left:
    new_x--;
    break;
  case Right:
    new_x++;
    break;
  case Down:
    new_y++;
    break;
  case Action:
    cleanPositionFigure(info, figure);
    rotateFigure(figure->figure);
    break;
  case Pause:
    info->pause = !info->pause;
    break;
  default:
    break;
  }

  // Проверка коллизий
  int move = checkCollision(new_x, new_y);

  if (move == 1) {
    cleanPositionFigure(info, figure);
    figure->x = new_x;
    figure->y = new_y;
    newPositionFigure(info, figure);
  } else if (move == -1) {
    *status = ATTACHING;
  }
  *action = 0;
  return move;
}

bool canMoving() {
  const GameInfo_t *info = getGameInfo();
  const Figure_t *figure = getFigure();
  bool check = true;
  for (int i = 0; i < figure->sizeY && check; i++) {
    for (int j = 0; j < figure->sizeX && check; j++) {
      if (figure->figure[i][j] != 0) {
        int x = figure->x + j;
        int y = figure->y + i + 1;

        if (y < 0 || x < 0 || y >= HEIGHT_PLAY_WIN || x >= WIDTH_PLAY_WIN ||
            (y > 0 && info->field[y][x] == 2))
          check = false;
      }
    }
  }

  return check;
}
void setFigureSize(Figure_t *figure) {
  figure->sizeY = 0;
  figure->sizeX = 0;
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      if (figure->figure[i][j] != 0) {
        if (i + 1 > figure->sizeY) {
          figure->sizeY = i + 1;
        }
        if (j + 1 > figure->sizeX) {
          figure->sizeX = j + 1;
        }
      }
    }
  }
}

void normalizeFigure(int size, int **matrix) {
  // по горизонтали, сдвиг вверх
  int empty_rows = 0;
  for (int i = 0; i < size; i++) {
    int row_empty = 1;
    for (int j = 0; j < size; j++) {
      if (matrix[i][j] != 0) {
        row_empty = 0;
        break;
      }
    }
    if (row_empty) {
      empty_rows++;
    } else if (empty_rows > 0) {
      // Сдвигаем непустую строку вверх
      for (int j = 0; j < size; j++) {
        matrix[i - empty_rows][j] = matrix[i][j];
        matrix[i][j] = 0;
      }
    }
  }

  // по вертикали, сдвиг влево
  int empty_cols = 0;
  for (int j = 0; j < size; j++) {
    int col_empty = 1;
    for (int i = 0; i < size; i++) {
      if (matrix[i][j] != 0) {
        col_empty = 0;
        break;
      }
    }
    if (col_empty) {
      empty_cols++;
    } else if (empty_cols > 0) {
      // Сдвигаем непустой столбец влево
      for (int i = 0; i < size; i++) {
        matrix[i][j - empty_cols] = matrix[i][j];
        matrix[i][j] = 0;
      }
    }
  }
}

bool checkGameOver() {
  GameInfo_t *info = getGameInfo();

  bool check = false;

  for (int j = 0; j < WIDTH_PLAY_WIN; j++) {
    check = info->field[0][j] == 2;
  }
  return check;
}
void attachFigure() {
  GameInfo_t *info = getGameInfo();
  GameStatus_t *status = getGameStatus();
  const Figure_t *figure = getFigure();

  bool gameOver = checkGameOver();
  staticFigur(info, figure);

  if (gameOver) {
    *status = GAME_OVER;
  } else {
    sumScore(info->field);
    *status = SPAWN;
  }
}
void cleanPositionFigure(GameInfo_t *info, const Figure_t *figure) {
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      if (figure->figure[i][j] != 0) {
        int sq_x = figure->x + j;
        int sq_y = figure->y + i;
        if (sq_y >= 0 && sq_y < HEIGHT_PLAY_WIN && sq_x >= 0 &&
            sq_x < WIDTH_PLAY_WIN) {
          info->field[sq_y][sq_x] = 0;
        }
      }
    }
  }
}
void newPositionFigure(GameInfo_t *info, Figure_t *figure) {
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      if (figure->figure[i][j] != 0) {
        int sq_x = figure->x + j;
        int sq_y = figure->y + i;
        if (sq_y >= 0 && sq_y < HEIGHT_PLAY_WIN && sq_x >= 0 &&
            sq_x < WIDTH_PLAY_WIN) {
          info->field[sq_y][sq_x] = figure->figure[i][j];
        }
      }
    }
  }
}
void staticFigur(GameInfo_t *info, const Figure_t *figure) {
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      if (figure->figure[i][j] != 0) {
        int sq_x = figure->x + j;
        int sq_y = figure->y + i;
        if (sq_y >= 0 && sq_y < HEIGHT_PLAY_WIN && sq_x >= 0 &&
            sq_x < WIDTH_PLAY_WIN) {
          info->field[sq_y][sq_x] = 2; // Помечаем как статичную фигуру
        }
      }
    }
  }
}
int checkCollision(int new_x, int new_y) {
  const GameInfo_t *info = getGameInfo();
  const Figure_t *figure = getFigure();
  const UserAction_t *action = getAction();
  int check = 1;
  for (int i = 0; i < figure->sizeY; i++) {
    for (int j = 0; j < figure->sizeX; j++) {
      if (figure->figure[i][j] != 0) {
        int sq_x = new_x + j;
        int sq_y = new_y + i;

        if (sq_y < 0 || sq_y >= HEIGHT_PLAY_WIN || sq_x < 0 ||
            sq_x >= WIDTH_PLAY_WIN || info->field[sq_y][sq_x] == 2) {
          check = (*action == Down) ? -1 : 0;
        }
      }
    }
  }
  return check;
}

int **randomFigure(int **figure) {
  int random = rand() % 7;
  useFigures(figure, random);
  return figure;
}
int **useFigures(int **figure, int random) {
  const int patterns[7][4][4] = {
      {{0, 0, 0, 0}, {0, 9, 9, 0}, {0, 9, 9, 0}, {0, 0, 0, 0}},     // O
      {{0, 0, 0, 0}, {10, 10, 10, 10}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // I
      {{0, 0, 0, 0}, {0, 0, 7, 0}, {7, 7, 7, 0}, {0, 0, 0, 0}},     // L
      {{0, 0, 0, 0}, {6, 0, 0, 0}, {6, 6, 6, 0}, {0, 0, 0, 0}},     // J
      {{0, 0, 0, 0}, {5, 5, 0, 0}, {0, 5, 5, 0}, {0, 0, 0, 0}},     // Z
      {{0, 0, 0, 0}, {0, 4, 4, 0}, {4, 4, 0, 0}, {0, 0, 0, 0}},     // S
      {{0, 0, 0, 0}, {0, 3, 0, 0}, {3, 3, 3, 0}, {0, 0, 0, 0}}      // T
  };
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      figure[i][j] = patterns[random][i][j];
    }
  }
  return figure;
}

int **copyMatrix(int **A, int **B, int rows, int columns) {
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      B[i][j] = A[i][j];
    }
  }

  return B;
}

int **fillingMatrix(int rows, int columns) {
  int **result = createMatrix(rows, columns);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      result[i][j] = 0;
    }
  }

  return result;
}

int **createMatrix(int rows, int columns) {

  int **result = (int **)calloc(rows, sizeof(int *));
  if (result != NULL) {
    for (int i = 0; i < rows; i++) {
      result[i] = (int *)calloc(columns, sizeof(int));
      if (result[i] == NULL) {
        removeMatrix(i, result);
        break;
      }
    }
  }
  return result;
}

void removeMatrix(int rows, int **matrix) {
  if (matrix != NULL) {
    for (int i = 0; i < rows; i++) {
      free(matrix[i]);
    }
    free(matrix);
  }
}

bool canRotate(int **newFigure) {
  const Figure_t *figure = getFigure();
  const GameInfo_t *info = getGameInfo();
  bool res = true;
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      if (newFigure[i][j] != 0) {
        int new_x = figure->x + j;
        int new_y = figure->y + i;

        // выход за границы поля
        if (new_x < 0 || new_x >= WIDTH_PLAY_WIN || new_y < 0 ||
            new_y >= HEIGHT_PLAY_WIN) {
          res = false;
        }

        // пересечение с другими фигурами
        if (new_y >= 0 && info->field[new_y][new_x] == 2) {
          res = false;
        }
      }
    }
  }
  return res;
}

void rotateFigure(int **figure) {

  int temp[4][4];
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      temp[j][3 - i] = figure[i][j];
    }
  }
  int **rotate = createMatrix(FIGURE_SIZE, FIGURE_SIZE);
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      rotate[i][j] = temp[i][j];
    }
  }
  normalizeFigure(FIGURE_SIZE, rotate);

  if (canRotate(rotate)) {
    for (int i = 0; i < FIGURE_SIZE; i++) {
      for (int j = 0; j < FIGURE_SIZE; j++) {
        figure[i][j] = temp[i][j];
      }
    }
    normalizeFigure(FIGURE_SIZE, figure);
  }

  removeMatrix(FIGURE_SIZE, rotate);
  Figure_t *fig = getFigure();
  setFigureSize(fig);
}

// Функция подсчета очков (из первого кода)
void sumScore(int **field) {
  GameInfo_t *info = getGameInfo();

  int **lines = createMatrix(1, HEIGHT_PLAY_WIN);
  for (int i = 0; i < HEIGHT_PLAY_WIN; i++) {
    lines[0][i] = 1;
    for (int j = 0; j < WIDTH_PLAY_WIN; j++) {
      if (field[i][j] == 0) {
        lines[0][i] = 0;
      }
    }
  }

  // Подсчет заполненных линий
  int sum = 0;
  for (int i = 0; i < HEIGHT_PLAY_WIN; i++) {
    sum += lines[0][i];
  }

  switch (sum) {
  case 1:
    info->score += 100;
    break;
  case 2:
    info->score += 300;
    break;
  case 3:
    info->score += 700;
    break;
  case 4:
    info->score += 1500;
    break;
  }

  for (int i = 0; i < HEIGHT_PLAY_WIN; i++) {
    if (lines[0][i] == 1) {
      for (int j = i; j > 0; j--) {
        memcpy(info->field[j], info->field[j - 1],
               WIDTH_PLAY_WIN * sizeof(int));
      }
    }
  }

  int lvl = (info->score / 600) + 1;
  info->level = (lvl > 10) ? 10 : lvl;

  if (info->score > info->high_score) {
    info->high_score = info->score;
  }
  int high_score = readHighScore();
  if (info->score > high_score) {
    writeHighScore(info->score);
  }
  removeMatrix(1, lines);
}

int readHighScore() {
  int score = 0;
  FILE *file = fopen(HIGH_SCORE_FILE, "r");
  if (file) {
    int ch;
    while ((ch = fgetc(file)) != EOF) {
      if (ch >= '0' && ch <= '9') {
        score = score * 10 + (ch - '0');
      }
    }
    fclose(file);
  }
  return score;
}

void writeHighScore(int high_score) {
  FILE *file = fopen(HIGH_SCORE_FILE, "w");
  if (file) {
    fprintf(file, "%d", high_score);
    fclose(file);
  }
}

void endGame() {
  GameInfo_t *info = getGameInfo();
  Figure_t *figure = getFigure();

  pthread_cancel(figure->drop_thread);
  pthread_join(figure->drop_thread, NULL);

  removeMatrix(HEIGHT_PLAY_WIN, info->field);
  removeMatrix(FIGURE_SIZE, figure->figure);
  removeMatrix(FIGURE_SIZE, info->next);
}