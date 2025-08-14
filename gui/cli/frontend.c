#include "frontend.h"

void initNcurses() {
  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  timeout(TIMEOUT);
}
void initColors() {
  start_color();
  init_pair(12, COLOR_MAGENTA, COLOR_MAGENTA);
  init_pair(1, COLOR_MAGENTA, COLOR_BLACK); // основной цвет
  init_pair(11, I_COLOR, I_COLOR);          // I
  init_pair(3, O_COLOR, O_COLOR);           // O
  init_pair(4, T_COLOR, T_COLOR);           // T
  init_pair(5, S_COLOR, S_COLOR);           // S
  init_pair(6, Z_COLOR, Z_COLOR);           // Z
  init_pair(7, J_COLOR, J_COLOR);           // J
  init_pair(8, L_COLOR, L_COLOR);           // L
  init_pair(10, O_COLOR, O_COLOR);
}

void printGame() {
  GameInfo_t gi = updateCurrentState();

  printTitle();
  printInstruction();
  printField(gi);
  printGameInfo();

}
void printField(GameInfo_t gi) {

  WINDOW *game_win =
      newwin(HEIGHT_PLAY_WIN + 2, WIDTH_PLAY_WIN * 2 + 2, CENTER_Y, CENTER_X);
  wclear(game_win);
  box(game_win, 0, 0);

  if (gi.pause == 0) {
    for (int i = 0; i < HEIGHT_PLAY_WIN; i++) {
      for (int j = 0; j < WIDTH_PLAY_WIN; j++) {
        if (gi.field[i][j] == 2) {
          wattron(game_win, COLOR_PAIR(12)); // цвет для статичных блоков
          mvwprintw(game_win, i + 1, 2 * j + 1, "  ");
          wattroff(game_win, COLOR_PAIR(12));
        } else if (gi.field[i][j] == 3) {
          wattron(game_win, COLOR_PAIR(6)); // цвет для яблока
          mvwprintw(game_win, i + 1, 2 * j + 1, "  ");
          wattroff(game_win, COLOR_PAIR(8));
        } else if (gi.field[i][j] == 4) {
          wattron(game_win, COLOR_PAIR(7)); 
          mvwprintw(game_win, i + 1, 2 * j + 1, "  ");
          wattroff(game_win, COLOR_PAIR(8));
        } else if (gi.field[i][j] == 5) {
          wattron(game_win, COLOR_PAIR(11)); 
          mvwprintw(game_win, i + 1, 2 * j + 1, "  ");
          wattroff(game_win, COLOR_PAIR(8));
        } else if (gi.field[i][j] == 6) {
          wattron(game_win, COLOR_PAIR(3)); 
          mvwprintw(game_win, i + 1, 2 * j + 1, "  ");
          wattroff(game_win, COLOR_PAIR(8));
        } else if (gi.field[i][j] == 7) {
          wattron(game_win, COLOR_PAIR(4)); 
          mvwprintw(game_win, i + 1, 2 * j + 1, "  ");
          wattroff(game_win, COLOR_PAIR(8));
        } else if (gi.field[i][j] == 10) {
          wattron(game_win, COLOR_PAIR(5)); 
          mvwprintw(game_win, i + 1, 2 * j + 1, "  ");
          wattroff(game_win, COLOR_PAIR(8));
        } else if (gi.field[i][j] == 9) {
          wattron(game_win, COLOR_PAIR(10)); 
          mvwprintw(game_win, i + 1, 2 * j + 1, "  ");
          wattroff(game_win, COLOR_PAIR(8));
        }else if (gi.field[i][j] != 0) {
          wattron(game_win, COLOR_PAIR(8)); // цвет для активной фигуры
          mvwprintw(game_win, i + 1, 2 * j + 1, "  ");
          wattroff(game_win, COLOR_PAIR(8));
        } else {
          mvwprintw(game_win, i + 1, 2 * j + 1, "  ");
        }
      }
    }

  } else if (gi.pause == 1) {
    mvwprintw(game_win, 7, 8, "PAUSE");
  } else if (gi.pause == 3) {
    mvwprintw(game_win, 7, 7, "GAME OVER");
    mvwprintw(game_win, 9, 1, "Press Enter to START");
    mvwprintw(game_win, 11, 6, "ESC to EXIT");
  } else if (gi.pause == 2) {
    mvwprintw(game_win, 7, 9, "START");
    mvwprintw(game_win, 8, 6, "Press Enter");
  }else if (gi.pause == 4) {
    mvwprintw(game_win, 7, 8, "YOU WIN");
    mvwprintw(game_win, 9, 1, "Press Enter to START");
    mvwprintw(game_win, 11, 6, "ESC to EXIT");
  }
  wbkgd(game_win, COLOR_PAIR(1));
  wrefresh(game_win);
}

