#ifndef LUPPP_INSTRUMENTSELECTOR
#define LUPPP_INSTRUMENTSELECTOR

#include <vector>
#include <iostream>

#include <gtkmm/drawingarea.h>

#include "top.hpp"
#include "g_statestore.hpp"
#include "clipselectorstate.hpp"

class InstrumentSelector : public Gtk::DrawingArea
{
  public:
    InstrumentSelector(Top*,GuiStateStore*);
    
    bool redraw();
  
  protected:
    Top* top;
    GuiStateStore* stateStore;
    
    int block;
    
    std::string currentDir;
    std::list<std::string> fileList;
    
    //Override default signal handler:
    bool on_expose_event			(GdkEventExpose* event);
    bool on_button_press_event(GdkEventButton* event);
    
    // drop functions
    void dropFunction( const Glib::RefPtr<Gdk::DragContext>& context, int x, int y,
                       const Gtk::SelectionData& selection_data, guint info, guint time);
    
    void dragFunction( const Glib::RefPtr<Gdk::DragContext>& context,
                       Gtk::SelectionData& selection_data, guint info, guint time);
    
    // For Right-Click Actions
    void on_menu_file_popup_edit();
    void on_menu_file_popup_generic();
};

#endif // SCENE_SELECTOR
