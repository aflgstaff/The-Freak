/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "LabeledSlider.h"

//==============================================================================
struct Placeholder : juce::Component
{
    Placeholder();

    void paint(juce::Graphics& g) override
    {
        g.fillAll(customColor);
    }
    juce::Colour customColor;
};

struct GlobalControls : juce::Component
{
    void paint(juce::Graphics& g) override;

};
/**
*/
class Thefreak23AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    Thefreak23AudioProcessorEditor (Thefreak23AudioProcessor&);
    ~Thefreak23AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Thefreak23AudioProcessor& audioProcessor;

    LabeledSlider releaseSlider;
    LabeledSlider thresholdSlider;
    LabeledSlider makeupSlider;
    LabeledSlider attackSlider;

    FlexBox flexBox;
    FlexBox flexRowArray[4];


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Thefreak23AudioProcessorEditor)
};
