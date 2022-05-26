%{
#include "Tonic.h"
%}

namespace Tonic {
  
  namespace Tonic_ 
  {

    class BasicDelay_ : public WetDryEffect_
    {
    public:
      
      BasicDelay_();
      ~BasicDelay_();
      

      void setInput( Generator input );      
      void initialize(float delayTime, float maxDelayTime);
      
      void setDelayTimeGen( Generator gen );
            
      void setFeedbackGen( Generator gen );
            
    };
    
  
  class BasicDelay : public TemplatedWetDryEffect<BasicDelay, Tonic_::BasicDelay_>
  {
    
  public:
  
    BasicDelay(float initialDelayTime = 0.5f, float maxDelayTime = -1);
    
    
    BasicDelay& delayTime(Generator arg);
    BasicDelay& delayTime(float arg);
    BasicDelay& delayTime(ControlGenerator arg);


    BasicDelay& feedback(Generator arg);
    BasicDelay& feedback(float arg);
    BasicDelay& feedback(ControlGenerator arg);

  };
}
