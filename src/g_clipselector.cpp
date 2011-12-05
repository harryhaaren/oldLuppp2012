
#include "g_clipselector.hpp"

#include "g_widgets.hpp"

using namespace std;
using namespace Luppp;

int ClipSelector::privateID = 0;

ClipSelector::ClipSelector(StateStore* s)
{
  ID = privateID++;
  
  stateStore = s;
  
  //std::cout << "Enterin ClipSelector contructor" << std::endl;
  add_events(Gdk::EXPOSURE_MASK | Gdk::BUTTON_PRESS_MASK);
  signal_button_press_event().connect(sigc::mem_fun(*this, &ClipSelector::on_button_press_event) );
  
  set_size_request(74,18 * 5);
}

bool ClipSelector::redraw()
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


bool ClipSelector::on_expose_event(GdkEventExpose* event)
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
    cr->set_source_rgb(0.1 , 0.1 , 0.1 );
    cr->fill();
    
    std::cout << "StateStore = " << &stateStore << std::endl;
    
    if ( stateStore->clipSelectorState.size() == 0 )
    {
      std::cout << "clipSelectorState == 0" << std::endl;
    }
    
    if ( ID >= stateStore->clipSelectorState.size() )
    {
      std::cout << "ClipSelector: ID > clipSelectorState.size()" << std::endl;
    }
    
    list<ClipSelectorState>::iterator i = stateStore->clipSelectorState.begin();
    advance(i,ID);
    
    ClipSelectorState state = *i;
    
    float y = 0.f;
    /*
    enum BlockState {
    BLOCK_STATE_EMPTY,
    BLOCK_STATE_LOADED,
    BLOCK_STATE_PLAYING,
    BLOCK_STATE_QUEUED,
    BLOCK_STATE_RECORDING,
    };
    */
    
    BlockState blockState = BLOCK_STATE_LOADED;
    
    
    // 5 should be replaced with "max num scenes" from Store
    for( int i = 0; i < 5; i++)
    {
      if ( i == ID )
        blockState = BLOCK_STATE_PLAYING;
      else
        blockState = BLOCK_STATE_EMPTY;
      
      // void Block(Cairo::RefPtr<Cairo::Context> cr, float x, float y, bool active);
      Block(cr, 0, y, blockState);
      y += 18;
    }
    
    /*
    if ( state.selected )
    {
      cr->rectangle(0, 0, 74, 102);
      setColour( cr, COLOUR_PURPLE_1 );
      cr->set_line_width(1);
      cr->stroke();
    }
    */
    
  }
  return true;
}

void ClipSelector::on_menu_file_popup_edit()
{
   std::cout << "The Edit menu item was selected." << std::endl;
}

void ClipSelector::on_menu_file_popup_generic()
{
   std::cout << "A popup menu item was selected." << std::endl;
}

bool ClipSelector::on_button_press_event(GdkEventButton* event)
{
  if( event->type == GDK_BUTTON_PRESS  ) // && event->button == 3
  {
    
    return true; //It's been handled.
  }
  else
    return false;
}

ClipSelector::~ClipSelector()
{
}
