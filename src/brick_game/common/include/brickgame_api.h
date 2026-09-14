#ifndef API_H_
#define API_H_

#include <stdbool.h>
#include <stdio.h>

#include <defines.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action,
  TimerTick,
  NoOp
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  bool pause;
} GameInfo_t;

void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState();

typedef enum {
  emptyCell,
  redBlockCell,
  yellowBlockCell,
  greenBlockCell,
  blueBlockCell,
  purpleBlockCell,
  snakeHeadCell,
  snakeBodyCell,
  appleCell,
  countCells,
} BrickGameCell_t;

extern int field_buf[FIELD_ROWS][FIELD_COLS];
extern int *field_ptr[FIELD_ROWS];
extern int next_buf[4][4];
extern int *next_ptr[4];

// Инициализация указателей (вызывается один раз в main)
static inline void init_viewbuffers() {
  for (int i = 0; i < FIELD_ROWS; ++i) field_ptr[i] = field_buf[i];
  for (int i = 0; i < 4; ++i) next_ptr[i] = next_buf[i];
}

#ifdef __cplusplus
}
#endif
#endif
