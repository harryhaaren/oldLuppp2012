
#include "g_compressor.hpp"

#include "g_widgets.hpp"

using namespace std;
using namespace Luppp;

int GCompressor::privateID = 0;

GCompressor::GCompressor(Top* t, GuiStateStore* s)
{
  ID = privateID++;
  
  top = t;
  stateStore = s;
  
  cutoff = 0.40;
  q = 1.0;
  
  mouseDown = false;
  
  //std::cout << "Enterin GCompressor contructor" << std::endl;
  add_events(Gdk::EXPOSURE_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK| Gdk::POINTER_MOTION_MASK);
  signal_button_press_event().connect(sigc::mem_fun(*this, &GCompressor::on_button_press_event) );
  signal_button_release_event().connect(sigc::mem_fun(*this, &GCompressor::on_button_release_event) );
  signal_motion_notify_event().connect( sigc::mem_fun( *this, &GCompressor::onMouseMove ) );
  
  set_size_request(130, 216);
}

bool GCompressor::redraw()
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


bool GCompressor::on_expose_event(GdkEventExpose* event)
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
    
    // update value from stateStore
    float cutoffRangeZeroOne = stateStore->effectState.at(0).values[0];
    float ratio = stateStore->effectState.at(0).values[1];
    
    // invert range, so 0 = most cut on threshold ( -30dB ), 1 = thresh @ 0dB
    float thresh = cutoffRangeZeroOne;
    
    bool active = true;
    
    int x = 10;
    int y = 22;
    xSize = 110;
    ySize = 95;
    
    // works but a bit simple
    cr -> move_to( x        , y         );
    cr -> line_to( x + xSize, y         );
    cr -> line_to( x + xSize, y + ySize );
    cr -> line_to( x        , y + ySize );
    cr -> close_path();
    
    // Draw outline shape
    cr -> set_source_rgb (0.1,0.1,0.1);
    cr -> fill();
    
    // draw "guides"
    std::valarray< double > dashes(2);
    dashes[0] = 2.0;
    dashes[1] = 2.0;
    cr->set_dash (dashes, 0.0);
    cr->set_line_width(1.0);
    cr->set_source_rgb (0.4,0.4,0.4);
    for ( int i = 0; i < 4; i++ )
    {
      cr->move_to( x + ((xSize / 4.f)*i), y );
      cr->line_to( x + ((xSize / 4.f)*i), y + ySize );
    }
    for ( int i = 0; i < 4; i++ )
    {
      cr->move_to( x       , y + ((ySize / 4.f)*i) );
      cr->line_to( x +xSize, y + ((ySize / 4.f)*i) );
    }
    cr->stroke();
    cr->unset_dash();
    
    // draw "normal" line
    setColour(cr, COLOUR_GREY_3 );
    cr->set_line_width(2.5);
    cr->move_to( x , y + ySize );
    cr->line_to( x + xSize, y );
    cr->stroke();
    
    float xDist = 0.1 * xSize;
    float yDist = 0.1 * ySize;
    
    float xThresh = x + xSize * thresh;
    float yThresh = y + ySize*(1-thresh);
    
    float startx = xThresh - xDist;
    float starty = yThresh + yDist;
    
    float cp1x = xThresh;
    float cp1y = yThresh;
    
    float cp2x = xThresh;
    float cp2y = yThresh;
    
    float endx = xThresh + xDist;
    float endy = yThresh; //(ySize-(ySize-yThresh))*(1-ratio);
    
    // move to bottom left, draw line to middle left
    cr->set_line_cap(Cairo::LINE_CAP_ROUND);
    cr->move_to( x , y + ySize );
    cr->line_to( startx, starty );
    
    cout << " Ratio = " << ratio << " CP1 : " << cp1x << "\t" << cp1y << "\t"<< cp2x<<  "\t" <<cp2y<<  "\t" <<endx<<  "\t" <<endy << endl;
    
    // draw curve
    cr->curve_to( cp1x, cp1y, cp2x, cp2y, endx, endy );
    
    cr->line_to(x + xSize, y + ySize*(1-thresh)*(ratio) );
    
    cr->line_to(x + xSize, y + ySize );
    cr->close_path();
    /*
    if ( xSizeCP1 > 234 )
      xSizeCP1 = 234;
    if ( xSizeCP2 > 234 )
      xSizeCP2 = 234;
    if ( xSizeEnd > 234 )
      xSizeEnd = 234;
    */
    
    setColour(cr, COLOUR_BLUE_1, 0.2 );
    cr->close_path();
    cr->fill_preserve();
    
    // stroke gain line
    cr->set_line_width(2.5);
    if ( active )
      setColour(cr, COLOUR_BLUE_1 );
    else
      setColour(cr, COLOUR_GREY_1 );
    cr->stroke();
    
    // debug rectangles of CP's
    setColour(cr, COLOUR_ORANGE_1);
    cr->rectangle(cp1x,cp1y,3,3);
    cr->rectangle(cp2x,cp2y,3,3);
    cr->fill();
    // start & end
    setColour(cr, COLOUR_GREEN_1);
    cr->rectangle(startx,starty,3,3);
    cr->rectangle(endx  ,endy  ,3,3);
    cr->fill();
    
    // threshold point
    setColour(cr, COLOUR_PURPLE_1);
    cr->rectangle( xThresh, yThresh,3,3);
    cr->fill();
    
    /*
    // click center
    setColour(cr, COLOUR_ORANGE_1, 0.9 );
    cr->arc( x + thresh * xSize , endy, 7, 0, 6.2830 );
    cr->stroke();
    */
    
    // dials
    Dial(cr, active, 70-48, 140-15, cutoffRangeZeroOne, DIAL_MODE_NORMAL);
    Dial(cr, active, 68   , 125   , ratio             , DIAL_MODE_NORMAL);
    
    // outline
    setColour(cr, COLOUR_GREY_3 );
    cr->rectangle( x, y , xSize, ySize );
    cr->set_line_width(3);
    
    if ( active )
      setColour(cr, COLOUR_GREY_2 );
    else
      setColour(cr, COLOUR_GREY_3 );
    cr->stroke();
    
    TitleBar(cr, 0,0 , 250, 216, "Compressor", true);
    
    /*
    if ( state.selected )
    {
      cr->rectangle(0, 0, 74, 216);
      setColour( cr, COLOUR_PURPLE_1 );
      cr->set_line_width(1);
      cr->stroke();
    }
    */
    
  }
  return true;
}

