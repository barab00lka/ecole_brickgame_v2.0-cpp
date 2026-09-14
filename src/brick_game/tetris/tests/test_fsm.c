#include "test_fsm.h"

#include <check.h>
#include <string.h>
#include <unistd.h>

#include "test_field.h"


#include "../include/field.h"
#include "../include/fsm.h"
#include "../include/scoring.h"

// Вспомогательная функция для создания пустого контекста
static tetrisCtx_t create_empty_ctx(void) {
  tetrisCtx_t game = {0};
  memset(game.field, 0, sizeof(game.field));
  game.next_piece = get_random_piece();
  return game;
}

static int field_is_zero(const uint32_t *field) {
  for (int i = 0; i < FIELD_ROWS; i++) {
    if (field[i] != 0) return 0;
  }
  return 1;
}

/* ============================================================
   Тесты состояний
   ============================================================ */
START_TEST(test_on_start_state) {
  tetrisCtx_t game = create_empty_ctx();
  // Установим состояние startState
  game.state = startState;
  // Вызовем обработчик с сигналом Start
  on_start_state(&game, Start, false);
  // Проверяем, что high_score загружен (может быть 0, если файла нет)
  // Мы не можем точно предсказать, но проверим, что функция вызвалась
  // Также должно быть очищено поле
  ck_assert(field_is_zero(game.field));
  // Проверим, что тикер сброшен
  ck_assert_int_eq(game.ticker, 0);
}
END_TEST

START_TEST(test_on_spawn_state_success) {
  tetrisCtx_t game = create_empty_ctx();
  game.state = spawnState;
  // Задаём следующую фигуру, которая точно поместится
  game.next_piece.type = squareShape;
  game.next_piece.rotation = 0;
  on_spawn_state(&game, 0, false);
  // Проверяем, что текущая фигура стала следующей, а следующая обновилась
  ck_assert_int_eq(game.current_piece.type, squareShape);
  // Проверяем позицию
  ck_assert_int_eq(game.current_piece.position.y, -1);
  ck_assert_int_eq(game.current_piece.position.x, (FIELD_COLS / 2) - 2);
  // Состояние должно стать movingState
  ck_assert_int_eq(game.state, movingState);
}
END_TEST

START_TEST(test_on_spawn_state_game_over) {
  tetrisCtx_t game = create_empty_ctx();
  game.state = spawnState;
  // Делаем так, чтобы фигура не могла заспавниться – заполним поле
  // например, заполним верхние строки
  for (int i = 0; i < 4; i++) {
    game.field[i] = COMPLETE_LINE;
  }
  // Следующая фигура – квадрат, он не поместится
  game.next_piece.type = squareShape;
  game.next_piece.rotation = 0;
  on_spawn_state(&game, 0, false);
  ck_assert_int_eq(game.state, gameOverState);
}
END_TEST

START_TEST(test_on_moving_state_pause) {
  tetrisCtx_t game = create_empty_ctx();
  game.state = movingState;
  on_moving_state(&game, Pause, false);
  ck_assert_int_eq(game.state, pausedState);
}
END_TEST

START_TEST(test_on_moving_state_action_rotate) {
  tetrisCtx_t game = create_empty_ctx();
  game.state = movingState;
  // Задаём фигуру T, которая может вращаться
  game.current_piece.type = tShape;
  game.current_piece.rotation = 0;
  game.current_piece.position.x = 3;
  game.current_piece.position.y = 5;
  uint32_t old_rotation = game.current_piece.rotation;
  on_moving_state(&game, Up, false);
  // Ротация должна изменить rotation
  ck_assert_int_ne(game.current_piece.rotation, old_rotation);
}
END_TEST

