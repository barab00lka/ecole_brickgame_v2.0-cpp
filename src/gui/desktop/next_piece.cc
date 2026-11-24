#include "include/next_piece.h"

#include "include/textures_embedded.h"

NextPiece::NextPiece() {
  // initialize viewmodels_ pixbufs
  for (int i = 0; i < int(countCells); ++i)
    viewmodels_[BrickGameCell_t(i)] = load_embedded_tex(BrickGameCell_t(i));
  for (int y = 0; y < TETRAMINO_SIZE_MAX; y++)
    for (int x = 0; x < TETRAMINO_SIZE_MAX; x++) {
      at(y, x).set_size_request(CELL_PX, CELL_PX);
      attach(at(y, x), x, y, 1, 1);
    }
}

void NextPiece::draw(const GameInfo_t& g) {
  for (int y = 0; y < TETRAMINO_SIZE_MAX; y++)
    for (int x = 0; x < TETRAMINO_SIZE_MAX; x++) {
      at(y, x).set_paintable(viewmodels_[BrickGameCell_t(g.next[y][x])]);
      at(y, x).set_visible(true);
    }
}
