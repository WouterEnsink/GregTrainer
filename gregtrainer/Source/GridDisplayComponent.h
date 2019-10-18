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
    
    GridDisplayComponent(int numRows, int numColumns, const Array<String>& rowsText) :
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
    }
    
    ~GridDisplayComponent()
    {
        removeAllChildren();
        
        for(auto* column : columns) delete column;
    }
    
    void paint(Graphics& g) override
    {
        g.setColour(Colours::lightslategrey);
        g.fillRoundedRectangle(getLocalBounds().toFloat(), 10.f);
    }
    
    void resized() override
    {
        const auto bounds = getLocalBounds().reduced(20, 20);
        
        const auto x = bounds.getX();
        const auto y = bounds.getY();
        const auto w = bounds.getWidth();
        const auto h = bounds.getHeight();
        
        const auto columnWidth = w / numColumns;
        
        for(int i = 0; i < numColumns; ++i)
            columns[i]->setBounds(x + i * columnWidth, y, columnWidth, h);
    }
    
    
private:
    

    class GridTileComponent     : public Component
    {
    public:
        GridTileComponent(const std::function<void(GridTileComponent*)>& callback): tilePressedCallback(callback)
        {
    
        }
        
        void mouseDown(const MouseEvent& e) override
        {
            tileOn = tileOn ? false : true;
            tilePressedCallback(this);
            repaint();
        }
        
        void paint(Graphics& g) override
        {
            tileOn ? g.setColour(tileOnColour) : g.setColour(tileOffColour);
            
            auto floatBounds = getLocalBounds().toFloat();
            
            g.fillRoundedRectangle(floatBounds, roundness);
            
            g.setColour(tileOnColour);
            g.drawRoundedRectangle(floatBounds, roundness, 2.f);
            
            tileOn ? g.setColour(tileOffColour) : g.setColour(tileOnColour);
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

        
    private:
        
        //to notify its parent column that the tile has been pressed
        std::function<void(GridTileComponent*)> tilePressedCallback;
        
        Font noteFont { "Arial", 30.f, Font::plain };
        
        float roundness { 10.f };
        
        String tileText { "A" };
        
        bool tileOn { false };
        Colour tileOnColour { Colours::gainsboro };
        Colour tileOffColour { Colours::lightslategrey };
        
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
            const auto bounds = getLocalBounds().reduced(5);
            const auto x = bounds.getX();
            const auto y = bounds.getY();
            const auto w = bounds.getWidth();
            const auto h = bounds.getHeight();
            
            const auto tileHeight = h / numTiles;
            
            for(auto i = 0; i < numTiles; ++i)
                tiles[i]->setBounds(x, i * tileHeight + y, w, tileHeight - 5);
        }
        
        void setTileTexts(const Array<String>& texts) noexcept
        {
            jassert(texts.size() == tiles.size());
            
            for(int i = 0; i < numTiles; ++i) tiles[i]->setText(texts[i]);
            repaint();
        }
        
    private:
        

        int numTiles;
        int activatedTileNumber;
        
        Array<GridTileComponent*> tiles;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GridColumnComponent)
    };
    
    
    
    int numRows, numColumns;
    
    Array<GridColumnComponent*> columns;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GridDisplayComponent)
    
};
