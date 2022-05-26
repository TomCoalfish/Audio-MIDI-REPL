%{
#include "Tonic.h"
%}

namespace Tonic {
  
  namespace Tonic_ 
  {

    class Compressor_ : public Effect_{
    public:
      
      Compressor_();

  
      void setInput( Generator input );
      void tick(TonicFrames &frames, const SynthesisContext_ & context );
      void tickThrough(TonicFrames & inFrames, TonicFrames & outFrames, const SynthesisContext_ & context);
        
      void setAudioInput( Generator gen );
      void setAmplitudeInput( Generator gen );
      
      void setMakeupGain( ControlGenerator gen );
      void setAttack( ControlGenerator gen );
      void setRelease( ControlGenerator gen );
      void setThreshold( ControlGenerator gen );
      void setRatio( ControlGenerator gen );
      void setLookahead( ControlGenerator gen );
            
      void setIsLimiter( bool isLimiter );
      void setIsStereo( bool isStereo );
      
    };    
  }
  
  class Compressor : public TemplatedEffect<Compressor, Tonic_::Compressor_>{
    
  public:
    
    Compressor(float threshold = 0.5, float ratio = 2, float attack = 0.001, float release = 0.05, float lookahead = 0.001);
    
    Compressor & audioInput( Generator input );
    
    Compressor & sidechainInput( Generator input );
    
    void setIsStereo( bool isStereo );
    
    Compressor& attack(float arg);
    Compressor& attack(ControlGenerator arg);
    Compressor& release(float arg);
    Compressor& release(ControlGenerator arg);
    Compressor& threshold(float arg);
    Compressor& threshold(ControlGenerator arg);
    Compressor& ratio(float arg);
    Compressor& ratio(ControlGenerator arg);
    Compressor& lookahead(float arg);
    Compressor& lookahead(ControlGenerator arg);
    Compressor& makeupGain(float arg);
    Compressor& makeupGain(ControlGenerator arg);
    
  };
  
  //! Special case of compressor where ratio is fixed to infinite and attack time is fixed to 0.1 ms
  //  Used for peak limiting
  class Limiter : public TemplatedEffect<Limiter, Tonic_::Compressor_>{
    
  public:
    
    Limiter();
    
    void setIsStereo( bool isStereo );
    
    Limiter& release(float arg);
    Limiter& release(ControlGenerator arg);
    Limiter& threshold(float arg);
    Limiter& threshold(ControlGenerator arg);
    Limiter& lookahead(float arg);
    Limiter& lookahead(ControlGenerator arg);
    Limiter& makeupGain(float arg);
    Limiter& makeupGain(ControlGenerator arg);

    
  };

}