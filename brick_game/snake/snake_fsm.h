#ifndef S21_SNAKE_FSM_H_
#define S21_SNAKE_FSM_H_

#include "snake.h"
/**
 * @file    snake_fsm.h
 * @brief   Конечный автомат (FSM) для управления состояниями игры Snake
 * @author  Tambrama
 * @date    2025
 *
 * - Обработка пользовательского ввода
 * - Управление переходами между состояниями игры
 * - Взаимодействие с основной логикой игры
 */
namespace s21 {
class SnakeGame;
/**
 * @class SnakeFSM
 * @brief Класс конечного автомата для игры Snake
 *
 * Обрабатывает действия пользователя и управляет переходами между состояниями
 * игры.
 */
class SnakeFSM {
 public:
  /**
   * @brief Конструктор класса
   * @param game Указатель на объект игры SnakeGame
   */
  explicit SnakeFSM(SnakeGame* game);
  /**
   * @brief Обработка пользовательского ввода
   * @param action Действие пользователя
   * @param hold Флаг удержания клавиши
   */
  void handleInput(UserAction_t action, bool hold);

 private:
  SnakeGame* game_;
};

}  // namespace s21

#endif  // S21_SNAKE_FSM_H_