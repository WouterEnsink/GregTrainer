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


class GridDisplayComponent  : public Component
{
public:
    
    GridDisplayComponent(int numColumns, int numRows, const Array<String>& rowsText) :
        numRows(numRows),
        numColumns(numColumns)
    {
        
        jassert(numRows == rowsText.size()); //you must have as many texts as you have rows
        
        columns.resize(numColumns);
        
        for(auto& column : columns)
        {
            column = new GridColumnComponent(numRows);
            addAndMakeVisible(column);
            column->setTileTexts(rowsText);
        }
        
        setSpaceBetweenTiles(6);
    }
    
    ~GridDisplayComponent()
    {
        removeAllChildren();
        
        for(auto* column : columns) delete column;
    }
    
    void paint(Graphics& g) override
    {
       // g.setColour(Colours::lightslategrey);
        g.setColour(Colours::black);
        g.fillRoundedRectangle(getLocalBounds().toFloat(), 10.f);
    }
    
    void resized() override
    {
        const auto bounds = getLocalBounds();
        
        const auto [x, y, w, h] = getRectangleDimentions(bounds);
        
        const auto columnWidth = w / numColumns;
        
        const auto halfSpaceBetweenTiles = spaceBetweenTiles / 2;
        
        for(int i = 0; i < numColumns; ++i)
            columns[i]->setBounds(x + i * columnWidth + halfSpaceBetweenTiles,
                                  y + halfSpaceBetweenTiles,
                                  columnWidth - halfSpaceBetweenTiles,
                                  h - halfSpaceBetweenTiles);
    }
    
    
    void setSpaceBetweenTiles(int space) noexcept
    {
        spaceBetweenTiles = space;
        for(auto* c : columns) c->setSpaceBetweenTiles(space);
        repaint();
    }
    
    //returns array with wich row is on in each colum
    Array<int> getGridStates() const noexcept
    {
        Array<int> states;
        states.resize(columns.size());
        for(auto* c : columns) states.add(c->getIndexTileOn());
        return states;
    }
    
    
    void setTile(int column, int row, bool on) noexcept
    {
        jassert(row < numRows && column < numColumns);
        
        columns[column]->setTile(row, on);
    }
    
    
    //set whether or not the user can set or unset a tile
    void setSettabilityTile(int column, int row, bool settable) noexcept
    {
        jassert(row < numRows && column < numColumns);
        
        
    }
    
    void setSettabilityColumn(int column, bool settable) noexcept
    {
        jassert(column < numColumns);
    }
    
private:
    

    class GridTileComponent     : public Component
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
        }
        
        
        void resized() override { }
        
        bool isTileOn() const noexcept                  { return tileOn; }
        
        void setTile(bool on) noexcept                  { tileOn = on; repaint(); }
        
        void setText(const String& text) noexcept       { tileText = text; repaint(); }
        
        void setTileOnColour(Colour colour) noexcept    { tileOnColour = colour; repaint(); }
        
        void setTileOffColour(Colour colour) noexcept   { tileOffColour = colour; repaint(); }
        
        void setRoundness(float roundness) noexcept     { roundness = roundness; repaint(); }

        void setSettability(bool settable) noexcept     { isSettable = settable; }
        
    private:
        
        //to notify its parent column that the tile has been pressed
        std::function<void(GridTileComponent*)> tilePressedCallback;
        
        Font noteFont { "Arial", 30.f, Font::plain };
        
        float roundness { 10.f };
        
        String tileText { "A" };
        
        bool tileOn { false };
        bool hoover { false };
        
        bool isSettable { true };
        
        Colour tileOnColour  { Colours::gainsboro };
        Colour tileOffColour { Colours::lightslategrey };
        Colour hooverColour  { Colours::lightblue };
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GridTileComponent)
        
    };
    
    
    
    class GridColumnComponent   : public Component
    {
    public:
        
        GridColumnComponent(int numTiles) : numTiles(numTiles)
        {
            tiles.resize(numTiles);
            
            for(auto& tile : tiles)
            {
                tile = new GridTileComponent([this](GridTileComponent* tilePressed){
                    if(tilePressed->isTileOn())
                    {
                        for(auto* tile : tiles)
                            if (tile != tilePressed)
                                tile->setTile(false);
                    }
                });
                
                addAndMakeVisible(tile);
            }
        }
        
        ~GridColumnComponent()
        {
            removeAllChildren();
            
            for(auto* tile : tiles)
                delete tile;
        }
        
        void paint(Graphics& g) override
        {
            
        }
        
        void resized() override
        {
            const auto bounds = getLocalBounds();
            
            const auto [x, y, w, h] = getRectangleDimentions(bounds);
            
            const auto tileHeight = h / numTiles;
            
            auto halfSpaceBetweenTiles = spaceBetweenTiles / 2;
            
            for(auto i = 0; i < numTiles; ++i)
                tiles[i]->setBounds(x,
                                    i * tileHeight + y + halfSpaceBetweenTiles,
                                    w - halfSpaceBetweenTiles,
                                    tileHeight - spaceBetweenTiles);
        }
        
        
        void setTileTexts(const Array<String>& texts) noexcept
        {
            jassert(texts.size() == tiles.size());
            
            for(int i = 0; i < numTiles; ++i) tiles[i]->setText(texts[i]);
            repaint();
        }
        
        void setSpaceBetweenTiles(int space) noexcept
        {
            spaceBetweenTiles = space;
            repaint();
        }
        
        int getIndexTileOn() const noexcept
        {
            for(int i = 0; i < tiles.size(); ++i)
                if (tiles[i]->isTileOn()) return i;
            
            return -1;
        }
        
        void setTile(int row, bool on) noexcept
        {
            tiles[row]->setTile(on);
        }
        
        void setSettabilityColumn(bool settable) noexcept
        {
            isSettable = settable;
            for (auto* t : tiles) t->setSettability(settable);
        }
        
        void setSettabilityTile(int row, bool settable) noexcept
        {
            tiles[row]->setSettability(settable);
        }
        
    private:
        

        int numTiles;
        int activatedTileNumber;
        int spaceBetweenTiles { 10 };
        
        bool isSettable { true };
        
        Array<GridTileComponent*> tiles;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GridColumnComponent)
    };
    
    
    
    int numRows, numColumns;
    int spaceBetweenTiles { 10 };
    Array<GridColumnComponent*> columns;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GridDisplayComponent)
    
};
