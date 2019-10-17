/*
  ==============================================================================

    TrainerAudioSource.h
    Created: 16 Oct 2019 10:39:49pm
    Author:  Wouter Ensink

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiGenerator.h"
#include "Synth.h"

class TrainerAudioSource : public AudioSource
{
public:
    
    TrainerAudioSource()
    {
        synth.addVoice(new SineWaveVoice());
        synth.addSound(new SineWaveSound());
    }
    
    void startPlaying(int timeInterval, const Array<int>& notes) noexcept
    {
        midiGenerator.setNotes(notes);
        midiGenerator.setTimeBetweenNotes(timeInterval);
        midiGenerator.setNoteLength(200);
        midiGenerator.startPlaying();
    }
    
    void stopPlaying() noexcept
    {
        midiGenerator.stopPlaying();
    }
    
    void prepareToPlay(int /*numSamplesExpected*/, double newSampleRate) override
    {
        sampleRate = newSampleRate;
        synth.setCurrentPlaybackSampleRate(sampleRate);
        midiGenerator.setSampleRate(sampleRate);
    }
    
    void getNextAudioBlock(const AudioSourceChannelInfo& channelInfo) override
    {
        auto& audioBuffer = *channelInfo.buffer;
        auto numSamples = channelInfo.numSamples;
        auto midiBuffer = MidiBuffer();
        
        midiGenerator.renderNextMidiBlock(midiBuffer, numSamples);
        
        synth.renderNextBlock(audioBuffer, midiBuffer, 0, numSamples);
    }
    
    void releaseResources() override { }
    
private:
    
    double sampleRate;
    MidiGenerator midiGenerator;
    Synthesiser synth;
};
