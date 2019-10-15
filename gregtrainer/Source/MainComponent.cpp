/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "GridDisplay.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (800, 600);

    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }
    else
    {
        setAudioChannels (2, 2);
    }
    
    addAndMakeVisible(playButton);
    midiGenerator.setTimeBetweenNotes(200);
    midiGenerator.setNotes({60, 62, 64, 65, 67, 69, 71, 72});
    playButton.onClick = [this]{
        midiGenerator.startPlaying();
    };
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    //test if midi fetching works
    auto buffer = midiGenerator.getNextMidiBuffer();
    if (auto numEvents = buffer.getNumEvents() != 0)
        std::cout << "buffer.numEvents: " << buffer.getNumEvents() << "\n";
    
    bufferToFill.clearActiveBufferRegion();
    
    
}

void MainComponent::releaseResources()
{
  
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    playButton.setBounds(getLocalBounds().removeFromLeft(200));
}
