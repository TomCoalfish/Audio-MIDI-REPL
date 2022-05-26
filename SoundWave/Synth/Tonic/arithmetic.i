%{
#include "Tonic.h"
%}

namespace Tonic {
  
  // -----------------------------------------
  //                 ADDER
  // -----------------------------------------
  
  namespace Tonic_{
      
    class Adder_ : public Generator_ 
    {
    public:
      
      Adder_();
      
      void input(Generator generator);
      void setIsStereoOutput( bool stereo );
      
      Generator getInput(unsigned int index);
      unsigned int numInputs();
            
    };
    
  class Adder : public TemplatedGenerator<Tonic_::Adder_>
  {
  public:
    
    Adder input(Generator input);
    Generator operator[](unsigned int index);
    unsigned long numInputs();
  };
  
  
  
  // -----------------------------------------
  //                SUBTRACTOR
  // -----------------------------------------
  
  namespace Tonic_ 
  {
    
    class Subtractor_ : public Generator_{
    public:
      
      Subtractor_();
      
      void setLeft(Generator arg);
      void setRight(Generator arg);
      void setIsStereoOutput( bool stereo );
      
    };        
  }
  
  class Subtractor : public TemplatedGenerator<Tonic_::Subtractor_>
  {
    
  public:
    
    Subtractor& left(Generator arg);
    Subtractor& left(float arg);
    Subtractor& right(Generator arg);
    Subtractor& right(float arg);
    Subtractor& right(ControlGenerator arg);
    
  };
  
  
  
  // -----------------------------------------
  //                MULTIPLIER
  // -----------------------------------------
  
  namespace Tonic_{
    
    class Multiplier_ : public Generator_
    {
    public:

      Multiplier_();
      
      void input(Generator generator);
      void setIsStereoOutput( bool stereo );
      
      Generator getInput(unsigned int index);
      unsigned int numInputs();

    };
    
    
  
  class Multiplier : public TemplatedGenerator<Tonic_::Multiplier_>
  {
  public:

    Multiplier input(Generator inputSource);

    %extend {
        Generator __getitem__(unsigned int index) { return (*$self)[index]; }
    }

    unsigned long numInputs();
  };
  
  
  // -----------------------------------------
  //                DIVIDER
  // -----------------------------------------
  
  namespace Tonic_ {
    
    class Divider_ : public Generator_
    {    
    public:

      Divider_();
      
      void setLeft(Generator arg);
      void setRight(Generator arg);
      void setIsStereoOutput( bool stereo );
      
    };
    
  }
  
  class Divider : public TemplatedGenerator<Tonic_::Divider_>
  {
  
  public:
    Divider& left(Generator arg);
    Divider& left(float arg);
    Divider& left(ControlGenerator arg);
    Divider& right(Generator arg);
    Divider& right(float arg);
    Divider& right(ControlGenerator arg);
    
  };
  
  
}

