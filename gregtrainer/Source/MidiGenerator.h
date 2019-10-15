/*
  ==============================================================================

    MidiGenerator.h
    Created: 15 Oct 2019 9:32:25pm
    Author:  Wouter Ensink

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//you give it an array of note numbers to play
//and it will generate the midi for that

class MidiGenerator : private Timer
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
    }
    
    void startPlaying() noexcept
    {
        iterator = notesToPlay.begin();
        startTimer(timeBetweenNotes);
    }
    
    void stopPlaying() noexcept
    {
        stopTimer();
    }
    
    int getNextMidiNote() noexcept
    {
        if ((iterator + 1) == nullptr){
            loopCompleted = true;
            return 0;
        }
        
        return *iterator++;
    }
    
    void timerCallback() override
    {
        
        if (!loopCompleted)
        {
            if (auto note = getNextMidiNote() != 0)
            {
                auto message = MidiMessage::noteOn(1, note, 0.9f);
                auto time = Time::getMillisecondCounterHiRes() - bufferStartTime;
                auto sample = static_cast<int>(sampleRate * time);
                buffer.addEvent(message, sample);
            }
        }
        else stopTimer();
    }
    
    MidiBuffer getNextMidiBuffer() noexcept
    {
        //set current time, needed for next midibuffer
        bufferStartTime = Time::getMillisecondCounterHiRes();
        auto returnBuffer = buffer;
        buffer.clear();
        return returnBuffer;
    }
    
    
private:
    
    int numSamplesBetweenNotes;
    Array<int> notesToPlay;
    int* iterator { nullptr };
    double sampleRate;
    int timeBetweenNotes; //in ms
    double bufferStartTime;
    bool loopCompleted;
    bool bufferReset;
    MidiBuffer buffer;
};

