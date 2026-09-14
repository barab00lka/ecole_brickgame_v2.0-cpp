#ifdef __MINGW64__
#include <ncursesw/ncurses.h>
#else
#include <curses.h>
#endif
#include <brickgame_api.h>
#include <entities.h>
#include <fsm.h>
#include <view.h>

int field_buf[FIELD_ROWS][FIELD_COLS];
int *field_ptr[FIELD_ROWS];
int next_buf[4][4];
int *next_ptr[4];

static tetrisCtx_t ctx = (tetrisCtx_t){};

void userInput(UserAction_t action, bool hold) {
  disp_table_fsm[ctx.state](&ctx, action, hold);
}

GameInfo_t updateCurrentState() {
  switch (ctx.state) {
    case pausedState:  // при состоянии паузы модель не обновляется
      break;
    default:
      if (++ctx.ticker >=
          TETRIS_ANIM_TICKRATE)  // при остальных states скорость статическая (анимации
                          // очистки поля, и т.д.)
      {
        ctx.ticker = 0;
        disp_table_fsm[ctx.state](&ctx, TimerTick, false);
      }
      break;
    case movingState:  // при movingState скорость динамическая и зависит от
                       // уровня
      if (++ctx.ticker >= TETRIS_TICKRATE - TETRIS_SPEED_STEP * ctx.stats.level) {
        ctx.ticker = 0;
        disp_table_fsm[ctx.state](&ctx, TimerTick, false);
      }
      break;
  }

  plot_field(&ctx, field_ptr);  //  передаем игровое поле в массив для рендера
  plot_next(&ctx, next_ptr);  // передаем окно со следующей фигурой
  return (GameInfo_t){
      .field = field_ptr,
      .next = next_ptr,
      .score = ctx.stats.score,
      .high_score = ctx.stats.high_score,
      .level = ctx.stats.level,
      .speed = TETRIS_SPEED_STEP * ctx.stats.level,
      .pause = (ctx.state == pausedState) ? 1 : 0,
  };
}

#ifdef DEBUG
#include "../../include/tetramino.h"
void debug_print_ctx(bool hold) {
  switch (ctx.state) {
    case startState:
      mvprintw(19, 25, "state: %-10s", "start");  // fsm state
      break;
    case spawnState:
      mvprintw(19, 25, "state: %-10s", "spawn");  // fsm state
      break;
    case movingState:
      mvprintw(19, 25, "state: %-10s", "moving");  // fsm state
      break;
    case clearingState:
      mvprintw(19, 25, "state: %-10s", "clearing");  // fsm state
      break;
    case pausedState:
      mvprintw(19, 25, "state: %-10s", "paused");  // fsm state
      break;
    case gameOverState:
      mvprintw(19, 25, "state: %-10s", "gameOver");  // fsm state
      break;
    case exitState:
      mvprintw(19, 25, "state: %-10s", "exit");  // fsm state
      break;
    default:
      mvprintw(19, 25, "state: %-10s", "no_state");  // fsm state
      break;
  }

  mvprintw(18, 25, "hold:%-5s", hold ? "true" : "false");
  mvprintw(20, 25, "Cur_piece x:%d y:%d", ctx.current_piece.position.x,
           ctx.current_piece.position.y);
  mvprintw(2 + ctx.current_piece.position.y,
           2 + ctx.current_piece.position.x * 2, "p");  // piece position mark
  mvprintw(21, 25, "cur_piece rot: %d", ctx.current_piece.rotation);
  mvprintw(22, 25, "cur_piece mask: %016b",
           shapes[ctx.current_piece.type]
                 [ctx.current_piece.rotation]);  // current piece shape bitmask
  mvprintw(23, 25, "tickrate: %llu", TICKRATE - 2 * ctx.stats.level);
  mvprintw(24, 25, "ticker: %u", ctx.ticker);
  mvprintw(25, 25, "timer: %u", ctx.timer);
  mvprintw(26, 25, "next_piece rot: %d", ctx.next_piece.rotation);
  /* print binary field */
  for (int i = 0; i < FIELD_ROWS; ++i)
    mvprintw(i, 57, "f[%d]: %032b", i, ctx.field[i]);
}
#endif
