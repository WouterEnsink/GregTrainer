/*
  ==============================================================================

    Utility.h
    Created: 16 Oct 2019 10:33:52pm
    Author:  Wouter Ensink

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

template<typename... T>
auto print(T&&... args) noexcept
{
    ([](auto&& arg){ std::cout << arg << " "; } (args), ...);
    std::cout << '\n';
}

template<typename Function>
auto visitComponents(const Array<Component*>& components, const Function& f) noexcept
{
    for(auto* c : components) f(*c);
}

template<typename T>
auto getRectangleDimentions(const Rectangle<T>& r) noexcept
{
    return std::tuple<T, T, T, T> { r.getX(), r.getY(), r.getWidth(), r.getHeight() };
}
