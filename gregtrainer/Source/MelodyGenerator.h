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
    String mode;
    Array<int> relativeNotes;
    Array<int> midiNotes;
    int noteLenght, timeBetweenNotes;
};


class MelodyGenerator final
{
    
    Melody generateMelody(int numNotes) noexcept
    {
        
    }
    
};
