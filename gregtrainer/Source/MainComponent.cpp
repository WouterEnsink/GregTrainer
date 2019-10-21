

#include "MainComponent.h"
#include "GridDisplayComponent.h"

//==============================================================================
MainComponent::MainComponent() : gridDisplay(10, 8, { "C", "B", "A", "G", "F", "E", "D", "C" })
{
    setSize (800, 600);

    initializeAudioSettings();
    
    visitComponents({ &playButton, &gridDisplay, &generateButton, &submitButton, &answerLabel },
                    [this](Component& c){ addAndMakeVisible(c); });
    
    playButton.onClick = [this]{
        audioSource.startPlaying(melody.noteLength, melody.timeBetweenNotes, melody.midiNotes);
    };
    
    generateButton.onClick = [this]{
        melody = melodyGenerator.generateMelody(10);
    };
    
    submitButton.onClick = []{
        Random r;
        print(r.nextInt(7));
    };
    
    answerLabel.setText("Answer will be shown here!", NotificationType::dontSendNotification);
    answerLabel.setJustificationType(Justification::centred);
    answerLabel.setBorderSize(BorderSize<int>{0});
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
    setSize(800, 600); //makes window nonresizable
    
    auto r = Rectangle { 50, 25, 200, 50 };
    
    //set bounds for buttons with even spacing
    visitComponents({ &playButton, &generateButton, &submitButton },
                    [&r](auto& c) { c.setBounds(r); r.translate(250, 0); });
    
    gridDisplay.setBounds(getLocalBounds().reduced(50, 100));
    
    answerLabel.setBounds(100, 500, 600, 100);
    
}
