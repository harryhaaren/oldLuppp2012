
#include "audiotrack.hpp"

#include "top.hpp"

#include "bufferaudiosource.hpp"
#include "fluidsynthaudiosource.hpp"

#include "beatsmash.hpp"
#include "ladspahost.hpp"

#include "trackoutputstate.hpp"
#include "audiosinkoutput.hpp"

int AudioTrack::privateID = 0;

using namespace std;

AudioTrack::AudioTrack( Top* t )
{
  top = t;
  
  ID = privateID++;
  
  std::cout << "Creating AudioTrack ID: " << ID << std::endl;
  
  trackBuffer.resize(top->bufferSize);
  
  
  // default source for a track is the BufferAudioSource, we can exchange
  // it for a synth, but that's not implemented (at time of writing)
  source = new BufferAudioSource(t);
  
  sink = new AudioSinkOutput(t);
}

AudioTrack::AudioTrack( Top* t, bool masterTrack )
{
  top = t;
  
  ID = -1;
  
  std::cout << "Creating MASTER AudioTrack ID: " << ID << std::endl;
  
  trackBuffer.resize(top->bufferSize);
  
  
  // default source for a track is the BufferAudioSource, we can exchange
  // it for a synth, but that's not implemented (at time of writing)
  //source = new BufferAudioSource(t);
  
  //sink = new AudioSinkOutput(t);
}

int AudioTrack::setSource( AudioSource* newSource )
{
  cout << "AudioTrack::setSource() got new source pointer!" << endl;
  AudioSource* tmpSource = source;
  source = newSource;
  // here we should send an EE to GUI thread to de-allocate the
  // existing source, so we don't leak mem.
}

int AudioTrack::addEffect( int pos,  Effect* eff )
{
  //cout << "AudioTrack::addEffect() pushing to back " << pos << flush;
  //std::list<Effect*>::iterator iter =  effects.begin();
  //std::advance(iter, pos);
  
  effects.push_back(eff);
  //cout << "\tdone!" << endl;
  return 0;
}

int AudioTrack::getEffectID(int pos)
{
  // range check the list
  if ( pos < effects.size() )
  {
    std::list<Effect*>::iterator iter =  effects.begin();
    std::advance(iter, pos);
    return (*iter)->getID();
  }
  else
  {
    cout << "AudioTrack::getEffectID() OOB! p: " << pos << endl;
  }
  return -1;
}

void AudioTrack::process(int nframes, float* buffer, float* W, float* X, float* Y, float* Z)
{
  //std::cout << "AudioTrack::process()" << std::endl;
  
  // get audio from source
  source->process(nframes, &trackBuffer[0]);
  
  for(std::list<Effect*>::iterator iter = effects.begin(); iter != effects.end(); iter++ )
  {
    (*iter)->process( nframes, &trackBuffer[0] );
  }
  
  // do volume & panning
  sink->process(nframes, &trackBuffer[0], W, X, Y, Z);
  
}


