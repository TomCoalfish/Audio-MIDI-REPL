%{
#include "Tonic.h"
%}

namespace Tonic {
  
  namespace Tonic_ 
  {

    class BLEPOscillator_ : public Generator_
    {
    public:

      BLEPOscillator_();
      ~BLEPOscillator_();
      
      void setFreqGen(Generator gen) { freqGen_ = gen; };
      
    };
}