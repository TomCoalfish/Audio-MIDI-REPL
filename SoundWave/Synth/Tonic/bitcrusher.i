%{
#include "Tonic.h"
%}

namespace Tonic {
  
  namespace Tonic_ {
  
    class BitCrusher_ : public Effect_{      
    public:
      BitCrusher_();
      void setBitDepth(ControlGenerator);
      void setInput( Generator input );
      void setIsStereoInput(bool stereo);
      
    };
    
  class BitCrusher : public TemplatedEffect<BitCrusher, Tonic_::BitCrusher_>{
    
    public:
    
         BitCrusher& bitDepth(float arg);
         BitCrusher& bitDepth(ControlGenerator arg);

  };
  
}