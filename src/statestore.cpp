
#include "statestore.hpp"

#include "top.hpp"

#include "jackclient.hpp"
#include "effectstate.hpp"

using namespace std;

StateStore::StateStore(Top* t)
{
  numTracks = 0;
  top = t;
}

void StateStore::addEffect(int t, int pos, Effect* effect)
{
  // here we are passed a pointer to the desired effect,
  // and told what track & position to insert it
}

void StateStore::addTrack(BufferAudioSourceState* bas, ClipSelectorState* css, TrackOutputState* tos)
{
  cout << "StateStore::addTrack() BuffAudioSource* = " << bas
                              << "  ClipSelectorState "<< css
                              << "  TrackOutputState " << tos << endl;
  bufferAudioSourceState.push_back( *bas );
  
  // initialize the clipSelectorState for this track
  clipSelectorState.push_back( ClipSelectorState() );
  ClipSelectorState* c = &clipSelectorState.back();
  c->ID = -1;
  for ( int i = 0; i < 10; i++ )
  {
    c->clipInfo.push_back( ClipInfo() );
  }
  
  // initialize the TrackOutputState
  trackoutputState.push_back( TrackOutputState() );
  std::list<TrackOutputState>::iterator outputStateIter = trackoutputState.begin();
  std::advance(outputStateIter, numTracks);
  outputStateIter->ID = numTracks;
  outputStateIter->selected = false;
  outputStateIter->volume = 0.f;
  outputStateIter->pan = 0.f;
  outputStateIter->panZ = 0.f;
  
  numTracks++;
}

void StateStore::setAudioBuffer(AudioBuffer* bufPtr)
{
  cout << "StateStore::setAudioBuffer() Got AudioBuffer ID " << bufPtr->getID() << endl;
  audiobufferList.push_back( *bufPtr );
}

AudioBuffer* StateStore::getAudioBuffer(int ID)
{
  if ( ID == -1 ) // -1 means not loaded, save itering the list :)
    return 0;
  
  std::list<AudioBuffer>::iterator iter;
  
  for ( iter = audiobufferList.begin(); iter != audiobufferList.end(); iter++ )
  {
    if ( iter->getID() == ID )
    {
      return &(*iter);
    }
  }
  
  return 0;
}

void StateStore::addEffectState(int ID)
{
  cout << "StateStore::addEffectState()  ID = " << ID << endl;
  // store the new ID in the EffectState, so it can be retrieved later
  effectStateList.push_back ( new EffectState( ID ) );
}

// this function takes the UniqueID of the Effect, and returns the corresponding
// EffectState as a pointer. This way we can shuffle the position of Effects,
// without having to update the stateStore in the same way (more effort / bugs)
EffectState* StateStore::getEffectState(int ID)
{
  std::list<EffectState*>::iterator iter;
  for ( iter = effectStateList.begin(); iter != effectStateList.end(); iter++ )
  {
    if ( (*iter)->ID == ID )
    {
      return &(*(*iter));
    }
  }
  
  return 0;
}

void StateStore::setVolume(int t, float v)
{
  if ( !trackCheck(t) ) {
    std::cout << "StateStore::setVolume() track OOB" << std::endl; return;
  }
  
  std::list<TrackOutputState>::iterator iter = trackoutputState.begin();
  std::advance(iter, t);
  
  // Clip vol input to range
  if (v > 1.0)
    v = 1.0;
  else if (v < 0.00001)
    v = 0.0;
  
  // linear input, hence we need to convert the [0,1] range
  // to a logarithmic scale. Formula used:
  // y = x ^ 3, this is an approximation of the human hearing curve.
  // http://www.dr-lex.be/info-stuff/logVolumecontrols.html for info
  float volMultiplier = (float)pow( v , 3);
  
  // apply scaling to volMultiplier, so we can amplify to +50%
  float logVolume = volMultiplier * 2;
  
  iter->volume = logVolume;
  
  EngineEvent* x = top->toEngineEmptyEventQueue.pull();
  x->setMixerVolume(t, v);
  top->toGuiQueue.push(x);
  top->guiDispatcher->emit();
  
  //std::cout << "StateStore::setVolume() Track: " << t << ", linVol:" << v << "  logVol:" << logVolume << std::endl;
}

