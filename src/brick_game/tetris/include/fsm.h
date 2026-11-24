#ifndef FSM_H_
#define FSM_H_
#include <stdbool.h>

#include <brickgame_api.h>
#include <defines.h>
#include <entities.h>

typedef void (*StateHandler)(tetrisCtx_t *game, UserAction_t sig, bool hold);

extern StateHandler disp_table_fsm[StateCount];

// сигнал == enter -> next_piece = get_random_piece(), state = spawn.
// сигнал == escape -> game->state = exitState.
void on_start_state(tetrisCtx_t *game, UserAction_t sig, bool hold);

// тут проверяется осталось ли место на сетке. Если да: спавнится next_piece в
// самом верху сетки и state=moving. Если нет -> state=gameOver. current_piece
// = next_piece, next_piece = get_random_piece()
void on_spawn_state(tetrisCtx_t *game, UserAction_t sig, bool hold);

// тут игрок управляет фигурой а так же фигура падает вниз.
// Если сигнал rotate , то пробуем прокрутить фигуру;
// Если сигнал moveLeft / moveRight двигает фигуру по X axis;
// Если сигнал moveDown еще раз инкрементируем Y axis
// Затем позиция по Y axis меняется на 1 ниже в зависимости от таймера.
void on_moving_state(tetrisCtx_t *game, UserAction_t sig, bool hold);

// where clear line animations happen
void on_clearing_state(tetrisCtx_t *game, UserAction_t sig, bool hold);

// bounds checking for static tetramino pieces
void on_attaching_state(tetrisCtx_t *game, UserAction_t sig, bool hold);

// тут ничего не происходит, ждет сигнал о продолжении игры
void on_paused_state(tetrisCtx_t *game, UserAction_t sig, bool hold);

// ждем escape, чтобы выйти из игры или enter, чтоб начать заново.
void on_gameover_state(tetrisCtx_t *game, UserAction_t sig, bool hold);

#endif
