%{
#include "Tonic.h"
%

namespace Tonic 
{  
  struct ControlGeneratorOutput{
    
    TonicFloat  value;
    bool        triggered;
    
    ControlGeneratorOutput();
  };

  namespace Tonic_
  {
    class ControlGenerator_
    {      
    public:
    
      ControlGenerator_();
      virtual ~ControlGenerator_();
            
      // mutex for swapping inputs, etc
      void lockMutex();
      void unlockMutex();
      
      // Only override tick if you need custom reuse behavior
      // Pass in a pointer to a TonicFloat to return a value. Some generators may not care about value.
      virtual ControlGeneratorOutput tick( const SynthesisContext_ & context );
      
      // Used for initializing other generators (see smoothed() method for example)
      virtual ControlGeneratorOutput initialOutput();
    };
    

  // forward declaration
  class RampedValue;

  class ControlGenerator : public TonicSmartPointer<Tonic_::ControlGenerator_>
  {

  public:
    
    ControlGenerator(Tonic_::ControlGenerator_ * cGen = new Tonic_::ControlGenerator_);
    
    ControlGeneratorOutput tick( const Tonic_::SynthesisContext_ & context );    
    RampedValue smoothed(float length = 0.05);
    
  };

  
  template<class GenType>
  class TemplatedControlGenerator : public ControlGenerator
  {    
  public:
    TemplatedControlGenerator();
  };  
}
