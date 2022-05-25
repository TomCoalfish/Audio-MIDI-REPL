%module chorus
%{
#include "chorus.h"
#include "SampleVector.h"
%}

%include "SampleVector.h"
%template(SampleVectorFloat) SoundWave::SampleVector<float>;
%include "chorus.h"
