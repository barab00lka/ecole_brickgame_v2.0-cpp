#include <fsm.h>

#include <save.h>
#include <snake.h>

namespace ec {

// Order of initializtion musnt be changed!
const SnakeCtx::StateHandler SnakeCtx::handlers_[stateCount] = {
    &SnakeCtx::OnStart,     // startState
    &SnakeCtx::OnMoving,    // movingState
    &SnakeCtx::OnPaused,    // pausedState
    &SnakeCtx::OnGameOver,  // gameOverState
};

void SnakeCtx::DispatchSignal(UserAction_t sig) {
  auto handler = handlers_[state_];
  if (handler) (this->*handler)(sig);
}

void SnakeCtx::OnStart(UserAction_t sig) {
  if (sig == Start) {
    hiscore_ = fetch_high_score(SNAKE_SAVE_FN);
    NewSnake();
    apple_ = GetRandomEmptyCell();
    current_dir_ = dirL;
    ticker_ = 0;
    lvl_ = 0;
    state_ = movingState;
  } else if (sig == Terminate) {
    if (hiscore_ > fetch_high_score(SNAKE_SAVE_FN))
      save_new_high_score(hiscore_, SNAKE_SAVE_FN);
  }
}

void SnakeCtx::OnMoving(UserAction_t sig) {
  switch (sig) {
    case Terminate:
      if (hiscore_ > fetch_high_score(SNAKE_SAVE_FN))
        save_new_high_score(hiscore_, SNAKE_SAVE_FN);
      break;
    case Left:
      TryChangeDir(dirL);
      break;
    case Right:
      TryChangeDir(dirR);
      break;
    case Up:
      TryChangeDir(dirU);
      break;
    case Down:
      TryChangeDir(dirD);
      break;
    case Action:
    case TimerTick:
      reset_tickers();
      if (!TryMoveSnake(current_dir_)) {
        if (get_score() > hiscore_) hiscore_ = get_score();
        state_ = gameOverState;
        return;
      }
      if (snake_[0] == apple_) {
        GrowSnake(apple_);
        apple_ = GetRandomEmptyCell();
        if (get_score() % 5 == 0 && lvl_ < 10) lvl_++;
      }
      if (hiscore_ > fetch_high_score(SNAKE_SAVE_FN))
        save_new_high_score(hiscore_, SNAKE_SAVE_FN);
      break;
    case Pause:
      state_ = pausedState;
      break;
    default:
      break;
  }
}

void SnakeCtx::OnPaused(UserAction_t sig) {
  if (sig == Pause)
    state_ = movingState;
  else if (sig == Terminate) {
    if (hiscore_ > fetch_high_score(SNAKE_SAVE_FN))
      save_new_high_score(hiscore_, SNAKE_SAVE_FN);
  }
}

void SnakeCtx::OnGameOver(UserAction_t sig) {
  save_new_high_score(hiscore_, SNAKE_SAVE_FN);
  if (sig == Start) state_ = startState;
}

}  // namespace ec
