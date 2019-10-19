

#include "MainComponent.h"
#include "GridDisplayComponent.h"

//==============================================================================
MainComponent::MainComponent() : gridDisplay(10, 8, { "C", "B", "A", "G", "F", "E", "D", "C" })
{
    setSize (800, 600);

    initializeAudioSettings();
    
    visitComponents({ &playButton, &gridDisplay }, [this](Component& c){ addAndMakeVisible(c); });
    
    playButton.onClick = [this]{
        //audioSource.startPlaying(300, 200, {60, 62, 60, 65, 67, 60, 71, 72, 74, 77});
        gridDisplay.setTile(0, 0, true);
    };
    
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    audioSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    audioSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    audioSource.releaseResources();
}

void MainComponent::initializeAudioSettings()
{
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
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll (Colours::lightslategrey);
}

void MainComponent::resized()
{
    
    auto r = Rectangle { 100, 50, 200, 50 };
    
    playButton.setBounds(r);
    gridDisplay.setBounds(getLocalBounds().reduced(50, 100));
    
}
