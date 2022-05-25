/*
  ==============================================================================

    SimpleKnob.h
    Created: 26 Jun 2020 1:32:51pm
    Author:  Admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SimpleLookAndFeel.h"

class SimpleKnob : public Slider
{
public:
    Label value;
    
    void init (float minVal, float maxVal, float snap, float skew, float initValue, String name);
    void init(String name);
    void drawOutline (Graphics& g);
    void setComponentBounds (Rectangle<int> bounds);
    void updateComponentBounds ();
    void setLabelValue (int type, String label);
    void changeStateText (int type);
    
private:
    SimpleLookAndFeel lookAndFeel;
    String name;
    HashMap<String, Rectangle<int>> boundMap;
    int labelType, paramTextHeight, nameFontSize;
};
