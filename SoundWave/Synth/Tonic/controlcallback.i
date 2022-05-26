%{
#include "Tonic.h"
%}

namespace Tonic {
  
  namespace Tonic_ {

    class ControlCallback_ : public ControlConditioner_{
      
    public:
      void setCallback(function<void(ControlGeneratorOutput)> fn);
      
    };
    
  }
  
  
  class ControlCallback : public TemplatedControlConditioner<ControlCallback, Tonic_::ControlCallback_>{
    
  public:
  
    ControlCallback(Synth* synth, function<void(ControlGeneratorOutput)>);
    

  };
}
