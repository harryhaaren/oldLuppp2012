#ifndef LUPPP_CLIPSELECTOR
#define LUPPP_CLIPSELECTOR

#include <vector>
#include <iostream>

#include <gtkmm/drawingarea.h>

#include "g_statestore.hpp"

#include "clipselectorstate.hpp"

class ClipSelector : public Gtk::DrawingArea
{
  public:
    ClipSelector(GuiStateStore*);
    ~ClipSelector();
    
    bool redraw();
  
  protected:
    int ID;
    static int privateID;
    GuiStateStore* stateStore;
    
    
    //Override default signal handler:
    bool on_expose_event			(GdkEventExpose* event);
    bool on_button_press_event(GdkEventButton* event);
    
    // For Right-Click Actions
    void on_menu_file_popup_edit();
    void on_menu_file_popup_generic();
};

#endif // SCENE_SELECTOR
