%{
#include "Tonic.h"
%}

namespace Tonic {
  
  namespace Tonic_ {
    
    //! Absrtract comb-filter gen_ base class
    class CombFilter_ : public Effect_{
      
    public:
      
      CombFilter_();
      
      void initialize(float initialDelayTime, float maxDelayTime);      
      void setDelayTimeGen(Generator gen);
      void setScaleFactorGen(ControlGenerator gen);
                  
    };
    
    //! Basic mono feed-forward comb filter
    class FFCombFilter_ : public CombFilter_{
            
    };
    
    //! Basic mono feedback comb filter with hook for aditional processing pre-feedback
    class FBCombFilter_ : public CombFilter_
    {
      
    };
  
    
    // ------------------------
    
    //! Feedback comb filter with 6dB/oct (one-pole) lpf and hpf, optimized for reverb
    class FilteredFBCombFilter6_ : public CombFilter_
    {      
    public:
      
      FilteredFBCombFilter6_();
      
      void setLowCutoff( ControlGenerator gen );
      void setHighCutoff( ControlGenerator gen );
      
    };
        
  }
  
  // -----------------------
  
  // TODO: Could template-ify these for DRY-er subclassing...
  
  class FFCombFilter : public TemplatedEffect<FFCombFilter, Tonic_::FFCombFilter_>{

  public:
    
    FFCombFilter(float initialDelayTime = 0.1f, float maxDelayTime = -1);
    FFCombFilter& delayTime(Generator arg);
    FFCombFilter& delayTime(float arg);
    FFCombFilter& delayTime(ControlGenerator arg);
    FFCombFilter& scaleFactor(float arg);
    FFCombFilter& scaleFactor(ControlGenerator arg);
  };
  
  // -----
  
  class FBCombFilter : public TemplatedEffect<FBCombFilter, Tonic_::FBCombFilter_>{
    
  public:
    
    FBCombFilter(float initialDelayTime = 0.1f, float maxDelayTime = -1);
    FBCombFilter& delayTime(Generator arg);
    FBCombFilter& delayTime(float arg);
    FBCombFilter& delayTime(ControlGenerator arg);
    FBCombFilter& scaleFactor(float arg);
    FBCombFilter& scaleFactor(ControlGenerator arg);
  };
  
  // ------
  
  class FilteredFBCombFilter6 : public TemplatedEffect<FilteredFBCombFilter6, Tonic_::FilteredFBCombFilter6_>
  {
  public:
    FilteredFBCombFilter6(float initialDelayTime = 0.1f, float maxDelayTime = -1);
    FilteredFBCombFilter6& delayTime(Generator arg);
    FilteredFBCombFilter6& delayTime(float arg);
    FilteredFBCombFilter6& delayTime(ControlGenerator arg);
    FilteredFBCombFilter6& scaleFactor(float arg);
    FilteredFBCombFilter6& scaleFactor(ControlGenerator arg);
    FilteredFBCombFilter6& lowpassCutoff(float arg);
    FilteredFBCombFilter6& lowpassCutoff(ControlGenerator arg);
    FilteredFBCombFilter6& highpassCutoff(float arg);
    FilteredFBCombFilter6& highpassCutoff(ControlGenerator arg);
  };
}