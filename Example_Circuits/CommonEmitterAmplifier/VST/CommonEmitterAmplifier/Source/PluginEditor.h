/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class CommonEmitterAmplifierAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    CommonEmitterAmplifierAudioProcessorEditor (CommonEmitterAmplifierAudioProcessor&);
    ~CommonEmitterAmplifierAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    CommonEmitterAmplifierAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CommonEmitterAmplifierAudioProcessorEditor)
};
