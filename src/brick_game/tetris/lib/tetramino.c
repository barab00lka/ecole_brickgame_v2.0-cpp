
#include <stdlib.h>

#include <tetramino.h>
#include <field.h>

static int kicks_standard[4][2] = {
    {0, 0},   // сначала пробуем без смещения
    {-1, 0},  // влево
    {1, 0},   // вправо
    {0, -1}  // вверх (редко нужно, но для подстраховки)
};

static int kicks_stick[5][2] = {{0, 0}, {-2, 0}, {1, 0}, {-1, 0}, {2, 0}};

piece_t get_random_piece() {
  return (piece_t){
      .type = (shapeType)(rand() % shapeCount),
      .rotation = 0,
      .position = {0, 0},
  };
}

// returns false if there is no space left to spawn new piece
bool can_spawn_piece(piece_t *p, const unsigned *grid) {
  p->rotation = 0;
  if (check_collision(p, grid)) {
    // Сразу при появлении фигура пересекается с заполненными клетками
    return false;
  }
  return true;
}

void rotate_piece_cw(piece_t *p, const unsigned *grid) {
  int original_rotation = p->rotation;
  p->rotation = (p->rotation + 1) % TETRAMINO_ORIENTATIONS;  // пробуем повернуть

  // Проверяем сначала без смещения
  if (!check_collision(p, grid)) {
    return;  // успешно
  }

  // Выбираем набор смещений в зависимости от типа фигуры
  int(*kicks)[2];
  int kicks_count;
  if (p->type == stickShape) {
    kicks = kicks_stick;
    kicks_count = sizeof(kicks_stick) / sizeof(kicks_stick[0]);
  } else {
    kicks = kicks_standard;
    kicks_count = sizeof(kicks_standard) / sizeof(kicks_standard[0]);
  }

  // Пробуем все смещения
  for (int i = 0; i < kicks_count; i++) {
    p->position.x += kicks[i][0];
    p->position.y += kicks[i][1];
    if (!check_collision(p, grid)) {
      return;  // нашли подходящее смещение
    }
    p->position.x -= kicks[i][0];  // откатываем, если не подошло
    p->position.y -= kicks[i][1];
  }

  // Ни одно смещение не помогло – отменяем поворот
  p->rotation = original_rotation;
}

void steer_piece(UserAction_t sig, piece_t *p, const unsigned *grid) {
  char dx;
  switch (sig) {
    case Left:
      dx = -1;
      break;
    case Right:
      dx = 1;
      break;
    default:
      return;
  }

  piece_t temp = *p;
  temp.position.x += dx;
  if (!check_collision(&temp, grid)) *p = temp;
}

// Moves piece down by 1 cell. If collision detected, returns false. If no
// collision - returns true and changes piece position
bool try_drop_down(piece_t *p, const unsigned *grid) {
  piece_t moved = *p;
  moved.position.y++;
  if (!check_collision(&moved, grid)) {
    *p = moved;
    return true;
  } else {
    return false;
  }
}

void attach_piece(const piece_t *p, unsigned *field) {
  for (int i = 0; i < TETRAMINO_SIZE_MAX; ++i) {
    unsigned short row_bits =
        (shapes[p->type][p->rotation] >> (i * TETRAMINO_SIZE_MAX)) &
        0xF;  // берем по одному ряду битов из фигуры
    if (row_bits == 0) continue;
    int field_y = p->position.y + i;
    unsigned field_mask = field[field_y] << MARGIN;
    field_mask |= (unsigned)row_bits << (MARGIN + p->position.x);
    field[field_y] = field_mask >> MARGIN;
  }
}
