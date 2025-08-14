#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>

#include "snake.h"

namespace s21 {

///////////////////////////гетеры

SnakeFSM& SnakeGame::getFSM() { return *fsm_; }
int SnakeGame::getScore() { return score_; }
int SnakeGame::getHighScore() { return high_score_; }
int SnakeGame::getLevel() { return level_; }
int SnakeGame::getSpeed() { return speed_; }
int SnakeGame::getPause() { return pause_; }
SnakeGame::GameState_t SnakeGame::getState() const { return state_; }
SnakeGame::Direction_t SnakeGame::getDirection() const { return direction_; }
SnakeGame::Point SnakeGame::getApple() const { return apple_; }
const std::vector<SnakeGame::Point>& SnakeGame::getSnake() const {
  return snake_;
}
bool SnakeGame::getBoost() const { return boost_; }

/////////////////////сетеры
void SnakeGame::setState(GameState_t state) { state_ = state; }
void SnakeGame::setScore(int score) { score_ = score; }
void SnakeGame::highScore(int high_score) { high_score_ = high_score; }
void SnakeGame::setDirection(Direction_t direction) { direction_ = direction; }
void SnakeGame::setPause(int pause) { pause_ = pause; }
void SnakeGame::setApple(const Point& apple) { apple_ = apple; }

////////////вспомогательные переключатели
void SnakeGame::togglePause() { pause_ = pause_ == 0 ? 1 : 0; }
void SnakeGame::toggleBoost() { boost_ = !boost_; }

SnakeGame::SnakeGame() {
  fsm_ = std::make_unique<SnakeFSM>(this);
  std::srand(std::time(nullptr));  // инициализация генератора случайных чисел
  snakeInitGame();
}
SnakeGame::~SnakeGame() { writeHighScore(high_score_); }

void SnakeGame::snakeInitGame() {
  for (auto& row : field_) {
    std::fill(row.begin(), row.end(), 0);
  }
  snake_.clear();
  snake_ = {{6, 10}, {5, 10}, {4, 10}, {3, 10}};
  direction_ = Direction_t::Right;
  next_direction_ = Direction_t::None;
  score_ = 0;
  high_score_ = readHighScore();
  level_ = 1;
  speed_ = 300;
  pause_ = 2;
  state_ = GameState_t::Start;
  boost_ = false;
  generateApple();

  last_move_time_ = std::chrono::steady_clock::now();

  field_.resize(FIELD_H, std::vector<int>(FIELD_W, 0));

  for (const auto& p : snake_) {
    if (p.x >= 0 && p.x < FIELD_W && p.y >= 0 && p.y < FIELD_H) {
      field_[p.y][p.x] = 1;
    }
  }

  if (apple_.x >= 0 && apple_.x < FIELD_W && apple_.y >= 0 &&
      apple_.y < FIELD_H) {
    field_[apple_.y][apple_.x] = 3;
  }
}

void SnakeGame::generateApple() {
  while (1) {
    apple_.x = std::rand() % FIELD_W;
    apple_.y = std::rand() % FIELD_H;
    if (std::find(snake_.begin(), snake_.end(), apple_) == snake_.end()) {
      break;
    }
  }
}

GameInfo_t SnakeGame::updateState() {
  GameInfo_t info = {};
  if (info.field != nullptr) {
    for (int i = 0; i < FIELD_H; ++i) {
      if (info.field[i] != nullptr) {
        delete[] info.field[i];
      }
    }
    delete[] info.field;
    info.field = nullptr;
  }

  info.field = new int*[FIELD_H];
  for (int i = 0; i < FIELD_H; ++i) {
    info.field[i] = new int[FIELD_W];

    for (int j = 0; j < FIELD_W; ++j) {
      info.field[i][j] = field_[i][j];  // Копируем данные из vector
    }
  }
  info.next = nullptr;
  info.score = getScore();
  info.high_score = getHighScore();
  info.level = getLevel();
  info.speed = getSpeed();
  info.pause = getPause();

  if (info.pause == 0) timing();

  if (score_ > info.high_score) {
    info.high_score = score_;
  }

  return info;
}

void SnakeGame::timing() {
  auto now = std::chrono::steady_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                     now - last_move_time_)
                     .count();
  if (elapsed <= 0) {
    return;
  }

