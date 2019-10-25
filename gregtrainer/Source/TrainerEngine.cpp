/*
  ==============================================================================

    TrainerEngine.cpp
    Created: 25 Oct 2019 3:32:16pm
    Author:  Wouter Ensink

  ==============================================================================
*/

#include "TrainerEngine.h"
#include "Synth.h"

TrainerEngine::TrainerEngine(ValueTree& tree) : engineState(tree)
{
    currentNumNotesInMelody = 8;
    noteLength = 300;
    timeBetweenNotes = 400;
    
    playbackInstrument.reset(new SineWaveSynthesizer());
}

TrainerEngine::~TrainerEngine()
{
    
}

//===================================================================


void TrainerEngine::prepareToPlay(int numSamplesPerBlockExpected, double sampleRate)
{
    if(playbackInstrument != nullptr)
        playbackInstrument->prepareToPlay(sampleRate, numSamplesPerBlockExpected);
    
    midiGenerator.setSampleRate(sampleRate);
    currentSampleRate = sampleRate;
}


void TrainerEngine::getNextAudioBlock(const AudioSourceChannelInfo& channelInfo)
{
    auto midi = MidiBuffer();
    auto numSamples = channelInfo.buffer->getNumSamples();

    midiGenerator.renderNextMidiBlock(midi, numSamples);
    
  
    if(playbackInstrument != nullptr)
        playbackInstrument->processBlock(*channelInfo.buffer, midi);
    
}


void TrainerEngine::releaseResources()
{
    if(playbackInstrument != nullptr)
        playbackInstrument->releaseResources();
}


//===================================================================


void TrainerEngine::setMelodyLength(int numNotes)
{
    melodyGenerator.setNumNotesInMelody(numNotes);
}

void TrainerEngine::setNoteIntervalTime(int intervalTimeMs)
{
    midiGenerator.setTimeBetweenNotes(intervalTimeMs);
}

void TrainerEngine::setNoteLength(int timeInMs)
{
    midiGenerator.setNoteLength(timeInMs);
}

void TrainerEngine::generateNextMelody()
{
    currentMelody = melodyGenerator.generateMelody();
    
    midiGenerator.setNotes(currentMelody.midiNotes);
}

void TrainerEngine::startPlayingMelody()
{
    setNoteIntervalTime(timeBetweenNotes);
    setNoteLength(noteLength);
    setMelodyLength(currentNumNotesInMelody);
    
    midiGenerator.startPlaying();
}

void TrainerEngine::stopPlayingMelody()
{
    midiGenerator.stopPlaying();
}

void TrainerEngine::checkIfMelodyIsSameAsPlayed(Melody&)
{
    
}


//===================================================================

void TrainerEngine::valueTreePropertyChanged(ValueTree&, const Identifier&)
{
    
}

bool TrainerEngine::openInstrumentEditor()
{
    return playbackInstrument->hasEditor();
}
