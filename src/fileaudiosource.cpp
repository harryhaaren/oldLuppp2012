
#include "top.hpp"

#include "fileaudiosource.hpp"

int FileAudioSource::privateID = 0;

FileAudioSource::FileAudioSource( std::string name)
{
  //top = t;
  
  ID = privateID++;
  
  index = 0.f;
  speed = 1.f;
  
  if (ID == 0)
    name = "sample.wav";
  else if ( ID == 1 )
    name = "sample2.wav";
  
  SndfileHandle infile( name , SFM_READ,  SF_FORMAT_WAV | SF_FORMAT_FLOAT , 1 , 44100);

  int size  = infile.frames();

  if ( size == 0 )
  {
    std::cout << "FileAudioSource() " << ID << ":  Could not load sound file, or empty file detected!" << std::endl;
    return;
  }
  
  //int tmpChan= infile.channels();
  int tmpSR  = infile.samplerate();

  //std::cout << "Sampler sample info: size:" << size << ", sr:" << tmpSR << std::flush;

  // initialize new buffer to hold <size> elements
  sampleBuffer.resize(size);

  //std::cout << "\t sampleBuffer->resize("<<size<<")"<<std::endl;

  infile.read( &sampleBuffer.at(0) , size );
}

void FileAudioSource::setParameter(int p, float s)
{
  speed = s;
}

void FileAudioSource::process (int nframes, float* buffer )
{
  //std::list<FileAudioSourceState>::iterator iter = top->fileAudioSourceState.begin();
  
  // playback the sample
  int size = sampleBuffer.size();
  
  if ( size == 0 ) 
    return;
  
  for (int i = 0; i < nframes; i++ )
  {
    if ( (int)index * size >= size )
    {
      std::cout << "Index = 0" << std::endl;
      index = 0.f;
    }
    
    *buffer++  += sampleBuffer.at(index * size);
    index = index + ( speed / size );
  }
  //std::cout << "FAS: process() " << index << "  nframes: " << nframes << std::endl;
}
