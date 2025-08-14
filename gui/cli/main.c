#include "../../brick_game/brick_games.h"

#ifdef SNAKE_GAME
    // #include "../../brick_game/snake/snake.h"
  // #include "../../brick_game/snake/snake_fsm.h"
  #include "../../brick_game/snake/snake_wrapper.h"
   
#elif defined(TETRIS_GAME)
  #include "../../brick_game/brick_games.h"
  #include "../../brick_game/tetris/backend.h"
  #include "../../brick_game/tetris/fsm.h"
    
#else
    #error "Game type not specified! Use -DSNAKE_GAME or -DTETRIS_GAME"
#endif

#include "frontend.h"

void gameLoop();

int main() {
  initNcurses(); // инициализация библиотеки
  initColors();
  refresh();
  gameLoop();
  endwin(); // завершение работы с библиотекой
  return 0;
}

void gameLoop() {
  initGame();
  int ch;
  while ((ch = getch()) != ESC_KEY) {
    UserAction_t action = getSignal(ch);
    userInput(action, false);
    updateCurrentState();
    printGame();
  }
  endGame();
}