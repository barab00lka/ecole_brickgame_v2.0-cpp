#ifdef __MINGW64__
#include <ncursesw/ncurses.h>
#else
#define _POSIX_C_SOURCE 200809L
#include <ncurses.h>
#endif
#include <locale.h>
#include <stdlib.h>
#include <time.h>

#include "../../brick_game/tetris/include/debug.h"
#include "cli.h"

int main(void) {
  WIN_INIT(50);
  setlocale(LC_ALL, "");
  srand(time(NULL));
  init_viewbuffers();

#ifdef DEBUG
  struct timespec t0, t1;
  clock_gettime(CLOCK_MONOTONIC, &t0);
  double fps = 0.0;
  int frames = 0;
#endif

  UserAction_t signal;
  do {
    const int ch = getch();
    LOG_DEBUG(mvprintw(0, 0, "Key code: %d    ", ch));
    bool flag = is_hold(ch);
    LOG_DEBUG(debug_print_ctx(flag));
    signal = get_signal(ch);
    userInput(signal, flag);
    const GameInfo_t state = updateCurrentState();
    render(&state);
#ifdef DEBUG
    frames++;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    double elapsed = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1e9;
    if (elapsed >= 0.5) {
      fps = frames / elapsed;
      frames = 0;
      t0 = t1;
    }
    mvprintw(0, 15, "FPS: %5.1f", fps);
    refresh();
#endif
  } while (signal != Terminate);
  endwin();
  return 0;
}
