/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Thefreak23AudioProcessor::Thefreak23AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    attack = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("Attack"));
    jassert(attack != nullptr);
    threshold = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("Threshold"));
    jassert(threshold != nullptr);
    release = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("Release"));
    jassert(release != nullptr);
    ratio = dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter("Ratio"));
    jassert(ratio != nullptr);
    makeup = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("Makeup"));
    jassert(makeup != nullptr);
    freakymode = dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter("Freaky Mode"));
    jassert(freakymode != nullptr);
}

Thefreak23AudioProcessor::~Thefreak23AudioProcessor()
{
}

//==============================================================================
const juce::String Thefreak23AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Thefreak23AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Thefreak23AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Thefreak23AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Thefreak23AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Thefreak23AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Thefreak23AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Thefreak23AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Thefreak23AudioProcessor::getProgramName (int index)
{
    return {};
}

void Thefreak23AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Thefreak23AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    spec.sampleRate = sampleRate;

    compressor.prepare(spec);
    gain.prepare(spec);
    verb.prepare(spec);

}

void Thefreak23AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Thefreak23AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Thefreak23AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    compressor.setAttack(attack->get());
    compressor.setRelease(release->get());
    compressor.setThreshold(threshold->get());
    compressor.setRatio(ratio->getCurrentChoiceName().getFloatValue());
    verbParam.roomSize = 1;
    verbParam.wetLevel = 1;
    verbParam.width = 1;
    verb.setParameters(verbParam);
    gain.setGainDecibels(makeup->get());
    auto block = juce::dsp::AudioBlock<float>(buffer);
    auto context = juce::dsp::ProcessContextReplacing<float>(block);
    compressor.process(context);
    gain.process(context);
    if (freakymode->get() == true) {
        verb.process(context);
    }
   // verb.process(context);
}

//==============================================================================
bool Thefreak23AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Thefreak23AudioProcessor::createEditor()
{
    return new Thefreak23AudioProcessorEditor (*this);
}

//==============================================================================
void Thefreak23AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Thefreak23AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

juce::AudioProcessorValueTreeState::ParameterLayout Thefreak23AudioProcessor::createParameterLayout()
{
    APVTS::ParameterLayout layout;
    using namespace juce;

    layout.add(std::make_unique<AudioParameterFloat>("Threshold", "Threshold", NormalisableRange<float>(-60, 12, 1, 1), 0));
    auto attackReleaseRange = NormalisableRange<float>(5, 500, 1, 1);
    auto makeupRange = NormalisableRange<float>(0, 10, 1, 1);
    layout.add(std::make_unique<AudioParameterFloat>("Attack", "Attack", attackReleaseRange, 50));
    layout.add(std::make_unique<AudioParameterFloat>("Release", "Release", attackReleaseRange, 250));
    layout.add(std::make_unique<AudioParameterFloat>("Makeup", "Makeup", makeupRange, 0));
    layout.add(std::make_unique<AudioParameterBool>("Freaky Mode", "Freaky Mode", false));

    auto choices = std::vector<double>{};
    for (int i = 1;i <= 100;++i) {
        choices.insert(choices.end(), static_cast<double>(i));
    }
    juce::StringArray sa;
    for (auto choice : choices)
    {
        sa.add(juce::String(choice, 1));
    }
    layout.add(std::make_unique<AudioParameterChoice>("Ratio", "Ratio", sa, 51));
    return layout;
}
//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Thefreak23AudioProcessor();
}
