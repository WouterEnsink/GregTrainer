/*
  ==============================================================================

    Utility.h
    Created: 16 Oct 2019 10:33:52pm
    Author:  Wouter Ensink

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

/*
 Some Utitlity functions and classes to make things a little cleaner and more confinient
 */

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

template<typename T>
class Property
{
public:
    operator T() { return get(value); }
    void operator= (T v) { value = set(v); }
    
    std::function<T(T)> set;
    std::function<T(T)> get;
    T value;
};
