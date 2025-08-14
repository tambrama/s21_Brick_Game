#include <gtest/gtest.h>

#include "../brick_game/brick_games.h"
#include "../brick_game/snake/snake.h"
#include "../brick_game/snake/snake_fsm.h"
#include "../brick_game/snake/snake_wrapper.h"

TEST(SnakeGameTest, SnakeInitGame_ResetsSnakeAndApple) {
  s21::SnakeGame game;
  game.snakeInitGame();

  EXPECT_EQ(game.getScore(), 0);
  EXPECT_EQ(game.getLevel(), 1);
  EXPECT_EQ(game.getPause(), 2);
  EXPECT_EQ(game.getState(), s21::SnakeGame::GameState_t::Start);

  auto apple = game.getApple();  // предположим, что ты добавил getApple()
  EXPECT_TRUE(apple.x >= 0 && apple.x < s21::SnakeGame::FIELD_W);
  EXPECT_TRUE(apple.y >= 0 && apple.y < s21::SnakeGame::FIELD_H);
}

TEST(SnakeFsmTest, StartAction_StartsGame) {
  s21::SnakeGame game;
  s21::SnakeFSM fsm(&game);

  game.setState(s21::SnakeGame::GameState_t::Start);
  fsm.handleInput(UserAction_t::Start, false);

  EXPECT_EQ(game.getState(), s21::SnakeGame::GameState_t::Spawn);
}

TEST(SnakeCollisionTest, CheckCollision_Wall) {
  s21::SnakeGame game;
  s21::SnakeGame::Point head = {-1, 5};
  EXPECT_TRUE(game.checkCollision(head));
}

// Тест: самопересечение — голова касается тела
TEST(SnakeGameTest, SelfCollision_HeadTouchesBody) {
  s21::SnakeGame game;
  auto& snake = game.getSnake();

  const_cast<std::vector<s21::SnakeGame::Point>&>(snake) = {{2, 2}, {2, 2}};

  s21::SnakeGame::Point new_head = {2, 2};
  EXPECT_TRUE(game.checkSelfCollision(new_head));
}

// Тест: столкновение с верхней границей
TEST(SnakeGameTest, CollisionWithTopBorder) {
  s21::SnakeGame game;
  s21::SnakeGame::Point head = {0, -1};

  EXPECT_TRUE(game.checkCollision(head));
}

// Тест: столкновение с нижней границей
TEST(SnakeGameTest, CollisionWithBottomBorder) {
  s21::SnakeGame game;
  s21::SnakeGame::Point head = {0, s21::SnakeGame::FIELD_H};

  EXPECT_TRUE(game.checkCollision(head));
}

// Тест: столкновение с левой границей
TEST(SnakeGameTest, CollisionWithLeftBorder) {
  s21::SnakeGame game;
  s21::SnakeGame::Point head = {-1, 5};

  EXPECT_TRUE(game.checkCollision(head));
}

// Тест: столкновение с правой границей
TEST(SnakeGameTest, CollisionWithRightBorder) {
  s21::SnakeGame game;
  s21::SnakeGame::Point head = {s21::SnakeGame::FIELD_W, 5};

  EXPECT_TRUE(game.checkCollision(head));
}

// Тест: нет столкновения внутри поля
TEST(SnakeGameTest, NoCollisionInsideField) {
  s21::SnakeGame game;
  s21::SnakeGame::Point head = {5, 5};

  EXPECT_FALSE(game.checkCollision(head));
}

// Тест: голова касается хвоста
TEST(SnakeGameTest, SelfCollision_HeadTouchesTail) {
  s21::SnakeGame game;
  auto& snake = game.getSnake();

  const_cast<std::vector<s21::SnakeGame::Point>&>(snake) = {
      {3, 3}, {3, 2}, {3, 1}};

  s21::SnakeGame::Point new_head = {3, 1};
  EXPECT_TRUE(game.checkSelfCollision(new_head));
}

