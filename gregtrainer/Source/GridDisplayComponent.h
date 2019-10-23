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

struct IDs
{
    static const Identifier TileColour;

};

/*****************************************************************************************************/

/* Should be updated to remove the GridColumnComponent class
 * as that was just an ugly but quick solution
 */

class GridDisplayComponent  : public Component, public TreeListener
{
public:
    
    GridDisplayComponent(ValueTree&, int numColumns, int numRows, const StringArray& rowsText);
    
    ~GridDisplayComponent();
    
    void paint(Graphics& g) override;
    
    void resized() override;
    
    void setSpaceBetweenTiles(int space) noexcept;
    
    /* returns array with wich row is on in each column */
    Array<int> getGridStates() const noexcept;
    
    void setTile(int column, int row, bool on) noexcept;
    
    void setSetabilityTile(int column, int row, bool settable) noexcept;
    
    void setSetabilityColumn(int column, bool settable) noexcept;
    
    int getNumRows() const noexcept;
    
    int getNumColumns() const noexcept;
    
    void turnAllTilesOff() noexcept;
    
    Rectangle<int> getBoundsForTile(int column, int row);
    
private:
    
    class GridTileComponent;
    class GridColumn;
    
    void forEachTile(const std::function<void(GridTileComponent&)>&) noexcept;
    
    void valueTreePropertyChanged(ValueTree&, const Identifier&) override;
    
    /* this property also computes the value for halfSpaceBetweenTiles */
    Property<int> spaceBetweenTiles {
        .value = 0,
        .set = [this](auto v){ halfSpaceBetweenTiles = v/2; return v; },
        .get = [](auto v){ return v; }
    };
    
    int numRows, numColumns;
    int halfSpaceBetweenTiles;
    
    Array<GridColumn*> gridColumns;
    
    ValueTree tree;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GridDisplayComponent)
    
};
