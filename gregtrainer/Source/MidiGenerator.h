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
        print("num samples between notes: ", numSamplesBetweenNotes);
    }
    
    void startPlaying() noexcept
    {
        notesIndex = 0;
        remainder = numSamplesBetweenNotes;
        
    }
    
    void stopPlaying() noexcept
    {
        
    }
    
    int getNextMidiNote() noexcept
    {
        if (notesIndex < notesToPlay.size())
        {
            return notesToPlay[notesIndex++];
        }
        
        return 0;
    }
    
    //fills the midibuffer with messages
    void renderNextMidiBlock(MidiBuffer& buffer, int numSamples)
    {
        //need to put a note on message in the buffer
        if(remainder < numSamples)
        {
            if (auto note = getNextMidiNote())
            {
                buffer.addEvent(MidiMessage::noteOn(1, note, 0.9f), remainder);
                remainder = numSamplesBetweenNotes - (numSamples - remainder);
            }
        }
        else remainder -= numSamples;
    }
    
    
private:
    
    int numSamplesBetweenNotes;
    int remainder; //how many samples to go until a note needs to be put into a buffer
    
    Array<int> notesToPlay;
    int notesIndex;
    double sampleRate;
    int timeBetweenNotes; //in ms
};