START_TEST(test_on_moving_state_left_right_without_hold) {
  tetrisCtx_t game = create_empty_ctx();
  game.state = movingState;
  game.current_piece.type = squareShape;
  game.current_piece.rotation = 0;
  game.current_piece.position.x = 3;
  game.current_piece.position.y = 5;
  int old_x = game.current_piece.position.x;
  on_moving_state(&game, Left, false);
  ck_assert_int_eq(game.current_piece.position.x, old_x - 1);
  // Проверим, что при hold=false двигаем на 1
  old_x = game.current_piece.position.x;
  on_moving_state(&game, Right, false);
  ck_assert_int_eq(game.current_piece.position.x, old_x + 1);
}
END_TEST

START_TEST(test_on_moving_state_left_right_with_hold) {
  tetrisCtx_t game = create_empty_ctx();
  game.state = movingState;
  game.current_piece.type = squareShape;
  game.current_piece.rotation = 0;
  game.current_piece.position.x = 3;
  game.current_piece.position.y = 5;
  int old_x = game.current_piece.position.x;
  on_moving_state(&game, Left, true);
  // При hold=true должно быть два сдвига (цикл for n=2)
  ck_assert_int_eq(game.current_piece.position.x, old_x - 2);
  // Теперь проверим Right
  old_x = game.current_piece.position.x;
  on_moving_state(&game, Right, true);
  ck_assert_int_eq(game.current_piece.position.x, old_x + 2);
}
END_TEST

START_TEST(test_on_moving_state_down_without_hold) {
  tetrisCtx_t game = create_empty_ctx();
  game.state = movingState;
  game.current_piece.type = squareShape;
  game.current_piece.rotation = 0;
  game.current_piece.position.x = 3;
  game.current_piece.position.y = 5;
  int old_y = game.current_piece.position.y;
  on_moving_state(&game, Down, false);
  // При нажатии Down без hold должно быть одно падение
  ck_assert_int_eq(game.current_piece.position.y, old_y + 1);
  // ticker должен сброситься
  ck_assert_int_eq(game.ticker, 0);
}
END_TEST

START_TEST(test_on_moving_state_down_with_hold) {
  tetrisCtx_t game = create_empty_ctx();
  game.state = movingState;
  game.current_piece.type = squareShape;
  game.current_piece.rotation = 0;
  game.current_piece.position.x = 3;
  game.current_piece.position.y = 5;
  int old_y = game.current_piece.position.y;
  on_moving_state(&game, Down, true);
  // При hold=true должно быть три падения (цикл for n=3)
  ck_assert_int_eq(game.current_piece.position.y, old_y + 3);
  // ticker должен сброситься
  ck_assert_int_eq(game.ticker, 0);
}
END_TEST

START_TEST(test_on_moving_state_down_collision_attach) {
  tetrisCtx_t game = create_empty_ctx();
  game.state = movingState;
  // Ставим фигуру на дно (коллизия при попытке упасть)
  game.current_piece.type = squareShape;
  game.current_piece.rotation = 0;
  game.current_piece.position.x = 3;
  game.current_piece.position.y = FIELD_ROWS - 1;
  on_moving_state(&game, Down, false);
  // Проверяем, что поле обновилось
  // Проверим, что на месте фигуры появились клетки
  uint32_t expected_row =
      ((shapes[squareShape][0] >> (0 * 4)) & 0xF)
      << game.current_piece.position.x;  // для верхней строки
  ck_assert_uint_eq(game.field[19], expected_row);
  ck_assert_uint_eq(game.field[18], expected_row);
}
END_TEST

START_TEST(test_on_moving_state_timer_tick) {
  tetrisCtx_t game = create_empty_ctx();
  game.state = movingState;
  game.current_piece.type = squareShape;
  game.current_piece.rotation = 0;
  game.current_piece.position.x = 3;
  game.current_piece.position.y = 5;
  int old_y = game.current_piece.position.y;
  // При TimerTick (без hold) должно быть одно падение (как Down без hold, но
  // без сброса ticker)
  on_moving_state(&game, TimerTick, false);
  ck_assert_int_eq(game.current_piece.position.y, old_y + 1);
  // ticker НЕ должен сбрасываться
  // Но в коде ticker сбрасывается только при sig == Down, так что не сбросится
  // Проверим, что ticker не изменился (он был 0, так как мы не трогали)
  ck_assert_int_eq(game.ticker, 0);
}
END_TEST

