#include <check.h>
#include <stdlib.h>

#include "../include/field.h"
#include "../include/tetramino.h"
#include "test_field.h"

/* ============================================================
   Тесты для get_random_piece
   ============================================================ */
START_TEST(test_get_random_piece_range) {
  srand(0);  // фиксируем seed для воспроизводимости
  for (int i = 0; i < 100; i++) {
    piece_t p = get_random_piece();
    ck_assert_int_ge(p.type, 0);
    ck_assert_int_lt(p.type, shapeCount);
    ck_assert_int_ge(p.rotation, 0);
    ck_assert_int_lt(p.rotation, TETRAMINO_ORIENTATIONS);
  }
}
END_TEST

/* ============================================================
   Тесты для can_spawn_piece
   ============================================================ */
START_TEST(test_can_spawn_piece_true) {
  uint32_t field[FIELD_ROWS] = {0};
  piece_t p = {0};
  p.type = tShape;
  p.rotation = 0;
  p.position.x = 3;
  p.position.y = 0;
  bool result = can_spawn_piece(&p, field);
  ck_assert(result == true);
  // После вызова rotation должен стать 0
  ck_assert_int_eq(p.rotation, 0);
}
END_TEST

START_TEST(test_can_spawn_piece_false) {
  uint32_t field[FIELD_ROWS] = {0};
  // Заполняем место, где появится фигура
  //
  field[1] = 1U << 2;
  piece_t p = {0};
  p.type = tShape;
  p.rotation = 0;
  p.position.x = 2;
  p.position.y = 0;
  bool result = can_spawn_piece(&p, field);
  ck_assert(result == false);
}
END_TEST

/* ============================================================
   Тесты для rotate_piece_cw
   ============================================================ */
START_TEST(test_rotate_piece_cw_no_collision) {
  uint32_t field[FIELD_ROWS] = {0};
  piece_t p = {0};
  p.type = tShape;
  p.rotation = 0;
  p.position.x = 3;
  p.position.y = 5;
  rotate_piece_cw(&p, field);
  // Ротация должна изменить rotation на 1 (если не было коллизий)
  ck_assert_int_eq(p.rotation, 1);
  ck_assert(!check_collision(&p, field));
  // Проверим, что фигура не вышла за границы (но это уже в check_collision)
}
END_TEST

/* ============================================================
   Тесты для steer_piece
   ============================================================ */
START_TEST(test_steer_piece_left) {
  uint32_t field[FIELD_ROWS] = {0};
  piece_t p = {0};
  p.type = tShape;
  p.rotation = 0;
  p.position.x = 3;
  p.position.y = 5;
  int original_x = p.position.x;
  steer_piece(Left, &p, field);
  ck_assert_int_eq(p.position.x, original_x - 1);
  ck_assert_int_eq(p.position.y, 5);  // y не меняется
}
END_TEST

START_TEST(test_steer_piece_right) {
  uint32_t field[FIELD_ROWS] = {0};
  piece_t p = {0};
  p.type = tShape;
  p.rotation = 0;
  p.position.x = 3;
  p.position.y = 5;
  int original_x = p.position.x;
  steer_piece(Right, &p, field);
  ck_assert_int_eq(p.position.x, original_x + 1);
}
END_TEST

START_TEST(test_steer_piece_blocked) {
  uint32_t field[FIELD_ROWS] = {0};
  // Ставим препятствие справа от фигуры
  piece_t p = {0};
  p.type = tShape;
  p.rotation = 0;
  p.position.x = 3;
  p.position.y = 5;
  // Предположим, что фигура занимает столбцы 3-5 (при x=3)
  // Заблокируем столбец 6 (справа)
  field[6] = 1U << 6;  // клетка (6,5) занята
  int original_x = p.position.x;
  steer_piece(Right, &p, field);
  // Движение должно быть заблокировано
  ck_assert_int_eq(p.position.x, original_x);
}
END_TEST

/* ============================================================
   Тесты для try_drop_down
   ============================================================ */
START_TEST(test_try_drop_down_success) {
  uint32_t field[FIELD_ROWS] = {0};
  piece_t p = {0};
  p.type = tShape;
  p.rotation = 0;
  p.position.x = 3;
  p.position.y = 5;
  int original_y = p.position.y;
  bool result = try_drop_down(&p, field);
  ck_assert(result == true);
  ck_assert_int_eq(p.position.y, original_y + 1);
}
END_TEST

START_TEST(test_try_drop_down_fail) {
  uint32_t field[FIELD_ROWS] = {0};
  // Занимаем клетку под фигурой
  piece_t p = {0};
  p.type = tShape;
  p.rotation = 0;
  p.position.x = 3;
  p.position.y = FIELD_ROWS - 1;  // уже на дне
  int original_y = p.position.y;
  bool result = try_drop_down(&p, field);
  ck_assert(result == false);
  ck_assert_int_eq(p.position.y, original_y);
}
END_TEST

/* ============================================================
   Сборка набора тестов
   ============================================================ */
Suite *tetramino_suite(void) {
  Suite *s = suite_create("Tetramino Logic");

  TCase *tc1 = tcase_create("get_random_piece");
  tcase_add_test(tc1, test_get_random_piece_range);
  suite_add_tcase(s, tc1);

  TCase *tc2 = tcase_create("can_spawn_piece");
  tcase_add_test(tc2, test_can_spawn_piece_true);
  tcase_add_test(tc2, test_can_spawn_piece_false);
  suite_add_tcase(s, tc2);

  TCase *tc3 = tcase_create("rotate_piece_cw");
  tcase_add_test(tc3, test_rotate_piece_cw_no_collision);
  suite_add_tcase(s, tc3);

  TCase *tc4 = tcase_create("steer_piece");
  tcase_add_test(tc4, test_steer_piece_left);
  tcase_add_test(tc4, test_steer_piece_right);
  tcase_add_test(tc4, test_steer_piece_blocked);
  suite_add_tcase(s, tc4);

  TCase *tc5 = tcase_create("try_drop_down");
  tcase_add_test(tc5, test_try_drop_down_success);
  tcase_add_test(tc5, test_try_drop_down_fail);
  suite_add_tcase(s, tc5);

  return s;
}
