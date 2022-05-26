%{
#include "Tonic.h"
%}

namespace Tonic 
{
  namespace Tonic_ 
  {
    
    class ControlConditioner_ : public ControlGenerator_
    {
    public:
      
      void input( ControlGenerator input );
      
    };
    
  }
  
  
  template<class ControlConditionerType, class ControlConditionerType_>
  class TemplatedControlConditioner : public TemplatedControlGenerator<ControlConditionerType_>
  {    
  public:
    
    ControlConditionerType input( ControlGenerator input );    
    ControlConditionerType input( float inputVal );
  };
}