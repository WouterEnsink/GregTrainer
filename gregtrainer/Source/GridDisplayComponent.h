/*
  ==============================================================================

    GridDisplay.h
    Created: 14 Oct 2019 9:56:58pm
    Author:  Wouter Ensink

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Utility.h"


/*****************************************************************************************************/


class GridDisplayComponent  : public Component
{
public:
    
    GridDisplayComponent(int numColumns, int numRows, const Array<String>& rowsText);
    
    ~GridDisplayComponent();
    
    void paint(Graphics& g) override;
    
    void resized() override;
    
    
    void setSpaceBetweenTiles(int space) noexcept;
    
    /*returns array with wich row is on in each colum*/
    Array<int> getGridStates() const noexcept;
    
    
    void setTile(int column, int row, bool on) noexcept;
    
    //set whether or not the user can set or unset a tile
    void setSettabilityTile(int column, int row, bool settable) noexcept;
    
    void setSettabilityColumn(int column, bool settable) noexcept;
    
private:
    
    class GridTileComponent;
    class GridColumnComponent;
    
    Property<int> spaceBetweenTiles {
        .value = 0,
        .set = [this](auto v){ halfSpaceBetweenTiles = v/2; return v; },
        .get = [](auto v){ return v; }
    };
    
    int numRows, numColumns;
    int halfSpaceBetweenTiles;
    Array<GridColumnComponent*> columns;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GridDisplayComponent)
    
};
