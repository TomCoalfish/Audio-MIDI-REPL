%{
#include "Tonic.h"
%}

namespace Tonic 
{
  
  namespace Tonic_ 
  {

    class BufferPlayer_ : public Generator_
    {
    public:
      BufferPlayer_();
      ~BufferPlayer_();
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
      void setBuffer(SampleTable sampleTable);
      void setDoesLoop(ControlGenerator doesLoop);
      void setTrigger(ControlGenerator trigger);
      void setStartPosition(ControlGenerator startPosition);      
    };
        
  }
  
  /*!
    Simply plays back a buffer. "loop" parameter works, but doesn't wrap between ticks, so mostly likely you'll wind up with a few zeroes at the end of 
    the last buffer if you're looping. In other words, buffer lenghts are rounded up to the nearest kSynthesisBlockSize 
   
    Usage:
    
    SampleTable buffer = loadAudioFile("/Users/morganpackard/Desktop/trashme/2013.6.5.mp3");
    bPlayer.setBuffer(buffer).loop(false).trigger(ControlMetro().bpm(100));
   
  */
  
  class BufferPlayer : public TemplatedGenerator<Tonic_::BufferPlayer_>{
    
  public:
  
    BufferPlayer& setBuffer(SampleTable buffer);
    
    BufferPlayer& loop(float arg);
    BufferPlayer& loop(ControlGenerator arg);
    BufferPlayer& trigger(float arg);
    BufferPlayer& trigger(ControlGenerator arg);
    BufferPlayer& startPosition(float arg);
    BufferPlayer& startPosition(ControlGenerator arg);

  };
}
