%{
#include "Tonic.h"
%}
namespace Tonic {
  
  namespace Tonic_ 
  {
    class ControlCounter_ : public ControlGenerator_
    {
    public:

      ControlCounter_();
      
      void setTrigger(ControlGenerator gen);
      void setEnd(ControlGenerator gen);
      
    };
    
  
  class ControlCounter : public TemplatedControlGenerator<Tonic_::ControlCounter_>{
    
  public:
  
    ControlCounter& trigger(float arg);
    ControlCounter& trigger(ControlGenerator arg);
    ControlCounter& end(float arg);
    ControlCounter& end(ControlGenerator arg);

  };
}