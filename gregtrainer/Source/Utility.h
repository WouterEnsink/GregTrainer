/*
  ==============================================================================

    Utility.h
    Created: 16 Oct 2019 10:33:52pm
    Author:  Wouter Ensink

  ==============================================================================
*/

#pragma once


template<typename... T>
auto print(T... args){
    ([](auto arg){ std::cout << arg << " "; }(args), ...);
    std::cout << '\n';
}
