/*
  ==============================================================================

    GridDisplayComponent.cpp
    Created: 21 Oct 2019 5:22:58pm
    Author:  Wouter Ensink

  ==============================================================================
*/

#include "GridDisplayComponent.h"
#include "Identifiers.h"
#include "MelodyGenerator.h"

/* Converts the grid's state to a Melody that contains relative notes
 * Midi notes are not inserted, because that is irrelevant for checking if the answer is correct
 */

class GridToMelodyConverter
{
public:
    virtual ~GridToMelodyConverter() { }
    
    virtual Melody convertGridToMelody(const Array<int>& gridState)
    {
        Melody melody;
        auto size = gridState.size();
        melody.relativeNotes.resize(size);
        melody.numNotes = size;
        
        for(int i = 0; i < melody.relativeNotes.size(); ++i)
            melody.relativeNotes.set(i, convertIndexToRelativeNote(gridState[i]));
        
        return melody;
    }
    
private:
    virtual int convertIndexToRelativeNote(int index)
    {
        print("index: ", index);
        if(index < 0) return -1;
        
        //index 7 = c -> 0, index 6 = d -> 2, etc...
        Array relativeNotes = { 12, 11, 9, 7, 5, 4, 2, 0 };
        return relativeNotes[index];
    }
    
   // JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GridToMelodyConverter)
};

/****************************************************************************************/

// GridTileComponent should be ReferenceCountedObject in ValueTree, because it doesn't
// matter what it's state was the previous time you use this app
// the index is defined by it's Identifier

class GridDisplayComponent::GridTileComponent     : public Component, public TreeListener
{
public:
    
    GridTileComponent(ValueTree& tree, const Identifier& id) : valueTree(tree), tileIdentifier(id)
    {
        valueTree.addListener(this);
        valueTree.setProperty(IDs::Grid::TileState, tileStateToString(TileState::tileInactive), nullptr);
        valueTree.setProperty(IDs::Grid::TileSetable, true, nullptr);
    }
    
    
    void mouseDown(const MouseEvent&) override
    {
        if (isSetable)
            isTileOn() ? valueTree.setProperty(IDs::Grid::TileState, tileStateToString(TileState::tileInactive), nullptr)
                        : valueTree.setProperty(IDs::Grid::TileState, tileStateToString(TileState::tileActive), nullptr);
        
        mouseDownOnTile = true;
        repaint();
    }
    
    void mouseEnter(const MouseEvent&) override
    {
        mouseHooveringOverTile = true;
        repaint();
    }
    
    void mouseExit(const MouseEvent&) override
    {
        mouseHooveringOverTile = false;
        repaint();
    }
    
    void mouseUp(const MouseEvent&) override
    {
        mouseDownOnTile = false;
        repaint();
    }
    
    void paint(Graphics& g) override
    {
        g.setColour(getCurrentFillColour());
        
        auto floatBounds = getLocalBounds().toFloat();
        
        g.fillRoundedRectangle(floatBounds, roundness);
        
        g.setColour(getCurrentTextColour());
        
        g.setFont(noteFont);
        
        auto bounds = getLocalBounds().reduced(10);
        
        g.drawText(tileText, bounds, Justification::centred);
    }
    
    
    void valueTreePropertyChanged(ValueTree& tree, const Identifier& id) override
    {
        if(tree.hasType(tileIdentifier))
        {
            if(id == IDs::Grid::TileState)
                setTileStateFromString(tree.getPropertyAsValue(IDs::Grid::TileState, nullptr).toString());
            
            if(id == IDs::Grid::TileSetable)
                isSetable = tree.getProperty(IDs::Grid::TileSetable);
        }
            
    }
    
    void resized() override { }

    bool isTileOn() const noexcept                  { return tileState == TileState::tileActive; }
    
    void setTileState(TileState state) noexcept
    {
        tileState = state;
        repaint();
    }
    
    TileState getTileState() const noexcept         { return tileState; }
    
    void setText(const String& text) noexcept       { tileText = text; repaint(); }
    
    void setRoundness(float roundness) noexcept     { roundness = roundness; repaint(); }
    
