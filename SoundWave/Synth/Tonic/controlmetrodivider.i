%{
#include "Tonic.h"
%}
namespace Tonic 
{  
  namespace Tonic_ {

    class ControlMetroDivider_ : public ControlConditioner_ 
    {
    public:

      ControlMetroDivider_();
      
      void setDivisionsGen( ControlGenerator gen );
      void setOffsetGen( ControlGenerator gen );
      
    };
  }
  
  class ControlMetroDivider : public TemplatedControlConditioner<ControlMetroDivider, Tonic_::ControlMetroDivider_>
  {
  public:
    
    ControlMetroDivider& divisions(float arg);
    ControlMetroDivider& divisions(ControlGenerator arg);
    ControlMetroDivider& offset(float arg);
    ControlMetroDivider& offset(ControlGenerator arg);
    
  };
}
