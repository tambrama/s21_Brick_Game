/**
 * @file    snake_wrapper.cpp
 * @brief   Обёртка для интеграции игры Snake с общим интерфейсом Brick Games
 * @author  Tambrama
 * @date    2025
 *
 * - Предоставление C-интерфейса для C++ реализации игры
 * - Управление жизненным циклом игры
 * - Маршрутизация пользовательского ввода к игровой логике
 */
#include "../brick_games.h"
#include "snake.h"
#include "snake_fsm.h"
#include "snake_wrapper.h"

namespace s21 {
class SnakeGame;
}
/**
 * @brief Получение единственного экземпляра игры (Singleton)
 * @return Ссылка на единственный экземпляр SnakeGame
 */
static s21::SnakeGame& getGameInstance() {
  static s21::SnakeGame instance;
  return instance;
}

extern "C" {
void userInput(UserAction_t action, bool hold) {
  getGameInstance().getFSM().handleInput(action, hold);
}

GameInfo_t updateCurrentState() { return getGameInstance().updateState(); }

void initGame() { getGameInstance().snakeInitGame(); }

void endGame() { getGameInstance().snakeEndGame(); }
}  // extern "C"