    void setSetability(bool setable) noexcept       { isSetable = setable; }
    
    
    void setColourForTileState(TileState state, Colour colour)
    {
        switch (state)
        {
            case TileState::tileActive:
                tileActiveColour = colour;
                break;
                
            case TileState::tileInactive:
                tileInactiveColour = colour;
                break;
                
            case TileState::tileRightAnswer:
                tileRightAnswerColour = colour;
                break;
                
            case TileState::tileWrongAnswer:
                tileWrongAnswerColour = colour;
                break;
        }
        repaint();
    }
    
    String getTileStateAsString()
    {
        return tileStateToString(tileState);
    }
    
    void setTileStateFromString(String string)
    {
        setTileState(tileStateFromString(string));
    }
    
private:
    
    
    Colour getCurrentTextColour() const
    {
        return tileState == TileState::tileActive ? tileInactiveColour : tileActiveColour;
    }
    
    //gets the colour, all factors concidered;
    Colour getCurrentFillColour() const
    {
        if(mouseDownOnTile)         return mouseDownColour;
        if(mouseHooveringOverTile)  return mouseHooverColour;
        
        return getColourForCurrentTileState();
    }
    
    Colour getColourForCurrentTileState() const
    {
        switch (tileState)
        {
            case TileState::tileActive:
                return tileActiveColour;
                break;
                
            case TileState::tileInactive:
                return tileInactiveColour;
                break;
                
            case TileState::tileRightAnswer:
                return tileRightAnswerColour;
                break;
                
            case TileState::tileWrongAnswer:
                return tileWrongAnswerColour;
                break;
        }
    }

    ValueTree valueTree;
    Identifier tileIdentifier;
    
    Font noteFont { "Arial", 30.f, Font::plain };
    
    float roundness { 5.f };
    
    String tileText { "A" };
    
    bool mouseHooveringOverTile { false };
    bool mouseDownOnTile        { false };
    
    bool isSetable { true };
    
    TileState tileState { TileState::tileInactive };
    
    Colour tileActiveColour         { Colours::gainsboro    };
    Colour tileInactiveColour       { Colours::black        };
    Colour tileRightAnswerColour    { Colours::green        };
    Colour tileWrongAnswerColour    { Colours::red          };
    
    Colour mouseHooverColour        { Colours::dimgrey      };
    Colour mouseDownColour          { Colours::white        };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GridTileComponent)
    
};


/*******************************************************************************************/


GridDisplayComponent::GridDisplayComponent(ValueTree& t, int numColumns, int numRows, const StringArray& rowsText) :
    numRows(numRows),
    numColumns(numColumns),
    tree(t)
{
    jassert(numRows == rowsText.size());
    
    tileIdentifiers.resize(numColumns);
    
    for (auto& c : tileIdentifiers)
        c.resize(numRows);
    
    gridToMelodyConverter.reset(new GridToMelodyConverter);
   

    for(int column = 0; column < numColumns; ++column)
    {
        OwnedArray<GridTileComponent>* col = new OwnedArray<GridTileComponent>;
        
        for(int row = 0; row < numRows; ++row)
        {
            auto type = tileIndexToIdentifier(column, row);
            ValueTree childTile { type };
            
            auto* tileptr = new GridTileComponent(childTile, type);
            
            col->set(row, tileptr);
            
            addAndMakeVisible(tileptr);
            
            
            tree.appendChild(childTile, nullptr);
        }
        tiles.set(column, col);
    }
    
    
    setSpaceBetweenTiles(2);
    tree.addListener(this);
}


GridDisplayComponent::~GridDisplayComponent()
{

}


void GridDisplayComponent::paint(Graphics& g)
{
    g.setColour(Colours::black);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 5.f);
}

Rectangle<int> GridDisplayComponent::getBoundsForTile(int column, int row)
{
    auto bounds = getLocalBounds();
    auto [x, y, w, h] = getRectangleDimentions(bounds);
    
    w /= numColumns;
    h /= numRows;

    return {
        x + column * w + halfSpaceBetweenTiles,
        y + row * h + halfSpaceBetweenTiles,
        w - spaceBetweenTiles,
        h - spaceBetweenTiles
    };
}