void StateStore::setPan(int t, float v)
{
  if ( !trackCheck(t) ) {
    std::cout << "StateStore::setPan() track OOB" << std::endl; return;
  }
  std::list<TrackOutputState>::iterator iter = trackoutputState.begin();
  std::advance(iter, t);
  iter->pan = v;
  std::cout << "New pan: " << iter->pan << std::endl;
  
  EngineEvent* x = top->toEngineEmptyEventQueue.pull();
  x->setTrackPan(t, v);
  top->toGuiQueue.push(x);
  top->guiDispatcher->emit();
}

void StateStore::setMute(int t, int v)
{
  if ( !trackCheck(t) ) {
    std::cout << "StateStore::setMute() track OOB" << std::endl; return;
  }
  std::list<TrackOutputState>::iterator iter = trackoutputState.begin();
  std::advance(iter, t);
  iter->mute = v;
  
  EngineEvent* x = top->toEngineEmptyEventQueue.pull();
  x->setTrackMute(t, v);
  top->toGuiQueue.push(x);
  top->guiDispatcher->emit();
}

void StateStore::setSolo(int t, int v)
{
  if ( !trackCheck(t) ) {
    std::cout << "StateStore::setSolo() track OOB" << std::endl; return;
  }
  std::list<TrackOutputState>::iterator iter = trackoutputState.begin();
  std::advance(iter, t);
  iter->solo = v;
  
  EngineEvent* x = top->toEngineEmptyEventQueue.pull();
  x->setTrackSolo(t, v);
  top->toGuiQueue.push(x);
  top->guiDispatcher->emit();
}

void StateStore::setRec(int t, int v)
{
  if ( !trackCheck(t) ) {
    std::cout << "StateStore::setRec() track OOB" << std::endl; return;
  }
  std::list<TrackOutputState>::iterator iter = trackoutputState.begin();
  std::advance(iter, t);
  iter->rec = v;
  
  EngineEvent* x = top->toEngineEmptyEventQueue.pull();
  x->setTrackRec(t, v);
  top->toGuiQueue.push(x);
  top->guiDispatcher->emit();
}

void StateStore::setPanZ(int t, float v)
{
  if ( !trackCheck(t) ) {
    std::cout << "StateStore::setPanZ() track OOB" << std::endl; return;
  }
  std::list<TrackOutputState>::iterator iter = trackoutputState.begin();
  std::advance(iter, t);
  iter->panZ = v;
}

void StateStore::setClipSelectorState(int t,int block, int bufferID)
{
  std::list<ClipSelectorState>::iterator iter = clipSelectorState.begin();
  std::advance(iter, t);

  std::list<ClipInfo>::iterator clipI = iter->clipInfo.begin();
  std::advance(clipI, block);
  (*clipI).state = CLIP_STATE_LOADED;
  (*clipI).bufferID = bufferID;
  
  // bufferID here is used to extract name from GUI list of AudioBuffer names
  EngineEvent* x = top->toEngineEmptyEventQueue.pull();
  x->looperLoad(t, block, bufferID);
  top->toGuiQueue.push(x);
  top->guiDispatcher->emit();
}

