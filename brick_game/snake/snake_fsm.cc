#include "snake_fsm.h"

#include <iostream>

#include "../brick_games.h"
#include "snake.h"

namespace s21 {

SnakeFSM::SnakeFSM(SnakeGame* game) : game_(game) {}

void SnakeFSM::handleInput(UserAction_t action, bool hold) {
  if (!game_) return;
  if (hold) {
  }

  switch (game_->getState()) {
    case SnakeGame::GameState_t::Start:
      if (action == UserAction_t::Start) {
        game_->setState(SnakeGame::GameState_t::Spawn);
        game_->setPause(0);
      } else if (action == UserAction_t::Terminate) {
        game_->setState(SnakeGame::GameState_t::Exit);
      }
      break;

    case SnakeGame::GameState_t::Spawn:
      game_->setState(SnakeGame::GameState_t::Moving);
      if (action == UserAction_t::Pause) game_->togglePause();
      break;
    case SnakeGame::GameState_t::Moving:
      if (action == UserAction_t::Pause)
        game_->togglePause();
      else if (action == UserAction_t::Up)
        game_->updateDirection(SnakeGame::Direction_t::Up);
      else if (action == UserAction_t::Down)
        game_->updateDirection(SnakeGame::Direction_t::Down);
      else if (action == UserAction_t::Left)
        game_->updateDirection(SnakeGame::Direction_t::Left);
      else if (action == UserAction_t::Right)
        game_->updateDirection(SnakeGame::Direction_t::Right);
      else if (action == UserAction_t::Action) {
        game_->toggleBoost();
      } else if (action == UserAction_t::Terminate) {
        game_->setState(SnakeGame::GameState_t::Exit);
      }
      break;

    case SnakeGame::GameState_t::GameOver:
      game_->setPause(3);
      if (action == UserAction_t::Start) {
        game_->snakeInitGame();
        game_->setState(SnakeGame::GameState_t::Spawn);
        game_->setPause(0);
      } else if (action == UserAction_t::Terminate) {
        game_->setState(SnakeGame::GameState_t::Exit);
      }
      break;

    case SnakeGame::GameState_t::Win:
      game_->setPause(4);
      if (action == UserAction_t::Start) {
        game_->snakeInitGame();
        game_->setState(SnakeGame::GameState_t::Spawn);
        game_->setPause(0);
      } else if (action == UserAction_t::Terminate) {
        game_->setState(SnakeGame::GameState_t::Exit);
      }
      break;

    case SnakeGame::GameState_t::Exit:
      game_->snakeEndGame();
      break;

    default:
      break;
  }
}

}  // namespace s21
