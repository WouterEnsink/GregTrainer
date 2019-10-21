/*
  ==============================================================================

    MelodyGenerator.h
    Created: 18 Oct 2019 10:03:48pm
    Author:  Wouter Ensink

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Utility.h"

struct Melody final
{
    int numNotes;
    String mode;
    Array<int> relativeNotes;
    Array<int> midiNotes;
    int noteLength, timeBetweenNotes;
};



class MelodyGenerator final
{
public:
    
    Melody generateMelody(int numNotes) noexcept
    {
        auto relativeNotes = generateRelativeNotesForMode(numNotes);
        return {
            .numNotes = numNotes,
            .mode = "D",
            .relativeNotes = relativeNotes,
            .midiNotes = generateMidiNotesFromRelativeNotes(relativeNotes),
            .noteLength = 200,
            .timeBetweenNotes = 400
        };
    }
    
    enum class Mode
    {
        D, E, F, G
    };
    
    Array<int> generateMidiNotesFromRelativeNotes(Array<int> relativeNotes) noexcept
    {
        auto transpose = random.nextInt(12) + 50;
        for(auto& note : relativeNotes)
            note += transpose;
        return relativeNotes;
    }
    
    Array<int> generateRelativeNotesForMode(int numNotes) noexcept
    {
        Array<int> notes;
        notes.resize(numNotes);
        
        Array distances = { 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 4, 5, 5, 6, 7 };
        
        auto groundIndex = 1; //D
        
        auto currentNoteIndex = groundIndex;
        
        for(int i = 0; i < numNotes; ++i)
        {
            notes.set(i, normalizedMidiNoteDistances[currentNoteIndex]);
            auto direction = random.nextInt(1) ? 1 : -1;
            auto interval = direction * distances[random.nextInt(distances.size())];
            currentNoteIndex = (currentNoteIndex + interval) % normalizedMidiNoteDistances.size();
            if(currentNoteIndex<0) currentNoteIndex += normalizedMidiNoteDistances.size();
            print(currentNoteIndex);
        }
        
        return notes;
    }
   
    Random random;
    static constexpr int numAvailableNotes = 7;
    
    const Array<int> normalizedMidiNoteDistances = { 0, 2, 4, 5, 7, 9, 11 };
};

//const Array<int> MelodyGenerator::normalizedMidiNoteDistances = { 0, 2, 4, 5, 7, 9, 11 };




