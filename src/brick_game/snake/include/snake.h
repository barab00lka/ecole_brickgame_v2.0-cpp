#ifndef SNAKE_H_
#define SNAKE_H_

#include <array>

#include <brickgame_api.h>
#include <defines.h>
#include <fsm.h>

namespace ec {
static constexpr int SnakeLenInit = 4;
static constexpr int SnakeLenMax = FIELD_COLS * FIELD_ROWS;

struct Pos2d {
  int y;
  int x;

  bool operator==(const Pos2d &o) const { return o.y == y && o.x == x; }
  bool operator!=(const Pos2d &o) const { return !(*this == o); }
  Pos2d operator+(const Pos2d &o) const { return Pos2d{o.y + y, o.x + x}; }
  Pos2d &operator+=(const Pos2d &o) {
    this->y += o.y, this->x += o.x;
    return *this;
  }
};

class SnakeCtx {
  using StateHandler = void (SnakeCtx::*)(UserAction_t);
  static const StateHandler handlers_[stateCount];

 public:
  static constexpr Pos2d EmptyCell = {-2, -2};
  static constexpr Pos2d dirL = Pos2d{0, -1};
  static constexpr Pos2d dirR = Pos2d{0, 1};
  static constexpr Pos2d dirU = Pos2d{-1, 0};
  static constexpr Pos2d dirD = Pos2d{1, 0};

  SnakeCtx();
  void DispatchSignal(UserAction_t sig);
  void PlotCtx(int **pfield);

  int get_score() const { return len_body_ - SnakeLenInit; }
  int get_level() const { return get_score() / 5; }
  int get_hiscore() const { return hiscore_; }
  SnakeState get_current_state() const { return state_; }
  int get_ticker() const { return ticker_; }
  Pos2d get_head_pos() const { return snake_[0]; }
  Pos2d get_curdir() const { return current_dir_; }
  void unset_dir_changed() { dir_changed_ = false; }
  int tick() { return ++ticker_; }
  void reset_tickers() { ticker_ = 0; }

 private:
  void NewSnake();
  Pos2d GetRandomEmptyCell() const;
  void GrowSnake(const Pos2d p);
  void TryChangeDir(const Pos2d dir);
  // returns false if cant move further
  bool TryMoveSnake(const Pos2d d);

  void OnStart(UserAction_t sig);
  void OnMoving(UserAction_t sig);
  void OnPaused(UserAction_t sig);
  void OnGameOver(UserAction_t sig);

  std::array<Pos2d, SnakeLenMax> snake_;
  Pos2d current_dir_;
  Pos2d apple_;
  int len_body_;
  int lvl_;
  int hiscore_;
  int ticker_;
  bool dir_changed_;
  SnakeState state_;
};

}  // namespace ec

#endif