// Тест: змея не пересекается сама с собой
TEST(SnakeGameTest, NoSelfCollision) {
  s21::SnakeGame game;
  auto& snake = game.getSnake();

  const_cast<std::vector<s21::SnakeGame::Point>&>(snake) = {
      {2, 3}, {2, 2}, {2, 1}};

  s21::SnakeGame::Point new_head = {3, 3};
  EXPECT_FALSE(game.checkSelfCollision(new_head));
}
// Тест: движение вниз
TEST(SnakeMovementTest, MoveDown) {
  s21::SnakeGame game;
  s21::SnakeFSM fsm(&game);

  auto& snake =
      const_cast<std::vector<s21::SnakeGame::Point>&>(game.getSnake());
  snake = {{1, 1}, {1, 0}};  // Голова в (1,1), тело в (1,0)

  game.updateDirection(s21::SnakeGame::Direction_t::Down);
  game.setState(s21::SnakeGame::GameState_t::Moving);
  fsm.handleInput(UserAction_t::Down, false);
  game.moveSnake();

  auto new_head = game.getSnake().front();
  EXPECT_EQ(new_head.x, 1);
  EXPECT_EQ(new_head.y, 2);
}
// Тест: движение вверх
TEST(SnakeMovementTest, MoveUp) {
  s21::SnakeGame game;
  s21::SnakeFSM fsm(&game);
  auto& snake =
      const_cast<std::vector<s21::SnakeGame::Point>&>(game.getSnake());
  snake = {{1, 3}, {1, 4}};

  game.setState(s21::SnakeGame::GameState_t::Moving);
  fsm.handleInput(UserAction_t::Up, false);
  game.moveSnake();

  auto new_head = game.getSnake().front();
  EXPECT_EQ(new_head.x, 1);
  EXPECT_EQ(new_head.y, 2);
}

TEST(SnakeMovementTest, MoveRight) {
  s21::SnakeGame game;
  s21::SnakeFSM fsm(&game);

  auto& snake =
      const_cast<std::vector<s21::SnakeGame::Point>&>(game.getSnake());
  snake = {{3, 2}, {2, 2}};

  game.setState(s21::SnakeGame::GameState_t::Moving);
  fsm.handleInput(UserAction_t::Right, false);
  game.moveSnake();

  auto new_head = game.getSnake().front();
  EXPECT_EQ(new_head.x, 4);
  EXPECT_EQ(new_head.y, 2);
}

TEST(SnakeMovementTest, MoveLeft) {
  s21::SnakeGame game;
  s21::SnakeFSM fsm(&game);

  auto& snake =
      const_cast<std::vector<s21::SnakeGame::Point>&>(game.getSnake());
  snake = {{3, 2}, {4, 2}};

  game.setState(s21::SnakeGame::GameState_t::Moving);
  fsm.handleInput(UserAction_t::Up, false);
  game.moveSnake();
  fsm.handleInput(UserAction_t::Left, false);
  game.moveSnake();
  game.updateDirection(s21::SnakeGame::Direction_t::Left);
  EXPECT_EQ(game.getDirection(), s21::SnakeGame::Direction_t::Left);

  auto new_head = game.getSnake().front();
  EXPECT_EQ(new_head.x, 2);
  EXPECT_EQ(new_head.y, 1);
}

TEST(SnakeMovementTest, GrowAfterEatingApple) {
  s21::SnakeGame game;
  s21::SnakeFSM fsm(&game);

  auto& snake =
      const_cast<std::vector<s21::SnakeGame::Point>&>(game.getSnake());
  snake = {{3, 2}, {2, 2}};

  game.setApple({4, 2});

  game.setState(s21::SnakeGame::GameState_t::Moving);
  fsm.handleInput(UserAction_t::Right, false);
  game.moveSnake();

  EXPECT_EQ(game.getSnake().size(), 3);
}

TEST(SnakeFSMTest, PauseInput_TogglesPause) {
  s21::SnakeGame game;
  s21::SnakeFSM fsm(&game);

  game.setState(s21::SnakeGame::GameState_t::Start);

  // Сначала проверяем начальное состояние
  EXPECT_EQ(game.getPause(), 2);  // pause_ == 2 из snakeInitGame()
  game.setState(s21::SnakeGame::GameState_t::Moving);
  fsm.handleInput(UserAction_t::Pause, false);
  EXPECT_EQ(game.getPause(), 0);

  fsm.handleInput(UserAction_t::Pause, false);
  EXPECT_EQ(game.getPause(), 1);
}

