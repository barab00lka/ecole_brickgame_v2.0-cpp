#include "../../include/view.h"
#include <assert.h>

#include "../../include/tetramino.h"

void plot_next(tetrisCtx_t *ctx, int **v) {
  assert(v);
  unsigned short mask = shapes[ctx->next_piece.type][ctx->next_piece.rotation];
  for (int y = 0; y < TETRAMINO_SIZE_MAX; y++) {
	assert(v[y]);
    unsigned short row = (mask >> (y * TETRAMINO_SIZE_MAX)) & 0xF;
    for (int x = 0; x < TETRAMINO_SIZE_MAX; x++) {
      v[y][x] = row & (1 << x) ? redBlockCell : emptyCell;
    }
  }
}

void plot_field(tetrisCtx_t *ctx, int **v) {
  assert(v);
  if (ctx->state == pausedState) {
    for (int y = 0; y < FIELD_ROWS; y++) {
	  assert(v[y]);
      for (int x = 0; x < FIELD_COLS; x++) {
        v[y][x] = !((y >= 5 && y <= 15) && (x == 7 || x == 2)) ? redBlockCell
                                                               : emptyCell;
      }
    }
  } else {
    // Заполнение field_buf из поля
    for (int y = 0; y < FIELD_ROWS; y++) {
	  assert(v[y]);
      unsigned row = ctx->field[y];
      for (int x = 0; x < FIELD_COLS; x++) {
        v[y][x] = (row >> x) & 1 ? redBlockCell : emptyCell;
      }
    }
  }

  // Наложение текущей фигуры
  if (ctx->state == movingState) {
    piece_t *p = &ctx->current_piece;
    unsigned short mask = shapes[p->type][p->rotation];
    for (int i = 0; i < TETRAMINO_SIZE_MAX; i++) {
      unsigned short row = (mask >> (i * TETRAMINO_SIZE_MAX)) & 0xF;
      int fy = p->position.y + i;
      if (fy < 0 || fy >= FIELD_ROWS) continue;
      for (int x = 0; x < TETRAMINO_SIZE_MAX; x++) {
        if (row & (1 << x)) {
          int fx = p->position.x + x;
          if (fx >= 0 && fx < FIELD_COLS) v[fy][fx] = redBlockCell;
        }
      }
    }
  }
}
