#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class VocalFXAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit VocalFXAudioProcessorEditor(
        VocalFXAudioProcessor& processor);

    ~VocalFXAudioProcessorEditor() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    VocalFXAudioProcessor& audioProcessor;

    juce::Label title;
    juce::Label subtitle;

    juce::Slider retuneSlider;
    juce::Slider reverbSlider;
    juce::Slider delaySlider;

    juce::Label retuneLabel;
    juce::Label reverbLabel;
    juce::Label delayLabel;

    juce::TextButton autotuneButton {"AUTOTUNE"};
    juce::TextButton analyzeButton {"ANALIZAR MI VOZ"};

    using SliderAttachment =
        juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<SliderAttachment> retuneAttachment;
    std::unique_ptr<SliderAttachment> reverbAttachment;
    std::unique_ptr<SliderAttachment> delayAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(
        VocalFXAudioProcessorEditor)
};
