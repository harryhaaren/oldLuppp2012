
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

#ifndef LUPPP_AUDIOTRACK
#define LUPPP_AUDIOTRACK

class Top;

#include <list>
#include <memory>

#include "effect.hpp"
#include "audiosink.hpp"
#include "audiosource.hpp"

class AudioTrack
{
  public:
    AudioTrack(Top*);
    AudioTrack(Top*,bool);//for master track
    
    int setSource( AudioSource* );
    int addEffect( int, Effect* );
    
    void process(int nframes, float* ports, float*,float*,float*,float*);
    
    int getEffectID(int pos);
  
  protected:
    int ID;
    static int privateID;
    
    Top* top;
    
    // Audio elements
    AudioSink*    sink;
    AudioSource*  source;
    
    // list of effects
    std::list<Effect*> effects;
    
    std::vector<float> trackBuffer;
};

#endif

