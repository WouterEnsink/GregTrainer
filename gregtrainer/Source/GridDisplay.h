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
    
    GridDisplay(Point<int> size) : size(size)
    {
        const auto [width, height] = size;
        
        gridTiles.resize(width);
        
        for (auto& row : gridTiles) row.resize(height);
        
        for (auto& column : gridTiles)
            for (auto& tile : column)
            {
                tile = new GridTile({10, 10});
                addAndMakeVisible(tile);
            }
    }
    
    ~GridDisplay()
    {
        //to avoid undefined behavior when the GridDisplay class is destroyed and the Component
        //base class tries to access the already destroyed tiles
        removeAllChildren();
        
        for(auto& column: gridTiles)
            for(auto* tile: column) delete tile;
    }
    
    void paint(Graphics& g) override
    {
        g.fillAll(Colours::grey);
    }
    
    void resized() override
    {
        const auto bounds = getLocalBounds().reduced(20, 20);
        
        auto x = bounds.getX();
        auto y = bounds.getY();
        auto w = bounds.getWidth()  - x;
        auto h = bounds.getHeight() - y;
        
        auto [numColumns, numRows] = size;
        
        auto tileWidth = w / numColumns;
        auto tileHeight = h / numRows;
        
        for(int i = 0; i < numRows; ++i)
            for (int j = 0; j < numColumns; ++j)
                gridTiles[i][j]->setBounds({ i * tileWidth, j * tileHeight, tileWidth - 10, tileHeight -10});
       
    }
    
    
private:
    
    class GridTile : public Component
    {
    public:
        GridTile(const Point<int>& size)
        {
            setSize(size.getX(), size.getY());
        }
        
        void mouseDown(const MouseEvent& e) override
        {
            tileOn  = tileOn ? false : true;
            repaint();
        }
        
        void paint(Graphics& g) override
        {
            tileOn ? g.fillAll(tileOnColour) : g.fillAll(tileOffColour);
        }
        
        void resized() override { }
        
        bool isTileOn() const noexcept { return tileOn; }
        
        void setTile(bool on) noexcept
        {
            tileOn = on;
            repaint();
        }
        
    private:
        
        bool tileOn { false };
        Colour tileOnColour { Colours::white };
        Colour tileOffColour { Colours::black };
    };
    
    Array<Array<GridTile*>> gridTiles;
    
    Point<int> size;
    
    
};
