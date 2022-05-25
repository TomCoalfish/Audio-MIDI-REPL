#pragma once
#include <cmath>


using std::pow;
using std::tan;
using std::sqrt;

namespace SoundWave {

//==============================================================================


//==============================================================================

/** The type of filter that the State Variable Filter will output. */
enum SVFType {
    SVFLowpass = 0,
    SVFBandpass,
    SVFHighpass,
    SVFUnitGainBandpass,
    SVFBandShelving,
    SVFNotch,
    SVFAllpass,
    SVFPeak
};

//==============================================================================
class StateVariableFilter {
public:
    /** Create and initialize the filter with default values defined in constructor. */
    StateVariableFilter();

    //------------------------------------------------------------------------------

    ~StateVariableFilter();
    
    //------------------------------------------------------------------------------
    
    /**	Sets the type of the filter that processAudioSample() or processAudioBlock() will 
        output. This filter can choose between 8 different types using the enums listed 
        below or the int given to each.
        0: SVFLowpass
        1: SVFBandpass
        2: SVFHighpass
        3: SVFUnitGainBandpass
        4: SVFBandShelving
        5: SVFNotch
        6: SVFAllpass
        7: SVFPeak
    */
    void setFilterType(const int& newType);

    //------------------------------------------------------------------------------
    /**	Used for changing the filter's cutoff parameter logarithmically by 
        pitch (MIDI note #)
    */
    void setCutoffPitch(const float& newCutoff);

    //------------------------------------------------------------------------------
    /**	Used for changing the filter's cutoff parameter linearly by frequency (Hz) */
    void setCutoffFreq(const float& newCutoff);

    //------------------------------------------------------------------------------
    /** Used for setting the resonance amount. This is then converted to a Q
        value, which is used by the filter.
        Range: (0-1)
    */
    void setResonance(const float& newResonance);

    //------------------------------------------------------------------------------
    /** Used for setting the filter's Q amount. This is then converted to a
        damping parameter called R, which is used in the original filter.
    */
    void setQ(const float& newQ);

    //------------------------------------------------------------------------------
    /**	Sets the gain of the shelf for the BandShelving filter only. */
    void setShelfGain(const float& newGain);

    //------------------------------------------------------------------------------
    /**	Statically set the filters parameters. */
    void setFilter(const int& newType, const float& newCutoff, 
                   const float& newResonance, const float& newShelfGain);

    //------------------------------------------------------------------------------	
    /**	Set the sample rate used by the host. Needs to be used to accurately
        calculate the coefficients of the filter from the cutoff.
        Note: This is often used in AudioProcessor::prepareToPlay
    */
    void setSampleRate(const float& newSampleRate);

    //------------------------------------------------------------------------------
    /**	Sets the time that it takes to interpolate between the previous value and
        the current value. For this filter, the smoothing is only happening for
        the filters cutoff frequency.
    */
    //void setSmoothingTimeInMs(const float& newSmoothingTimeMs);

    //------------------------------------------------------------------------------
    /** Sets whether the filter will process data or not.
        - If (isActive = true) then the filter will process data
        - If (isActive = false) then the filter will be bypassed
    */
    void setIsActive(bool isActive);

    //------------------------------------------------------------------------------
    /**	Performs the actual processing for one sample of data, on 2 channels.
        If 2 channels are needed (stereo), use channel index (channelIdx) to 
        specify which channel is being processed (i.e. 0 for left, 1 for right).
    */
    float processAudioSample(const float& input, const int& channelIndex);

    //------------------------------------------------------------------------------	
    /**	Performs the actual processing for a block of samples, on 2 channels.
        If 2 channels are needed (stereo), use channel index (channelIdx) to 
        specify which channel is being processed (i.e. 0 for left, 1 for right).
        Note:
        This processes the information sent to the samples argument and 
        does it through a pointer. Therefore, no value needs to be
        returned.
    */
    void processAudioBlock(float* const samples, const int& numSamples,
                           const int& channelIndex);

    //------------------------------------------------------------------------------

    float Tick(float input) {        
        return processAudioSample(input,0);        
    }
    void Process(size_t n, float * input, float * output) {
        for(size_t i = 0; i < n; i++) output[i] = Tick(input[i]);
    }
    void Process(float * samples, size_t n) {
        Process(n,samples,samples);
    }
    
private:
    //==============================================================================
    //	Calculate the coefficients for the filter based on parameters.
    void calcFilter();

    //	Parameters:
    int filterType;
    float cutoffFreq;
    float Q;
    float shelfGain;

    float sampleRate;
    bool active = true;	// is the filter processing or not

    //	Coefficients:
    float gCoeff;		// gain element 
    float RCoeff;		// feedback damping element
    float KCoeff;		// shelf gain element

    float z1_A[2], z2_A[2];		// state variables (z^-1)

};

};
