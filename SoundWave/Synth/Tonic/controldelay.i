%{
#include "Tonic.h"
%}

namespace Tonic 
{ 
  namespace Tonic_ 
  {

    class ControlDelay_ : public ControlConditioner_
    {
    public:      
      ControlDelay_();      
      void initialize(float maxDelayTime);      
      void setDelayTimeGen( ControlGenerator gen );      
    };
        
  }
  
  class ControlDelay : public TemplatedControlConditioner<ControlDelay, Tonic_::ControlDelay_>
  {  
  public:
    
    ControlDelay(float maxDelayTime = 1.0f);
    
    ControlDelay& delayTime(float arg);
    ControlDelay& delayTime(ControlGenerator arg);
    
  };
}