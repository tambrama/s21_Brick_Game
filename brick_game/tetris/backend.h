#ifndef BACKEND_H
#define BACKEND_H

#define _DEFAULT_SOURCE

#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "../brick_games.h"
/**
 * @file    backend.c
 * @brief   Логика игры Tetris
 * @author  Tambrama
 * @date    2025
 *
 * - Управление игровым процессом
 * - Обработку пользовательского ввода
 * - Генерацию и движение фигур
 * - Проверку коллизий
 * - Подсчет очков
 */

/* Стартовые координаты фигур */
#define START_X 4
#define START_Y 0
#define FIGURE_SIZE 4
#define HIGH_SCORE_FILE "high_score_t.txt"
#define HEIGHT_PLAY_WIN 20
#define WIDTH_PLAY_WIN 10
#define SPACE_KEY 32
#define ENTER_KEY 10
#define ESC_KEY 27

/**
 * @struct GameStatus_t
 * @brief Структура, содержащая информацию о всех состояниях игры.
 */
typedef enum {
  START,   // старт
  SPAWN,   // переход в состояние создания блока
  MOVING,  // перемещение
  ATTACHING,  // соединение
  GAME_OVER,  // конец игры
  WIN,        // победа
  EXIT        // выход
} GameStatus_t;

/**
 * @struct State_t
 * @brief Структура, содержащая информацию о координатах фигуры и самой фигуре.
 */
typedef struct {
  int **figure;      // матрица фигуры
  int x, y;          // координаты фигуры
  int sizeX, sizeY;  // размеры фигуры
  pthread_t drop_thread;  // переменная для создания потока
} Figure_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Получает указатель на структуру с игровой информацией
 * @return Указатель на GameInfo_t
 */
GameInfo_t *getGameInfo();
/**
 * @brief Получает указатель на текущее действие пользователя
 * @return Указатель на UserAction_t
 */
UserAction_t *getAction();
/**
 * @brief Получает указатель на текущий статус игры
 * @return Указатель на GameStatus_t
 */
GameStatus_t *getGameStatus();
/**
 * @brief Получает указатель на данные текущей фигуры
 * @return Указатель на Figure_t
 */
Figure_t *getFigure();
/**
 * @brief Инициализирует игровой процесс
 */
void initGame();
/**
 * @brief Генерирует новую фигуру
 * @return Указатель на матрицу 4x4 с данными фигуры
 */
int **newFigire();
/**
 * @brief Создает случайную фигуру
 * @return Указатель на матрицу 4x4

*/
int **randomFigure();
/**
 * @brief Заполняет матрицу фигуры по шаблону
 * @param figure Целевая матрица 4x4
 * @param random Индекс фигуры (0-6)
 * @return Указатель на заполненную матрицу
 */
int **useFigures(int **figure, int random);
/**
 * @brief Обрабатывает движение фигуры
 * @param action Запрошенное действие
 * @return Результат движения:
 *         - 1: успешно
 *         - 0: движение невозможно
 *         - -1: фигура зафиксирована
 */
int moveFigure(UserAction_t *ction);
/**
 * @brief Вращает фигуру на поле
 * @param figure матрица фигуры
 */
void rotateFigure(int **figure);
/**
 * @brief Нормализует положение фигуры в матрице
 * @param size Размер матрицы (4)
 * @param matrix Матрица 4x4
 * @details Сдвигает фигуру к левому верхнему углу матрицы
 */
void normalizeFigure(int size, int **matrix);
/**
 * @brief Проверяет возможность движения
 * @param new_x Предполагаемая X-координата
 * @param new_y Предполагаемая Y-координата
 * @return Результат проверки:
 *         - 1: движение возможно
 *         - 0: движение невозможно
 *         - -1: достигнуто дно/фигура
 */
int checkCollision(int new_x, int new_y);
/**
 * @brief Копирует матрицу
 * @param A Исходная матрица
 * @param B Целевая матрица
 * @param rows Количество строк
 * @param columns Количество столбцов
 * @return Указатель на скопированную матрицу
 */
int **copyMatrix(int **A, int **B, int rows, int columns);
/**
 * @brief Создает нулевую матрицу
 * @param rows Количество строк
 * @param columns Количество столбцов
 * @return Указатель на созданную матрицу
 */
int **fillingMatrix(int rows, int columns);
/**
 * @brief Выделяет память под матрицу
 * @param rows Количество строк
 * @param columns Количество столбцов
 * @return Указатель на созданную матрицу
 */
int **createMatrix(int rows, int columns);
/**
 * @brief Освобождает память матрицы
 * @param rows Количество строк
 * @param matrix Указатель на матрицу
 */
void removeMatrix(int rows, int **matrix);
/**
 * @brief Проверяет возможность поворота
 * @param newFigure Матрица с повернутой фигурой
 * @return true если поворот возможен
 */
bool canRotate(int **newFigure);
/**
 * @brief Поворачивает фигуру на 90 градусов
 * @param figure Матрица с фигурой
 */
void rotateFigure(int **figure);
/**
 * @brief Подсчитывает и обновляет счет
 * @param field Игровое поле
 */
void sumScore(int **field);
/**
 * @brief Очищает предыдущую позицию фигуры
 * @param info Данные игры
 * @param figure Данные фигуры
 */
void cleanPositionFigure(GameInfo_t *info, const Figure_t *figure);
/**
 * @brief Отрисовывает фигуру в новой позиции
 * @param info Данные игры
 * @param figure Данные фигуры
 */
void newPositionFigure(GameInfo_t *info, Figure_t *figure);
/**
 * @brief Фиксирует фигуру на поле
 * @param info Данные игры
 * @param figure Данные фигуры
 */
void staticFigur(GameInfo_t *info, const Figure_t *figure);
/**
 * @brief Читает рекорд из файла
 * @return Значение рекорда
 */
int readHighScore();
/**
 * @brief Записывает рекорд в файл
 * @param high_score Новое значение рекорда
 */
void writeHighScore(int high_score);
/**
 * @brief Завершает игровой процесс
 */
void endGame();
/**
 * @brief Поток автоматического падения фигур
 * @return NULL
 */
void *autoDropFigure();
/**
 * @brief Проверяет условие завершения игры
 * @return true если фигуры достигли верха
 */
bool checkGameOver();
/**
 * @brief Фиксирует фигуру и проверяет Game Over
 */
void attachFigure();
/**
 * @brief Проверяет возможность движения вниз
 * @return true если движение возможно
 */
bool canMoving();
/**
 * @brief Устанавливает фактические размеры фигуры
 * @param figure Данные фигуры
 */
void setFigureSize(Figure_t *figure);

#ifdef __cplusplus
}
#endif

#endif