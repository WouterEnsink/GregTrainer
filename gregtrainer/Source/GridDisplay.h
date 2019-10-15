/*
  ==============================================================================

    GridDisplay.h
    Created: 14 Oct 2019 9:56:58pm
    Author:  Wouter Ensink

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//a schale is just some midi info of the notes it containes

template<char const* Name, int... Notes>
struct Scale {
    String getName() const { return Name; }
    Array<int> getNoteNumbers() const { return { Notes... }; }
};

constexpr char d_Plagaal[]= { "D-Plagaal" };


using D_Plagaal = Scale<d_Plagaal, 62, 64, 65, 67, 69, 71, 72, 73>;





class GridDisplay : public Component
{
public:
    
    GridDisplay(Point<int> size) : size(size) { }
    ~GridDisplay() { }
    
private:
    
    class GridTile : public Component
    {
        GridTile(Colours colour);
        
        void mouseDown(const MouseEvent& e) override;
        
    };
    
    Array<GridTile> gridTiles;
    
    Point<int> size;
    
    
};
