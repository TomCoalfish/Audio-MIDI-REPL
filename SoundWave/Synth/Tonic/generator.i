%{
#include "Tonic.h"
%}

namespace Tonic 
{
    class Generator_
    {      
        public:
        
        Generator_();
        virtual ~Generator_();
        
        virtual void tick( TonicFrames& frames, const SynthesisContext_ &context );

        bool isStereoOutput();
        virtual void setIsStereoOutput( bool stereo );
        
    };
 
    class SineWave : public TemplatedGenerator<Tonic_::TableLookupOsc_>
    {
    public:

        SineWave();
        SineWave& freq(Generator arg);

    };
}