/*
  ==============================================================================

    GridDisplayComponent.cpp
    Created: 21 Oct 2019 5:22:58pm
    Author:  Wouter Ensink

  ==============================================================================
*/

#include "GridDisplayComponent.h"

const Identifier IDs::TileColour = "TileColour";


class GridDisplayComponent::GridTileComponent     : public Component
{
public:
    GridTileComponent(const std::function<void(GridTileComponent*)>& callback) :
    tilePressedCallback(callback)
    {
        
    }
    
    void mouseDown(const MouseEvent&) override
    {
        tileOn = tileOn ? false : true;
        tilePressedCallback(this);
        mouseDownOnTile = true;
        repaint();
    }
    
    void mouseEnter(const MouseEvent&) override
    {
        hoover = true;
        repaint();
    }
    
    void mouseExit(const MouseEvent&) override
    {
        hoover = false;
        repaint();
    }
    
    void mouseUp(const MouseEvent&) override
    {
        mouseDownOnTile = false;
        repaint();
    }
    
    void paint(Graphics& g) override
    {
        tileOn ? g.setColour(tileOnColour) : g.setColour(tileOffColour);
        
        if (hoover) g.setColour(hooverColour);
        
        auto floatBounds = getLocalBounds().toFloat();
        
        g.fillRoundedRectangle(floatBounds, roundness);
        
        g.setColour(tileOnColour);
        g.drawRoundedRectangle(floatBounds, roundness, 2.f);
        
        tileOn ? g.setColour(tileOffColour) : g.setColour(tileOnColour);
        
        if (hoover) g.setColour(Colours::black);
        
        g.setFont(noteFont);
        
        auto bounds = getLocalBounds().reduced(10);
        
        g.drawText(tileText, bounds, Justification::centred);
        
        if(mouseDownOnTile) {
            g.setColour(Colours::white);
            g.fillRoundedRectangle(floatBounds, roundness);
        }
    }
    
    
    void resized() override { }

    bool isTileOn() const noexcept                  { return tileOn; }
    
    void setTile(bool on) noexcept                  { tileOn = on; repaint(); }
    
    void setText(const String& text) noexcept       { tileText = text; repaint(); }
    
    void setTileOnColour(Colour colour) noexcept    { tileOnColour = colour; repaint(); }
    
    void setTileOffColour(Colour colour) noexcept   { tileOffColour = colour; repaint(); }
    
    void setRoundness(float roundness) noexcept     { roundness = roundness; repaint(); }
    
    void setSetability(bool setable) noexcept       { isSetable = setable; }
    
private:
    
    //to notify its parent column that the tile has been pressed
    std::function<void(GridTileComponent*)> tilePressedCallback;
    
    Font noteFont { "Arial", 30.f, Font::plain };
    
    float roundness { 5.f };
    
    String tileText { "A" };
    
    bool tileOn { false };
    bool hoover { false };
    bool mouseDownOnTile { false };
    
    bool isSetable { true };
    
    Colour tileOnColour  { Colours::gainsboro };
    Colour tileOffColour { /*Colours::lightslategrey*/ Colours::dimgrey };
    Colour hooverColour  { /*Colours::lightblue */ Colours::dimgrey };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GridTileComponent)
    
};


/*******************************************************************************************/

/** Manages the behaviour of a column of GridTiles, but doesn't draw them
 *  it's not a component!
 */


class GridDisplayComponent::GridColumn
{
public:
    
    GridColumn(int numRows) : numRows(numRows)
    {
        tiles.resize(numRows);
        for(auto& t : tiles)
            t = new GridTileComponent([this](auto* tile){ tileClicked(tile); });
    }
    
    ~GridColumn()
    {
        for (auto* t : tiles) delete t;
    }
    
    void tileClicked(GridTileComponent* tile) noexcept
    {
        //if this turned the tile on, then all other tiles should be turned off
        if(tile->isTileOn())
            for(auto* t: tiles)
                if (t != tile) t->setTile(false);
    }
    
    void setAllTiles(bool state) noexcept
    {
        for(auto& t : tiles) t->setTile(state);
    }
    
    void forEachTile(const std::function<void(GridTileComponent&)>& function)
    {
        for(auto& t : tiles) function(*t);
    }
    
    void setTile(int row, bool state) noexcept
    {
        tiles[row]->setTile(state);
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
    
    for(auto& c : gridColumns)
    {
        c = new GridColumn(numRows);
        c->forEachTile([this](auto& tile){ addAndMakeVisible(tile); });
        c->setText(rowsText);
    }
    
    setSpaceBetweenTiles(2);
    tree.addListener(this);
    ValueTree child { IDs::TileColour };
    Colour c = Colours::black;
    
    tree.setProperty(IDs::TileColour, c.toString(), nullptr);
   // tree.appendChild(child, nullptr);
}


GridDisplayComponent::~GridDisplayComponent()
{
    removeAllChildren();
    
    for(auto* column : gridColumns) delete column;
}


void GridDisplayComponent::paint(Graphics& g)
{
    // g.setColour(Colours::lightslategrey);
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
    states.resize(gridColumns.size());
    for(auto* c : gridColumns) states.add(c->getIndexActivatedTile());
    return states;
}


void GridDisplayComponent::setTile(int column, int row, bool state) noexcept
{
    jassert(row < numRows && column < numColumns);
    
    gridColumns[column]->setTile(row, state);
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
    for(int c = 0; c < numColumns; ++c)
        for(int r = 0; r < numRows; ++r)
            gridColumns[c]->setTile(r, false);
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
        forEachTile([colour](GridTileComponent& tile){ tile.setTileOffColour(colour); });
    }
}

/*******************************************************************************************/


