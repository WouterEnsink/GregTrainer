/*
  ==============================================================================

    GridDisplayComponent.cpp
    Created: 21 Oct 2019 5:22:58pm
    Author:  Wouter Ensink

  ==============================================================================
*/

#include "GridDisplayComponent.h"

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


class GridDisplayComponent::GridTileComponent     : public Component, public TreeListener
{
public:
    GridTileComponent(const std::function<void(GridTileComponent*)>& callback,
                      ValueTree& tree,
                      const Identifier& id) :
        tilePressedCallback(callback),
        valueTree(tree),
        tileIdentifier(id)
    {
        valueTree.addListener(this);
        valueTree.setProperty(tileIdentifier, getTileStateAsString(), nullptr);
    }
    
    void mouseDown(const MouseEvent&) override
    {
        isTileOn() ? valueTree.setProperty(tileIdentifier, tileStateToString(TileState::tileInactive), nullptr)
                        : valueTree.setProperty(tileIdentifier, tileStateToString(TileState::tileActive), nullptr);
        
        tilePressedCallback(this);
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
        if(id == tileIdentifier)
            setTileStateFromString(tree.getPropertyAsValue(tileIdentifier, nullptr).toString());
    }
    
    void resized() override { }

    bool isTileOn() const noexcept                  { return tileState == TileState::tileActive; }
    
    void setTileState(TileState state) noexcept     { tileState = state; repaint(); }
    
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
        setTileState(stringToTileState(string));
    }
    
private:
    
    //to notify its parent column that the tile has been pressed
    //should be replaced by value tree
    std::function<void(GridTileComponent*)> tilePressedCallback;
    
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

    ValueTree& valueTree;
    Identifier tileIdentifier;
    
    Font noteFont { "Arial", 30.f, Font::plain };
    
    float roundness { 5.f };
    
    String tileText { "A" };
    
    bool mouseHooveringOverTile { false };
    bool mouseDownOnTile        { false };
    
    bool isSetable { true };
    
    TileState tileState { TileState::tileInactive };
    
    Colour tileActiveColour         { Colours::gainsboro    };
    Colour tileInactiveColour       { Colours::dimgrey      };
    Colour tileRightAnswerColour    { Colours::green        };
    Colour tileWrongAnswerColour    { Colours::red          };
    
    Colour mouseHooverColour        { Colours::dimgrey      };
    Colour mouseDownColour          { Colours::white        };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GridTileComponent)
    
};


/*******************************************************************************************/

/** Manages the behaviour of a column of GridTiles, but doesn't draw them
 *  it's not a component!
 */


class GridDisplayComponent::GridColumn
{
public:
    
    GridColumn(int column, int numRows, ValueTree& tree) : numRows(numRows)
    {
        tiles.resize(numRows);
        
        for(int row = 0; row < numRows; ++row)
        {
            auto* tile = new GridTileComponent([this](auto* tile){ tileClicked(tile); },
                                                tree,
                                                tileIndexToIdentifier(column, row));
            
            tiles.set(row, tile);
        }
    }
    
    ~GridColumn()
    {
        for (auto* t : tiles) delete t;
    }
    
    //should be replaced by value tree
    void tileClicked(GridTileComponent* tile) noexcept
    {
        //if this turned the tile on, then all other tiles should be turned off
        if(tile->isTileOn())
            for(auto* t : tiles)
                if (t != tile) t->setTileState(TileState::tileInactive);
    }
    
    void setAllTiles(TileState state) noexcept
    {
        for(auto& t : tiles) t->setTileState(state);
    }
    

    
    void forEachTile(const std::function<void(GridTileComponent&)>& function)
    {
        for(auto& t : tiles) function(*t);
    }
    
    void setTileState(int row, TileState state)
    {
        tiles[row]->setTileState(state);
    }
    
    void setText(const StringArray& text)
    {
        jassert(text.size() == tiles.size());
        
        for(int i = 0; i < numRows; ++i)
            tiles[i]->setText(text[i]);
    }
    
    Component* getTileComponent(int index)
    {
        return tiles[index];
    }
    
