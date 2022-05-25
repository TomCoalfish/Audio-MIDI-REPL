/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SimpleLookAndFeel.h"
#include "SimpleKnob.h"

//==============================================================================
StateVariableFilterAudioProcessorEditor::StateVariableFilterAudioProcessorEditor (StateVariableFilterAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    setSize (500, 300);
    reduction = 5;
    
    
    String cutoffParamID;
    int cutoffIndex;
    cutoffParamID = "cutoff";
    cutoffIndex = 1;
    initSlider(cutoffParamID, cutoffIndex, &cutoffAttachment);
    
    String qParamID;
    int qIndex;
    qParamID = "q";
    qIndex = 2;
    initSlider(qParamID, qIndex, &qAttachment);
    
    // define state knob
    String typeParamID;
    int typeIndex;
    typeParamID = "type";
    typeIndex = 3;
    initSlider(typeParamID, typeIndex, &typeAttachment);

    String inGainParamID;
    int inGainIndex;
    inGainParamID = "inGain";
    inGainIndex = 0;
    initSlider(inGainParamID, inGainIndex, &inGainAttachment);

    String outGainParamID;
    int outGainIndex;
    outGainParamID = "outGain";
    outGainIndex = 4;
    initSlider(outGainParamID, outGainIndex, &outGainAttachment);
    calculateBounds();
}

StateVariableFilterAudioProcessorEditor::~StateVariableFilterAudioProcessorEditor()
{
}

//==============================================================================
void StateVariableFilterAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    
    g.fillAll (Colours::black);
    g.setColour (Colours::yellow);
    
    g.setFont (Font ("Monospaced", titleFontSize, Font::plain));
    g.drawRect(title);
    for (int i = 0; i < knobArraySize; i++)
    {
        knobArray[i].drawOutline(g);
    }
    
    g.setColour(Colours::white);
    g.setFont (Font ("Monospaced", titleFontSize, Font::plain));
    g.drawFittedText ("State Variable Filter", title, Justification::centred, 1);
    
}

void StateVariableFilterAudioProcessorEditor::sliderValueChanged(Slider *slider)
{
    if (slider == &knobArray[knobMap["cutoff"]])
    {
        setSliderText("cutoff");
    }
    if (slider == &knobArray[knobMap["q"]])
    {
        setSliderText("q");
    }
    if (slider == &knobArray[knobMap["type"]])
    {
        setSliderText("type");
    }
    if (slider == &knobArray[knobMap["inGain"]])
    {
        setSliderText("inGain");
    }
    if (slider == &knobArray[knobMap["outGain"]])
    {
        setSliderText("outGain");
    }
}

void StateVariableFilterAudioProcessorEditor::resized()
{
    this->calculateBounds();
}

void StateVariableFilterAudioProcessorEditor::setSliderText(String paramID)
{
    if (paramID.equalsIgnoreCase("type"))
    {
        changeTypeText(knobArray[knobMap[paramID]].getValue());
    }
    else
    {
        knobArray[knobMap[paramID]].value.setText((String) knobArray[knobMap[paramID]].getValue(), dontSendNotification);
    }
}

void StateVariableFilterAudioProcessorEditor::calculateBounds()
{
    // rectangle containing local bounds
    Rectangle<int> r = getLocalBounds();
    
    titleHeight = r.getHeight() / 4;

    title = r.removeFromTop(titleHeight);
    title.reduce(reduction, reduction);
    titleFontSize = title.getHeight() * 0.5;
    
    int knobWidth = r.getWidth() / knobArraySize;
    
    for (int i = 0; i < knobArraySize; i++)
    {
        Rectangle<int> thisRect;
        if (r.getWidth() > knobWidth)
        {
            thisRect = r.removeFromLeft(knobWidth);
        }
        else
        {
            thisRect = r;
        }
        thisRect.reduce(reduction, reduction);
        knobArray[i].setComponentBounds(thisRect);
    }
}


void StateVariableFilterAudioProcessorEditor::initSlider(String paramID, int index, std::unique_ptr<SliderAttachment> * attachment)
{
    addAndMakeVisible(knobArray[index]);
    addAndMakeVisible(knobArray[index].value);
    knobMap.set(paramID, index);
    *attachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, paramID, knobArray[index]);
    knobArray[index].init(paramID);
    knobArray[index].addListener(this);
    setSliderText(paramID);

    /*
    knobArray[index].init(minVal, maxVal, snap, skew, initValue, name);
    //knobArray[index].addListener(this);
    addAndMakeVisible(knobArray[index]);
    addAndMakeVisible(knobArray[index].value);
    knobMap.set(name, index);
     */
}

void StateVariableFilterAudioProcessorEditor::changeTypeText(int typeValue)
{
    switch(typeValue)
    {
        case 0:
            knobArray[knobMap["type"]].value.setText("low", dontSendNotification);
            break;
        case 1:
            knobArray[knobMap["type"]].value.setText("band", dontSendNotification);
            break;
        case 2:
            knobArray[knobMap["type"]].value.setText("notch", dontSendNotification);
            break;
        case 3:
            knobArray[knobMap["type"]].value.setText("high", dontSendNotification);
            break;
        default:
            knobArray[knobMap["type"]].value.setText("low", dontSendNotification);
    }
}
