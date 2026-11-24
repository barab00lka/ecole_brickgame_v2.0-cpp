#pragma once
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/separator.h>

#include <brickgame_api.h>
#include "next_piece.h"
#include "stat_widget.h"

class Sidebar : public Gtk::Box {
 public:
  Sidebar();
  void update_and_draw(const GameInfo_t& g);

 private:
  StatWidget score_group_;
  StatWidget hiscore_group_;
  NextPiece next_;
  StatWidget speed_group_;
  StatWidget level_group_;
  Gtk::Label pause;
};
