

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GridDisplayComponent.h"
#include "TrainerAudioSource.h"
#include "MelodyGenerator.h"


class MainComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    
    void initializeAudioSettings();
    
    TextButton playButton { "Start Playing" };
    TextButton generateButton { "Generate Melody" };
    TextButton submitButton { "Submit Answer" };
    
    Label answerLabel ;
    
    MelodyGenerator melodyGenerator;
    Melody melody;
    
    TrainerAudioSource audioSource;
    
    GridDisplayComponent gridDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
