#ifndef NEXT_PIECE_H_
#define NEXT_PIECE_H_
#include <gtkmm/grid.h>
#include <gtkmm/picture.h>

#include <brickgame_api.h>
#include <defines.h>

class NextPiece : public Gtk::Grid {
 public:
  NextPiece();
  void draw(const GameInfo_t& g);

 private:
  static constexpr int CELL_PX = 12;
  std::array<Gtk::Picture, TETRAMINO_SIZE_MAX * TETRAMINO_SIZE_MAX>
      cells_;  // 2d array of blocks
  Gtk::Picture& at(int y, int x) {
    return cells_.at(y * TETRAMINO_SIZE_MAX + x);
  };
  std::unordered_map<BrickGameCell_t, Glib::RefPtr<Gdk::Texture>> viewmodels_;
};
#endif
