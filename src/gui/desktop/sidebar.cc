#include "include/sidebar.h"

#include <gtkmm/separator.h>

Sidebar::Sidebar()
    : Gtk::Box(Gtk::Orientation::VERTICAL, 4),
      score_group_("SCORE", "0"),
      hiscore_group_("HI-SCORE", "0"),
      speed_group_("SPEED", "1"),
      level_group_("LEVEL", "1"),
      pause("Paused") {
  set_spacing(10);

  // Append the member objects (they are already constructed)
  append(score_group_);
  append(hiscore_group_);
  append(next_);  // next_ is also a member object
  append(speed_group_);
  append(level_group_);
  append(pause);
}

void Sidebar::update_and_draw(const GameInfo_t& g) {
  score_group_.set_value(std::to_string(g.score));
  hiscore_group_.set_value(std::to_string(g.high_score));
  next_.draw(g);
  speed_group_.set_value(std::to_string(g.speed));
  level_group_.set_value(std::to_string(g.level));
  pause.set_visible(g.pause);
}
