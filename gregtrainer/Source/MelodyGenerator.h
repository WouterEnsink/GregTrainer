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
#include "Identifiers.h"

//===============================================================================================
// This structure represents everything we need to know about a melody in this program

class Melody final   : public ReferenceCountedObject
{
public:
    using Ptr = ReferenceCountedObjectPtr<Melody>;
    
    Melody (String mode, Array<int> relativeNotes, int normalizedGround,
            int midiOffset, int noteLength, int timeBetweenNotes) :
        mode (mode),
        relativeNotes (relativeNotes),
        normalizedGroundNoteIndex (normalizedGround),
        midiOffset (midiOffset),
        noteLength (noteLength),
        timeBetweenNotes (timeBetweenNotes)
    {
    }
    
    ~Melody() {}
    
    int getNumNotes() const
    {
        return relativeNotes.size();
    }
    
    Array<int> getMidiNotes() const
    {
        auto midiNotes = relativeNotes;
        
        for (auto& note : midiNotes)
            note += midiOffset;
        
        return midiNotes;
    }
    
    int getTimeBetweenNotes() const
    {
        return timeBetweenNotes;
    }
    
    int getNoteLength() const
    {
        return noteLength;
    }
    
private:
    
    String mode;
    Array<int> relativeNotes;
    int normalizedGroundNoteIndex;
    int midiOffset;
    int noteLength, timeBetweenNotes;
};



//===============================================================================================
// MelodyGenerator is the source for all training material in the program
//
// In MelodyGenerator there are 3 terms for note, which makes things a little confusing
// we have:
//      - Midi Notes that are the normal, known midi notes
//      - Normalized notes that are Midi Notes in terms of their distances, just reduces to lower numbers
//        to make it easier to work with them to generate new midi
//      - Index notes, which are basically the index at which you can find a note in the array of normalized notes,
//        these are neccesairy to make modulation possible without very complex algorithms


class MelodyGenerator final
{
public:
    
    MelodyGenerator (ValueTree& t, int numNotes)  : tree (t), numNotes (numNotes) {}
    
    // Fix Me
    Array<int> compareMelodiesAndGetDifference (Melody::Ptr first, Melody::Ptr second) const
    {
        return { };
    }
    
    Melody::Ptr generateMelody (int numNotes) noexcept
    {
        auto mode = getRandomMode();
        
        auto relativeNotes = generateRelativeNotesForMode (mode, numNotes);
        
        auto groundNoteIndex = getIndexGroundNoteForMode (mode);
        
        auto midiOffset = generateRandomMidiOffset();
        
        auto timeBetweenNotes = 400;
        auto noteLength = 200;
        
        print ("mode:", mode);
        
        return new Melody { mode, relativeNotes, groundNoteIndex, midiOffset, noteLength, timeBetweenNotes };
    }
    
    void setNumNotesInMelody (int num)
    {
        numNotes = num;
    }
    
    int generateRandomMidiOffset() { return random.nextInt (12) + 60; }
    
    void generateMelody()
    {
        Melody::Ptr melody = generateMelody (numNotes);
        tree.setProperty (IDs::Engine::EngineMelody, melody.get(), nullptr);
    }
    
    String getRandomMode() noexcept { return modes[random.nextInt (modes.size())]; }
    
    int getIndexGroundNoteForMode (const String& mode) const noexcept
    {
        if (mode == "D") return 1;
        if (mode == "E") return 2;
        if (mode == "F") return 3;
        if (mode == "G") return 4;
        return 0;
    }
    
    
    Array<int> generateMidiNotesFromRelativeNotes (Array<int> relativeNotes) noexcept
    {
        auto transpose = random.nextInt (12) + 60;
        
        for (auto& note : relativeNotes)
            note += transpose;
        
        return relativeNotes;
    }
    
    
    Array<int> generateRelativeNotesForMode (const String& mode, int numNotes) noexcept
    {
        Array<int> notes;
        
        Array distances = { 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 4 };
        
        auto groundIndex = getIndexGroundNoteForMode (mode);
        
        auto currentNoteIndex = groundIndex;
        
        notes.add (normalizedMidiNoteDistances[currentNoteIndex]);
        
        for (int i = 1; i < numNotes; ++i)
        {
            auto direction = random.nextInt (2) ? 1 : -1;

            auto interval = direction * distances[random.nextInt (distances.size())];

            currentNoteIndex += interval;

            if (currentNoteIndex > normalizedMidiNoteDistances.size() - 1)
                currentNoteIndex = normalizedMidiNoteDistances.size() - 1;
            
            if (currentNoteIndex < 0)
                currentNoteIndex = 0;

            notes.add (normalizedMidiNoteDistances[currentNoteIndex]);
        }
        
        std::reverse (std::begin (notes), std::end (notes));
        
        return notes;
    }
   
    Random random;
    
    ValueTree tree;
    
    const Array<String> modes { "D", "E", "F", "G" };
    int numNotes;
    const Array<int> normalizedMidiNoteDistances = { 0, 2, 4, 5, 7, 9, 11 };
    const Array<String> noteNames { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
};





