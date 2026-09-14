#include "include/brickgame_window.h"

#include <brickgame_api.h>
#include "include/input_handler.h"

GameWindow::GameWindow() {
  set_child(root_);

  root_.set_valign(Gtk::Align::CENTER);
  root_.append(board_);
  Gtk::Separator sep_{Gtk::Orientation::VERTICAL};
  root_.append(sep_);
  root_.append(sidebar_);
  root_.set_spacing(20);

  init_viewbuffers();

  add_tick_callback(sigc::mem_fun(*this, &GameWindow::on_tick));
  InputHandler::attach(*this);
  InputHandler::signal_escape.connect([this]() {
    if (auto app = get_application()) {
      app->quit();
    }
  });
}

bool GameWindow::on_tick(const Glib::RefPtr<Gdk::FrameClock>&) {
  GameInfo_t g = updateCurrentState();
  board_.redraw(g);
  sidebar_.update_and_draw(g);
  return true;
}
