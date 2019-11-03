/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OpampAmpStageAudioProcessor::OpampAmpStageAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

OpampAmpStageAudioProcessor::~OpampAmpStageAudioProcessor()
{
}

//==============================================================================
const String OpampAmpStageAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OpampAmpStageAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OpampAmpStageAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OpampAmpStageAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double OpampAmpStageAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OpampAmpStageAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int OpampAmpStageAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OpampAmpStageAudioProcessor::setCurrentProgram (int index)
{
}

const String OpampAmpStageAudioProcessor::getProgramName (int index)
{
    return {};
}

void OpampAmpStageAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void OpampAmpStageAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    stateSpaceProcessor = std::make_unique<StateSpaceProcessor>("/abs/path/to/file.json", sampleRate);
}

void OpampAmpStageAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OpampAmpStageAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void OpampAmpStageAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        
        if (channel == 0) {
            for (int j = 0; j < buffer.getNumSamples(); ++j) {
                channelData[j] = stateSpaceProcessor->process(channelData[j]) - 1.9;
            }
        } else {
            memcpy(channelData, buffer.getReadPointer(0), buffer.getNumSamples() * sizeof(float));
        }
        
        
        
        
    }
}

//==============================================================================
bool OpampAmpStageAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* OpampAmpStageAudioProcessor::createEditor()
{
    return new OpampAmpStageAudioProcessorEditor (*this);
}

//==============================================================================
void OpampAmpStageAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void OpampAmpStageAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OpampAmpStageAudioProcessor();
}
