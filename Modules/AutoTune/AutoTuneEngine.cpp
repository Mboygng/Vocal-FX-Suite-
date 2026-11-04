
#include "AutoTuneEngine.h"
#include <cmath>
#include <algorithm>

AutoTuneEngine::AutoTuneEngine()
{
}

void AutoTuneEngine::prepare(
    double sampleRate,
    int maximumBlockSize)
{
    juce::ignoreUnused(maximumBlockSize);

    currentSampleRate = sampleRate;

    history.clear();
    history.reserve(4096);

    writePosition = 0;

    detectedFrequency = 0.0f;
    detectedMidiNote = -1;
    targetMidiNote = -1;
    pitchDetected = false;
}

void AutoTuneEngine::reset()
{
    history.clear();

    writePosition = 0;

    detectedFrequency = 0.0f;
    detectedMidiNote = -1;
    targetMidiNote = -1;
    pitchDetected = false;
}

void AutoTuneEngine::process(
    const float* input,
    int numSamples,
    double sampleRate)
{
    if (input == nullptr || numSamples <= 0)
        return;

    currentSampleRate = sampleRate;

    for (int i = 0; i < numSamples; ++i)
    {
        if (history.size() < 4096)
        {
            history.push_back(input[i]);
        }
        else
        {
            history[writePosition] = input[i];

            writePosition = (writePosition + 1) % 4096;
        }
    }

    if (history.size() >= 2048)
        analyzePitch();
}

void AutoTuneEngine::analyzePitch()
{
    const int size = static_cast<int>(history.size());

    if (size < 2048 || currentSampleRate <= 0.0)
        return;

    const int minimumLag =
        static_cast<int>(currentSampleRate / 1000.0);

    const int maximumLag =
        juce::jmin(
            size / 2,
            static_cast<int>(currentSampleRate / 65.0));

    if (minimumLag < 1 || maximumLag <= minimumLag)
        return;

    double mean = 0.0;

    for (int i = 0; i < size; ++i)
    {
        const int index = size < 4096
            ? i
            : (writePosition + i) % size;

        mean += history[index];
    }

    mean /= size;

    double signalEnergy = 0.0;

    for (int i = 0; i < size; ++i)
    {
        const int index = size < 4096
            ? i
            : (writePosition + i) % size;

        const double sample = history[index] - mean;

        signalEnergy += sample * sample;
    }

    const double rms = std::sqrt(signalEnergy / size);

    if (rms < 0.003)
    {
        pitchDetected = false;
        detectedFrequency = 0.0f;
        detectedMidiNote = -1;
        targetMidiNote = -1;
        return;
    }

    double bestCorrelation = -1.0;
    int bestLag = -1;

    for (int lag = minimumLag;
         lag <= maximumLag;
         ++lag)
    {
        double correlation = 0.0;
        double energyA = 0.0;
        double energyB = 0.0;

        for (int i = 0; i < size - lag; i += 2)
        {
            const int indexA = size < 4096
                ? i
                : (writePosition + i) % size;

            const int indexB = size < 4096
                ? i + lag
                : (writePosition + i + lag) % size;

            const double a = history[indexA] - mean;
            const double b = history[indexB] - mean;

            correlation += a * b;
            energyA += a * a;
            energyB += b * b;
        }

        const double denominator =
            std::sqrt(energyA * energyB);

        if (denominator <= 0.0)
            continue;

        const double normalizedCorrelation =
            correlation / denominator;

        if (normalizedCorrelation > bestCorrelation)
        {
            bestCorrelation = normalizedCorrelation;
            bestLag = lag;
        }
    }

    if (bestLag < 1 || bestCorrelation < 0.70)
    {
        pitchDetected = false;
        return;
    }

    detectedFrequency =
        static_cast<float>(currentSampleRate / bestLag);

    detectedMidiNote =
        juce::roundToInt(
            69.0 + 12.0 *
            std::log2(detectedFrequency / 440.0f));

    pitchDetected = true;

    // Find the nearest note allowed by the selected scale.
    double bestDistance = 1000.0;

    targetMidiNote = detectedMidiNote;

    for (int note = detectedMidiNote - 12;
         note <= detectedMidiNote + 12;
         ++note)
    {
        const int pitchClass = (note % 12 + 12) % 12;

        const bool allowed =
            (scaleMask & (1u << pitchClass)) != 0;

        if (!allowed)
            continue;

        const double distance =
            std::abs(note - detectedMidiNote);

        if (distance < bestDistance)
        {
            bestDistance = distance;
            targetMidiNote = note;
        }
    }
}

float AutoTuneEngine::getDetectedFrequency() const
{
    return detectedFrequency;
}

int AutoTuneEngine::getDetectedMidiNote() const
{
    return detectedMidiNote;
}

int AutoTuneEngine::getTargetMidiNote() const
{
    return targetMidiNote;
}

void AutoTuneEngine::setScaleMask(uint16_t mask)
{
    scaleMask = mask & 0x0FFF;

    if (scaleMask == 0)
        scaleMask = 0x0FFF;
}

bool AutoTuneEngine::isPitchDetected() const
{
    return pitchDetected;
}
