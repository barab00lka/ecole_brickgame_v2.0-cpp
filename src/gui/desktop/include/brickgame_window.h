#pragma once
#include <gtkmm/applicationwindow.h>
#include <gtkmm/box.h>
#include <gtkmm/frame.h>
#include <gtkmm/separator.h>

#include "board.h"
#include "sidebar.h"

class GameWindow : public Gtk::ApplicationWindow {
 public:
  GameWindow();
  void AskGameOver();

 private:
  bool on_tick(const Glib::RefPtr<Gdk::FrameClock>&);

  Gtk::Box root_;
  Board board_;
  Sidebar sidebar_;
};
