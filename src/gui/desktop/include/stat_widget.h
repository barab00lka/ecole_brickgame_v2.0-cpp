#ifndef STAT_WIDGET_H_
#define STAT_WIDGET_H_

#include <gtkmm/box.h>
#include <gtkmm/label.h>

class StatWidget : public Gtk::Box {
 public:
  // Constructor: takes the header text and initial value text
  StatWidget(const Glib::ustring& header_text,
             const Glib::ustring& initial_value = "");

  // Public method to update the value
  void set_value(const Glib::ustring& value);

 private:
  Gtk::Label m_label_header;
  Gtk::Label m_label_value;
};

#endif  // STAT_WIDGET_H_
