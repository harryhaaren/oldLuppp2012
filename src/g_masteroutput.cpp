
#include "g_masteroutput.hpp"

#include "g_widgets.hpp"

using namespace std;
using namespace Luppp;

int GMasterOutput::privateID = 0;

GMasterOutput::GMasterOutput(Top* t, GuiStateStore* s)
{
  ID = privateID++;
  
  top = t;
  stateStore = s;
  
  mouseX = -1;
  mouseY = -1;
  
  //std::cout << "Enterin GMasterOutput contructor" << std::endl;
  add_events(Gdk::EXPOSURE_MASK | Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
  signal_button_press_event().connect(sigc::mem_fun(*this, &GMasterOutput::on_button_press_event) );
  signal_button_release_event().connect(sigc::mem_fun(*this, &GMasterOutput::on_button_release_event) );
  signal_motion_notify_event().connect( sigc::mem_fun( *this, &GMasterOutput::onMouseMove ) );
  
  set_size_request(74*2,106);
}

bool GMasterOutput::redraw()
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


bool GMasterOutput::on_expose_event(GdkEventExpose* event)
{
  // This is where we draw on the window
  Glib::RefPtr<Gdk::Window> window = get_window();
  
  if(window)    // Only run if Window does exist
  {
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
    
    
    TrackOutputState* state = new TrackOutputState(); // = &stateStore->trackoutputState.at(0);
    
    if ( state->selected )
      setColour(cr, COLOUR_GREY_3 );
    else
      setColour(cr, COLOUR_GREY_4 );
    
    cr->rectangle(0, 0, 74*2, 104);
    cr->fill();
    
    //Dial(cr,true, 7,4,state->pan,DIAL_MODE_PAN); // pan
    //Mute(cr, 9  , 41 , state->ID, state->mute ); // mute button
    //Solo(cr, 9  , 68 , state->ID, state->solo ); // solo button
    //Rec (cr, 9  , 85 , state->ID, state->recEnable); // rec button
    
    Fader(cr,46 , 4  , state->volume, 0, 0 ); // fader
    
    if ( state->selected )
    {
      cr->rectangle(0, -10, 74, 200);
      setColour( cr, COLOUR_PURPLE_1 );
      cr->set_line_width(1);
      cr->stroke();
    }
    
  }
  return true;
}

void GMasterOutput::on_menu_file_popup_edit()
{
   std::cout << "The Edit menu item was selected." << std::endl;
}

void GMasterOutput::on_menu_file_popup_generic()
{
   std::cout << "A popup menu item was selected." << std::endl;
}

bool GMasterOutput::on_button_press_event(GdkEventButton* event)
{
  
  /*
  if( event->type == GDK_BUTTON_PRESS && event->button == 1 )
  {
    // reset click, will be set again if needed
    clickedWidget = CLICKED_WIDGET_NONE;
    
    if ( event->x > 38 ) // fader
    {
      clickedWidget = CLICKED_WIDGET_FADER;
      mouseX = event->x;
      mouseY = event->y;
    }
    else if ( event->x > 5 && event->y > 39 && event->y < 39 + 23 ) // mute
    {
      EngineEvent* x = new EngineEvent();
      TrackOutputState* state = new TrackOutputState(); //  = &stateStore->trackoutputState.at(0);
      x->setTrackMute(ID, !state->mute);
      top->toEngineQueue.push(x);
    }
    else if ( event->x > 5 && event->y > 67 && event->y < 67 + 13 ) // solo
    {
      std::cout << "SOLO" << std::endl;
      EngineEvent* x = new EngineEvent();
      TrackOutputState* state = new TrackOutputState(); //  = &stateStore->trackoutputState.at(0);
      x->setTrackSolo(ID, !state->solo);
      top->toEngineQueue.push(x);
    }
    else if ( event->x > 5 && event->y > 85 && event->y < 85 + 13 ) // rec
    {
      std::cout << "REC" << std::endl;
      EngineEvent* x = new EngineEvent();
      TrackOutputState* state = new TrackOutputState(); //  = &stateStore->trackoutputState.at(0);
      x->setTrackRec(ID, !state->recEnable);
      top->toEngineQueue.push(x);
    }
    else if ( event->x > 3 && event->y > 7 && event->y < 37 )
    {
      std::cout << "DIAL" << std::endl;
      clickedWidget = CLICKED_WIDGET_DIAL;
    }
    
  }
  
  // update Engine that we have a different track selected:
  EngineEvent* x = new EngineEvent();
  x->setTrackSelectDevice(ID, 0);
  top->toEngineQueue.push(x);
  */
  
  return true;
}
bool GMasterOutput::on_button_release_event(GdkEventButton* event)
{
  if( event->type == GDK_BUTTON_RELEASE && event->button == 1 )
  {
    mouseX = -1;
    mouseY = -1;
  }
  
  clickedWidget = CLICKED_WIDGET_NONE;
  
  return true;
}

bool GMasterOutput::onMouseMove(GdkEventMotion* event)
{
  /*
  if ( clickedWidget == CLICKED_WIDGET_FADER )
  {
    TrackOutputState* state = new TrackOutputState(); //  = &stateStore->trackoutputState.at(0);
    
    float mouseYdelta = (mouseY - event->y) / 25.f;
    //std::cout << "MouseYdelta: " << mouseYdelta << std::endl;
    
    EngineEvent* x = new EngineEvent();
    
    // move volume relative to current value
    x->setMixerVolume(ID, mouseYdelta + state->volume);
    top->toEngineQueue.push(x);
    
    // reset mouseY
    mouseY = event->y;
  }
  else if ( clickedWidget == CLICKED_WIDGET_DIAL )
  {
    TrackOutputState* state = new TrackOutputState(); //  = &stateStore->trackoutputState.at(0);
    
    float mouseYdelta = (mouseY - event->y) / 225.f;
    std::cout << "MouseYdelta: " << mouseYdelta << std::endl;
    
    EngineEvent* x = new EngineEvent();
    
    // move volume relative to current value
    x->setTrackPan(ID, mouseYdelta + state->pan);
    top->toEngineQueue.push(x);
    
    // reset mouseY
    mouseY = event->y;
  }
  */
  
  return true;
  
}

GMasterOutput::~GMasterOutput()
{
}
