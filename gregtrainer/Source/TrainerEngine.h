/*
  ==============================================================================

    TrainerEngine.h
    Created: 25 Oct 2019 3:32:16pm
    Author:  Wouter Ensink

  ==============================================================================
*/

#pragma once

#include "Utility.h"
#include "MelodyGenerator.h"
#include "MidiGenerator.h"

//=======================================================================
// This is the main engine for the trainer
// it provides everything we need that is not the interface

class TrainerEngine final   : public TreeListener,
                              public AudioSource,
                              private AsyncUpdater
{
public:
    
    enum class PlayState
    {
        playing,
        checkingAnswer,
        stopped
    };
    
    TrainerEngine (ValueTree&, int numNotes);
    
    ~TrainerEngine();
    
    //===================================================================
    
    void prepareToPlay (int, double) override;
    
    void getNextAudioBlock (const AudioSourceChannelInfo&) override;
    
    void releaseResources() override;
    
    //===================================================================
    
    void setNumNotesInMelody (int);
    
    void setTimeBetweenNotesInMs (int);
    
    void setNoteLengthInMs (int);
    
    void generateNextMelody();
    
    void startPlayingMelody();
    
    void stopPlayingMelody();
    
    void checkIfMelodyIsSameAsPlayed (Melody&);
    
    //===================================================================
    
    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    
    //===================================================================
    
    bool openInstrumentEditor();
    
    //===================================================================
    
    void handleAsyncUpdate() override;
    
private:
    
    ValueTree engineState;
    
    CachedValue<PlayState> playState;
    
    std::unique_ptr<AudioProcessor> playbackInstrument;
    
    double currentSampleRate;
    
    MelodyGenerator melodyGenerator;
    MidiGenerator midiGenerator;
    CachedValue<bool> isPlaying;
    //===================================================================
    // maybe the engine should use a reference counted melody object
    // since this is compatible with the value tree
    // and we don't need to safe or restore any melodies
    // this way everything is notified automatically when the new melody is generated..
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrainerEngine)
};




template <>
class VariantConverter<TrainerEngine::PlayState> final
{
public:
    using State = TrainerEngine::PlayState;
    
    static var toVar (const State& state)
    {
        if (state == State::playing)        return "p";
        if (state == State::checkingAnswer) return "c";
        if (state == State::stopped)        return "s";
        
        return "undefined";
    }
    
    static State fromVar (const var& state)
    {
        if (state == "p") return State::playing;
        if (state == "c") return State::checkingAnswer;
        if (state == "s") return State::stopped;
        
        return State::stopped;
    }
};