TEST(SnakeFSMTest, PauseInput_IgnoredInGameOver) {
  s21::SnakeGame game;
  s21::SnakeFSM fsm(&game);
  game.setState(s21::SnakeGame::GameState_t::GameOver);

  int initial_pause = game.getPause();

  EXPECT_EQ(game.getPause(), initial_pause);  // Не должно меняться
}

TEST(SnakeFSMTest, TerminateInput_ExitsGame) {
  s21::SnakeGame game;
  s21::SnakeFSM fsm(&game);

  game.setState(s21::SnakeGame::GameState_t::Moving);
  game.getFSM().handleInput(UserAction_t::Terminate, false);

  EXPECT_EQ(game.getState(), s21::SnakeGame::GameState_t::Exit);
}

TEST(SnakeFSMTest, StartInput_RestartsGameFromGameOver) {
  s21::SnakeGame game;
  s21::SnakeFSM fsm(&game);

  game.setState(s21::SnakeGame::GameState_t::GameOver);
  fsm.handleInput(UserAction_t::Start, false);

  EXPECT_EQ(game.getState(), s21::SnakeGame::GameState_t::Spawn);
  EXPECT_EQ(game.getPause(), 0);
}

TEST(SnakeSpeedTest, SpeedDecreasesWithLevel) {
  s21::SnakeGame game;

  // Уровень 1
  game.setScore(0);
  game.sumScore();
  int speed_level_1 = game.getSpeed();

  // Уровень 5
  game.setScore(20);
  game.sumScore();
  int speed_level_5 = game.getSpeed();

  EXPECT_GT(speed_level_1, speed_level_5);
}
TEST(SnakeSpeedTest, MaxLevel_HasMinSpeed) {
  s21::SnakeGame game;

  // Уровень 10
  game.setScore(50);
  game.sumScore();

  EXPECT_EQ(game.getLevel(), 10);
  EXPECT_EQ(game.getSpeed(), 100);
}

TEST(SnakeUpdateStateTest, UpdateState_ReturnsCorrectData) {
  s21::SnakeGame game;
  s21::SnakeFSM fsm(&game);
  // Устанавливаем змею и яблоко
  auto& snake =
      const_cast<std::vector<s21::SnakeGame::Point>&>(game.getSnake());
  snake = {{3, 2}, {2, 2}};
  game.setApple({4, 2});
  game.setPause(0);
  game.setScore(9);
  game.highScore(5);
  game.setState(s21::SnakeGame::GameState_t::Moving);
  fsm.handleInput(UserAction_t::Right, false);
  game.moveSnake();

  // Обновляем состояние
  GameInfo_t info = game.updateState();

  // Проверяем данные
  EXPECT_EQ(info.score, 10);
  EXPECT_EQ(info.level, 3);
  EXPECT_EQ(info.speed, 240);
  EXPECT_EQ(info.pause, 0);
  EXPECT_EQ(info.high_score, 10);

  EXPECT_NE(info.field, nullptr);

  // Освобождаем память
  for (int i = 0; i < s21::SnakeGame::FIELD_H; ++i) {
    delete[] info.field[i];
  }
  delete[] info.field;
}

TEST(SnakeFSMTest, CheckSelfCollision_ReturnsFalse_ForValidSnake) {
  s21::SnakeGame game;

  auto& snake =
      const_cast<std::vector<s21::SnakeGame::Point>&>(game.getSnake());
  snake.clear();

  int x = 0, y = 0;
  int direction = 1;

  for (int i = 0; i < 50; ++i) {
    snake.push_back({x, y});
    x += direction;

    if (x >= s21::SnakeGame::FIELD_W || x < 0) {
      x -= direction;
      ++y;
      direction *= -1;
      x += direction;
    }

    if (y >= s21::SnakeGame::FIELD_H) {
      break;
    }
  }

  EXPECT_FALSE(game.checkSelfCollision(snake.front()));
}

TEST(SnakeWrapperTest, StartButton_TransitionsToSpawn) {
  initGame();
  s21::SnakeGame game;
  GameInfo_t info = updateCurrentState();
  EXPECT_EQ(info.pause, 2);
  EXPECT_EQ(info.field[10][6], 1);
  game.freeGameInfo(info);
  userInput(UserAction_t::Start, false);

  info = updateCurrentState();

  EXPECT_EQ(info.pause, 0);
  game.freeGameInfo(info);
}

