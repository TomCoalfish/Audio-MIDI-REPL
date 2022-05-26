namespace Tonic {
  
  namespace Tonic_ {
  
    class ADSR_ : public Generator_{
            
    public:
      
      ADSR_();
      ~ADSR_();
            
      void setTrigger(ControlGenerator trig);
      void setAttack(ControlGenerator gen);
      void setDecay(ControlGenerator gen);
      void setSustain(ControlGenerator gen);
      void setRelease(ControlGenerator gen);
      
      //! Exponential or linear ramp
      void setIsExponential(ControlGenerator gen);
      
      //! Controls whether the envelope picks up from current position or zero when re-triggered while still releasing
      void setIsLegato(ControlGenerator gen);
      
      //! Controls whether or not the envelope pauses on the SUSTAIN stage
      void setDoesSustain(ControlGenerator gen);
      
    };
  }    
  
  /*!
    Classic ADSR envlelope. Non-zero trigger values correspond to key down. Trigger values of zero correspond to keyup.
    Time values are rounded up to the nearest buffer size.
    Time values are in milliseconds. 
  */
  
  class ADSR : public TemplatedGenerator<Tonic_::ADSR_>
  {    
    public:
    
      ADSR(float attack = 0.001f, float decay = 0.03f, float sustain = 1.0f, float release = 0.05f);
    
      ADSR& trigger(float arg);
      ADSR& trigger(ControlGenerator arg);
      ADSR& attack(float arg);
      ADSR& attack(ControlGenerator arg);
      ADSR& decay(float arg);
      ADSR& decay(ControlGenerator arg);
      ADSR& sustain(float arg);
      ADSR& sustain(ControlGenerator arg);
      ADSR& release(float arg);
      ADSR& release(ControlGenerator arg);
      ADSR& exponential(float arg);
      ADSR& exponential(ControlGenerator arg);
      ADSR& doesSustain(float arg);
      ADSR& doesSustain(ControlGenerator arg);
      ADSR& legato(float arg);
      ADSR& legato(ControlGenerator arg);
  };
  
}
