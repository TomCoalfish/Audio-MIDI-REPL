%module tonic
%{
#include "Tonic.h"
using namespace Tonic;
%}

%include "generator.i"
%include "tablelookupssc.i"
%include "adsr.i"
%include "arithmetic.i"
%include "audiofileutils.i"

namespace Tonic {
  SampleTable loadAudioFile(string path, int numChannels = 2);  
}
