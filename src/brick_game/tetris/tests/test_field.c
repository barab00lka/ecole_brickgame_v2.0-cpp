#include "test_field.h"

void print_field_bin(uint32_t *grid) {
  for (int i = 0; i < FIELD_ROWS; ++i)
    fprintf(stderr, "f[%d]: %032b\n", i, grid[i]);
}

/* ============================================================
   complete_linecount
   ============================================================ */
START_TEST(test_complete_linecount_empty) {
  uint32_t field[FIELD_ROWS] = {0};
  int count = complete_linecount(field);
  ck_assert_int_eq(count, 0);
  for (int i = 0; i < FIELD_ROWS; i++) ck_assert(!IS_MARKED(field[i]));
}
END_TEST

START_TEST(test_complete_linecount_full_lines) {
  uint32_t field[FIELD_ROWS] = {0};
  field[1] = COMPLETE_LINE;
  field[3] = COMPLETE_LINE;
  field[5] = COMPLETE_LINE;
  int count = complete_linecount(field);
  ck_assert_int_eq(count, 3);
  ck_assert(IS_MARKED(field[1]));
  ck_assert(IS_MARKED(field[3]));
  ck_assert(IS_MARKED(field[5]));
  ck_assert(!IS_MARKED(field[0]));
  ck_assert(!IS_MARKED(field[2]));
  ck_assert(!IS_MARKED(field[4]));
}
END_TEST

START_TEST(test_complete_linecount_with_garbage_high_bits) {
  uint32_t field[FIELD_ROWS] = {0};
  field[2] = COMPLETE_LINE | (1U << 20);
  int count = complete_linecount(field);
  ck_assert_int_eq(count, 1);
  ck_assert(IS_MARKED(field[2]));
}
END_TEST

/* ============================================================
   attach_piece
   ============================================================ */
START_TEST(test_attach_piece_center) {
  uint32_t field[FIELD_ROWS] = {0};
  piece_t p = {0};
  p.type = tShape;
  p.rotation = 0;
  p.position.x = 2;
  p.position.y = 0;
  attach_piece(&p, field);

  // Вычисляем ожидаемые строки из shapes
  uint32_t row0 = ((shapes[tShape][0] >> (0 * 4)) & 0xF) << p.position.x;
  uint32_t row1 = ((shapes[tShape][0] >> (1 * 4)) & 0xF) << p.position.x;
  uint32_t row2 = ((shapes[tShape][0] >> (2 * 4)) & 0xF) << p.position.x;
  uint32_t row3 = ((shapes[tShape][0] >> (3 * 4)) & 0xF) << p.position.x;

  ck_assert_uint_eq(field[0], row0);
  ck_assert_uint_eq(field[1], row1);
  ck_assert_uint_eq(field[2], row2);
  ck_assert_uint_eq(field[3], row3);
}
END_TEST

START_TEST(test_attach_piece_negative_x) {
  uint32_t field[FIELD_ROWS] = {0};
  piece_t p = {0};
  p.type = tShape;
  p.rotation = 0;
  p.position.x = -1;
  p.position.y = 0;
  attach_piece(&p, field);

  // Ожидаемые строки: сдвиг вправо на 1 (так как x = -1)
  uint32_t row1 = ((shapes[p.type][p.rotation] >> (1 * 4)) & 0xF) >> 1;
  uint32_t row2 = ((shapes[p.type][p.rotation] >> (2 * 4)) & 0xF) >> 1;

  ck_assert_uint_eq(field[1], row1);
  ck_assert_uint_eq(field[2], row2);
  ck_assert_uint_eq(field[0], 0);
  ck_assert_uint_eq(field[3], 0);
}
END_TEST

/* ============================================================
   check_collision
   ============================================================ */
START_TEST(test_check_collision_empty) {
  uint32_t field[FIELD_ROWS] = {0};
  piece_t p = {0};
  p.type = 0;
  p.rotation = 0;
  p.position.x = 3;
  p.position.y = 5;
  ck_assert(!check_collision(&p, field));
}
END_TEST

START_TEST(test_check_collision_bottom) {
  uint32_t field[FIELD_ROWS] = {0};
  piece_t p = {0};
  p.type = 0;
  p.rotation = 0;
  p.position.x = 3;
  p.position.y = FIELD_ROWS - 1;
  ck_assert(check_collision(&p, field));
}
END_TEST

START_TEST(test_check_collision_left_wall) {
  uint32_t field[FIELD_ROWS] = {0};
  piece_t p = {0};
  p.type = 0;
  p.rotation = 0;
  /* 0b0000
   *   0100
   *   1110
   *   0000,
   */
  p.position.x = -1;  // T row1 имеет биты 1110, они уйдут за левую стенку
  ck_assert(check_collision(&p, field));
}
END_TEST

START_TEST(test_check_collision_left_wall_empty_column) {
  uint32_t field[FIELD_ROWS] = {0};
  piece_t p = {0};
  p.type = squareShape;
  p.rotation = 0;
  p.position.x = -1;  // реальные клетки остаются внутри (0,1)
  ck_assert(check_collision(&p, field) == false);
}
END_TEST