void StateStore::clipSelectorActivateClip(int t, int b)
{
  // we get a track & scene number, so we set them in the ClipSelectorState
  // later the playback will request the bufferID ClipInfo of the right position in the list
  std::cout << "StateStore::clipSelectorQueue() " << t << ", " << b << endl;
  std::list<ClipSelectorState>::iterator iter = clipSelectorState.begin();
  std::advance(iter, t);
  
  // get info of current Clip & update APC off / loaded for previous block
  std::list<ClipInfo>::iterator infoIter = iter->clipInfo.begin();
  std::advance(infoIter, iter->playing);
  
  std::cout << "ClipInfo = " << infoIter->state << "  Loaded = " << CLIP_STATE_LOADED << endl;
  
  if ( infoIter->state == CLIP_STATE_EMPTY )
    top->jackClient->writeMidi( top->jackClient->getApcOutputBuffer(), 128 + t, 53 + iter->playing, 0 ); // off
  if ( infoIter->state == CLIP_STATE_LOADED )
    top->jackClient->writeMidi( top->jackClient->getApcOutputBuffer(), 144 + t, 53 + iter->playing, 5 ); // orange
  
  std::list<BufferAudioSourceState>::iterator iterBASS = bufferAudioSourceState.begin();
  std::advance(iterBASS, t);
  iterBASS->index = 0; // restart sample from beginning
  
  // update *actual* Engine value for currently playing Scene
  iter->playing = b;
  
  // update GUI
  EngineEvent* x = top->toEngineEmptyEventQueue.pull();
  x->looperSelectBuffer(t, b);
  top->toGuiQueue.push(x);
  top->guiDispatcher->emit();
  
  // update APC : (concider moving this functionality into a Controller class,
  // that can then access the MIDI port / OSC / protocolX to update the HW device
  top->jackClient->writeMidi( top->jackClient->getApcOutputBuffer(), 144 + t, b + 53, 1 );
}

void StateStore::setPluginActive(int UID, int active)
{
  std::list<EffectState*>::iterator iter;
  for ( iter = effectStateList.begin(); iter != effectStateList.end(); iter++ )
  {
    if ( (*iter)->ID == UID )
    {
      cout << "StateStore::setPluginActive() " << UID << "  " << active << endl;
      (*iter)->active = active;
      
      EngineEvent* x = top->toEngineEmptyEventQueue.pull();
      x->setTrackDeviceActive(UID,
                              -1,
                              active);
      top->toGuiQueue.push(x);
      
      break;
    }
  }
}

void StateStore::setPluginParameter(int ID, int param, float value)
{
  // all "dynamic" elements in engine ( ladspaHosts, lv2host, etc ) all use
  // the same generic "EffectState" as thier settings. Up to 8 floats of control
  // for now, more is *not* suitable for RT LIVE performance.
  
  //cout << "StateStore::setPluginParameter() " << t << ", " << pos << ", " << param << ", " << value << endl;
  
  // iter over the list, and 
  std::list<EffectState*>::iterator iter;
  for ( iter = effectStateList.begin(); iter != effectStateList.end(); iter++ )
  {
    if ( (*iter)->ID == ID )
    {
      break;
    }
  }
  
  if ( iter == effectStateList.end() )
  {
    std::cout << "StateStore::setPluginPara() Error, did not find EffectState with ID " << ID << std::endl;
    return;
  }
  else
  {
    //std::cout << "StateStore::setPluginParam() writing value " << value << " to param " << param << endl;
    (*iter)->values[param] = value;
    
    // bounce on to GUI (-1 not used)
    EngineEvent* x = top->toEngineEmptyEventQueue.pull();
    x->setPluginParameter(ID, -1, param, value);
    top->toGuiQueue.push(x);
  }
}

TrackOutputState* StateStore::getAudioSinkOutput(int t)
{
  if ( !trackCheck(t) ) {
    std::cout << "StateStore::getAudioSinkOutput() track OOB: " << t << std::endl; return 0;
  }
  
  std::list<TrackOutputState>::iterator iter = trackoutputState.begin();
  std::advance(iter, t);
  
  return &(*iter);
}

ClipSelectorState* StateStore::getClipSelectorState(int t)
{
  if ( !trackCheck(t) ) {
    std::cout << "StateStore::getClipSelectorState() track OOB: " << t << std::endl; return 0;
  }
  
  std::list<ClipSelectorState>::iterator iter = clipSelectorState.begin();
  std::advance(iter, t);
  
  return &(*iter);
}

// convinience functions
bool StateStore::trackCheck(int t)
{
  if ( t >= numTracks )
    return false;
  
  return true;
}

bool StateStore::effectCheck(int t)
{
  if ( t >= numEffects )
    return false;
  
  return true;
}
