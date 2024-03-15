/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

Placeholder::Placeholder()
{
    juce::Random r;
    customColor = juce::Colour(r.nextInt(255), r.nextInt(255), r.nextInt(255));

}

void GlobalControls::paint(juce::Graphics& g)
{
    using namespace juce;
    auto bounds = getLocalBounds();
    g.fillAll(Colour::fromRGB(255, 255, 255));
    g.setOpacity(1);
}

//==============================================================================
Thefreak23AudioProcessorEditor::Thefreak23AudioProcessorEditor (Thefreak23AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    addAndMakeVisible(thresholdSlider);
    addAndMakeVisible(releaseSlider);
    addAndMakeVisible(attackSlider);
    addAndMakeVisible(makeupSlider);

    

    setSize (585, 666);
}

Thefreak23AudioProcessorEditor::~Thefreak23AudioProcessorEditor()
{
}

//==============================================================================
void Thefreak23AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    juce::MemoryBlock imageData(BinaryData::thefreakplugin_png, BinaryData::thefreakplugin_pngSize);
    juce::Image backgroundImage = juce::ImageFileFormat::loadFrom(imageData.getData(), imageData.getSize());
    juce::Rectangle<int> bounds = getLocalBounds();
    g.drawImage(backgroundImage, bounds.toFloat());
 
}

void Thefreak23AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto bounds = getLocalBounds();

    thresholdSlider.setBounds(bounds.removeFromRight(150).removeFromBottom(150).removeFromLeft(140).removeFromTop(140) );
    thresholdSlider.setTransform(AffineTransform::translation(-180, -200));
    thresholdSlider.reset(audioProcessor.apvts,"Threshold");
    attackSlider.setBounds(bounds.removeFromRight(150).removeFromBottom(150).removeFromLeft(140).removeFromTop(140));
    attackSlider.setTransform(AffineTransform::translation(-30, -30));
    attackSlider.reset(audioProcessor.apvts, "Attack");
    releaseSlider.setBounds(bounds.removeFromRight(150).removeFromBottom(150).removeFromLeft(140).removeFromTop(140));
    releaseSlider.setTransform(AffineTransform::translation(280, -30));
    releaseSlider.reset(audioProcessor.apvts, "Release");
    makeupSlider.setBounds(bounds.removeFromRight(150).removeFromBottom(150).removeFromLeft(140).removeFromTop(140));
    makeupSlider.setTransform(AffineTransform::translation(410, -200));
    makeupSlider.reset(audioProcessor.apvts, "Makeup");



}
