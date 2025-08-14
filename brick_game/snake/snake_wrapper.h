#ifndef SNAKE_WRAPPER_H
#define SNAKE_WRAPPER_H
/**
 * @file    snake_wrapper.h
 * @brief   Интерфейс взаимодействия с логикой игры Snake
 * @author  Tambrama
 * @date    2025
 *
 * - Обёртка над основной логикой
 * - Упрощённый доступ к данным игры
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Инициализация игры
 */
void initGame();
/**
 * @brief Завершение игры
 */
void endGame();

#ifdef __cplusplus
}
#endif

#endif  // SNAKE_WRAPPER_H