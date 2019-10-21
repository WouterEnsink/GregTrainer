

#include "MainComponent.h"
#include "GridDisplayComponent.h"


class InfoComponent : public Component
{
    void paint(Graphics& g) override
    {
        g.fillAll(Colours::lightslategrey);
        g.setColour(Colours::black);
        g.setFont(30.f);
        
        g.drawText("Creator: Wouter Ensink",
                   getLocalBounds().toFloat().withTrimmedBottom(getHeight()/2),
                   Justification::centred);
        
        g.setFont(25.f);
        
        auto bounds = getLocalBounds().toFloat();
        
        auto textBounds = bounds.withTrimmedTop(getHeight()/3).withTrimmedBottom(getHeight()/3);
        
        g.drawText("send bugs or suggestions to:",
                   textBounds.translated(0, getHeight()/6),
                   Justification::horizontallyCentred);
        
        g.drawText("wouter.ensink@student.hku.nl",
                   textBounds.translated(0, getHeight()/3),
                   Justification::horizontallyCentred);
    }
    
    void resized() override
    {
        
    }
};


//==============================================================================
MainComponent::MainComponent() : gridDisplay(10, 8, { "C", "B", "A", "G", "F", "E", "D", "C" })
{
    setSize (800, 600);

    initializeAudioSettings();
    
    visitComponents({ &playButton, &gridDisplay, &generateButton, &submitButton, &answerLabel, &infoButton },
                    [this](Component& c){ addAndMakeVisible(c); });
    
    playButton.onClick = [this]{
        audioSource.startPlaying(melody.noteLength, melody.timeBetweenNotes, melody.midiNotes);
        playButton.setButtonText("Play Again");
    };
    
    generateButton.onClick = [this]{
        melody = melodyGenerator.generateMelody(10);
        gridDisplay.turnAllTilesOff();
        gridDisplay.setTile(gridDisplay.getNumColumns()-1, gridDisplay.getNumRows() - melody.normalizedGroundNoteIndex - 1, true);
        playButton.setButtonText("Start Playing");
    };
    
    submitButton.onClick = [this]{
        answerLabel.setText("You Suck", NotificationType::dontSendNotification);
    };
    
    infoButton.onClick = [this]{
        auto* infoPanel = new InfoComponent();
        infoPanel->setSize(400, 200);
        CallOutBox::launchAsynchronously(infoPanel, infoButton.getScreenBounds(), nullptr);
        
    };
    
    answerLabel.setText("Answer will be shown here!", NotificationType::dontSendNotification);
    answerLabel.setJustificationType(Justification::centred);
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
    infoButton.setBounds(10, 10, 25, 25);
    
}
