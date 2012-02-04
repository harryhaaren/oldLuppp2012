
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
#include "g_waveview.hpp"

#include <iostream>
#include <cairomm/context.h>
#include <sstream>

#include "g_widgets.hpp"

#include "top.hpp"
#include "g_statestore.hpp"

using namespace std;
using namespace Luppp; // for COLOUR_*

GWaveView::GWaveView(Top* t)
{
  top = t;
  
  // initial size for the waveview
  waveviewSize = 11050;
  
  // initial sample to start editing
  sampleID = 0;
  
  // Gives Exposure & Button presses to the widget.
  add_events(Gdk::EXPOSURE_MASK);
  add_events(Gdk::BUTTON_PRESS_MASK );
  
  // add labels to menu, attach them to the right function, and add to menu
  setSize1Sec.set_label("1 second");
  setSizeHalfSec.set_label("1/2 second");
  setSizeQuaterSec.set_label("1/4 second");
  setSizeEigthSec.set_label("1/8 second");
  
  setSize1Sec.signal_activate().connect     ( sigc::bind (sigc::mem_fun(*this, &GWaveView::setSampleID) , 0 ) );
  setSizeHalfSec.signal_activate().connect  ( sigc::bind (sigc::mem_fun(*this, &GWaveView::setSampleID) , 1 ) );
  setSizeQuaterSec.signal_activate().connect( sigc::bind (sigc::mem_fun(*this, &GWaveView::setSampleID) , 2 ) );
  setSizeEigthSec.signal_activate().connect ( sigc::bind (sigc::mem_fun(*this, &GWaveView::setSampleID) , 3 ) );
  
  pMenu.add( setSize1Sec );
  pMenu.add( setSizeHalfSec );
  pMenu.add( setSizeQuaterSec );
  pMenu.add( setSizeEigthSec );
  pMenu.show_all();
  
  // set default widget size
  set_size_request(148 + 74,22 + 95 + 2);
}

void GWaveView::setSize(int size)
{
  std::cout << "GWaveView setting size to " << size << std::endl;
  waveviewSize = size;
  
  if ( waveviewSize > sample.size() )
  {
    sample.clear();
    sample.push_back(0);
    sample.push_back(0);
  }
  
  redraw();
  
  return;
}

void GWaveView::setSampleID(int id)
{
  std::cout << "GWaveView setting sample ID to " << id << std::endl;
  sampleID = id;
  redraw();
  return;
}


void GWaveView::setSample(const std::vector<float>& inSample)
{
  //std::cout << "void setSample() inSamp.size() = " << inSample.size() << std::endl;
  
  for ( int i = 0; i < inSample.size(); i++ )
  {
    sample.push_back(inSample.at(i));
  }
  
  while ( sample.size() > waveviewSize )
    sample.erase( sample.begin(),sample.begin()+ 128);
}

GWaveView::~GWaveView()
{
}

