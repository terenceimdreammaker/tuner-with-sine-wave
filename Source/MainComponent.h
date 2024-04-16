/*
  
Tuner With Sine Wave is published under the pseudonym: Terence Im.
The copyright notice below is under the software developer's legal name.

Copyright 2024 Tae Joon Kim

This file is part of Tuner With Sine Wave.

Tuner With Sine Wave is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Tuner With Sine Wave is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Tuner With Sine Wave. If not, see <https://www.gnu.org/licenses/>.

Tuner With Sine Wave was developed using the JUCE framework.

*/

#pragma once

#include <JuceHeader.h>
#include <map>


class MainComponent : public juce::AudioAppComponent, public juce::Button::Listener
{

public:

    MainComponent()
    {
        addAndMakeVisible(appTitleText);

        appTitleText.setText("Tuner with Sine Wave", juce::dontSendNotification);
        appTitleText.setFont(juce::Font(52.0f));
        appTitleText.setColour(juce::Label::textColourId, juce::Colour::fromString("ff#3A8989"));
        appTitleText.setJustificationType(juce::Justification::centred);

        addAndMakeVisible(selectNoteText);

        selectNoteText.setText("Select a note", juce::dontSendNotification);
        selectNoteText.setFont(juce::Font(37.0f));
        selectNoteText.setColour(juce::Label::textColourId, juce::Colour::fromString("ff#3A8989"));
        selectNoteText.setJustificationType(juce::Justification::centred);

        addAndMakeVisible(notesMenu);

        //octave 3 choices
        notesMenu.addItem("C3", 1);
        notesMenu.addItem("C#3", 2);
        notesMenu.addItem("D3", 3);
        notesMenu.addItem("D#3", 4);
        notesMenu.addItem("E3", 5);
        notesMenu.addItem("F3", 6);
        notesMenu.addItem("F#3", 7);
        notesMenu.addItem("G3", 8);
        notesMenu.addItem("G#3", 9);
        notesMenu.addItem("A3", 10);
        notesMenu.addItem("A#3", 11);
        notesMenu.addItem("B3", 12);


        //octave 4 choices
        notesMenu.addItem("C4", 13);
        notesMenu.addItem("C#4", 14);
        notesMenu.addItem("D4", 15);
        notesMenu.addItem("D#4", 16);
        notesMenu.addItem("E4", 17);
        notesMenu.addItem("F4", 18);
        notesMenu.addItem("F#4", 19);
        notesMenu.addItem("G4", 20);
        notesMenu.addItem("G#4", 21);
        notesMenu.addItem("A4", 22);
        notesMenu.addItem("A#4", 23);
        notesMenu.addItem("B4", 24);


        //octave 5 choices
        notesMenu.addItem("C5", 25);
        notesMenu.addItem("C#5", 26);
        notesMenu.addItem("D5", 27);
        notesMenu.addItem("D#5", 28);
        notesMenu.addItem("E5", 29);
        notesMenu.addItem("F5", 30);
        notesMenu.addItem("F#5", 31);
        notesMenu.addItem("G5", 32);
        notesMenu.addItem("G#5", 33);
        notesMenu.addItem("A5", 34);
        notesMenu.addItem("A#5", 35);
        notesMenu.addItem("B5", 36);

        
        //octave 6 choices
        notesMenu.addItem("C6", 37);
        notesMenu.addItem("C#6", 38);
        notesMenu.addItem("D6", 39);
        notesMenu.addItem("D#6", 40);
        notesMenu.addItem("E6", 41);
        notesMenu.addItem("F6", 42);
        notesMenu.addItem("F#6", 43);
        notesMenu.addItem("G6", 44);
        notesMenu.addItem("G#6", 45);
        notesMenu.addItem("A6", 46);
        notesMenu.addItem("A#6", 47);
        notesMenu.addItem("B6", 48);

        notesMenu.setSelectedId(13);

        notesMenu.setColour(juce::ComboBox::backgroundColourId, juce::Colour::fromString("ff#C8E6C9"));

        notesMenu.onChange = [this] { noteSelected(); };

        addAndMakeVisible(levelSlider);
        levelSlider.setRange(0.0, 0.125);
        levelSlider.setValue((double)currentLevel, juce::dontSendNotification);
        levelSlider.onValueChange = [this] { 
            
            targetLevel = (float)levelSlider.getValue(); 
            sineIsPlaying = true;
            muteButton.setButtonText("Mute");
            
            };

        levelSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
               
        addAndMakeVisible(muteButton);

        muteButton.setButtonText("Mute");
        muteButton.addListener(this);

        addAndMakeVisible(creditsText);

        creditsText.setText("Developed by Terence Im", juce::dontSendNotification);
        creditsText.setFont(juce::Font(32.0f));
        creditsText.setColour(juce::Label::textColourId, juce::Colour::fromString("#ff3A8989"));
        creditsText.setJustificationType(juce::Justification::centred);
          
        setSize(600, 600);
        setAudioChannels(0, 2);
    }

