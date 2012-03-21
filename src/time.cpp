
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

#include "time.hpp"

#include "top.hpp"
#include "engineevent.hpp"

using namespace std;

Time::Time(Top* t)
{
  top = t;
  
  beat = -1;
  
  automoveDuration = 4;
}

void Time::startAutomoveType(int type)
{
  cout << "starting Automove type " << type << " now!" << endl;
  automoveType = type;
  automoveStartFrame = top->frameNumber;
}

// this function gets called by JACK before processing any audio for
// nframes, allowing it to modify Engine's state before it's run.
void Time::process(int frameNumber)
{
  float bpm = top->bpm;
  
  int framesPerBeat = (int) top->samplerate / (bpm / 60.0);
  
  int newBeat = frameNumber / framesPerBeat;
  
  top->frameNumber = frameNumber;
  
  
  if ( automoveType != AUTOMOVE_TYPE_NONE )
  {
    // work out the current progress & value, then send that to statestore
    int automoveProgressFrames = frameNumber - automoveStartFrame;
    
    float automoveProgress = float(automoveProgressFrames) / ( automoveDuration * framesPerBeat );
    //cout << "Time::process() Automove progress = " << automoveProgress << " automoveProgressFrames: " << automoveProgressFrames << endl;
    
    switch ( automoveType )
    {
      case AUTOMOVE_TYPE_UP: top->state.globalUnit = automoveProgress / automoveDuration; break;
      default: break;
    }
    
    // line done, now turn off!
    if ( automoveProgress > automoveDuration )
    {
      top->state.globalUnit = 0.f;
      automoveType = AUTOMOVE_TYPE_NONE;
    }
  }
  
  // here we handle *all* events that should occur on *a* beat
  if ( newBeat != beat )
  {
    //cout << "Time:P() bpm = " << bpm << "  FPB: " << framesPerBeat << "  beat: " << newBeat << " Q1: " << q1.size() << "  Q4: " << q4.size() << endl;
    beat = newBeat;
    
    // always process q1 on new beat
    if ( !q1.empty() )
    {
      if ( !q1.empty() ) {
        //cout << "Time:process() doing list 1!" << endl;
        doEngineEventList(q1);
        q1.clear();
      }
    }
    
    if ( newBeat % 4 == 0 )
    {
      cout << "Time:process() doing 4th list!" << endl;
      if ( !q4.empty() ) {
        doEngineEventList(q4);
      }
    }
    
    /*
    if ( newBeat % 8 == 0 )
    {
      cout << "Time:process() doing 8th list!  q8.size() = " << q8.size() << endl;
      if ( !q8.empty() ) {
        doEngineEventList(q8);
      }
      cout << " done processing 8 list, clearing NOW! " << endl;
      q8.clear();
    }
    
    if ( newBeat % 16 == 0 )
    {
      cout << "Time:process() doing 16th list!  q16.size() = " << q16.size() << endl;
      if ( !q16.empty() ) {
        doEngineEventList(q16);
      }
      cout << " done processing 16 list, clearing NOW! " << endl;
    }
    */
    
  } // new beat
  
}

void Time::processEngineEvent(EngineEvent* e)
{
  if ( e->type == EE_LOOPER_SELECT_BUFFER )
  {
    cout << "Time::processEE() LOOPER_SELECT_BUFFER queue in 4... type = " << e->type << "  T: " << e->ia << "  clip = " << e->ib << endl;
    if ( false ) // quantize check here?
    {
      top->state.clipSelectorQueueClip(e->ia, e->ib);
      q4.push_back(e);
    }
    else
      doEngineEvent(e);
  }
  if ( e->type == EE_TRACK_SET_PLUGIN_PARAMETER )
  {
    //cout << "Time::processEE() SET_PLUGIN_PARAM queue in 1" << endl;
    //q1.push_back(e);
    doEngineEvent(e);
  }
  if ( e->type == EE_TRACK_DEVICE_ACTIVE )
  {
    doEngineEvent(e);
    //q1.push_back(e);
  }
  if ( e->type == EE_SCENE_NUMBER )
  {
    doEngineEvent(e);
  }
}

// this function will do *all* events in a queue, and then return
void Time::doEngineEventList(std::list<EngineEvent*>& list)
{
  int eventCounter = 0;
  std::list<EngineEvent*>::iterator iter;
  for ( iter = list.begin(); iter != list.end(); iter++ )
  {
    eventCounter++;
    doEngineEvent( *iter );
  }
  
  //cout << "Time::doEngineEventList() Done " << eventCounter << " events, clearing NOW!" << endl;
  list.clear();
  
  return;
}

void Time::doEngineEvent(EngineEvent* e)
{
  cout << "doEngineEvent()  type = " << e->type << endl;
  switch ( e->type )
  {
    case EE_LOOPER_PROGRESS: cout << "Time::doEngineEvent() got PROGRESS!!! ERROR!" << endl;;
    case EE_LOOPER_SELECT_BUFFER:
    {
      cout << "Doing SELECT BUFFER NOW! t=" << e->ia << " clip#: " << e->ib << endl;
      top->state.clipSelectorActivateClip(e->ia, e->ib);
      break;
    }
    case EE_SCENE_NUMBER:
    {
      cout << "Doing EE_SCENE_NUMBER NOW! scene = " << e->ia << endl;
      int block = e->ia;
      int numTracks = top->state.getNumTracks();
      for ( int i = 0; i < numTracks; i++)
      {
        cout << "EE_SCENE_NUMBER " << block << " on track " << i << endl; 
        top->state.clipSelectorActivateClip(i, block);
      }
      break;
    }
    case EE_TRACK_DEVICE_ACTIVE: top->state.setPluginActive(e->ia, e->ib); break;
    case EE_TRACK_SET_PLUGIN_PARAMETER: top->state.setPluginParameter( e->ia, e->ic, e->fa ); break;
    default: cout << "Time::doEE() Error: got type " << e->type << " which is not handled yet!" << endl;
  }
}














