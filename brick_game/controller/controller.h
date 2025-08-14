#ifndef CONTROLLER_H
#define CONTROLLER_H

/**
 * @file    controller.h
 * @brief   Контроллер игры для графического интерфейса Qt
 * @author  Tambrama
 * @date    2025
 *
 * - Управление игровым процессом через Qt сигналы и слоты
 * - Обработка пользовательского ввода с клавиатуры
 * - Маршрутизация действий к соответствующей игровой логике
 * - Поддержка нескольких типов игр (Snake/Tetris) через препроцессор
 */

#include <QApplication>
#include <QObject>

#include "../brick_games.h"

#ifdef SNAKE_GAME
#include "../snake/snake_wrapper.h"
#define START Right
#elif defined(TETRIS_GAME)
#include "../tetris/backend.h"
#define START Up
#else
#error "Game type not specified! Use -DSNAKE_GAME or -DTETRIS_GAME"
#endif

namespace s21 {
/**
 * @class GameController
 * @brief Контроллер для управления игровым процессом в Qt интерфейсе
 *
 * Обеспечивает связь между графическим интерфейсом и игровой логикой,
 * обрабатывает пользовательский ввод и передает обновления состояния игры.
 */
class GameController : public QObject {
  Q_OBJECT

 public:
  /**
   * @brief Конструктор контроллера
   * @param parent Родительский объект Qt (по умолчанию nullptr)
   */
  explicit GameController(QObject* parent = nullptr) : QObject(parent) {}
  /**
   * @brief Получение текущего состояния игры
   * @return Структура GameInfo_t с актуальной информацией об игре
   */
  GameInfo_t getCurrentState();
  /**
   * @brief Обработка пользовательского ввода
   * @param action Действие пользователя
   * @param hold Флаг удержания клавиши
   */
  void userInput(UserAction_t action, bool hold);
  /**
   * @brief Запуск новой игры
   */
  void startGame();
  /**
   * @brief Завершение текущей игры
   */
  void closeGame();

 signals:
  /**
   * @brief Сигнал об изменении состояния игры
   * @param info Новое состояние игры
   */
  void stateChanged(
      const GameInfo_t& info);  // передача сигнала об изменении состояния игры

};
}  // namespace s21

#endif