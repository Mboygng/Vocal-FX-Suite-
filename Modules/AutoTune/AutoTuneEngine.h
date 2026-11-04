
#pragma once

#include <JuceHeader.h>
#include <vector>
#include <cstdint>

class AutoTuneEngine
{
public:

    AutoTuneEngine();

    void prepare(double sampleRate, int maximumBlockSize);

    void reset();

    void process(
        const float* input,
        int numSamples,
        double sampleRate);

    float getDetectedFrequency() const;

    int getDetectedMidiNote() const;

    int getTargetMidiNote() const;

    void setScaleMask(uint16_t mask);

    bool isPitchDetected() const;

private:

    void analyzePitch();

    std::vector<float> history;

    int writePosition = 0;

    double currentSampleRate = 44100.0;

    float detectedFrequency = 0.0f;

    int detectedMidiNote = -1;

    int targetMidiNote = -1;

    uint16_t scaleMask = 0x0FFF;

    bool pitchDetected = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(
        AutoTuneEngine)
};
