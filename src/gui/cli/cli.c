#include "cli.h"

static void print_rectangle(int top_y, int bottom_y, int left_x, int right_x) {
  mvaddch(top_y, left_x, ACS_ULCORNER);
  int i = left_x + 1;
  for (; i < right_x; i++) mvaddch(top_y, i, ACS_HLINE);
  mvaddch(top_y, i, ACS_URCORNER);

  for (int y = top_y + 1; y < bottom_y; y++) {
    mvaddch(y, left_x, ACS_VLINE);
    mvaddch(y, right_x, ACS_VLINE);
  }

  mvaddch(bottom_y, left_x, ACS_LLCORNER);
  i = left_x + 1;
  for (; i < right_x; i++) mvaddch(bottom_y, i, ACS_HLINE);
  mvaddch(bottom_y, i, ACS_LRCORNER);
}

static void print_board(const GameInfo_t *g) {
  int top = FIELD_TOP - 1;
  int bottom = FIELD_TOP + FIELD_ROWS;
  int left = FIELD_LEFT - 1;
  int right = FIELD_LEFT + FIELD_COLS * CELL_W;

  print_rectangle(top, bottom, left, right);

  for (int y = 0; y < FIELD_ROWS; y++) {
    for (int x = 0; x < FIELD_COLS; x++) {
      int cx = FIELD_LEFT + x * CELL_W;
      int cy = FIELD_TOP + y;
      if (g->field[y][x]) {
        attron(A_REVERSE);
        if (g->field[y][x] == 2)  // apple
          mvprintw(cy, cx, "%*s", CELL_W, "ap");
        else
          mvprintw(cy, cx, "%*s", CELL_W, "");
        attroff(A_REVERSE);
      } else {
        mvprintw(cy, cx, "%*s", CELL_W, "");
      }
    }
  }
}

static void print_next(int topY, int leftX, const GameInfo_t *g) {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (g->next[x][y]) {
        attron(A_BOLD);
        mvaddch(topY + y, leftX + x, '#');
        attroff(A_BOLD);
      } else {
        mvaddch(topY + y, leftX + x, ' ');
      }
    }
  }
}

static void print_sidebar(const GameInfo_t *g) {
  int row = FIELD_TOP;
  int col = PANEL_X;
  mvprintw(row++, col, "%s", "SCORE");
  mvprintw(row++, col, "%-20d", g->score);

  mvprintw(row++, col, "%s", "HI-SCORE");
  mvprintw(row++, col, "%-20d", g->high_score);

  print_next(row++, col, g);
  row += 3;

  mvprintw(row++, col, "SPEED");
  mvprintw(row++, col, "%-20d", g->speed);
  mvprintw(row++, col, "LEVEL");
  mvprintw(row++, col, "%-20d", g->level);

  mvprintw(row++, col, "%s", g->pause ? "Paused" : "      ");
}

void render(const GameInfo_t *game) {
  move(0, 0);
  print_board(game);
  print_sidebar(game);
  refresh();
}

UserAction_t get_signal(int key) {
  UserAction_t rc = NoOp;

  switch (key) {
    case KEY_DOWN:
      rc = Down;
      break;
    case ' ':
    case 'r':
    case 'R':
      rc = Action;
      break;
    case 'p':
    case 'P':
      rc = Pause;
      break;
    case KEY_LEFT:
      rc = Left;
      break;
    case KEY_RIGHT:
      rc = Right;
      break;
    case ESCAPE:
    case 'q':
    case 'Q':
      rc = Terminate;
      break;
    case '\n':
      rc = Start;
      break;
    case KEY_UP:
      rc = Up;
      break;
  }
  return rc;
}

bool is_hold(int key) {
  static int last_key = ERR;  
  static int repeat_count = 0;
  const int REPEAT_THRESHOLD = 2;  
  if (key == ERR) {
    last_key = ERR;
    repeat_count = 0;
    return false;
  }

  if (key == last_key) {
    repeat_count++;
    if (repeat_count >= REPEAT_THRESHOLD) {
      return true;
    }
    return false;
  } else {
    last_key = key;
    repeat_count = 0;
    return false;
  }
}
