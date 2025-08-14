#ifndef FRONTEND_H
#define FRONTEND_H

#include <ncurses.h>

#include "../../brick_game/brick_games.h"

/**
 * @file    frontend.c
 * @brief   Графический интерфейс Tetris на ncurses
 * @author  Tambrama
 * @date    2025
 */

/* Размеры окна */
#define HEIGHT_WIN \
  (HEIGHT_PLAY_WIN + 2)  ///< Общая высота окна (поле + границы)
#define WIDTH_WIN \
  (WIDTH_PLAY_WIN + 10)  ///< Ширина окна (поле + панель информации)

/* Позиционирование элементов */
#define NEXT_START_Y 15  ///< Стартовая Y-координата блока "Следующая фигура"
#define NEXT_START_X 6  ///< Стартовая X-координата блока "Следующая фигура"
#define CENTER_Y \
  ((LINES - HEIGHT_WIN) / 2)  ///< Вертикальное центрирование окна
#define CENTER_X \
  ((COLS - WIDTH_WIN) / 2)  ///< Горизонтальное центрирование окна

/* Цвета фигур (для init_pair()) */
#define I_COLOR COLOR_CYAN  ///< I-фигура (голубой)
#define O_COLOR 226  ///< O-фигура (желтый, ANSI 256 цветов)
#define T_COLOR 202  ///< T-фигура (оранжевый)
#define S_COLOR COLOR_GREEN  ///< S-фигура (зеленый)
#define Z_COLOR COLOR_RED    ///< Z-фигура (красный)
#define J_COLOR COLOR_BLUE   ///< J-фигура (синий)
#define L_COLOR 105          ///< L-фигура (фиолетовый)

#define TIMEOUT 100
#define HEIGHT_PLAY_WIN 20
#define WIDTH_PLAY_WIN 10

/* Константы клавиш управления */
#define SPACE_KEY 32  ///< Код клавиши пробел
#define ENTER_KEY 10  ///< Код клавиши Enter
#define ESC_KEY 27    ///< Код клавиши Escape

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Инициализация библиотеки ncurses
 */
void initNcurses();
/**
 * @brief Инициализация цветовых пар
 */
void initColors();
/*
 * @brief Отрисовка начального окна
 */
void printStarsMap();
/**
 * @brief Главная функция отрисовки игры
 */
void printGame();
/**
 * @brief Отрисовка информации об игре
 */
void printGameInfo();
/**
 * @brief Отрисовывает игровое поле
 * @param[in] gi Данные игры (поле, счет, уровень)
 */
void printField(GameInfo_t gi);
/**
 * @brief Отрисовка блока "Следующая фигура"
 * @param[in] win Указатель на ncurses-окно
 */
void printNextFigure(WINDOW *win);
/**
 * @brief Отрисовка инструкции по управлению
 */
void printInstruction();
/**
 * @brief Отрисовка заголовка игры
 */
void printTitle();
/**
 * @brief Обработка ввода пользователя
 * @param input_user Ввод пользователя
 * @return Действие пользователя
 */
UserAction_t getSignal(int input_user);
#ifdef __cplusplus
}
#endif

#endif