bool GWaveView::on_expose_event(GdkEventExpose* event)
{
    // This is where we draw on the window
    Glib::RefPtr<Gdk::Window> window = get_window();
    
    if(window)    // Only run if Window does exist
    {
      Gtk::Allocation allocation = get_allocation();
      width = allocation.get_width();
      height = allocation.get_height();
      
      // coordinates for the center of the window
      int xc, yc;
      xc = width / 2;
      yc = height / 2;
      
      Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
      
      // clip to the area indicated by the expose event so that we only redraw
      // the portion of the window that needs to be redrawn
      cr->rectangle(event->area.x, event->area.y,event->area.width, event->area.height);
      cr->clip();
      
      int x = 10;
      int y = 22;
      int xSize = width - 20;
      int ySize = height - 20;
      
      // works but a bit simple
      cr -> move_to( x        , y         );
      cr -> line_to( x + xSize, y         );
      cr -> line_to( x + xSize, y + ySize );
      cr -> line_to( x        , y + ySize );
      cr -> close_path();
      
      // Draw outline shape
      cr -> set_source_rgb (0.1,0.1,0.1);
      cr -> fill();
      
      // get waveform details
      WaveformCache* waveviewCache = top->guiState->getWaveformCache(sampleID);
      if ( waveviewCache == 0 )
      {
        // no cache generated for this waveform yet!
        //std::cout << "No waveview cache exists for ID " << sampleID << endl;
        
        //draw X shape over full widget graph size:
        cr->move_to( x        , y         );
        cr->line_to( x + xSize, y + ySize );
        
        cr->move_to( x        , y + ySize );
        cr->line_to( x + xSize, y         );
        
        setColour(cr, COLOUR_GREY_2 );
        cr->stroke();
        
        // outline
        setColour(cr, COLOUR_GREY_3 );
        cr->rectangle( x, y , xSize, ySize );
        cr->set_line_width(3);
        cr->stroke();
        
        return;
      }
      
      
      // draw guides
      std::valarray< double > dashes(2);
      dashes[0] = 2.0;
      dashes[1] = 2.0;
      cr->set_dash (dashes, 0.0);
      cr->set_line_width(1.5);
      cr->set_source_rgb (0.7,0.7,0.7);
      
      // horizontal lines
      for ( int i = 0; i < 2; i++ )
      {
        cr->move_to( x       , y + ((ySize / 2.f)*i) );
        cr->line_to( x +xSize, y + ((ySize / 2.f)*i) );
      }
      
      // float type for divides later
      float verticals = waveviewCache->getBeats();
      
      // vertical lines
      for ( int i = 0; i < verticals; i++ )
      {
        cr->move_to( x + ((xSize / verticals)*i), y );
        cr->line_to( x + ((xSize / verticals)*i), y + ySize );
      }
      cr->stroke();
      
      // 8th lines
      cr->set_line_width(1.0);
      cr->set_source_rgb (0.2,0.2,0.2);
      for ( int i = 0; i < 8; i++ )
      {
        cr->move_to( x + ((xSize / 8.f)*i), y );
        cr->line_to( x + ((xSize / 8.f)*i), y + ySize );
      }
      cr->stroke();
      cr->unset_dash();
      
      cr->move_to( x, y + ySize*0.5 );
      
      float previousTop = 0;
      float previousLow = 0;
      
      float currentTop = 0;
      
      int sampleCountForDrawing = 25;
      
      sample = *( waveviewCache->getPointer() );
      
      // get the current "playing" beat of the loop
      float progress = top->guiState->bufferAudioSourceState.at(sampleID).index;
      int numBeats = waveviewCache->getBeats();
      
      cout << "Got waveviewCache for ID " << sampleID << " sample size = " << sample.size() << " num beats: " << numBeats << "  progress = " << progress << endl;
      
      int beat = (int)(numBeats * progress);
      
      setColour( cr, COLOUR_GREEN_1, 0.3 );
      int beatStart = x + beat * (float(width-x*2) / numBeats);
      cr->rectangle( beatStart , y, float(width-x*2) / numBeats, ySize );
      cr->fill();
      
      // loop for drawing each Point on the widget.
      for (long index=0; index <(long)sample.size(); index++ )
      {
        float currentSample = sample.at(index);
        
        if ( currentSample > 0 && currentTop < currentSample ) // top
        {
          currentTop = currentSample;
        }
        sampleCountForDrawing--;
        
        if ( sampleCountForDrawing < 0 ) // top line
        {
          float drawSample = currentTop / 4.f;
          
          int xCoord = x + ( xSize * ( float(index) / sample.size() ) );
          
          cr->move_to( xCoord, y + (ySize/2) - (previousTop * ySize )  ); // top
          cr->line_to( xCoord, y + (ySize/2) + (drawSample  * ySize )  ); // bottom
          
          sampleCountForDrawing = 15;
          previousTop = drawSample;
          currentTop = 0;
        }
      }
      
      setColour(cr, COLOUR_GREY_1 );
      cr->stroke();
      
      // outline
      setColour(cr, COLOUR_GREY_3 );
      cr->rectangle( x, y , xSize, ySize );
      cr->set_line_width(3);
      cr->stroke();
      
      std::string name = "Start";
      std::string mid = "Middle";
      std::string nameStop = "Stop";
      
      drawMarker(cr, 0.f, COLOUR_GREEN_1, MARKER_START, name);
      
      drawMarker(cr, 0.5f, COLOUR_PURPLE_2, MARKER_MIDDLE, mid);
      
      drawMarker(cr, 1.f, COLOUR_RECORD_RED, MARKER_END, nameStop );
    }
  return true;
} // on_expose_event()

void GWaveView::drawMarker(Cairo::RefPtr<Cairo::Context> cr, float position, Colour col, MarkerType type, std::string& name)
{
  int x = 10;
  int y = 22;
  
  // marks for start
  int markXsize = 40;
  int markX = x + position * ((width-markXsize) - x * 2);
  int markY = y / 2;
  
  int markYsize = y - 6;
  
  setColour(cr, col);
  if ( type == MARKER_START || type == MARKER_MIDDLE )
    cr->rectangle( markX, markY, 1, height);
  else
    cr->rectangle( markX + markXsize, markY, 1, height);
  cr->fill();
  
  setColour(cr, col);
  cr->rectangle( markX, markY, markXsize, markYsize );
  cr->fill();
  
  if ( type == MARKER_START || type == MARKER_MIDDLE )
  {
    cr->move_to( markX + markXsize, markY );
    cr->line_to( markX + markXsize + 9, markY + markYsize/2 );
    cr->line_to( markX + markXsize, markY + markYsize );
    cr->close_path();
    cr->fill();
  }
  else
  {
    cr->move_to( markX    , markY               );
    cr->line_to( markX - 9, markY + markYsize/2 );
    cr->line_to( markX    , markY + markYsize   );
    cr->close_path();
    cr->fill();
  }
  
  cr->select_font_face ("Impact" , Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD);
  cr->set_font_size ( 13 );
  cr->move_to ( markX + 3, y + 2 );
  setColour(cr, COLOUR_GREY_4);
  cr->show_text ( name );
}

void GWaveView::redraw()
{
  // force our program to redraw the entire widget.
  Glib::RefPtr<Gdk::Window> win = get_window();
  if (win)
  {
    Gdk::Rectangle r(0, 0, get_allocation().get_width(),get_allocation().get_height());
    win->invalidate_rect(r, false);
  }
}

bool GWaveView::on_button_press_event(GdkEventButton* event)
{
  if( event->type == GDK_BUTTON_PRESS && event->button == 3 )
  {
    std::cout << "Event Type: " << event->type << ". Event Button: " << event->button << "." << std::endl;
    pMenu.popup(event->button, event->time);
  }
  
  return true;
}
