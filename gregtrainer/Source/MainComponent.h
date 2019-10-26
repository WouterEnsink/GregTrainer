

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GridDisplayComponent.h"
#include "TrainerAudioSource.h"
#include "MelodyGenerator.h"
#include "TrainerEngine.h"
#include "ExtraMenus.h"

class MainComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    MainComponent (ValueTree& v);
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;
    //==============================================================================
    
    
    
private:
    //==============================================================================
    
    std::unique_ptr<ColourPickerWindow> colourPicker;
    
    void initializeAudioSettings();
    
    TextButton playButton       { "Start Playing"       };
    TextButton generateButton   { "Generate Melody"     };
    TextButton submitButton     { "Submit Answer"       };
    TextButton infoButton       { "i"                   };
    TextButton colourPickButton { "Open Colour Picker"  };
    
    Label answerLabel ;
    
    ValueTree tree;

    GridDisplayComponent gridDisplay;
    
    TrainerEngine trainerEngine;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

