#include "include/stat_widget.h"

StatWidget::StatWidget(const Glib::ustring& header_text,
                       const Glib::ustring& initial_value)
    : Gtk::Box(
          Gtk::Orientation::VERTICAL),  // Arrange header and value vertically
      m_label_header(header_text),
      m_label_value(initial_value) {
  // --- Style the header label ---
  // Make it bold and smaller to look like a title
  m_label_header.set_markup("<b>" + header_text + "</b>");
  m_label_header.set_halign(Gtk::Align::CENTER);

  // --- Style the value label ---
  // Make it larger and bold to stand out
  m_label_value.set_markup("<span size='x-large' weight='bold'>" +
                           initial_value + "</span>");
  m_label_value.set_halign(Gtk::Align::CENTER);

  // --- Add the child widgets to the box ---
  // The order they are appended determines their vertical order.
  append(m_label_header);
  append(m_label_value);

  // Optional: Add some spacing between the header and value
  set_spacing(5);
  // Center the whole widget group
  set_halign(Gtk::Align::CENTER);
  set_valign(Gtk::Align::CENTER);
}

void StatWidget::set_value(const Glib::ustring& value) {
  // Update the value label's text
  m_label_value.set_markup("<span size='x-large' weight='bold'>" + value +
                           "</span>");
}
