#ifndef BRICK_GAME_H
#define BRICK_GAME_H
/**
 * @file    brick_games.h
 * @brief   Общий файл спецификации для игр серии Brick Games
 * @author  Tambrama
 * @date    2025
 *
 * - Универсальные структуры данных для игр
 * - Общий интерфейс взаимодействия с играми
 * - Определение пользовательских действий
 */

#include <stdbool.h>
#include <stdio.h>

/**
 * @struct GameInfo_t
 * @brief Структура, содержащая основную информацию об игре.
 */
typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

/**
 * @struct UserAction_t
 * @brief Структура, содержащая информацию о возможных действиях пользователя.
 */
typedef enum {
  Start,
  Pause,
  Terminate,  // завершение
  Left,
  Right,
  Up,     // вверх
  Down,   // вниз
  Action  // действие
} UserAction_t;

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Действия пользователя, смена состояния игры
 * @param action пользовательское действие
 * @param hold зажатие клавиши
 */
void userInput(UserAction_t action, bool hold);
/**
 * @brief Получает текущее состояние игрового поля
 * @return Структура GameInfo_t с актуальными данными
 * @note Обновляет статус игры при необходимости
 */
GameInfo_t updateCurrentState();

#ifdef __cplusplus
}
#endif

#endif