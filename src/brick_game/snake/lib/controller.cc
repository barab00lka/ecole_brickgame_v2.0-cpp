#ifdef DEBUG
#include <stdexcept>
#endif

#include <brickgame_api.h>
#include <defines.h>
#include <snake.h>
#include <cassert>

using namespace ec;

int field_buf[FIELD_ROWS][FIELD_COLS];
int *field_ptr[FIELD_ROWS];
int next_buf[4][4];
int *next_ptr[4];

static SnakeCtx ctx = SnakeCtx();

void SnakeCtx::PlotCtx(int **field_ptr) {
  assert(field_ptr != nullptr);
  for (int y = 0; y < FIELD_ROWS; ++y) {
	assert(field_ptr[y] != nullptr);
    for (int x = 0; x < FIELD_COLS; ++x) {
      field_ptr[y][x] = state_ == gameOverState ? appleCell : 0;
	}
  }

  if (state_ == movingState) field_ptr[apple_.y][apple_.x] = appleCell;

  int i = 0;
  Pos2d pos = snake_[i];
  if (pos.y >= 0 && pos.y < FIELD_ROWS && pos.x >= 0 && pos.x < FIELD_COLS)
    field_ptr[pos.y][pos.x] = snakeHeadCell;
  i += 1;
  for (; i < len_body_; ++i) {
    pos = snake_[i];
    if (pos.y >= 0 && pos.y < FIELD_ROWS && pos.x >= 0 && pos.x < FIELD_COLS)
      field_ptr[pos.y][pos.x] = snakeBodyCell;
  }
  if (false == true) int boom = 1;
}

void userInput(UserAction_t action, bool hold) {
  (void)hold;
  ctx.DispatchSignal(action);
}

GameInfo_t updateCurrentState() {
  if (ctx.get_current_state() == movingState) {
    if (ctx.tick() >= SNAKE_TICKRATE - SNAKE_SPEED_STEP * ctx.get_level()) {
      ctx.DispatchSignal(TimerTick);
      ctx.unset_dir_changed();
    }
  }

  ctx.PlotCtx(field_ptr);

  return (GameInfo_t){
      .field = field_ptr,
      .next = next_ptr,
      .score = ctx.get_score(),
      .high_score = ctx.get_hiscore(),
      .level = ctx.get_level(),
      .speed = SNAKE_SPEED_STEP * ctx.get_level(),
      .pause = (ctx.get_current_state() == pausedState),
  };
}

#ifdef DEBUG
#include <curses.h>
void debug_print_ctx() {
  switch (ctx.get_current_state()) {
    case startState:
      mvprintw(19, 25, "state: %-10s", "start");
      break;
    case movingState:
      mvprintw(19, 25, "state: %-10s", "moving");
      break;
    case pausedState:
      mvprintw(19, 25, "state: %-10s", "paused");
      break;
    case gameOverState:
      mvprintw(19, 25, "state: %-10s", "gameOver");
      break;
    default:
      mvprintw(19, 25, "state: %-10s", "no_state");
      break;
  }
  mvprintw(20, 25, "ticker: %d", ctx.get_ticker());
  mvprintw(21, 25, "tickrate: %llu",
           (unsigned long long)(SNAKE_TICKRATE - SNAKE_SPEED_STEP * ctx.get_level()));
  mvprintw(22, 50, "head moved to %d %d", ctx.get_head_pos().y,
           ctx.get_head_pos().x);
  mvprintw(23, 50, "current dir %d %d", ctx.get_curdir().y, ctx.get_curdir().x);
}
#endif
