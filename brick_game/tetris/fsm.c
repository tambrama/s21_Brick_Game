#include "fsm.h"

void userInput(UserAction_t action, bool hold) {
  if (hold == true) {
  }
  GameInfo_t *info = getGameInfo();
  GameStatus_t *status = getGameStatus();
  UserAction_t *current_action = getAction();

  *current_action = action;

  switch (*status) {
  case START:
    if (action == Start) {
      *status = SPAWN;
      info->pause = 0;
    } else if (action == Terminate) {
      *status = EXIT;
    }
    break;
  case SPAWN:
    newFigire();
    *status = MOVING;
    info->pause = 0;
    break;
  case MOVING:
    if (action == Pause) {
        info->pause = (info->pause == 0) ? 1 : 0;
    } else if (info->pause == 0) {
        moveFigure(&action);
    }
    if (action == Terminate) {
        *status = EXIT;
    }
    break;
  case ATTACHING:
    attachFigure();
    if (action == Terminate) {
      *status = EXIT;
    } else if (action == Pause) {
      info->pause = info->pause == 0 ? 1 : 0;
    }
    break;
  case WIN:
    info->pause = 4;
    if (action == Start) {
      initGame();
      info->pause = 0;
      *status = SPAWN;
    } else if (action == Terminate) {
      *status = EXIT;
    }
    break;
  case GAME_OVER:
    info->pause = 3;
    if (action == Start) {
      // Рестарт игры
      initGame();
      info->pause = 0;
      *status = SPAWN;
    } else if (action == Terminate) {
      *status = EXIT;
    }
    break;
  case EXIT:
    endGame();
    break;
  }
}