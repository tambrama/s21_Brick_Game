#ifndef MAINWINDOW_H
#define MAINWINDOW_H
/**
 * @file    mainwindow.h
 * @brief   Главное окно приложения с графическим интерфейсом для игры Snake
 * @author  Tambrama
 * @date    2025
 *
 * - Управление графическим интерфейсом
 * - Обработка событий клавиатуры
 * - Таймер обновления игры
 * - Взаимодействие с визуальным компонентом
 */

#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QMainWindow>
#include <QTimer>

#include "../../brick_game/brick_games.h"
#include "../../brick_game/controller/controller.h"
#include "view.h"

/**
 * @class MainWindow
 * @brief Главное окно приложения Qt
 *
 * Обеспечивает графический интерфейс для игр, обрабатывает
 * пользовательский ввод и управляет циклом обновления игры.
 */
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  /**
   * @brief Конструктор главного окна
   * @param parent Родительский виджет (по умолчанию nullptr)
   */
  explicit MainWindow(QWidget* parent = nullptr);

 private slots:
  /**
   * @brief Слот таймера для обновления игры
   */
  void onTimer();

  void onStateUpdated(const GameInfo_t& info);

 protected:
  /**
   * @brief Обработка нажатий клавиш
   * @param event Событие клавиатуры
   */
  void keyPressEvent(QKeyEvent* event) override;

 private:
  QTimer* timer;       ///< Таймер для обновления игры
  GameView* gameView;  ///< Визуальный компонент игры
  s21::GameController* controller;  ///< Указатель на контроллер

  /**
   * @brief Восстановление фокуса окна
   */
  void restoreFocus();
    /**
   * @brief Обработка нажатия клавиши
   * @param key Код нажатой клавиши
   */
  void processUserInput(int key);
    /**
   * @brief Преобразование кода клавиши в действие пользователя
   * @param key Код клавиши
   * @return Соответствующее действие пользователя
   */
  UserAction_t getActionForKey(int key);
};

#endif  // MAINWINDOW_H