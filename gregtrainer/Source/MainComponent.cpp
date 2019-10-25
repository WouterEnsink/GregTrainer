

#include "MainComponent.h"
#include "Identifiers.h"

/** compares two melodies, should probably be put in a class that also handles giving visual feedback
 ** to the user
 */

void compareRelativeMelodies(const Melody& actualMelody, const Melody& gridMelody)
{
    if (actualMelody.relativeNotes.size() != gridMelody.relativeNotes.size())
        print("melodies not the same length");
    
    for (int i = 0; i < actualMelody.numNotes; ++i)
    {
        if (actualMelody.relativeNotes[i] != gridMelody.relativeNotes[i])
        {
            print("note", i, "not the same:", "actual note:", actualMelody.relativeNotes[i],
                  "grid note:", gridMelody.relativeNotes[i]);
        }
        else print("note ", i, "is the same");
    }
}


//==============================================================================


MainComponent::MainComponent(ValueTree& t) :
    tree(t), gridDisplay(tree, 8, 8, { "C", "B", "A", "G", "F", "E", "D", "C" }), trainerEngine(tree)
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
    }, [this](Component& c){ addAndMakeVisible(c); });
    
    
    playButton.onClick = [this]{
        //audioSource.startPlaying(melody.noteLength, melody.timeBetweenNotes, melody.midiNotes);
        trainerEngine.startPlayingMelody();
        playButton.setButtonText("Play Again");
    };
    
    generateButton.onClick = [this]{
       // melody = melodyGenerator.generateMelody(8);
        gridDisplay.turnAllTilesOff();
      //  gridDisplay.setStateForTile(gridDisplay.getNumColumns()-1,
      //                              gridDisplay.getNumRows() - melody.normalizedGroundNoteIndex - 1,
      //                              GridDisplayComponent::TileState::tileActive);
        trainerEngine.generateNextMelody();
        playButton.setButtonText("Start Playing");
        
        
    };
    
    submitButton.onClick = [this]{
        answerLabel.setText("You Suck", NotificationType::dontSendNotification);
       // compareRelativeMelodies(melody, gridDisplay.getGridStateAsMelody());
    };
    
    infoButton.onClick = [this]{
        auto* infoPanel = new InfoPanelComponent();
        infoPanel->setSize(400, 200);
        CallOutBox::launchAsynchronously(infoPanel, infoButton.getScreenBounds(), nullptr);
    };
    
    colourPickButton.onClick = [this]{
        if(!colourPicker) colourPicker.reset(new ColourPickerWindow(tree, [this]{ colourPicker = nullptr; }));
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
    trainerEngine.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    trainerEngine.getNextAudioBlock(bufferToFill);
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

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

void MainComponent::resized()
{
    setSize(800, 600); //makes window nonresizable
    
    auto r = Rectangle { 50, 25, 200, 50 };
    
    //set bounds for buttons with even spacing
    visitComponents({ &playButton, &generateButton, &submitButton },
                    [&r](auto& c) { c.setBounds(r); r.translate(250, 0); });
    
    gridDisplay.setBounds({ 52, 100, 696, 320 });
    
    answerLabel.setBounds(100, 500, 600, 100);
    infoButton.setBounds(10, 10, 25, 25);
    
    colourPickButton.setBounds(50, 450, 200, 50);
    
}