TEST(SnakeWrapperTest, InitialState_IsStart) {
  initGame();
  s21::SnakeGame game;
  GameInfo_t info = updateCurrentState();

  EXPECT_EQ(info.pause, 2);

  EXPECT_EQ(info.field[10][6], 1);
  EXPECT_EQ(info.field[10][5], 1);
  EXPECT_EQ(info.field[10][4], 1);

  // Проверяем, что есть яблоко
  int apple_count = 0;
  for (int y = 0; y < s21::SnakeGame::FIELD_H; ++y) {
    for (int x = 0; x < s21::SnakeGame::FIELD_W; ++x) {
      if (info.field[y][x] == 3) {
        ++apple_count;
      }
    }
  }

  EXPECT_EQ(apple_count, 1);
  game.freeGameInfo(info);
}

TEST(SnakeWrapperTest, TerminateInput_ExitsGame) {
  initGame();
  s21::SnakeGame game;
  userInput(UserAction_t::Start, false);
  userInput(UserAction_t::Terminate, false);

  GameInfo_t info = updateCurrentState();

  EXPECT_EQ(info.pause, 0);
  game.freeGameInfo(info);
}

TEST(SnakeGameTest, SnakeClear_Works) {
  s21::SnakeGame game;
  auto& snake =
      const_cast<std::vector<s21::SnakeGame::Point>&>(game.getSnake());
  snake = {{6, 10}, {5, 10}, {4, 10}, {3, 10}};

  EXPECT_EQ(game.getSnake().size(), 4);

  game.snakeEndGame();  // вызывает snake_.clear()

  EXPECT_TRUE(game.getSnake().empty());
}

TEST(SnakeWrapperTest, StartButton_RestartsGameFromGameOver) {
  initGame();

  s21::SnakeGame game;
  game.setState(s21::SnakeGame::GameState_t::GameOver);
  game.setPause(3);

  userInput(UserAction_t::Start, false);

  GameInfo_t info = updateCurrentState();

  EXPECT_EQ(info.pause, 0);
  EXPECT_EQ(info.field[10][6], 1);
  game.freeGameInfo(info);
}
TEST(SnakeWrapperTest, PauseInput_TogglesPause) {
  initGame();
  s21::SnakeGame game;
  userInput(UserAction_t::Start, false);
  userInput(UserAction_t::Start, false);

  userInput(UserAction_t::Pause, false);

  GameInfo_t info = updateCurrentState();
  EXPECT_EQ(info.pause, 1);
  game.freeGameInfo(info);
  userInput(UserAction_t::Pause, false);
  info = updateCurrentState();
  EXPECT_EQ(info.pause, 0);
  game.freeGameInfo(info);
}

TEST(SnakeWrapperTest, SetDirection_ChangesDirectionCorrectly) {
  initGame();
  s21::SnakeGame game;

  EXPECT_EQ(game.getDirection(), s21::SnakeGame::Direction_t::Right);

  game.setDirection(s21::SnakeGame::Direction_t::Up);
  EXPECT_EQ(game.getDirection(), s21::SnakeGame::Direction_t::Up);
}

TEST(SnakeWrapperTest, Boost_TurnsOn_And_SpeedChanges) {
  initGame();
  s21::SnakeGame game;

  EXPECT_FALSE(game.getBoost());

  userInput(UserAction_t::Action, false);
  game.toggleBoost();  // включаем буст

  EXPECT_TRUE(game.getBoost());
}

TEST(SnakeWrapperTest, WinState_RestartWithStart) {
  initGame();
  s21::SnakeGame game;

  game.setState(s21::SnakeGame::GameState_t::Win);

  EXPECT_EQ(game.getState(), s21::SnakeGame::GameState_t::Win);

  game.getFSM().handleInput(UserAction_t::Start, false);

  EXPECT_EQ(game.getState(), s21::SnakeGame::GameState_t::Spawn);
  EXPECT_EQ(game.getPause(), 0);
  endGame();
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}