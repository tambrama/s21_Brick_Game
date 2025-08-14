#ifndef VIEW_H
#define VIEW_H
/**
 * @file    view.h
 * @brief   Визуальный компонент для отображения игры Snake
 * @author  Tambrama
 * @date    2025
 *
 * - Отрисовка игрового поля
 * - Отображение змейки и яблок
 * - Визуализация следующей фигуры (заглушка для совместимости)
 * - Управление цветовой схемой элементов
 */
#include <QDebug>
#include <QFontDatabase>
#include <QPainter>
#include <QWidget>

#include "../../brick_game/brick_games.h"
/**
 * @class GameView
 * @brief Виджет для визуального отображения игры Snake
 *
 * Наследуется от QWidget и отвечает за графическое представление
 * текущего состояния игры, включая игровое поле, змейку и интерфейсные
 * элементы.
 */
class GameView : public QWidget {
  Q_OBJECT

 public:
  /**
   * @brief Конструктор виджета отображения
   * @param parent Родительский виджет (по умолчанию nullptr)
   */
  explicit GameView(QWidget* parent = nullptr);
  /**
   * @brief Обновление состояния игры для отрисовки
   * @param info Текущая информация об игре
   */
  void updateState(const GameInfo_t& info);

  void initializeField();

 protected:
  /**
   * @brief Обработчик события отрисовки
   * @param event Событие отрисовки
   */
  void paintEvent(QPaintEvent*) override;

 private:
  GameInfo_t currentInfo;  ///< Текущее состояние игры для отображения
                           /**
                            * @brief Отрисовка состояни игры (старт/пауза/проигрыш/выигрыш)
                            * @param painter Объект QPainter для рисования
                            */
  void drawOverlay(QPainter& painter);
  /**
   * @brief Отрисовка следующей фигуры (для тетриса)
   * @param painter Объект QPainter для рисования
   */
  void drawNextFigure(QPainter& painter);
  /**
   * @brief Отрисовка управления
   * @param painter Объект QPainter для рисования
   */
  void drawControls(QPainter& painter);
  /**
   * @brief Получение цвета для ячейки в зависимости от её значения
   * @param cellValue Значение ячейки
   * @return Цвет ячейки
   */
  QColor getCellColor(int cellValue);
};

#endif  // VIEW_H