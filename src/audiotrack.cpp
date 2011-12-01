
#include "audiotrack.hpp"

#include "top.hpp"
#include "ladspahost.hpp"

int AudioTrack::privateID = 0;

AudioTrack::AudioTrack( Top* t) :
                        source(t, "sample.wav")
{
  top = t;
  addEffect( 1, new LadspaHost(EFFECT_TRANSIENT, top->samplerate) );
}

void AudioTrack::addEffect( int pos,  Effect* eff )
{
  std::list<Effect*>::iterator iter =  effects.begin();
  
  std::advance(iter, pos);
  
  effects.insert( iter , eff);
}

void AudioTrack::process(int nframes, float* buffer)
{
  
  // get audio from source
  source.process(nframes, buffer);
  
  std::list<Effect*>::iterator iter;
  
  for(iter = effects.begin(); iter != effects.end(); iter++ )
  {
    (*iter)->process( nframes, &buffer[0] );
  }
  
}


