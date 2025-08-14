#include "controller.h"

namespace s21 {
GameInfo_t GameController::getCurrentState() {
  GameInfo_t info = ::updateCurrentState();
  emit stateChanged(info);
  return info;
}

void GameController::userInput(UserAction_t action, bool hold) {
  ::userInput(action, hold);
  emit stateChanged(getCurrentState());
}

void GameController::startGame() { ::initGame(); }

void GameController::closeGame() { ::endGame(); }

}  // namespace s21