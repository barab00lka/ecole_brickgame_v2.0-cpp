#include "test_scoring.h"

#include <check.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // для unlink

static const char *filename_save = "./test_save.bin";

// Подключаем заголовочные файлы, которые используют filename_save
#include <save.h>
#include <scoring.h>

// Вспомогательная функция: удаляем файл после теста
static void remove_test_file(void) { unlink(filename_save); }

/* ============================================================
   Тесты для score_update
   ============================================================ */
START_TEST(test_score_update_zero_lines) {
  stats_t stats = {0};
  score_update(&stats, 0);
  ck_assert_uint_eq(stats.score, 0);
  ck_assert_uint_eq(stats.level, 0);
}
END_TEST

START_TEST(test_score_update_one_line) {
  stats_t stats = {0};
  score_update(&stats, 1);
  ck_assert_uint_eq(stats.score, 100);
  ck_assert_uint_eq(stats.level, 0);
}
END_TEST

START_TEST(test_score_update_two_lines) {
  stats_t stats = {0};
  score_update(&stats, 2);
  ck_assert_uint_eq(stats.score, 300);
  ck_assert_uint_eq(stats.level, 0);
}
END_TEST

START_TEST(test_score_update_three_lines) {
  stats_t stats = {0};
  score_update(&stats, 3);
  ck_assert_uint_eq(stats.score, 700);
  ck_assert_uint_eq(stats.level, 1);  // 700/600 = 1
}
END_TEST

START_TEST(test_score_update_four_lines) {
  stats_t stats = {0};
  score_update(&stats, 4);
  ck_assert_uint_eq(stats.score, 1500);
  ck_assert_uint_eq(stats.level, 2);  // 1500/600 = 2
}
END_TEST

START_TEST(test_score_update_level_cap) {
  stats_t stats = {0};
  // Набираем очки до уровня 10
  for (int i = 0; i < 10; i++) {
    score_update(&stats, 4);  // каждый раз +1500
  }
  ck_assert_uint_eq(stats.level, 10);
  ck_assert_uint_eq(stats.score, 15000);
  // Проверяем, что уровень не превышает 10
  score_update(&stats, 4);
  ck_assert_uint_eq(stats.level, 10);
}
END_TEST

/* ============================================================
   Тесты для save/load high score
   ============================================================ */
START_TEST(test_save_and_fetch_high_score) {
  remove_test_file();  // гарантируем, что файла нет
  uint64_t saved_score = 12345;
  save_new_high_score(saved_score, filename_save);
  uint64_t loaded = fetch_high_score(filename_save);
  ck_assert_uint_eq(loaded, saved_score);
  remove_test_file();
}
END_TEST

START_TEST(test_fetch_high_score_no_file) {
  remove_test_file();
  uint64_t loaded = fetch_high_score(filename_save);
  ck_assert_uint_eq(loaded, 0);
}
END_TEST

START_TEST(test_fetch_high_score_last_record) {
  remove_test_file();
  // Сохраняем две записи: первую с меньшим счётом, вторую с большим
  save_new_high_score(100, filename_save);
  save_new_high_score(200, filename_save);
  uint64_t loaded = fetch_high_score(filename_save);
  ck_assert_uint_eq(loaded, 200);  // должна вернуть последнюю
  remove_test_file();
}
END_TEST

/* ============================================================
   Сборка набора тестов
   ============================================================ */
Suite *scoring_suite(void) {
  Suite *s = suite_create("Scoring Logic");

  TCase *tc_update = tcase_create("score_update");
  tcase_add_test(tc_update, test_score_update_zero_lines);
  tcase_add_test(tc_update, test_score_update_one_line);
  tcase_add_test(tc_update, test_score_update_two_lines);
  tcase_add_test(tc_update, test_score_update_three_lines);
  tcase_add_test(tc_update, test_score_update_four_lines);
  tcase_add_test(tc_update, test_score_update_level_cap);
  suite_add_tcase(s, tc_update);

  TCase *tc_file = tcase_create("file_io");
  // В тестах с файлом используем временный файл, очищаем перед и после
  tcase_add_test(tc_file, test_save_and_fetch_high_score);
  tcase_add_test(tc_file, test_fetch_high_score_no_file);
  tcase_add_test(tc_file, test_fetch_high_score_last_record);
  suite_add_tcase(s, tc_file);

  return s;
}
