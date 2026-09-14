#include "include/board.h"

#include "include/textures_embedded.h"

Board::Board() {
  // initialize viewmodels_ pixbufs
  for (int i = 0; i < int(countCells); ++i)
    viewmodels_[BrickGameCell_t(i)] = load_embedded_tex(BrickGameCell_t(i));

  for (int y = 0; y < FIELD_ROWS; y++)
    for (int x = 0; x < FIELD_COLS; x++) {
      at(y, x).set_size_request(CELL_PX, CELL_PX);
      attach(at(y, x), x, y, 1, 1);
    }
}

void Board::redraw(const GameInfo_t& g) {
  for (int y = 0; y < FIELD_ROWS; y++)
    for (int x = 0; x < FIELD_COLS; x++) {
      at(y, x).set_paintable(viewmodels_[BrickGameCell_t(g.field[y][x])]);
      at(y, x).set_visible(true);
    }
}