void printNextFigure(WINDOW *win) {
  GameInfo_t gi = updateCurrentState();
  if (gi.next != NULL) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (gi.next[i][j] != 0) {
          wattron(win, COLOR_PAIR(8)); // цвет для активной фигуры
          mvwprintw(win, NEXT_START_Y + i, NEXT_START_X + j * 2, "  ");
          wattroff(win, COLOR_PAIR(8));
        }
      }
    }
  }
}

void printGameInfo() {
  GameInfo_t gi = updateCurrentState();
  WINDOW *game_info = newwin(HEIGHT_PLAY_WIN + 2, 19, CENTER_Y, CENTER_X + 22);

  box(game_info, 0, 0);

  mvwprintw(game_info, 2, 5, "LEVEL  %d", gi.level);
  mvwhline(game_info, 3, 2, ACS_HLINE, 14);
  mvwprintw(game_info, 5, 2, "High score %d", gi.high_score);
  mvwprintw(game_info, 7, 2, "Score %d", gi.score);

  mvwprintw(game_info, 13, 7, "Next");

  printNextFigure(game_info);

  wbkgd(game_info, COLOR_PAIR(1));
  wrefresh(game_info);
}
void printInstruction() {
  WINDOW *game_instruction =
      newwin(HEIGHT_PLAY_WIN + 2, 16 + 2, CENTER_Y, CENTER_X - 18);
  box(game_instruction, 0, 0);
  mvwprintw(game_instruction, 2, 2, "Start - ENTER");
  mvwprintw(game_instruction, 3, 2, "Pause - P");
  mvwprintw(game_instruction, 4, 2, "Rotate - SPACE");
  mvwprintw(game_instruction, 5, 2, "Down - v");
  mvwprintw(game_instruction, 6, 2, "Right - >");
  mvwprintw(game_instruction, 7, 2, "Left - <");
  mvwprintw(game_instruction, 8, 2, "Exit - ESC");
  wbkgd(game_instruction, COLOR_PAIR(1));
  wrefresh(game_instruction);
}

void printTitle() {
  int width = WIDTH_PLAY_WIN * 2 + 18 + 21;
  int start_y = CENTER_Y - 3;
  int start_x = CENTER_X - 18;

  WINDOW *title_win = newwin(3.5, width, start_y, start_x);
  box(title_win, 0, 0);
  wbkgd(title_win, COLOR_PAIR(1));
  mvwprintw(title_win, 1, CENTER_X / 2 , "BRICK GAME");

  wrefresh(title_win);
}

UserAction_t getSignal(int input_user) {
  UserAction_t action = {-1};
  switch (input_user) {
    case ENTER_KEY:
      action = Start;
      break;
    case 'P':
    case 'p':
      action = Pause;
      break;
    case KEY_UP:
      action = Up;
      break;
    case KEY_DOWN:
      action = Down;
      break;
    case KEY_LEFT:
      action = Left;
      break;
    case KEY_RIGHT:
      action = Right;
      break;
    case SPACE_KEY:
      action = Action;
      break;
    case ESC_KEY:
      action = Terminate;
      break;
    default:
      break;
  }

  return action;
}