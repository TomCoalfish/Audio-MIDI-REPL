/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "SimpleLookAndFeel.h"
#include "SimpleKnob.h"

//==============================================================================
/**
*/
class StateVariableFilterAudioProcessorEditor  : public AudioProcessorEditor,
                                                 public Slider::Listener
{
public:
    StateVariableFilterAudioProcessorEditor (StateVariableFilterAudioProcessor&);
    ~StateVariableFilterAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    // void sliderValueChanged (Slider* slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    void sliderValueChanged (Slider* slider) override;
    void initSlider(String paramID, int index, std::unique_ptr<SliderAttachment> * attachment);
    void setSliderText(String paramID);
    void calculateBounds();
    void changeTypeText(int typeValue);
    
    StateVariableFilterAudioProcessor& processor;
    
    SimpleLookAndFeel simpleLookAndFeel;

    int knobArraySize {5};
    SimpleKnob knobArray [5];
    
    std::unique_ptr<SliderAttachment> cutoffAttachment;
    std::unique_ptr<SliderAttachment> qAttachment;
    std::unique_ptr<SliderAttachment> typeAttachment;
    std::unique_ptr<SliderAttachment> inGainAttachment;
    std::unique_ptr<SliderAttachment> outGainAttachment;
    
    HashMap<String, int> knobMap;
    Rectangle<int> title;
    Label mStateText;
    String stateString;
    int titleHeight, labelHeight, reduction, textScale;
    // Rectangle<int> title, cutoffText, cutoffKnob, qText, qKnob, stateText, stateKnob, stateBox, cutoffBox, qBox;
    float titleFontSize, labelFontSize;
    int dragSensitivity;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StateVariableFilterAudioProcessorEditor)
};