void GCompressor::on_menu_file_popup_edit()
{
   std::cout << "The Edit menu item was selected." << std::endl;
}

void GCompressor::on_menu_file_popup_generic()
{
   std::cout << "A popup menu item was selected." << std::endl;
}

bool GCompressor::onMouseMove(GdkEventMotion* event)
{
  if ( mouseDown )
  {
    if ( (event->x > 50) && (event->x < 216) )
    {
      cutoff = event->x / float(xSize);
      
      EngineEvent* x = new EngineEvent();
      x->setPluginParameter(0,0,0, cutoff);
      top->toEngineQueue.push(x);
    }
    
    if ( (event->y > 35) && (event->y < 103) )
    {
      q = event->y / float(ySize);
      EngineEvent* x = new EngineEvent();
      x->setPluginParameter(0,0,1, q );
      top->toEngineQueue.push(x);
    }
    redraw();
    std::cout << "GCompressor: Cutoff = " << cutoff << "  Q: " << q << "  X, Y: " << event->x << ", " << event->y << std::endl;
  }
}

bool GCompressor::on_button_press_event(GdkEventButton* event)
{
  if( event->type == GDK_BUTTON_PRESS  ) // && event->button == 3
  {
    int x = 10;
    int y = 22;
    xSize = 225;
    ySize = 95;
    
    // graph area
    if ( (event->x > 10) && (event->x < 235) &&
         (event->y > 22) && (event->y < 117 ) )
    {
      std::cout << "graph area click!" << std::endl;
      mouseDown = true; // for pointer motion "drag" operations
      
      int evX = event->x;
      // inform engine of "click" and position co-efficents as such
      if ( evX < 50) evX = 50;
      if ( evX > 216)evX = 216;
      
      stateStore->cutoff = evX / float(xSize);
      cutoff = stateStore->cutoff;
      EngineEvent* x = new EngineEvent();
      x->setPluginParameter(0,0,0, cutoff );
      top->toEngineQueue.push(x);
      
      int evY = event->y;
      if (evY < 35 ) evY = 35;
      if (evY > 103) evY = 103;
      
      q = evY / float(ySize);
      x = new EngineEvent();
      x->setPluginParameter(0,0,1, q );
      top->toEngineQueue.push(x);
      redraw();
    }
    
    if ( event->y < 20 )
    {
      std::cout << "GCompressor Enable / Disable click event!" << std::endl;
      EngineEvent* x = new EngineEvent();
      x->setTrackDeviceActive(0,0,1 );
      top->toEngineQueue.push(x);
    }
    
    return true; //It's been handled.
  }
  else
    return false;
}

bool GCompressor::on_button_release_event(GdkEventButton* event)
{
  if( event->type == GDK_BUTTON_RELEASE  ) // && event->button == 3
  {
    mouseDown = false;
    
    return true; //It's been handled.
  }
  else
    return false;
}

GCompressor::~GCompressor()
{
}