
/*
  This file is part of Luppp.
  Copyright 2012, Harry van Haaren

  Luppp is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Luppp is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Luppp.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "g_masterreturn.hpp"

#include "g_widgets.hpp"

using namespace std;
using namespace Luppp;

int GMasterReturn::privateID = 0;

GMasterReturn::GMasterReturn(Top* t, GuiStateStore* s)
{
  ID = privateID++;
  
  top = t;
  stateStore = s;
  
  mouseX = -1;
  mouseY = -1;
  
  //std::cout << "Enterin GMasterReturn contructor" << std::endl;
  add_events(Gdk::EXPOSURE_MASK | Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
  signal_button_press_event().connect(sigc::mem_fun(*this, &GMasterReturn::on_button_press_event) );
  signal_button_release_event().connect(sigc::mem_fun(*this, &GMasterReturn::on_button_release_event) );
  signal_motion_notify_event().connect( sigc::mem_fun( *this, &GMasterReturn::onMouseMove ) );
  
  set_size_request(74,102);
}

bool GMasterReturn::redraw()
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


bool GMasterReturn::on_expose_event(GdkEventExpose* event)
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
    
    /*
    GMasterReturnState* state = &stateStore->trackoutputState.at(ID);
    
    if ( state->selected )
      setColour(cr, COLOUR_GREY_3 );
    else
      setColour(cr, COLOUR_GREY_4 );
    
    cr->rectangle(0, 0, 74, 102);
    cr->fill();
    
    Dial(cr,true, 7,4,state->pan,DIAL_MODE_PAN); // pan
    Mute(cr, 9  , 41 , state->ID, state->mute ); // mute button
    Solo(cr, 9  , 68 , state->ID, state->solo ); // solo button
    Rec (cr, 9  , 85 , state->ID, state->recEnable); // rec button
    */
    
    Fader(cr,46 , 4  , 0.7, 0.6, 0.8 ); // fader
    
    // Dial text "A"
    cr->select_font_face ("Impact" , Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD);
    cr->set_font_size ( 20 );
    cr->move_to ( 10, 28);
    switch ( ID )
    {
      case 0: setColour(cr, COLOUR_BLUE_1 ); cr->show_text ( "A" ); break;
      case 1: setColour(cr, COLOUR_BLUE_1 ); cr->show_text ( "B" ); break;
      case 2: setColour(cr, COLOUR_BLUE_1 ); cr->show_text ( "C" ); break;
    }
    
  }
  return true;
}

void GMasterReturn::on_menu_file_popup_edit()
{
   std::cout << "The Edit menu item was selected." << std::endl;
}

void GMasterReturn::on_menu_file_popup_generic()
{
   std::cout << "A popup menu item was selected." << std::endl;
}

bool GMasterReturn::on_button_press_event(GdkEventButton* event)
{
  if( event->type == GDK_BUTTON_PRESS && event->button == 1 )
  {
    // reset click, will be set again if needed
    clickedWidget = CLICKED_WIDGET_NONE;
    
    /*
    if ( event->x > 38 ) // fader
    {
      clickedWidget = CLICKED_WIDGET_FADER;
      mouseX = event->x;
      mouseY = event->y;
      
      float volume = 1 - ((event->y-4) / 94.f);
      EngineEvent* x = new EngineEvent();
      x->setMixerVolume(ID, volume);
      top->toEngineQueue.push(x);
      
    }
    else if ( event->x > 5 && event->y > 39 && event->y < 39 + 23 ) // mute
    {
      EngineEvent* x = new EngineEvent();
      GMasterReturnState* state = &stateStore->trackoutputState.at(ID);
      x->setTrackMute(ID, !state->mute);
      top->toEngineQueue.push(x);
    }
    else if ( event->x > 5 && event->y > 67 && event->y < 67 + 13 ) // solo
    {
      std::cout << "SOLO" << std::endl;
      EngineEvent* x = new EngineEvent();
      GMasterReturnState* state = &stateStore->trackoutputState.at(ID);
      x->setTrackSolo(ID, !state->solo);
      top->toEngineQueue.push(x);
    }
    else if ( event->x > 5 && event->y > 85 && event->y < 85 + 13 ) // rec
    {
      std::cout << "REC" << std::endl;
      EngineEvent* x = new EngineEvent();
      GMasterReturnState* state = &stateStore->trackoutputState.at(ID);
      x->setTrackRec(ID, !state->recEnable);
      top->toEngineQueue.push(x);
    }
    else if ( event->x > 3 && event->y > 7 && event->y < 37 )
    {
      std::cout << "DIAL" << std::endl;
      clickedWidget = CLICKED_WIDGET_DIAL;
    }
    */
  }
  
  return true;
}
bool GMasterReturn::on_button_release_event(GdkEventButton* event)
{
  if( event->type == GDK_BUTTON_RELEASE && event->button == 1 )
  {
    mouseX = -1;
    mouseY = -1;
  }
  
  clickedWidget = CLICKED_WIDGET_NONE;
  
  return true;
}

bool GMasterReturn::onMouseMove(GdkEventMotion* event)
{
  /*
  if ( clickedWidget == CLICKED_WIDGET_FADER )
  {
    GMasterReturnState* state = &stateStore->trackoutputState.at(ID);
    
    float volume = 1 - ((event->y-4) / 94.f);
    
    //float mouseYdelta = (mouseY - event->y) / 25.f;
    //std::cout << "MouseYdelta: " << mouseYdelta << std::endl;
    
    EngineEvent* x = new EngineEvent();
    
    // move volume relative to current value
    x->setMixerVolume(ID, volume);
    top->toEngineQueue.push(x);
    
    // reset mouseY
    mouseY = event->y;
  }
  else if ( clickedWidget == CLICKED_WIDGET_DIAL )
  {
    GMasterReturnState* state = &stateStore->trackoutputState.at(ID);
    
    float mouseYdelta = (mouseY - event->y) / 94.f;
    std::cout << "MouseYdelta: " << mouseYdelta << std::endl;
    
    float tmpPan = mouseYdelta + state->pan;
    
    if ( tmpPan > 1.f )
      tmpPan = 1.f;
    else if ( tmpPan < -1.f )
      tmpPan = -1.f;
    
    EngineEvent* x = new EngineEvent();
    x->setTrackPan(ID, tmpPan);
    top->toEngineQueue.push(x);
    
    // reset mouseY
    mouseY = event->y;
  }
  */
  
  return true;
  
}

GMasterReturn::~GMasterReturn()
{
}
