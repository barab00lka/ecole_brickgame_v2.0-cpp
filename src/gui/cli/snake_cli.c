#ifdef __MINGW64__
#include <ncursesw/ncurses.h>
#else
#define _POSIX_C_SOURCE 200809L
#include <ncurses.h>
#endif
#include <locale.h>
#include <stdlib.h>
#include <time.h>

#include "cli.h"

int main(void) {
  WIN_INIT(30);
  setlocale(LC_ALL, "");
  srand(time(NULL));

#ifdef DEBUG
#include "../../brick_game/snake/include/debug.h"
  struct timespec t0, t1;
  clock_gettime(CLOCK_MONOTONIC, &t0);
  double fps = 0.0;
  int frames = 0;
#endif

  init_viewbuffers();
  UserAction_t signal;
  do {
    const int ch = getch();
#ifdef DEBUG
    mvprintw(0, 0, "Key code: %d    ", ch);
#endif
    signal = get_signal(ch);
    userInput(signal, false);
    const GameInfo_t state = updateCurrentState();
    render(&state);
#ifdef DEBUG
    debug_print_ctx();
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