  int current_speed = boost_ ? boost_speed_ : speed_;
  if (elapsed >= current_speed) {
    moveSnake();
    last_move_time_ = now;
  }
}

void SnakeGame::sumScore() {
  int lvl = (score_ / 5) + 1;
  level_ = (lvl > MAX_LEVEL) ? 10 : lvl;

  speed_ = std::max(100, 300 - (level_ * 20));
  if (score_ > high_score_) {
    high_score_ = score_;
    writeHighScore(score_);
  }
}

int SnakeGame::readHighScore() {
  std::ifstream file("high_score_sn.txt");
  int score = 0;
  if (file) {
    file >> score;
    file.close();
  }
  return score;
}

void SnakeGame::writeHighScore(int score) {
  std::ofstream file("high_score_sn.txt");
  if (file) {
    file << score;
    file.close();
  }
}

void SnakeGame::updateDirection(Direction_t new_dir) {
  if (new_dir == Direction_t::None) return;

  if ((new_dir == Direction_t::Up && direction_ != Direction_t::Down) ||
      (new_dir == Direction_t::Down && direction_ != Direction_t::Up) ||
      (new_dir == Direction_t::Left && direction_ != Direction_t::Right) ||
      (new_dir == Direction_t::Right && direction_ != Direction_t::Left)) {
    next_direction_ = new_dir;
  }
}

void SnakeGame::moveSnake() {
  if (next_direction_ != Direction_t::None) {
    direction_ = next_direction_;
    next_direction_ = Direction_t::None;
  }

  if (state_ != GameState_t::Moving) return;

  Point new_head = snake_.front();
  switch (direction_) {
    case Direction_t::Up:
      new_head.y -= 1;
      break;
    case Direction_t::Down:
      new_head.y += 1;
      break;
    case Direction_t::Left:
      new_head.x -= 1;
      break;
    case Direction_t::Right:
      new_head.x += 1;
      break;
    default:
      return;
  }

  if (checkCollision(new_head) || checkSelfCollision(new_head)) {
    state_ = GameState_t::GameOver;
    pause_ = 3;
    return;
  }

  clearOldPosition();
  snake_.insert(snake_.begin(), new_head);

  if (new_head == apple_) {
    score_++;
    generateApple();
    sumScore();
    if (snake_.size() >= MAX_SNAKE_LENGTH) {
      state_ = GameState_t::Win;
      pause_ = 4;
    }
  } else {
    snake_.pop_back();
  }

  drawNewPosition();
}

bool SnakeGame::checkCollision(const Point& new_head) {
  return new_head.x < 0 || new_head.x >= FIELD_W || new_head.y < 0 ||
         new_head.y >= FIELD_H;
}

bool SnakeGame::checkSelfCollision(const Point& new_head) {
  for (size_t i = 1; i < snake_.size(); ++i) {
    if (new_head == snake_[i]) return true;
  }
  return false;
}

void SnakeGame::clearOldPosition() {
  const auto& tail = snake_.back();
  if (tail.x >= 0 && tail.x < FIELD_W && tail.y >= 0 && tail.y < FIELD_H) {
    field_[tail.y][tail.x] = 0;
  }
}

void SnakeGame::drawNewPosition() {
  const auto& head = snake_.front();
  if (head.x >= 0 && head.x < FIELD_W && head.y >= 0 && head.y < FIELD_H) {
    field_[head.y][head.x] = 1;
  }

  if (apple_.x >= 0 && apple_.x < FIELD_W && apple_.y >= 0 &&
      apple_.y < FIELD_H) {
    field_[apple_.y][apple_.x] = 3;
  }
}

void SnakeGame::snakeEndGame() { snake_.clear(); }

void SnakeGame::freeGameInfo(GameInfo_t& info) {
  for (int i = 0; i < SnakeGame::FIELD_H; ++i) {
    delete[] info.field[i];
  }
  delete[] info.field;
  info.field = nullptr;
}

}  // namespace s21