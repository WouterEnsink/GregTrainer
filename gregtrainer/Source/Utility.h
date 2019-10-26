/*
  ==============================================================================

    Utility.h
    Created: 16 Oct 2019 10:33:52pm
    Author:  Wouter Ensink

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//========================================================================================
// Some Utitlity functions and classes to make things a little cleaner and more confinient


template<typename... T>
auto print(T&&... args) noexcept
{
    ([](auto&& arg){ std::cout << arg << " "; } (args), ...);
    std::cout << '\n';
}

//========================================================================================
// Enables you to do an action to multiple components at once

template<typename Function>
auto visitComponents(const Array<Component*>& components, const Function& f) noexcept
{
    for(auto* c : components) f(*c);
}

//========================================================================================
// Combined with structured bindings this provides a much cleaner and quicker
// way of getting x, y, w, h from some bounds for example


template<typename T>
auto getRectangleDimentions(const Rectangle<T>& r) noexcept
{
    return std::tuple<T, T, T, T> { r.getX(), r.getY(), r.getWidth(), r.getHeight() };
}

//========================================================================================
// Swift like Property with a getter and setter that can compute
// both the internal value aswel as the value of another variable
// when assigned a new value


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


//========================================================================================
// So we don't have to implement all of these methods everytime we inherent from ValueTree::Listener

class TreeListener : public ValueTree::Listener
{
public:
    void valueTreePropertyChanged(ValueTree&, const Identifier&) override { }
    void valueTreeChildAdded(ValueTree&, ValueTree&) override { }
    void valueTreeChildRemoved(ValueTree&, ValueTree&, int) override { }
    void valueTreeChildOrderChanged(ValueTree&, int, int) override { }
    void valueTreeParentChanged(ValueTree&) override { }
    void valueTreeRedirected(ValueTree&) override { }
};
