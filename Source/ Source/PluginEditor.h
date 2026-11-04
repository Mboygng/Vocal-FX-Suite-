
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

    // Header
    juce::Label title;
    juce::Label subtitle;

    // Main controls
    juce::Slider retuneSlider;
    juce::Slider reverbSlider;
    juce::Slider delaySlider;

    // Control labels
    juce::Label retuneLabel;
    juce::Label reverbLabel;
    juce::Label delayLabel;

    // Buttons
    juce::TextButton autotuneButton { "AUTOTUNE" };
    juce::TextButton analyzeButton { "ANALIZAR MI VOZ" };

    // Parameter connections
    using SliderAttachment =
        juce::AudioProcessorValueTreeState::SliderAttachment;

    using ButtonAttachment =
        juce::AudioProcessorValueTreeState::ButtonAttachment;

    std::unique_ptr<SliderAttachment> retuneAttachment;
    std::unique_ptr<SliderAttachment> reverbAttachment;
    std::unique_ptr<SliderAttachment> delayAttachment;

    std::unique_ptr<ButtonAttachment> autotuneAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(
        VocalFXAudioProcessorEditor)
};
