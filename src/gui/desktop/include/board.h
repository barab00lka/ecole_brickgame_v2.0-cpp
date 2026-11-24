#pragma once
#include <gtkmm/grid.h>
#include <gtkmm/picture.h>
#include <gtkmm/widget.h>

#include <array>
#include <unordered_map>

#include <brickgame_api.h>
#include "../../../gui/cli/cli.h"
#include "textures_embedded.h"

class Board : public Gtk::Grid {
 public:
  Board();
  void redraw(const GameInfo_t& g);

 private:
  static constexpr int CELL_PX = 24;
  std::array<Gtk::Picture, FIELD_COLS * FIELD_ROWS>
      cells_;  // 2d array of blocks
  Gtk::Picture& at(int y, int x) { return cells_.at(y * FIELD_COLS + x); };
  std::unordered_map<BrickGameCell_t, Glib::RefPtr<Gdk::Texture>> viewmodels_;
};
