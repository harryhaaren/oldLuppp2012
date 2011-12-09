
#include "g_statestore.hpp"

using namespace std;

GuiStateStore::GuiStateStore()
{
  numTracks = 0;
}

void GuiStateStore::addTrack()
{
  std::cout << "GuiStateStore::addTrack()" << std::endl;
  
  fileAudioSourceState.push_back( FileAudioSourceState() );
  FileAudioSourceState* i = &fileAudioSourceState.back();
  i->index = 0.f;
  i->speed = 1.f;
  
  clipSelectorState.push_back( ClipSelectorState() );
  ClipSelectorState* c = &clipSelectorState.back();
  c->ID = 1;
  
  eqState.active = true;
  eqState.type = FREQUENCY_GRAPH_TYPE_PARAMETRIC;
  for(int i = 0; i < 4; i++)
  {
    eqState.Q[i] = 1.0;
    eqState.cutoffFreq[i] = (i/4.f) * 10000;
    eqState.gain[i] = 0.f;
  }
  
  trackoutputState.push_back( TrackOutputState() );
  trackoutputState.back().ID = numTracks;
  trackoutputState.back().selected = false;
  trackoutputState.back().volume = 0.f;
  trackoutputState.back().pan = 0.f;
  
  if ( numTracks == 1 )
    trackoutputState.back().selected = true;
  
  
  cout << "GuiStateStore::addTrack()  index , speed = " << i->index << ", " << i->speed << endl;
  
  numTracks++;
}
