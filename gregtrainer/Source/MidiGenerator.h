/*
  ==============================================================================

    MidiGenerator.h
    Created: 15 Oct 2019 9:32:25pm
    Author:  Wouter Ensink

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Utility.h"

//you give it an array of note numbers to play
//and it will generate the midi for that

class MidiGenerator
{
public:
    
    MidiGenerator() { }
    ~MidiGenerator() { }
    
    void setSpeed() { }
    
    void setSampleRate(double newSampleRate) noexcept
    {
        sampleRate = newSampleRate;
    }
    
    void setNotes(const Array<int>& notes) noexcept
    {
        notesToPlay = notes;
    }
    
    void setTimeBetweenNotes(int timeInMs) noexcept
    {
        timeBetweenNotes = timeInMs;
        numSamplesBetweenNotes = timeBetweenNotes * sampleRate * 0.001;
    }
    
    //make sure sampleRate is set before this function is called
    //also call this function after sampleRate is reset, to assert the right note length
    void setNoteLength(int timeInMs) noexcept
    {
        noteLenghtInSamples = timeInMs * sampleRate * 0.001;
    }
    
    void startPlaying() noexcept
    {
        notesIndexNoteOn = 0;
        notesIndexNoteOff = 0;
        remainderNoteOn = numSamplesBetweenNotes;
        remainderNoteOff = remainderNoteOn + noteLenghtInSamples;
    }
    
    void stopPlaying() noexcept
    {
        
    }
    
    int getNextMidiNoteOn() noexcept
    {
        return notesIndexNoteOn < notesToPlay.size() ? notesToPlay[notesIndexNoteOn++] : 0;
    }
    
    int getNextMidiNoteOff() noexcept
    {
        return notesIndexNoteOff < notesToPlay.size() ? notesToPlay[notesIndexNoteOff++] : 0;
    }
    
    
    //fills the midibuffer with messages if needed
    void renderNextMidiBlock(MidiBuffer& buffer, int numSamples) noexcept
    {
        
        //need to put a note on message in the buffer
        if(remainderNoteOn < numSamples)
        {
            if (auto note = getNextMidiNoteOn())
            {
                buffer.addEvent(MidiMessage::noteOn(1, note, .9f), remainderNoteOn);
                remainderNoteOn = numSamplesBetweenNotes - (numSamples - remainderNoteOn);
            }
        }
        else remainderNoteOn -= numSamples;
        
        //need to put a note off message in the buffer
        if (remainderNoteOff < numSamples)
        {
            if (auto note = getNextMidiNoteOff())
            {
                buffer.addEvent(MidiMessage::noteOff(1, note, 0.f), remainderNoteOff);
                remainderNoteOff = numSamplesBetweenNotes - (numSamples - remainderNoteOff);
            }
        }
        else remainderNoteOff -= numSamples;
    }
    
    
private:
    
    int numSamplesBetweenNotes;
    int remainderNoteOn; //how many samples to go until a note needs to be put into a buffer
    int remainderNoteOff;
    Array<int> notesToPlay;
    int notesIndexNoteOn, notesIndexNoteOff;
    double sampleRate;
    int timeBetweenNotes; //in ms
    int noteLenghtInSamples;
};