void GridDisplayComponent::resized()
{
    for(int c = 0; c < numColumns; c++)
        for(int r = 0; r < numRows; r++)
        {
            auto bounds = getBoundsForTile(c, r);
            auto* column = tiles[c];
            (*column)[r]->setBounds(bounds);
        }
}


void GridDisplayComponent::setSpaceBetweenTiles(int space) noexcept
{
    spaceBetweenTiles = space;
    halfSpaceBetweenTiles = space/2;
    repaint();
}

// FIX ME
Array<int> GridDisplayComponent::getGridStates() const noexcept
{
    Array<int> states;

    return states;
}


void GridDisplayComponent::setStateForTile(int column, int row, TileState state) noexcept
{
    jassert(row < numRows && column < numColumns);
    
    auto tile = tree.getChildWithName(tileIndexToIdentifier(column, row));
    tile.setProperty(IDs::Grid::TileState, tileStateToString(state), nullptr);
}


void GridDisplayComponent::setSetabilityTile(int column, int row, bool settable) noexcept
{
    jassert(row < numRows && column < numColumns);
    
    auto type = tileIndexToIdentifier(column, row);
    auto tile = tree.getChildWithName(type);
    tile.setProperty(IDs::Grid::TileSetable, settable, nullptr);
}

void GridDisplayComponent::setSetabilityColumn(int column, bool settable) noexcept
{
    jassert(column < numColumns);
}

int GridDisplayComponent::getNumRows() const noexcept       { return numRows; }

int GridDisplayComponent::getNumColumns() const noexcept    { return numColumns; }


void GridDisplayComponent::turnAllTilesOff() noexcept
{
    for(int column = 0; column < numColumns; ++column)
        for(int row = 0; row < numRows; ++row)
            setStateForTile(column, row, TileState::tileInactive);
}


void GridDisplayComponent::valueTreePropertyChanged(ValueTree& tree, const Identifier& id)
{
    if (tree.hasProperty(IDs::Grid::TileState))
        if (tileStateFromString(tree[IDs::Grid::TileState].toString()) == TileState::tileActive)
        {
            auto [column, row] = tileIndexFromIdentifier(tree.getType());
            turnOffAllRowsInColumnExceptThisOne(column, row);
        }
}


void GridDisplayComponent::turnOffAllRowsInColumnExceptThisOne(int column, int indexToLeaveActive)
{
    for(int row = 0; row < numRows; ++row)
        if(row != indexToLeaveActive)
        {
            auto tile = tree.getChildWithName(tileIndexToIdentifier(column, row));
            tile.setProperty(IDs::Grid::TileState, tileStateToString(TileState::tileInactive), nullptr);
        }
}

Melody GridDisplayComponent::getGridStateAsMelody() noexcept
{
    return gridToMelodyConverter->convertGridToMelody(getGridStates());
}


String GridDisplayComponent::tileStateToString(TileState state)
{
    switch (state)
    {
        case TileState::tileActive:
            return "active";
            break;
        
        case TileState::tileInactive:
            return "inactive";
            break;
            
        case TileState::tileRightAnswer:
            return "right";
            break;
            
        case TileState::tileWrongAnswer:
            return "wrong";
            break;
    }
}

GridDisplayComponent::TileState GridDisplayComponent::tileStateFromString(String string)
{
    if(string == "active")      return TileState::tileActive;
    if(string == "inactive")    return TileState::tileInactive;
    if(string == "right")       return TileState::tileRightAnswer;
    
    return TileState::tileWrongAnswer;
}


Identifier GridDisplayComponent::tileIndexToIdentifier(int column, int row)
{
    StringArray a { String { column }, String { row } };
    
    return a.joinIntoString("_");
}

std::tuple<int, int> GridDisplayComponent::tileIndexFromIdentifier(Identifier id)
{
    auto identifier = id.toString();
    auto array = StringArray::fromTokens(id, "_", "");
    auto column = array[0].getIntValue();
    auto row = array[1].getIntValue();
    return { column, row };
}


