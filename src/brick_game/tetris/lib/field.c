#include "../include/field.h"

int complete_linecount(unsigned *grid) {
  int ans = 0;
  for (int i = 0; i < FIELD_ROWS; ++i) {
    if (IS_FULL_LINE(grid[i])) {
      SET_MARK(grid[i]);
      ans += 1;
    }
  }
  return ans;
}

bool check_collision(const piece_t *p, const unsigned *field) {
  for (int i = 0; i < TETRAMINO_SIZE_MAX; i++) {
    unsigned row_bits =
        (shapes[p->type][p->rotation] >> (i * TETRAMINO_SIZE_MAX)) & 0xF;
    if (row_bits == 0) continue;  // если в ряду клеток нет, можно пропустить

    int rel_y = p->position.y + i;
    if (rel_y < 0 || rel_y >= FIELD_ROWS)  // верхняя и нижняя граница
      return true;

    unsigned rel_mask = row_bits << (MARGIN + p->position.x);

    if (rel_mask & ~(COMPLETE_LINE << MARGIN))  // левая и правая граница
      return true;
    else if (rel_mask & (field[rel_y] << MARGIN))  // уже занятые клетки
      return true;
  }
  return false;
}

// Clears full lines and returns how much are full
int clearing_animation(unsigned *field) {
  int cleared = 0;
  for (int r_row = FIELD_ROWS - 1; r_row >= 0; --r_row) {
    if (IS_MARKED(field[r_row]) && (COMPLETE_LINE & field[r_row])) {
      cleared += 1;

      int i = FIELD_COLS;
      while (!(field[r_row] & 1U << i)) --i;

      int k = 0;
      while (!(field[r_row] & 1U << k)) ++k;

      field[r_row] ^= 1U << i, field[r_row] ^= 1U << k;
    }
  }
  return cleared;
}

void shift_remove_lines(unsigned *field) {
  int w_row = FIELD_ROWS - 1;
  for (int n = FIELD_ROWS - 1; n >= 0; --n) {
    if (!IS_MARKED(field[n])) {
      field[w_row--] = field[n];
    }
  }
  // Зануляем оставшиеся верхние строки
  while (w_row >= 0) {
    field[w_row--] = 0U;
  }
}

void clear_field(unsigned *field) {
  for (int n = 0; n < FIELD_ROWS; ++n) field[n] = 0U;
}