START_TEST(test_check_collision_occupied) {
  uint32_t field[FIELD_ROWS] = {0};
  field[6] = 1U << 3;
  piece_t p = {0};
  p.type = tShape;
  p.rotation = 0;
  p.position.x = 2;
  p.position.y = 4;  // T row2 с битом 2, после сдвига x=2 → бит 4
  ck_assert(check_collision(&p, field));
  //     /* 00000000000000000000000000000000
  //      * 00000000000000000000000000000000
  //      * 00000000000000000000000000000000
  //      * 00000000000000000000000000000000
  //      * 00000000000000000000000000000000
  //      * 00000000000000000000000000000000
  //      * 00000000000000000000000000001000
  //      * 00000000000000000000000000000000
  //      * 00000000000000000000000000000000
  //      * 00000000000000000000000000000000
  //      * 00000000000000000000000000000000
  //      * 00000000000000000000000000000000
  //      * 00000000000000000000000000000000
  //      * 00000000000000000000000000000000
  //      * 00000000000000000000000000000000
  //      * 00000000000000000000000000000000
  //      * 00000000000000000000000000000000
  //      * 00000000000000000000000000000000
  //      * 00000000000000000000000000000000
  //      * 00000000000000000000000000000000
  //      */
}
END_TEST

/* ============================================================
   clearing_animation
   ============================================================ */
START_TEST(test_clearing_animation_basic) {
  uint32_t field[FIELD_ROWS] = {0};
  field[3] = COMPLETE_LINE;
  SET_MARK(field[3]);
  int cleared = clearing_animation(field);
  ck_assert_int_eq(cleared, 1);
  // удаляются крайние биты: 0 и 9
  uint32_t expected = COMPLETE_LINE ^ (1U << 9) ^ (1U << 0);
  SET_MARK(expected);
  ck_assert_uint_eq(field[3], expected);
}
END_TEST

START_TEST(test_clearing_animation_multiple_calls) {
  uint32_t field[FIELD_ROWS] = {0};
  field[1] = COMPLETE_LINE;
  SET_MARK(field[1]);
  clearing_animation(field);  // убрать биты 0 и 9
  uint32_t clear_first = 0x1FE;
  SET_MARK(clear_first);
  uint32_t clear_second = 0xFC;
  SET_MARK(clear_second);

  ck_assert_uint_eq(field[1], clear_first);
  clearing_animation(field);  // теперь крайние: биты 1 и 8
  ck_assert_uint_eq(field[1], clear_second);  // 0x1FE ^ 0x102 = 0xFC
}
END_TEST

/* ============================================================
   shift_remove_lines
   ============================================================ */
START_TEST(test_shift_remove_lines) {
  uint32_t field[FIELD_ROWS] = {0};
  // заполним первые 5 строк, пометим 1 и 3
  for (int i = 0; i < 5; i++) field[i] = 0x55;
  field[1] = COMPLETE_LINE;
  SET_MARK(field[1]);
  field[3] = COMPLETE_LINE;
  SET_MARK(field[3]);
  shift_remove_lines(field);
  ck_assert_uint_eq(field[0], 0);
  ck_assert_uint_eq(field[1], 0);
  ck_assert_uint_eq(field[2], 0x55);  // бывшая строка 0
  ck_assert_uint_eq(field[3], 0x55);  // бывшая строка 2
  ck_assert_uint_eq(field[4], 0x55);  // бывшая строка 4
}
END_TEST

/* ============================================================
   clear_field
   ============================================================ */
START_TEST(test_clear_field) {
  uint32_t field[FIELD_ROWS];
  for (int i = 0; i < FIELD_ROWS; i++) field[i] = 0x12345678;
  clear_field(field);
  for (int i = 0; i < FIELD_ROWS; i++) ck_assert_uint_eq(field[i], 0);
}
END_TEST

/* ============================================================
   Сборка набора тестов
   ============================================================ */
Suite *field_suite(void) {
  Suite *s = suite_create("Field Logic");

  TCase *tc1 = tcase_create("complete_linecount");
  tcase_add_test(tc1, test_complete_linecount_empty);
  tcase_add_test(tc1, test_complete_linecount_full_lines);
  tcase_add_test(tc1, test_complete_linecount_with_garbage_high_bits);
  suite_add_tcase(s, tc1);

  TCase *tc2 = tcase_create("attach_piece");
  tcase_add_test(tc2, test_attach_piece_center);
  tcase_add_test(tc2, test_attach_piece_negative_x);
  suite_add_tcase(s, tc2);

  TCase *tc3 = tcase_create("check_collision");
  tcase_add_test(tc3, test_check_collision_empty);
  tcase_add_test(tc3, test_check_collision_bottom);
  tcase_add_test(tc3, test_check_collision_left_wall);
  tcase_add_test(tc3, test_check_collision_left_wall_empty_column);
  tcase_add_test(tc3, test_check_collision_occupied);
  suite_add_tcase(s, tc3);

  TCase *tc4 = tcase_create("clearing_animation");
  tcase_add_test(tc4, test_clearing_animation_basic);
  tcase_add_test(tc4, test_clearing_animation_multiple_calls);
  suite_add_tcase(s, tc4);

  TCase *tc5 = tcase_create("shift_remove_lines");
  tcase_add_test(tc5, test_shift_remove_lines);
  suite_add_tcase(s, tc5);

  TCase *tc6 = tcase_create("clear_field");
  tcase_add_test(tc6, test_clear_field);
  suite_add_tcase(s, tc6);

  return s;
}
