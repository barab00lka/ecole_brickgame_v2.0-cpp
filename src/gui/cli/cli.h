#ifndef CLI_H_
#define CLI_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __MINGW64__
#include <ncursesw/ncurses.h>
#else
#include <curses.h>
#endif

#include <brickgame_api.h>

#define ESCAPE 27

#define WIN_INIT(time)    \
  {                       \
    initscr();            \
    timeout(time);        \
    noecho();             \
    curs_set(0);          \
    keypad(stdscr, TRUE); \
  }

/* Render constants */
#define CELL_W 2      // ширина клетки поля (символов)
#define FIELD_TOP 2   // верхняя строка поля (с рамкой)
#define FIELD_LEFT 2  // левая колонка поля (с рамкой)

// правая панель
#define PANEL_X (FIELD_LEFT + FIELD_COLS * CELL_W + 3)  // отступ от рамки

#define CLEAR_BACKPOS(y, x) mvaddch((y), (x), ' ')

UserAction_t get_signal(int key);
void render(const GameInfo_t *game);
bool is_hold(int key);

#ifdef __cplusplus
}
#endif
#endif
