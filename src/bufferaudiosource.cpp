
#include "bufferaudiosource.hpp"

#include "audiobuffer.hpp"

using namespace std;

BufferAudioSource::BufferAudioSource(Top* t)
{
  top = t;
  ID = AudioSource::getID();
  cout << "BufferAudioSource() ID = " << ID << endl;
}

void BufferAudioSource::process (int nframes, float* buffer )
{
  // get playback variables
  std::list<BufferAudioSourceState>::iterator iter = top->state.bufferAudioSourceState.begin();
  std::advance(iter, ID);
  float index = iter->index;
  float speed = iter->speed;
  //int audioBufferID = iter->bufferID;
  
  // get buffer variables
  ClipSelectorState* clipSelState = top->state.getClipSelectorState(ID);
  if ( !clipSelState )
    return;
  
  int playingScene = clipSelState->playing;
  if ( playingScene < 0 )
    return;
  
  std::list<ClipInfo>::iterator clipIter = clipSelState->clipInfo.begin();
  std::advance(clipIter, playingScene);
  int playingBuffer = (*clipIter).bufferID;
  //std::cout << "FAS:P()   sceneID: " << playingScene << "   bID = " << playingBuffer << endl;
  
  // get AudioBuffer pointer
  AudioBuffer* bufPtr = top->state.getAudioBuffer(playingBuffer);
  if ( bufPtr == 0 ) {
    // happens until a valid file has been loaded for this 
    //cout << "BufferAudioSource, bufPtr == 0! returning..." << endl;
    return;
  }
  
  // playback the sample
  int size = bufPtr->getPointer()->size();
  std::vector<float>* tmpVector = bufPtr->getPointer();
  
  if ( size == 0 )
    return;
  
  for (int i = 0; i < nframes; i++ )
  {
    if ( (int)(index*size) >= size )
    {
      //std::cout << "Index = 0" << std::endl;
      index = 0.f;
    }
    
    *buffer++  += tmpVector->at( (int)(index * size) );
    index = index + ( speed / size );
  }
  
  // write values to state
  iter->index = index;
  iter->speed = speed;
  
  //std::cout << "FAS: process() " << index << "  nframes: " << nframes << std::endl;
}