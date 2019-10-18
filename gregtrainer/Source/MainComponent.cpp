

#include "MainComponent.h"
#include "GridDisplayComponent.h"

//==============================================================================
MainComponent::MainComponent() : gridDisplay(8, 10, { "C", "B", "A", "G", "F", "E", "D", "C" })
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
    addAndMakeVisible(gridDisplay);

    
    playButton.onClick = [this]{
        print("start playing button");
        audioSource.startPlaying(500, 400, {60, 62, 64, 65, 67, 69, 71, 72});
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

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll (Colours::lightslategrey);
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds();
    auto r = Rectangle { 100, 50, 200, 50 };
    
    playButton.setBounds(r);
    gridDisplay.setBounds(getLocalBounds().reduced(50, 100));
    
}
