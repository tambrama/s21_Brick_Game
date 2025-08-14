#ifndef SNAKE_H
#define SNAKE_H
/**
 * @file    snake.h
 * @brief   Основная логика игры Snake
 * @author  Tambrama
 * @date    2025
 *
 * - Управление игровым процессом
 * - Движение змейки и генерация яблок
 * - Проверка коллизий и подсчет очков
 * - Сохранение и загрузка рекордов
 */
#include <chrono>
#include <ctime>
#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "../brick_games.h"
#include "snake_fsm.h"

namespace s21 {

class SnakeFSM;
/**
 * @class SnakeGame
 * @brief Основной класс игры Snake
 *
 * Содержит всю игровую логику: состояние поля, змейки, счет, управление
 * движением и т.д.
 */
class SnakeGame {
 public:
  /**
   * @enum Direction_t
   * @brief Направления движения змейки
   */
  enum class Direction_t { None, Up, Down, Left, Right };
  /**
   * @struct GameState_t
   * @brief Состояние игры
   */
  enum class GameState_t { Start, Spawn, Moving, GameOver, Win, Exit };
  /**
   * @struct Point_t
   * @brief Точки на игровом поле
   */
  struct Point {
    int x;  ///< Координата X
    int y;  ///< Координата Y
            /**
             * @brief Оператор сравнения точек
             */
    bool operator==(const Point& other) const = default;
  };

 private:
  /**
   * @brief Тайминг движения змейки
   */
  void timing();
  /**
   * @brief Чтение лучшего результата из файла
   * @return Лучший результат
   */
  int readHighScore();
  /**
   * @brief Запись лучшего результата в файл
   * @param score Новый результат
   */
  void writeHighScore(int score);

  std::vector<Point> snake_;  ///< Вектор точек тела змейки
  Point apple_;               ///< Позиция яблока
  Direction_t direction_;  ///< Текущее направление движения
  Direction_t next_direction_;  ///< Следующее направление движения
  GameState_t state_;  ///< Текущее состояние игры
  std::vector<std::vector<int>> field_;  ///< Игровое поле
  int score_;                            ///< Текущий счет
  int high_score_;                       ///< Лучший результат
  int level_;                            ///< Уровень
  int speed_;                            ///< Скорость игры
  int pause_;                            ///< Состояние паузы
  bool boost_;                           ///< Флаг ускорения
  int boost_speed_;  ///< Скорость при ускорении

  std::unique_ptr<SnakeFSM> fsm_;  ///< Указатель на конечный автомат
  std::chrono::steady_clock::time_point
      last_move_time_;  ///< Время последнего движения

 public:
  /**
   * @brief Обновление направления движения
   * @param new_dir Новое направление
   */
  void updateDirection(Direction_t new_dir);
  /**
   * @brief Движение змейки
   */
  void moveSnake();
  /**
   * @brief Генерация нового яблока
   */
  void generateApple();
  /**
   * @brief Проверка коллизии с границами
   * @param new_head Новая позиция головы
   * @return true если есть коллизия, false иначе
   */
  bool checkCollision(const Point& new_head);
  /**
   * @brief Проверка самопересечения
   * @param new_head Новая позиция головы
   * @return true если змейка пересекает себя, false иначе
   */
  bool checkSelfCollision(const Point& new_head);
  /**
   * @brief Увеличение счета
   */
  void sumScore();
  /**
   * @brief Очистка старой позиции змейки с поля
   */
  void clearOldPosition();
  /**
   * @brief Отрисовка новой позиции змейки на поле
   */
  void drawNewPosition();
  /**
   * @brief Переключение режима паузы
   */
  void togglePause();

  /**
   * @brief Переключение режима ускорения
   */
  void toggleBoost();

  static constexpr int FIELD_W = 10;  ///< Ширина игрового поля
  static constexpr int FIELD_H = 20;  ///< Высота игрового поля
  static constexpr int MAX_SNAKE_LENGTH = 200;  ///< Максимальная длина змейки
  static constexpr int MAX_LEVEL = 10;  ///< Максимальный уровень
                                        /**
                                         * @brief Конструктор класса
                                         */
  SnakeGame();
  /**
   * @brief Деструктор класса
   */
  ~SnakeGame();

  /**
   * @brief Обновление состояния игры
   * @return Текущая информация об игре
   */
  GameInfo_t updateState();
  /**
   * @brief Освобождение ресурсов GameInfo_t
   * @param info Ссылка на GameInfo_t для освобождения
   */
  void freeGameInfo(GameInfo_t& info);
  /**
   * @brief Инициализация новой игры
   */
  void snakeInitGame();
  /**
   * @brief Завершение игры
   */
  void snakeEndGame();

  ////////////гетеры
  int** getField();
  int** getNext();
  int getScore();
  int getHighScore();
  int getLevel();
  int getSpeed();
  int getPause();
  GameState_t getState() const;
  Direction_t getDirection() const;
  Direction_t getNextDirection() const;
  SnakeFSM& getFSM();
  Point getApple() const;
  const std::vector<Point>& getSnake() const;
  bool getBoost() const;

  /////////////сетеры
  void setState(GameState_t state);
  void setPause(int pause);
  void setScore(int score);
  void highScore(int high_score);
  void setDirection(Direction_t direction);
  void setApple(const Point& apple);
};

}  // namespace s21

#endif  // S21_SNAKE_GAME_H_