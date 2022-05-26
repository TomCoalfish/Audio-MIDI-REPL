%{
#include "Tonic.h"
%}

namespace Tonic {
  
  namespace Tonic_ {

    class ControlMetro_ : public ControlGenerator_
    {
    public:

      ControlMetro_();
      
      void setBPMGen( ControlGenerator bpmGen );
      
    };    
  }
  
  
  class ControlMetro : public TemplatedControlGenerator<Tonic_::ControlMetro_>
  {    
  public:
    
    ControlMetro(float bpm = 120);
     ControlMetro& bpm(float arg);
     ControlMetro& bpm(ControlGenerator arg);
  };
}