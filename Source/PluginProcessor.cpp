
#include "PluginProcessor.h"
#include "PluginEditor.h"

// CONSTRUCTOR
VocalFXAudioProcessor::VocalFXAudioProcessor()
    : AudioProcessor(
        BusesProperties()
            .withInput(
                "Input",
                juce::AudioChannelSet::stereo(),
                true)
            .withOutput(
                "Output",
                juce::AudioChannelSet::stereo(),
                true)
    ),
    parameters(
        *this,
        nullptr,
        "PARAMETERS",
        createParameterLayout())
{
}

VocalFXAudioProcessor::~VocalFXAudioProcessor() = default;

// PARAMETERS
juce::AudioProcessorValueTreeState::ParameterLayout
VocalFXAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(
        std::make_unique<juce::AudioParameterBool>(
            "AUTOTUNE",
            "AutoTune",
            false));

    layout.add(
        std::make_unique<juce::AudioParameterFloat>(
            "RETUNE",
            "Retune Speed",
            juce::NormalisableRange<float>(
                0.0f, 100.0f, 0.1f),
            50.0f));

    layout.add(
        std::make_unique<juce::AudioParameterFloat>(
            "REVERB",
            "Reverb",
            juce::NormalisableRange<float>(
                0.0f, 100.0f, 0.1f),
            0.0f));

    layout.add(
        std::make_unique<juce::AudioParameterFloat>(
            "DELAY",
            "Delay",
            juce::NormalisableRange<float>(
                0.0f, 100.0f, 0.1f),
            0.0f));

    return layout;
}

// AUDIO ENGINE
void VocalFXAudioProcessor::prepareToPlay(
    double sampleRate,
    int samplesPerBlock)
{
    juce::ignoreUnused(sampleRate, samplesPerBlock);
}

void VocalFXAudioProcessor::releaseResources()
{
}

bool VocalFXAudioProcessor::isBusesLayoutSupported(
    const BusesLayout& layouts) const
{
    const auto input = layouts.getMainInputChannelSet();
    const auto output = layouts.getMainOutputChannelSet();

    return input == output
        && (output == juce::AudioChannelSet::mono()
            || output == juce::AudioChannelSet::stereo());
}

void VocalFXAudioProcessor::processBlock(
    juce::AudioBuffer<float>& buffer,
    juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    juce::ignoreUnused(midiMessages);

    const bool autotuneEnabled =
        parameters.getRawParameterValue("AUTOTUNE")->load() > 0.5f;

    const float retune =
        parameters.getRawParameterValue("RETUNE")->load();

    const float reverb =
        parameters.getRawParameterValue("REVERB")->load();

    const float delay =
        parameters.getRawParameterValue("DELAY")->load();

    juce::ignoreUnused(
        autotuneEnabled,
        retune,
        reverb,
        delay);

    // Clean audio passthrough.
    // Real vocal processing modules will be added later.
}

// PLUGIN INFORMATION
const juce::String VocalFXAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool VocalFXAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor*
VocalFXAudioProcessor::createEditor()
{
    return new VocalFXAudioProcessorEditor(*this);
}

bool VocalFXAudioProcessor::acceptsMidi() const
{
    return false;
}

bool VocalFXAudioProcessor::producesMidi() const
{
    return false;
}

bool VocalFXAudioProcessor::isMidiEffect() const
{
    return false;
}

double VocalFXAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

// PROGRAMS
int VocalFXAudioProcessor::getNumPrograms()
{
    return 1;
}

int VocalFXAudioProcessor::getCurrentProgram()
{
    return 0;
}

void VocalFXAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String
VocalFXAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void VocalFXAudioProcessor::changeProgramName(
    int index,
    const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

// SAVE SETTINGS
void VocalFXAudioProcessor::getStateInformation(
    juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();

    std::unique_ptr<juce::XmlElement> xml(
        state.createXml());

    if (xml != nullptr)
        copyXmlToBinary(*xml, destData);
}

void VocalFXAudioProcessor::setStateInformation(
    const void* data,
    int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(
        getXmlFromBinary(data, sizeInBytes));

    if (xml != nullptr
        && xml->hasTagName(parameters.state.getType()))
    {
        parameters.replaceState(
            juce::ValueTree::fromXml(*xml));
    }
}

// PLUGIN ENTRY POINT
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VocalFXAudioProcessor();
}
