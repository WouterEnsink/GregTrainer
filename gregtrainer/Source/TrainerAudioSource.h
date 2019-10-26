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

//===============================================================================================
// Main audio source to use in the trainer.
// In the future this should become part of a generalized engine,
// toghether with the MelodyGenerator to have all sound related stuff linked together
// without any Component being the bridge..


// DEPRECATED

class TrainerAudioSource : public AudioSource
{
public:
    
    TrainerAudioSource()
    {
        synth.addVoice(new SineWaveVoice());
        synth.addSound(new SineWaveSound());
    }
    
    void startPlaying(int noteLength, int timeInterval, const Array<int>& notes) noexcept
    {
        midiGenerator.setNotes(notes);
        midiGenerator.setTimeBetweenNotesInMs(timeInterval);
        midiGenerator.setNoteLengthInMs(noteLength);
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
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrainerAudioSource)
    
};
