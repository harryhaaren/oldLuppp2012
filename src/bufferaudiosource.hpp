
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

#ifndef LUPPP_FILEAUDIOSOURCE
#define LUPPP_FILEAUDIOSOURCE

#include <list>
#include <string>
#include <vector>
#include <iostream>
#include <sndfile.hh>

#include "audiosource.hpp"

#include "bufferaudiosourcestate.hpp"

#include "top.hpp"

using std::string;

class BufferAudioSource : public AudioSource
{
  public:
    BufferAudioSource(Top*);
    void process (int, float* );
  
  private:
    Top* top;
    int ID;
    
    // pitch shifting variables (extraced from faust)
    int    IOTA;
    float  fVec0[65536];
    float  fslider0;
    float  fslider1;
    float  fRec0[2];
    float  fslider2;
    float  fSamplingFreq;
    
    int guiUpdateCounter;
    
    std::vector<float> tmpBuffer;
    
    void pitchShift(int count, float* input, float* output);
};


#endif
