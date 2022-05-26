%{
#include "Tonic.h"
%}

namespace Tonic{
  
  namespace Tonic_ 
  {
    
    class BufferFiller_ : public Generator_ 
    {
    public:
      
      BufferFiller_();
      ~BufferFiller_();
      
      void lockMutex();
      void unlockMutex();
      void tick( TonicFrames& frames );      
      void fillBufferOfFloats(float *outData,  unsigned int numFrames, unsigned int numChannels);

    };
    
    
  class BufferFiller : public Generator 
  {
    
  public:
    
    BufferFiller(Tonic_::BufferFiller_ * newBf);

    void fillBufferOfFloats(float *outData,  unsigned int numFrames, unsigned int numChannels);
  };
  
  template<class GenType>
  class TemplatedBufferFiller : public BufferFiller 
  {  
  public:
    TemplatedBufferFiller();
  };
  
}
