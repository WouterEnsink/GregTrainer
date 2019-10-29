

#include "MainComponent.h"
#include "Identifiers.h"


//===============================================================================================


MainComponent::MainComponent (ValueTree& t) :
                                                tree (t),
                                                gridDisplay (tree, 8, 8, { "C", "B", "A", "G", "F", "E", "D", "C" },
                                                             { 12, 11, 9, 7, 5, 4, 2, 0 }),
                                                trainerEngine (tree, 8)
{
    setSize (800, 600);

    initializeAudioSettings();
    
    visitComponents({
        &playButton,
        &gridDisplay,
        &generateButton,
        &submitButton,
        &answerLabel,
        &infoButton,
        &colourPickButton
    }, [this] (Component& c) { addAndMakeVisible (c); });
    
    
    playButton.onClick = [this]()
    {
        trainerEngine.startPlayingMelody();
        playButton.setButtonText ("Play Again");
    };
    
    generateButton.onClick = [this]()
    {
        gridDisplay.turnAllTilesOff();
        Random rand;
        gridDisplay.setStateForTile (gridDisplay.getNumColumns()-1,
                                     rand.nextInt (gridDisplay.getNumRows() - 1),
                                     GridDisplayComponent::TileState::tileActive);
        
        gridDisplay.setSetabilityColumn (gridDisplay.getNumColumns() - 1, false);
        
        trainerEngine.generateNextMelody();
        playButton.setButtonText ("Start Playing");
        
        if (auto engine = tree.getChildWithName (IDs::Engine::EngineRoot); engine.isValid())
        {
            engine.setProperty (IDs::Engine::PlayState, true, nullptr);
        }
        
        
    };
    
    
    submitButton.onClick = [this]
    {
        answerLabel.setText ("You Suck", NotificationType::dontSendNotification);
    };
    
    infoButton.onClick = [this]()
    {
        auto* infoPanel = new InfoPanelComponent();
        infoPanel->setSize (400, 200);
        CallOutBox::launchAsynchronously (infoPanel, infoButton.getScreenBounds(), nullptr);
    };
    
    colourPickButton.onClick = [this]()
    {
        if (colourPicker == nullptr)
            colourPicker.reset (new ColourPickerWindow(tree, [this]() { colourPicker = nullptr; }));
    };
    
    
    answerLabel.setText ("Answer will be shown here!", NotificationType::dontSendNotification);
    answerLabel.setJustificationType (Justification::centred);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

//===============================================================================================


void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    trainerEngine.prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    trainerEngine.getNextAudioBlock (bufferToFill);
}

void MainComponent::releaseResources()
{
    trainerEngine.releaseResources();
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

//===============================================================================================

void MainComponent::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}


void MainComponent::resized()
{
    setSize (800, 600); // makes window nonresizable
    
    auto r = Rectangle { 50, 25, 200, 50 };
    
    // set bounds for buttons with even spacing
    visitComponents({ &playButton, &generateButton, &submitButton },
                    [&r] (auto& c) { c.setBounds(r); r.translate(250, 0); });
    
    gridDisplay.setBounds ({ 52, 100, 696, 320 });
    
    answerLabel.setBounds (100, 500, 600, 100);
    infoButton.setBounds (10, 10, 25, 25);
    
    colourPickButton.setBounds (50, 450, 200, 50);
    
}


