#ifndef ENTITIES_H_
#define ENTITIES_H_

#include <defines.h>

// Индексы фигур для понятного обращения
typedef enum {
  tShape = 0,
  gShape,
  lShape,
  squareShape,
  stickShape,
  rzShape,
  zShape,
  shapeCount,  // anchor, for enum count
} shapeType;

typedef struct {
  shapeType type;
  short rotation;
  struct {
    int x, y;
  } position;
} piece_t;

// FSM States
typedef enum {
  startState,
  spawnState,
  movingState,
  clearingState,
  pausedState,
  gameOverState,
  exitState,
  StateCount,  // anchor, for enum count
} tetrisState;

typedef struct statistics {
  unsigned score;
  unsigned high_score;
  unsigned level;
} stats_t;

typedef struct {
  // игровая сетка в виде 20 32битных беззнаковых чисел.
  // младшие 10 битов используются для представления сетки, последний бит для
  // логики анимации
  unsigned field[FIELD_ROWS];
  piece_t current_piece;
  piece_t next_piece;
  stats_t stats;
  tetrisState state;
  unsigned ticker;
  unsigned timer;

} tetrisCtx_t;

#endif
