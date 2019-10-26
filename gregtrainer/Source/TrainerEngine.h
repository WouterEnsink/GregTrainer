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

class TrainerEngine final   : public TreeListener, public AudioSource, public AsyncUpdater
{
public:
    
    TrainerEngine (ValueTree&);
    
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
    
    Melody currentMelody;
    
    
    
    int currentNumNotesInMelody, noteLength, timeBetweenNotes;
    
    ValueTree engineState;
    
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
