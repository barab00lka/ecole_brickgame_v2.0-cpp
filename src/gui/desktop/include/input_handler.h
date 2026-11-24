#pragma once
#include <gtkmm/widget.h>

#include <brickgame_api.h>

class InputHandler {
 public:
  static void attach(Gtk::Widget& target);
  // Signal emitted when the game should end
  static sigc::signal<void()> signal_escape;

 private:
  static UserAction_t keyval_to_action(guint keyval);
};
