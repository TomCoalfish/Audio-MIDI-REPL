/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class StateVariableFilterAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    StateVariableFilterAudioProcessor();
    ~StateVariableFilterAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    // parameters
    // float cutoff, q, inGain, outGain; // the Q (resonance) of the filter = probably best in the 0.5 to 20 range, but you may want to set your highest Q to a lower value
    /*AudioParameterFloat * cutoff, * q, * inGain, * outGain;
    AudioParameterInt * type;
     
     */
    AudioProcessorValueTreeState apvts;
    
private:
    
    float input; // input sample
    float low, band, high; // output samples, corresponding to lowpass, bandpass, highpass outputs
    float prevInGain, prevOutGain;
    float notch; // notch filter output
    float f1, q1; // internal variables generated from parameters
    float d1 [2], d2 [2]; // state variables
    AudioProcessorValueTreeState::ParameterLayout createParameters();
    void setGain (AudioBuffer<float>& buffer, float curGain, bool isInGain);
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StateVariableFilterAudioProcessor)
};
