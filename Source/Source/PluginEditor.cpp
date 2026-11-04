
#include "PluginEditor.h"

VocalFXAudioProcessorEditor::VocalFXAudioProcessorEditor(
    VocalFXAudioProcessor& processor)
    : AudioProcessorEditor(&processor),
      audioProcessor(processor)
{
    setSize(760, 520);

    title.setText("VOCAL FX SUITE", juce::dontSendNotification);
    title.setFont(juce::Font(30.0f, juce::Font::bold));
    title.setColour(juce::Label::textColourId,
                    juce::Colour(0xffff3030));
    title.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(title);

    subtitle.setText("PROFESSIONAL VOCAL PROCESSING",
                     juce::dontSendNotification);
    subtitle.setColour(juce::Label::textColourId,
                       juce::Colours::grey);
    subtitle.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(subtitle);

    auto setupSlider = [this](juce::Slider& slider,
                              juce::Label& label,
                              const juce::String& text)
    {
        slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle(
            juce::Slider::TextBoxBelow, false, 65, 22);
        slider.setColour(
            juce::Slider::rotarySliderFillColourId,
            juce::Colour(0xffff3030));
        slider.setColour(
            juce::Slider::thumbColourId,
            juce::Colours::white);
        slider.setColour(
            juce::Slider::textBoxTextColourId,
            juce::Colours::white);
        slider.setColour(
            juce::Slider::textBoxBackgroundColourId,
            juce::Colour(0xff202020));

        label.setText(text, juce::dontSendNotification);
        label.setColour(juce::Label::textColourId,
                        juce::Colours::white);
        label.setJustificationType(juce::Justification::centred);

        addAndMakeVisible(slider);
        addAndMakeVisible(label);
    };

    setupSlider(retuneSlider, retuneLabel, "RETUNE SPEED");
    setupSlider(reverbSlider, reverbLabel, "REVERB");
    setupSlider(delaySlider, delayLabel, "DELAY");

    retuneAttachment =
        std::make_unique<SliderAttachment>(
            audioProcessor.parameters, "RETUNE", retuneSlider);

    reverbAttachment =
        std::make_unique<SliderAttachment>(
            audioProcessor.parameters, "REVERB", reverbSlider);

    delayAttachment =
        std::make_unique<SliderAttachment>(
            audioProcessor.parameters, "DELAY", delaySlider);

    autotuneButton.setClickingTogglesState(true);
    autotuneButton.setColour(
        juce::TextButton::buttonColourId,
        juce::Colour(0xffa51d1d));
    autotuneButton.setColour(
        juce::TextButton::textColourOffId,
        juce::Colours::white);
    addAndMakeVisible(autotuneButton);

    analyzeButton.setColour(
        juce::TextButton::buttonColourId,
        juce::Colour(0xff303030));
    analyzeButton.setColour(
        juce::TextButton::textColourOffId,
        juce::Colour(0xffff4545));

    analyzeButton.onClick = [this]()
    {
        juce::AlertWindow::showMessageBoxAsync(
            juce::MessageBoxIconType::InfoIcon,
            "Vocal FX Suite",
            "El analizador inteligente de voz se implementara en una proxima etapa.");
    };

    addAndMakeVisible(analyzeButton);
}

void VocalFXAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff101010));

    g.setColour(juce::Colour(0xff242424));
    g.fillRoundedRectangle(
        20.0f, 105.0f, 720.0f, 370.0f, 14.0f);

    g.setColour(juce::Colour(0xffff3030));
    g.fillRect(20.0f, 105.0f, 720.0f, 3.0f);

    g.setColour(juce::Colours::darkgrey);
    g.drawRoundedRectangle(
        20.0f, 105.0f, 720.0f, 370.0f, 14.0f, 1.0f);
}

void VocalFXAudioProcessorEditor::resized()
{
    title.setBounds(20, 12, 720, 45);
    subtitle.setBounds(20, 55, 720, 25);

    autotuneButton.setBounds(45, 125, 200, 42);
    analyzeButton.setBounds(505, 125, 200, 42);

    retuneLabel.setBounds(55, 215, 180, 25);
    retuneSlider.setBounds(55, 240, 180, 160);

    reverbLabel.setBounds(290, 215, 180, 25);
    reverbSlider.setBounds(290, 240, 180, 160);

    delayLabel.setBounds(525, 215, 180, 25);
    delaySlider.setBounds(525, 240, 180, 160);
}
