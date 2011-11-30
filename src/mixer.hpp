
#ifndef LUPPP_MIXER
#define LUPPP_MIXER

#include <list>
#include <iostream>
#include <memory>

#include "audiotrack.hpp"

using std::list;

class Top;

class Mixer
{
  public:
    Mixer(Top*);
    
    void process( int nframes, float* portList );
    
  private:
    Top* top;
    
    list<AudioTrack> audiotrackList;
    
    std::vector<float> outputBuffer;
    
};

#endif
