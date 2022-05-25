%module soundwave
%{


#include <vector>
#include "kissfft.h"
#include "FaustFilters.h"
#include "StateVariableFilter.h"
#include "HybridCurtisVCF.h"
#include "Moog.h"
#include "Envelope.h"
#include "SndFile.hpp"
#include "Resampler.h"
#include "NoiseGenerator.h"
#include "Noise.h"
#include "BandLimitedOscillator.h"
#include "WaveTable.h"

#include "AudioFFT.h"
#include "FFTConvolver.h"
#include "TwoStageFFTConvolver.h"


#include "fo_apf.h"
#include "fo_lpf.h"
#include "fo_hpf.h"
#include "fo_shelving_high.h"
#include "fo_shelving_low.h"
#include "so_apf.h"
#include "so_bpf.h"
#include "so_bsf.h"
#include "so_lpf.h"
#include "so_hpf.h"
#include "so_butterworth_bpf.h"
#include "so_butterworth_bsf.h"
#include "so_butterworth_hpf.h"
#include "so_butterworth_lpf.h"
#include "so_linkwitz_riley_hpf.h"
#include "so_linkwitz_riley_lpf.h"
#include "so_parametric_cq_boost.h"
#include "so_parametric_cq_cut.h"
#include "so_parametric_ncq.h"

#undef pi
#include "samples.hpp"


using namespace SoundWave;
using namespace audiofft;
using namespace fftconvolver;
using namespace FFT;

%}

%include "stdint.i"
%include "std_vector.i"
%include "std_string.i"

%include "sample.i"


%include "AudioFFT.h"
%include "FFTConvolver.h"
%include "kissfft.h"
%include "FaustFilters.h"
%include "StateVariableFilter.h"
%include "HybridCurtisVCF.h"
%include "ADSR.h"
%include "ParametricEnvelope.h"
%include "GammaEnv.h"
%include "SndFile.hpp"
%include "Resampler.h"
%include "Moog.h"
%include "NoiseGenerator.h"
%include "Noise.h"
%include "BandLimitedOscillator.h"
%include "WaveTable.h"                               


%include "filter_common.h"
%include "filter_includes.h"
%include "biquad.h"
%include "biquad_modified.h"
%include "fo_apf.h"
%include "fo_lpf.h"
%include "fo_hpf.h"
%include "fo_shelving_high.h"
%include "fo_shelving_low.h"
%include "so_apf.h"
%include "so_bpf.h"
%include "so_bsf.h"
%include "so_lpf.h"
%include "so_hpf.h"
%include "so_butterworth_bpf.h"
%include "so_butterworth_hpf.h"
%include "so_butterworth_lpf.h"
%include "so_linkwitz_riley_hpf.h"
%include "so_linkwitz_riley_lpf.h"
%include "so_parametric_cq_boost.h"
%include "so_parametric_cq_cut.h"
%include "so_parametric_ncq.h"


