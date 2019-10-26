/*
  ==============================================================================

    TrainerEngine.cpp
    Created: 25 Oct 2019 3:32:16pm
    Author:  Wouter Ensink

  ==============================================================================
*/

#include "TrainerEngine.h"
#include "Synth.h"
#include "Identifiers.h"

TrainerEngine::TrainerEngine (ValueTree& tree) : engineState {IDs::Engine::EngineRoot}
{
    
    engineState.addListener (this);
    tree.appendChild (engineState, nullptr);
    isPlaying.referTo (engineState, IDs::Engine::PlayState, nullptr);
    
    setTimeBetweenNotesInMs (500);
    setNoteLengthInMs (400);
    setNumNotesInMelody (8);
    
    playbackInstrument.reset (new SineWaveSynthesizer());
}

TrainerEngine::~TrainerEngine()
{
    
}

//==================================================================================


void TrainerEngine::prepareToPlay (int numSamplesPerBlockExpected, double sampleRate)
{
    if (playbackInstrument != nullptr)
        playbackInstrument->prepareToPlay (sampleRate, numSamplesPerBlockExpected);
    
    midiGenerator.setSampleRate (sampleRate);
    currentSampleRate = sampleRate;
}


void TrainerEngine::getNextAudioBlock (const AudioSourceChannelInfo& channelInfo)
{
    auto midi = MidiBuffer();
    auto numSamples = channelInfo.buffer->getNumSamples();

    midiGenerator.renderNextMidiBlock (midi, numSamples);
    
  
    if (playbackInstrument != nullptr)
        playbackInstrument->processBlock (*channelInfo.buffer, midi);
    
}


void TrainerEngine::releaseResources()
{
    if (playbackInstrument != nullptr)
        playbackInstrument->releaseResources();
}


//==================================================================================


void TrainerEngine::setNumNotesInMelody (int numNotes)
{
    melodyGenerator.setNumNotesInMelody (numNotes);
}

void TrainerEngine::setTimeBetweenNotesInMs (int intervalTimeMs)
{
    midiGenerator.setTimeBetweenNotesInMs (intervalTimeMs);
}

void TrainerEngine::setNoteLengthInMs (int timeInMs)
{
    midiGenerator.setNoteLengthInMs (timeInMs);
}

void TrainerEngine::generateNextMelody()
{
    currentMelody = melodyGenerator.generateMelody();
    
    midiGenerator.setNotes (currentMelody.midiNotes);
}

void TrainerEngine::startPlayingMelody()
{
    midiGenerator.startPlaying();
}

void TrainerEngine::stopPlayingMelody()
{
    midiGenerator.stopPlaying();
}

void TrainerEngine::checkIfMelodyIsSameAsPlayed (Melody&)
{
    
}


//==================================================================================

void TrainerEngine::valueTreePropertyChanged (ValueTree& t, const Identifier& id)
{
    print ("Engine: Tree:", t.getType().toString(), "ID:", id.toString(), "value:", t[id].toString());
    triggerAsyncUpdate();
}

//==================================================================================

bool TrainerEngine::openInstrumentEditor()
{
    return playbackInstrument->hasEditor();
}

void TrainerEngine::handleAsyncUpdate()
{
    print ("cached value:", isPlaying.get());
}