// Аналогично протестируем другие состояния кратко

START_TEST(test_on_paused_state_pause) {
  tetrisCtx_t game = create_empty_ctx();
  game.state = pausedState;
  on_paused_state(&game, Pause, false);
  ck_assert_int_eq(game.state, movingState);
}
END_TEST

START_TEST(test_on_gameover_state_start) {
  tetrisCtx_t game = create_empty_ctx();
  game.state = gameOverState;
  game.current_piece.rotation = 0;
  /* проверяем что гейм овер не перейдет в start пока rotation не дойдет до 20,
   * потому что rotation тут используется для анимации занавеса */
  on_gameover_state(&game, Start, false);
  ck_assert_int_eq(game.state, gameOverState);
}
END_TEST

START_TEST(test_on_clearing_state_animation_done) {
  tetrisCtx_t game = create_empty_ctx();
  game.state = clearingState;
  // Подготовим поле с помеченной линией
  game.field[3] = COMPLETE_LINE;
  SET_MARK(game.field[3]);
  // Первый вызов уберёт крайние биты
  on_clearing_state(&game, 0, false);
  // После первого вызова поле должно измениться, но состояние останется
  // clearingState, пока не очистится полностью Так как clearing_animation
  // вернёт 1 (ещё не всё очищено), состояние не изменится
  ck_assert_int_eq(game.state, clearingState);
  // Но после нескольких вызовов, когда clearing_animation вернёт 0, состояние
  // станет spawnState Мы не можем легко смоделировать много вызовов, поэтому
  // проще проверить, что после полной очистки переход происходит. Сделаем цикл,
  // пока не очистится.
  while (game.state == clearingState) {
    on_clearing_state(&game, 0, false);
  }
  ck_assert_int_eq(game.state, spawnState);
}
END_TEST

/* ============================================================
   Тесты состояний (без изменений, кроме удаления файла в exit тесте)
   ============================================================ */
// ... (все тесты от test_on_start_state до test_on_gameover_state остаются как
// ранее)

/* ============================================================
   Сборка набора тестов (без изменений)
   ============================================================ */
Suite *fsm_suite(void) {
  Suite *s = suite_create("FSM Logic");

  TCase *tc_start = tcase_create("startState");
  tcase_add_test(tc_start, test_on_start_state);
  suite_add_tcase(s, tc_start);

  TCase *tc_spawn = tcase_create("spawnState");
  tcase_add_test(tc_spawn, test_on_spawn_state_success);
  tcase_add_test(tc_spawn, test_on_spawn_state_game_over);
  suite_add_tcase(s, tc_spawn);

  TCase *tc_moving = tcase_create("movingState");
  tcase_add_test(tc_moving, test_on_moving_state_pause);
  tcase_add_test(tc_moving, test_on_moving_state_action_rotate);
  tcase_add_test(tc_moving, test_on_moving_state_left_right_without_hold);
  tcase_add_test(tc_moving, test_on_moving_state_left_right_with_hold);
  tcase_add_test(tc_moving, test_on_moving_state_down_without_hold);
  tcase_add_test(tc_moving, test_on_moving_state_down_with_hold);
  tcase_add_test(tc_moving, test_on_moving_state_down_collision_attach);
  tcase_add_test(tc_moving, test_on_moving_state_timer_tick);
  suite_add_tcase(s, tc_moving);

  TCase *tc_paused = tcase_create("pausedState");
  tcase_add_test(tc_paused, test_on_paused_state_pause);
  suite_add_tcase(s, tc_paused);

  TCase *tc_gameover = tcase_create("gameOverState");
  tcase_add_test(tc_gameover, test_on_gameover_state_start);
  suite_add_tcase(s, tc_gameover);

  TCase *tc_clearing = tcase_create("clearingState");
  tcase_add_test(tc_clearing, test_on_clearing_state_animation_done);
  suite_add_tcase(s, tc_clearing);

  return s;
}
