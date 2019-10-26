/*
  ==============================================================================

    Identifiers.h
    Created: 25 Oct 2019 3:49:50pm
    Author:  Wouter Ensink

  ==============================================================================
*/

#pragma once

// Global ValueTree Identifiers

#define DECLARE_ID(name, value) static inline const Identifier name = #value

struct IDs final
{
    DECLARE_ID(GlobalRoot,  "Root");
    
    struct Grid final
    {
        DECLARE_ID(GridRoot,    "GridRoot"      );
        DECLARE_ID(TileColour,  "TileColour"    );
        DECLARE_ID(GridState,   "GridState"     );
        DECLARE_ID(TilePointer, "TilePointer"   );
        DECLARE_ID(TileState,   "TileState"     );
        DECLARE_ID(TileSetable, "TileSetable"   );
        DECLARE_ID(TileText,    "TileText"      );
    };
    
    struct Engine final
    {
        DECLARE_ID(EngineRoot,      "EngineRoot"    );
        DECLARE_ID(PlayState,       "PlayState"     );
        DECLARE_ID(MelodyLength,    "MelodyLength"  );
        
        static bool isAnEgineIdentifier(Identifier&)
        {
            
            return false;
        }
        
    };
};

#undef DECLARE_ID
