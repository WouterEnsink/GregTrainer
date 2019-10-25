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


/* MidiGenerator is the piece of code that translates the information from a Melody object into
 * actual MIDI and fills buffers with that MIDI once startPlaying() is called
 */


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
    
    // set note length needs to be called after this one
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
        // should stop somehow
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
            if (auto note = getNextMidiNoteOn(); note != 0)
            {
                buffer.addEvent(MidiMessage::noteOn(1, note, 0.9f), remainderNoteOn);
                remainderNoteOn = numSamplesBetweenNotes - (numSamples - remainderNoteOn);
            }
        }
        else remainderNoteOn -= numSamples;
        
        //need to put a note off message in the buffer
        if (remainderNoteOff < numSamples)
        {
            if (auto note = getNextMidiNoteOff(); note != 0)
            {
                buffer.addEvent(MidiMessage::noteOff(1, note, 0.0f), remainderNoteOff);
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
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiGenerator)
    
};


