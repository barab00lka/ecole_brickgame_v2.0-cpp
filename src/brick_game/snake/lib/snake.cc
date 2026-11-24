#include "../include/snake.h"

#include <stdlib.h>

#include <array>
#ifdef DEBUG
#include <stdexcept>
#endif
using namespace ec;

SnakeCtx::SnakeCtx() {
  state_ = startState;
  ticker_ = lvl_ = hiscore_ = 0;
  apple_ = EmptyCell;
  current_dir_ = dirL;
  dir_changed_ = false;
  NewSnake();
}

void SnakeCtx::NewSnake() {
#ifdef DEBUG
  if (FIELD_COLS <= SnakeLenInit || FIELD_ROWS <= SnakeLenInit)
    throw std::length_error("Incorrect size of game field.");
  else if (SnakeLenInit >= SnakeLenMax)
    throw std::length_error("Incorrect snake length");
#endif

  /* Place snake in the center */
  int startY = FIELD_ROWS / 2, startX = 3;

  snake_.fill(EmptyCell);

  for (int i = 0; i < SnakeLenInit; ++i) {
    snake_[i] = {startY, startX + i};
  }

  len_body_ = SnakeLenInit;
}

// returns false if cant move further
bool SnakeCtx::TryMoveSnake(const Pos2d d) {
  Pos2d new_pos_head = snake_[0] + d;
  if (new_pos_head.x == FIELD_COLS || new_pos_head.y == FIELD_ROWS ||
      new_pos_head.x == -1 || new_pos_head.y == -1)
    return false;

  for (int idx = 1; idx < len_body_; ++idx)
    if (snake_[idx] == new_pos_head) return false;

  for (int idx = len_body_ - 1; idx > 0; --idx) {
    snake_[idx] = snake_[idx - 1];
  }
  snake_[0] = new_pos_head;

  return true;
}

void SnakeCtx::TryChangeDir(Pos2d dir) {
  if (dir_changed_) return;

  Pos2d opposite;
  opposite.x = -current_dir_.x;
  opposite.y = -current_dir_.y;
  if (dir != current_dir_ && dir != opposite) {
    current_dir_ = dir;
    dir_changed_ = true;
  }
}

void SnakeCtx::GrowSnake(const Pos2d p) {
  if (p != snake_[0] || len_body_ == SnakeLenMax) return;
  for (int i = len_body_ - 1; i >= 0; --i) snake_[i + 1] = snake_[i];

  snake_[0] = p;
  len_body_ += 1;
  int new_score = get_score();
  if (new_score > hiscore_) hiscore_ = new_score;
}

Pos2d SnakeCtx::GetRandomEmptyCell() const {
  bool field[FIELD_ROWS][FIELD_COLS] = {};
  int node_idx = 0;
  Pos2d node_pos = EmptyCell;

  while (node_idx < SnakeLenMax && node_idx != len_body_) {
    node_pos = snake_[node_idx++];
#ifdef DEBUG
    if (node_pos == EmptyCell)
      throw std::out_of_range("snake_ body_ is not initialized");
#endif

    field[node_pos.y][node_pos.x] = (node_pos != EmptyCell);
  }

  std::array<Pos2d, SnakeLenMax> free_cells;
  int free_count = 0;
  for (int y = 0; y < FIELD_ROWS; ++y) {
    for (int x = 0; x < FIELD_COLS; ++x) {
      if (!field[y][x]) {
        free_cells[free_count].y = y;
        free_cells[free_count].x = x;
        free_count += 1;
      }
    }
  }
#ifdef DEBUG
  if (free_count == 0) {
    throw std::out_of_range("All cells are occupied");
  }
#endif
  return free_cells[rand() % free_count];
}
