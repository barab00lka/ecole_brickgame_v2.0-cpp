#include "include/input_handler.h"

#include <gtkmm/eventcontrollerkey.h>

UserAction_t InputHandler::keyval_to_action(guint keyval) {
  switch (keyval) {
    case GDK_KEY_Down:
      return Down;
    case GDK_KEY_space:
    case GDK_KEY_r:
    case GDK_KEY_R:
      return Action;
    case GDK_KEY_p:
    case GDK_KEY_P:
      return Pause;
    case GDK_KEY_Left:
      return Left;
    case GDK_KEY_Right:
      return Right;
    case GDK_KEY_Escape:
    case GDK_KEY_q:
    case GDK_KEY_Q:
      return Terminate;
    case GDK_KEY_Return:
      return Start;
    case GDK_KEY_Up:
      return Up;
    default:
      return NoOp;
  }
}
sigc::signal<void()> InputHandler::signal_escape;

void InputHandler::attach(Gtk::Widget& target) {
  auto kc = Gtk::EventControllerKey::create();
  kc->signal_key_pressed().connect(
      [](guint keyval, guint, Gdk::ModifierType) -> bool {
        UserAction_t act = keyval_to_action(keyval);
        userInput(act, false);
        if (act == Terminate) signal_escape.emit();
        return true;
      },
      false);
  target.add_controller(kc);
}
