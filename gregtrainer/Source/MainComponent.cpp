

#include "MainComponent.h"
#include "GridDisplay.h"

//==============================================================================
MainComponent::MainComponent() : gridDisplay({5, 5})
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
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    playButton.setBounds(getLocalBounds().removeFromLeft(200));
    gridDisplay.setBounds(getLocalBounds());
}
