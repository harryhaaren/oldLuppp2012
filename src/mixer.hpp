
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
    
    // need to pass in pointer, not create
    void addTrack();
    
    int addEffect(int, int, Effect* );
    
    // returns the UniqueID of an Effect in an AudioTrack
    int getEffectID(int track, int pos);
    
    void process( int nframes, float* outBuffer );
    
  private:
    Top* top;
    
    list<AudioTrack> audiotrackList;
    
    std::vector<float> inputBuffer;
    
    std::vector<float> outputW;
    std::vector<float> outputX;
    std::vector<float> outputY;
    std::vector<float> outputZ;
    
};

#endif
