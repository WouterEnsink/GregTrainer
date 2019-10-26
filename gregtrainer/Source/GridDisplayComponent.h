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




//==========================================================================
// Converts the grid's state to a Melody that contains relative notes
// Midi notes are not inserted, because that is irrelevant
// for checking if the answer is correct


class GridToMelodyConverter;


//==========================================================================
// The Grid to be used by the player to fill in their guess


class GridDisplayComponent final  : public Component, public TreeListener
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
    
    //======================================================================
    
    void paint(Graphics& g) override;
    
    void resized() override;
    
    //======================================================================
    
    void setSpaceBetweenTiles(int space) noexcept;
    
    void setStateForTile(int column, int row, TileState on) noexcept;
    
    void setSetabilityTile(int column, int row, bool settable) noexcept;
    
    void setSetabilityColumn(int column, bool settable) noexcept;
    
    void turnAllTilesOff() noexcept;
    
    //======================================================================
    
    int getNumRows() const noexcept;
    
    int getNumColumns() const noexcept;
    
    //======================================================================
    
    static Identifier tileIndexToIdentifier(int column, int row);
    
    static std::tuple<int, int> tileIndexFromIdentifier(Identifier);
    
    static String tileStateToString(TileState state);
    
    static TileState tileStateFromString(String);
    
    //======================================================================
    
private:
    
    class GridTileComponent;
    
    void turnOffAllRowsInColumnExceptThisOne(int column, int row);
    
    Rectangle<int> getBoundsForTile(int column, int row);
    
    //======================================================================
    
    void valueTreePropertyChanged(ValueTree&, const Identifier&) override;
    
    int spaceBetweenTiles, halfSpaceBetweenTiles;
    int numRows, numColumns;
    
    std::unique_ptr<GridToMelodyConverter> gridToMelodyConverter;
    
    ValueTree tree;
    
    OwnedArray<OwnedArray<GridTileComponent>> tiles;
    
    Array<Array<Identifier>> tileIdentifiers;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GridDisplayComponent)
    
};
