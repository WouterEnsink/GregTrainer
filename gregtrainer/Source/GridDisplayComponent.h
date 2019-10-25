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
#include "MelodyGenerator.h"




/*****************************************************************************************************/

/* Converts the grid's state to a Melody that contains relative notes
 * Midi notes are not inserted, because that is irrelevant for checking if the answer is correct
 */

class GridToMelodyConverter;


/*****************************************************************************************************/

/* Should be updated to remove the GridColumnComponent class
 * as that was just an ugly but quick solution
 */

class GridDisplayComponent  : public Component, public TreeListener
{
public:
    
    GridDisplayComponent(ValueTree&, int numColumns, int numRows, const StringArray& rowsText);
    
    ~GridDisplayComponent();
    
    enum class TileState
    {
        tileActive,
        tileInactive,
        tileWrongAnswer,
        tileRightAnswer
    };
    
    static String tileStateToString(TileState state);
    static TileState tileStateFromString(String);
    
    void paint(Graphics& g) override;
    
    void resized() override;
    
    void setSpaceBetweenTiles(int space) noexcept;
    
    // returns array with wich row is on in each column
    Array<int> getGridStates() const noexcept;
    
    void setStateForTile(int column, int row, TileState on) noexcept;
    
    void setSetabilityTile(int column, int row, bool settable) noexcept;
    
    void setSetabilityColumn(int column, bool settable) noexcept;
    
    int getNumRows() const noexcept;
    
    int getNumColumns() const noexcept;
    
    void turnAllTilesOff() noexcept;
    
    Melody getGridStateAsMelody() noexcept;
    
    Rectangle<int> getBoundsForTile(int column, int row);
    
    static Identifier tileIndexToIdentifier(int column, int row);
    
    static std::tuple<int, int> tileIndexFromIdentifier(Identifier);
    
private:
    
    class GridTileComponent;
    
    void turnOffAllRowsInColumnExceptThisOne(int column, int row);
    
    void valueTreePropertyChanged(ValueTree&, const Identifier&) override;
    
    int spaceBetweenTiles, halfSpaceBetweenTiles;
    int numRows, numColumns;
    
    std::unique_ptr<GridToMelodyConverter> gridToMelodyConverter;
    
    ValueTree tree;
    
    OwnedArray<OwnedArray<GridTileComponent>> tiles;
    
    // all Identifiers are predefined, to safe processing power
    Array<Array<Identifier>> tileIdentifiers;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GridDisplayComponent)
    
};