    int getIndexActivatedTile()
    {
        for(int r = 0; r < numRows; ++r)
            if(tiles[r]->isTileOn()) return r;
        
        return -1;
    }
    
private:
    
    int columnNumber;
    int numRows;
    Array<GridTileComponent*> tiles;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GridColumn)
};


/*******************************************************************************************/



GridDisplayComponent::GridDisplayComponent(ValueTree& t, int numColumns, int numRows, const StringArray& rowsText) :
    numRows(numRows),
    numColumns(numColumns),
    tree(t)
{
    jassert(numRows == rowsText.size()); //you must have as many texts as you have rows
    
    gridColumns.resize(numColumns);
    
    gridToMelodyConverter.reset(new GridToMelodyConverter);
    
    for(int column = 0; column < numColumns; ++column)
    {
        auto* c = new GridColumn(column, numRows, tree);
        c->forEachTile([this](auto& tile){ addAndMakeVisible(tile); });
        c->setText(rowsText);
        gridColumns.set(column, c);
    }

    
    setSpaceBetweenTiles(2);
    tree.addListener(this);
    
    Colour c = Colours::black;
    tree.setProperty(IDs::TileColour, c.toString(), nullptr);
}


GridDisplayComponent::~GridDisplayComponent()
{
    removeAllChildren();
    
    for(auto* column : gridColumns) delete column;
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
            auto b = getBoundsForTile(c, r);
            gridColumns[c]->getTileComponent(r)->setBounds(b);
        }
}


void GridDisplayComponent::setSpaceBetweenTiles(int space) noexcept
{
    spaceBetweenTiles = space;
    repaint();
}


Array<int> GridDisplayComponent::getGridStates() const noexcept
{
    Array<int> states;

    for(auto* c : gridColumns) states.add(c->getIndexActivatedTile());
    return states;
}


void GridDisplayComponent::setStateForTile(int column, int row, TileState state) noexcept
{
    jassert(row < numRows && column < numColumns);
    
    tree.setProperty(tileIndexToIdentifier(column, row), tileStateToString(state), nullptr);
}


void GridDisplayComponent::setSetabilityTile(int column, int row, bool settable) noexcept
{
    jassert(row < numRows && column < numColumns);
    
}

void GridDisplayComponent::setSetabilityColumn(int column, bool settable) noexcept
{
    jassert(column < numColumns);
}

int GridDisplayComponent::getNumRows() const noexcept { return numRows; }

int GridDisplayComponent::getNumColumns() const noexcept { return numColumns; }


void GridDisplayComponent::turnAllTilesOff() noexcept
{
    for(int column = 0; column < numColumns; ++column)
        for(int row = 0; row < numRows; ++row)
            setStateForTile(column, row, TileState::tileInactive);
}


void GridDisplayComponent::forEachTile(const std::function<void (GridTileComponent&)>& func) noexcept
{
    for(auto* c : gridColumns)
        c->forEachTile(func);
}


void GridDisplayComponent::valueTreePropertyChanged(ValueTree& tree, const Identifier& id)
{
    if(id == IDs::TileColour)
    {
        auto colourString = tree.getPropertyAsValue(IDs::TileColour, nullptr).toString();
        auto colour = Colour::fromString(colourString);
        forEachTile([colour] (GridTileComponent& tile) { tile.setColourForTileState(TileState::tileInactive, colour); });
    }
}


Melody GridDisplayComponent::getGridStateAsMelody() noexcept
{
    return gridToMelodyConverter->convertGridToMelody(getGridStates());
}

String GridDisplayComponent::getGridStateAsString()
{
    String state;
    Array<StringArray> stateInSeperateStrings;
    
    for (int column = 0; column < numColumns; ++column)
        for(int row = 0; row < numRows; ++row)
            stateInSeperateStrings[column].add(String(row));
    
    StringArray columns;
    for(auto& column : stateInSeperateStrings)
        columns.add(column.joinIntoString(","));
    
    return columns.joinIntoString("_");
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

GridDisplayComponent::TileState GridDisplayComponent::stringToTileState(String string)
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
    auto array = StringArray::fromTokens(id, "_");
    auto column = array[0].getIntValue();
    auto row = array[1].getIntValue();
    return { column, row };
}

/*******************************************************************************************/


