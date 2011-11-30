
#ifndef LUPPP_JACK_CLIENT
#define LUPPP_JACK_CLIENT

#include <jack/jack.h>
#include <jack/types.h>
#include <jack/midiport.h>
#include <jack/transport.h>

#include <vector>
#include <sndfile.hh>

typedef jack_client_t JClient;
typedef jack_port_t JPort;

class JackClient
{
  public:
    JackClient();
    
    void deactivate();
    
  private:
    JClient* client;
    
    JPort* sidechainPort;
    JPort* inputPort;
    JPort* outputPort;
    JPort* midiInputPort;
    
    int process(jack_nframes_t);
    static int static_process(jack_nframes_t, void *);
    
    int framesPerBeat;
    int nframeCounter;
    
    void writeMidi(int,int,int);
    void* midiBuffer;
    void* midiInputBuffer;
};

#endif
