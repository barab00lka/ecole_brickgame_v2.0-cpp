#include "include/brickgame_window.h"

int main(int argc, char** argv) {
  auto app = Gtk::Application::create("org.brickgame.snake.gtkmm4");
  return app->make_window_and_run<GameWindow>(argc, argv);
}
