
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
#ifndef LUPPP_GCOMPRESSOR
#define LUPPP_GCOMPRESSOR

#include <vector>
#include <iostream>

#include <gtkmm/drawingarea.h>

#include "top.hpp"
#include "g_statestore.hpp"

#include "g_widgetbase.hpp"

// inherit from WidgetBase so that we can define the ID of the widget,
// which tell us which indice of the EffectStateVector to use to redraw
class GCompressor : public Gtk::DrawingArea, public WidgetBase
{
  public:
    GCompressor(Top*, GuiStateStore*);
    ~GCompressor();
    
    bool redraw();
  
  protected:
    int ID;
    GuiStateStore* stateStore;
    
    Top* top;
    
    float cutoff;
    float q;
    int xSize, ySize;
    
    bool mouseDown;
    
    
    //Override default signal handler:
    bool onMouseMove(GdkEventMotion* event);
    bool on_expose_event			(GdkEventExpose* event);
    bool on_button_press_event(GdkEventButton* event);
    bool on_button_release_event(GdkEventButton* event);
    
    // For Right-Click Actions
    void on_menu_file_popup_edit();
    void on_menu_file_popup_generic();
};

#endif // SCENE_SELECTOR
