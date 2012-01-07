
#include "g_progress.hpp"

#include "g_widgets.hpp"

using namespace std;
using namespace Luppp;

GProgress::GProgress()
{
  set_size_request(74, 20);
}

bool GProgress::redraw()
{
  // force our program to redraw the entire widget.
  Glib::RefPtr<Gdk::Window> win = get_window();
  if (win)
  {
      Gdk::Rectangle r(0, 0, get_allocation().get_width(),
              get_allocation().get_height());
      win->invalidate_rect(r, false);
  }
  return true;
}

void GProgress::setValue(float v)
{
  value = v;
  redraw();
}

bool GProgress::on_expose_event(GdkEventExpose* event)
{
  // This is where we draw on the window
  Glib::RefPtr<Gdk::Window> window = get_window();
  
  if(window)    // Only run if Window does exist
  {
    Gtk::Allocation allocation = get_allocation();
    int width = allocation.get_width();
    int height = allocation.get_height();
    
    // clip to the area indicated by the expose event so that we only redraw
    // the portion of the window that needs to be redrawn
    Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
    cr->rectangle(event->area.x, event->area.y,
            event->area.width, event->area.height);
    cr->clip();
    
    cr->rectangle(event->area.x, event->area.y,
        event->area.width, event->area.height);
    setColour(cr, COLOUR_GREY_3 );
    cr->fill();
    
    
    cr->rectangle(event->area.x, event->area.y,
        value * event->area.width, event->area.height);
    setColour(cr, COLOUR_BLUE_1 );
    cr->fill();
    
  }
  return true;
}