    ~MainComponent() override
    {
        shutdownAudio();
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colour::fromString("ff#FFF3BF"));
    }

    void resized() override
    {   
        juce::FlexBox flexBox;
        flexBox.flexDirection = juce::FlexBox::Direction::column;

        flexBox.items.add(juce::FlexItem(appTitleText).withFlex(1).withWidth(500).withHeight(40).withMargin({ 0, 0, 0, 0 }));
        flexBox.items.add(juce::FlexItem(selectNoteText).withFlex(1).withWidth(400).withHeight(40).withMargin({ 0, 0, 0, 0 }));
        flexBox.items.add(juce::FlexItem(notesMenu).withFlex(1).withWidth(400).withHeight(1).withMargin({ 0, 0, 0, 0 }));
        flexBox.items.add(juce::FlexItem(levelSlider).withFlex(1).withWidth(418).withHeight(40).withMargin({ 0, 0, 0, 0 }));
        flexBox.items.add(juce::FlexItem(muteButton).withFlex(1).withWidth(80).withHeight(1).withMargin({ 0, 0, 0, 0 }));
        flexBox.items.add(juce::FlexItem(creditsText).withFlex(1).withWidth(400).withHeight(40).withMargin({ 0, 0, 0, 0 }));

        flexBox.alignContent = juce::FlexBox::AlignContent::center;
        flexBox.alignItems = juce::FlexBox::AlignItems::center;
        flexBox.justifyContent = juce::FlexBox::JustifyContent::center;

        flexBox.performLayout(getLocalBounds());
    }

    inline void updateAngleDelta()
    {
        auto cyclesPerSample = currentFrequency / currentSampleRate;
        angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
    }

    void prepareToPlay(int, double sampleRate) override
    {
        currentSampleRate = sampleRate;
        updateAngleDelta();
    }

    void releaseResources() override {}

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override {
        auto* leftBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
        auto* rightBuffer = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
        
        auto localTargetFrequency = targetFrequency;

        if (!juce::approximatelyEqual(targetFrequency, currentFrequency))
        {
            auto frequencyIncrement = (targetFrequency - currentFrequency) / bufferToFill.numSamples;
            
            for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
            {
                auto currentSample = (float)std::sin(currentAngle);
                currentFrequency += frequencyIncrement;
                updateAngleDelta();
                currentAngle += angleDelta;
                leftBuffer[sample] = currentSample;
                rightBuffer[sample] = currentSample;
            }

            currentFrequency = localTargetFrequency;

        }
        else
        {
            for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
            {
                auto currentSample = (float)std::sin(currentAngle);
                currentAngle += angleDelta;
                leftBuffer[sample] = currentSample;
                rightBuffer[sample] = currentSample;
            }
        }

        auto localTargetLevel = targetLevel;
        bufferToFill.buffer->applyGainRamp(bufferToFill.startSample, bufferToFill.numSamples, currentLevel, localTargetLevel);
        currentLevel = localTargetLevel;

    }

    void buttonClicked(juce::Button* button) override
    {
        if (button == &muteButton) 
        {
            if (sineIsPlaying == true)
            {
                targetLevel = 0.0f;
                sineIsPlaying = false;
                muteButton.setButtonText("Play");
            }
            else if (sineIsPlaying == false) {
                targetLevel = levelSlider.getValue();
                sineIsPlaying = true;
                muteButton.setButtonText("Mute");
               
            }
            }
    }

private:
    
    void noteSelected()
    {
        switch (notesMenu.getSelectedId())
        {
            //octave three
            case 1: targetFrequency = 130.81; break;
            case 2: targetFrequency = 138.59; break;
            case 3: targetFrequency = 146.83; break;
            case 4: targetFrequency = 155.56; break;
            case 5: targetFrequency = 164.81; break;
            case 6: targetFrequency = 174.61; break;
            case 7: targetFrequency = 185.00; break;
            case 8: targetFrequency = 196.00; break;
            case 9: targetFrequency = 207.65; break;
            case 10: targetFrequency = 220.00; break;
            case 11: targetFrequency = 233.08; break;
            case 12: targetFrequency = 246.94; break;
            
                
            //octave four
            case 13: targetFrequency = 261.63; break;
            case 14: targetFrequency = 277.18; break;
            case 15: targetFrequency = 293.67; break;
            case 16: targetFrequency = 311.13; break;
            case 17: targetFrequency = 329.63; break;
            case 18: targetFrequency = 349.23; break;
            case 19: targetFrequency = 369.99; break;
            case 20: targetFrequency = 392.00; break;
            case 21: targetFrequency = 415.30; break;
            case 22: targetFrequency = 440.00; break;
            case 23: targetFrequency = 466.16; break;
            case 24: targetFrequency = 493.88; break;

            //octave five
            case 25: targetFrequency = 523.25; break;
            case 26: targetFrequency = 554.37; break;
            case 27: targetFrequency = 587.33; break;
            case 28: targetFrequency = 622.25; break;
            case 29: targetFrequency = 659.26; break;
            case 30: targetFrequency = 698.46; break;
            case 31: targetFrequency = 739.99; break;
            case 32: targetFrequency = 783.99; break;
            case 33: targetFrequency = 830.61; break;
            case 34: targetFrequency = 880.00; break;
            case 35: targetFrequency = 932.33; break;
            case 36: targetFrequency = 987.77; break;
            
            //octave six
            case 37: targetFrequency = 1046.5; break;
            case 38: targetFrequency = 1108.7; break;
            case 39: targetFrequency = 1174.7; break;
            case 40: targetFrequency = 1244.5; break;
            case 41: targetFrequency = 1318.5; break;
            case 42: targetFrequency = 1396.9; break;
            case 43: targetFrequency = 1480.0; break;
            case 44: targetFrequency = 1568.0; break;
            case 45: targetFrequency = 1661.2; break;
            case 46: targetFrequency = 1760.0; break;
            case 47: targetFrequency = 1864.7; break;
            case 48: targetFrequency = 1975.5; break;

            default: break;
        }
    }
    
    juce::Label appTitleText;
    juce::Label selectNoteText;
    juce::ComboBox notesMenu;
    juce::Slider levelSlider;
    juce::TextButton muteButton;
    juce::Label creditsText;

    double currentSampleRate = 0.0, currentAngle = 0.0, angleDelta = 0.0;
    double currentFrequency = 261.63, targetFrequency = 261.63;
    float currentLevel = 0.1f, targetLevel = 0.1f;
    bool sineIsPlaying = true;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};


