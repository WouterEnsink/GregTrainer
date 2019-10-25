/*
  ==============================================================================

    Identifiers.h
    Created: 25 Oct 2019 3:49:50pm
    Author:  Wouter Ensink

  ==============================================================================
*/

#pragma once

// global value tree identifiers

#define DECLARE_ID(name, value) static inline const Identifier name = #value

struct IDs final
{
    struct Grid final
    {
        DECLARE_ID(TileColour,  "TileColour");
        DECLARE_ID(GridState,   "GridState");
        DECLARE_ID(TilePointer, "TilePointer");
        DECLARE_ID(TileState,   "TileState");
        DECLARE_ID(TileSetable, "TileSetable");
    };
    
    struct Engine final
    {
        DECLARE_ID(playState,       "PlayState");
        DECLARE_ID(melodyLength,    "MelodyLength");
        
        static bool isAnEgineIdentifier(Identifier&)
        {
            
            return false;
        }
        
    };
};

#undef DECLARE_ID
