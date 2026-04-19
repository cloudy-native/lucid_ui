#include "EmitterCard.h"
#include <cmath>

namespace ui
{

EmitterCard::EmitterCard(int colorIndex)
    : activityLookAndFeel(UIColors::Semantic::density)  // Cyan for activity
    , pitchDriftLookAndFeel(UIColors::Semantic::pitchDrift)
    , volumeLookAndFeel(UIColors::Semantic::volume)
    , lfoRateLookAndFeel(UIColors::jewelTopaz)  // Orange for LFO rate
    , lfoDepthLookAndFeel(UIColors::jewelRuby)  // Red for LFO depth
{
    // colorIndex parameter kept for future use but not used for colors anymore
    
    // Setup soundpack selector (left side)
    soundpackLabel.setText("Sound Pack", juce::dontSendNotification);
    soundpackLabel.setFont(juce::Font(UIConstants::tinyFontSize));
    soundpackLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(soundpackLabel);
    
    soundpackCombo.setTextWhenNothingSelected("Select...");
    soundpackCombo.setColour(juce::ComboBox::backgroundColourId, UIColors::cardBackgroundLight);
    soundpackCombo.setColour(juce::ComboBox::textColourId, UIColors::textPrimary);
    soundpackCombo.setColour(juce::ComboBox::outlineColourId, UIColors::jewelCyan.withAlpha(0.3f));
    addAndMakeVisible(soundpackCombo);
    
    playingLabel.setText("", juce::dontSendNotification);
    playingLabel.setFont(juce::Font(UIConstants::smallFontSize));
    playingLabel.setJustificationType(juce::Justification::centredLeft);
    playingLabel.setColour(juce::Label::textColourId, UIColors::jewelTopaz);
    addAndMakeVisible(playingLabel);
    
    countdownLabel.setText("--:--", juce::dontSendNotification);
    countdownLabel.setFont(juce::Font(UIConstants::smallFontSize));
    countdownLabel.setJustificationType(juce::Justification::centredRight);
    countdownLabel.setColour(juce::Label::textColourId, UIColors::jewelCyan);
    addAndMakeVisible(countdownLabel);
    
    closeButton.setButtonText("X");
    closeButton.setColour(juce::TextButton::buttonColourId, UIColors::cardBackgroundLight);
    closeButton.setColour(juce::TextButton::textColourOffId, UIColors::textSecondary);
    closeButton.setColour(juce::TextButton::textColourOnId, UIColors::jewelRuby);
    addAndMakeVisible(closeButton);
    
    // Setup main parameter sliders
    setupSlider(activitySlider, activityLabel, "Activity", 0.0, 100.0, 50.0, &activityLookAndFeel);
    setupSlider(pitchDriftSlider, pitchDriftLabel, "Pitch Drift", -24.0, 24.0, 0.0, &pitchDriftLookAndFeel);
    setupSlider(volumeSlider, volumeLabel, "Volume", -60.0, 0.0, 0.0, &volumeLookAndFeel);
    
    // Setup LFO controls
    lfoEnableButton.setButtonText("LFO");
    lfoEnableButton.setColour(juce::ToggleButton::textColourId, UIColors::textPrimary);
    lfoEnableButton.setColour(juce::ToggleButton::tickColourId, UIColors::jewelTopaz);
    addAndMakeVisible(lfoEnableButton);
    
    setupSlider(lfoRateSlider, lfoRateLabel, "Rate", 0.01, 20.0, 0.0, &lfoRateLookAndFeel);
    setupSlider(lfoDepthSlider, lfoDepthLabel, "Depth", 0.0, 100.0, 0.0, &lfoDepthLookAndFeel);
    
    lfoWaveShapeLabel.setText("Wave", juce::dontSendNotification);
    lfoWaveShapeLabel.setFont(juce::Font(UIConstants::tinyFontSize));
    lfoWaveShapeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(lfoWaveShapeLabel);
    
    lfoWaveShapeCombo.addItem("Sine", 1);
    lfoWaveShapeCombo.addItem("Triangle", 2);
    lfoWaveShapeCombo.addItem("Trapezoid", 3);
    lfoWaveShapeCombo.addItem("Ease In/Out", 4);
    lfoWaveShapeCombo.addItem("Square", 5);
    lfoWaveShapeCombo.addItem("Sawtooth", 6);
    lfoWaveShapeCombo.setSelectedId(1);
    lfoWaveShapeCombo.setColour(juce::ComboBox::backgroundColourId, UIColors::cardBackgroundLight);
    lfoWaveShapeCombo.setColour(juce::ComboBox::textColourId, UIColors::textPrimary);
    addAndMakeVisible(lfoWaveShapeCombo);
    
    lfoTargetLabel.setText("Target", juce::dontSendNotification);
    lfoTargetLabel.setFont(juce::Font(UIConstants::tinyFontSize));
    lfoTargetLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(lfoTargetLabel);
    
    lfoTargetCombo.addItem("Volume", 1);
    lfoTargetCombo.addItem("Pitch", 2);
    lfoTargetCombo.addItem("Pan", 3);
    lfoTargetCombo.addItem("Filter", 4);
    lfoTargetCombo.setSelectedId(1);
    lfoTargetCombo.setColour(juce::ComboBox::backgroundColourId, UIColors::cardBackgroundLight);
    lfoTargetCombo.setColour(juce::ComboBox::textColourId, UIColors::textPrimary);
    addAndMakeVisible(lfoTargetCombo);
    
    // Custom formatting for each slider
    activitySlider.textFromValueFunction = [](double value) {
        return juce::String(static_cast<int>(value)) + "%";
    };
    activitySlider.valueFromTextFunction = [](const juce::String& text) {
        return text.trimCharactersAtEnd("%").getDoubleValue();
    };
    
    pitchDriftSlider.textFromValueFunction = [](double value) {
        return juce::String(value, 1);
    };
    pitchDriftSlider.valueFromTextFunction = [](const juce::String& text) {
        return text.getDoubleValue();
    };
    
    volumeSlider.textFromValueFunction = [](double value) {
        if (value <= -60.0)
            return juce::String("-inf dB");
        return juce::String(value, 1) + " dB";
    };
    volumeSlider.valueFromTextFunction = [](const juce::String& text) {
        if (text.contains("inf"))
            return -60.0;
        return text.trimCharactersAtEnd(" dB").getDoubleValue();
    };
    
    // LFO Rate formatting (Hz)
    lfoRateSlider.textFromValueFunction = [](double value) {
        return juce::String(value, 2) + " Hz";
    };
    lfoRateSlider.valueFromTextFunction = [](const juce::String& text) {
        return text.trimCharactersAtEnd(" Hz").getDoubleValue();
    };
    
    // LFO Depth formatting (percentage)
    lfoDepthSlider.textFromValueFunction = [](double value) {
        return juce::String(static_cast<int>(value)) + "%";
    };
    lfoDepthSlider.valueFromTextFunction = [](const juce::String& text) {
        return text.trimCharactersAtEnd("%").getDoubleValue();
    };
    
    // Now set default values - formatting will be applied
    activitySlider.setValue(50.0);
    pitchDriftSlider.setValue(0.0);
    volumeSlider.setValue(-6.0);
    lfoRateSlider.setValue(1.0);
    lfoDepthSlider.setValue(50.0);
    
    // Force text update to apply formatting
    activitySlider.updateText();
    pitchDriftSlider.updateText();
    volumeSlider.updateText();
    lfoRateSlider.updateText();
    lfoDepthSlider.updateText();
    
    activitySlider.onValueChange = [this]()
    {
        if (onActivityChanged)
            onActivityChanged(static_cast<float>(activitySlider.getValue()) / 100.0f);
    };
    
    pitchDriftSlider.onValueChange = [this]()
    {
        if (onPitchDriftChanged)
            onPitchDriftChanged(static_cast<float>(pitchDriftSlider.getValue()));
    };
    
    volumeSlider.onValueChange = [this]()
    {
        if (onVolumeChanged)
        {
            // Convert dB to linear gain (0 dB = 1.0, -6 dB = 0.5, -60 dB = ~0.001)
            float dbValue = static_cast<float>(volumeSlider.getValue());
            float linearGain = std::pow(10.0f, dbValue / 20.0f);
            onVolumeChanged(linearGain);
        }
    };
    
    lfoEnableButton.onClick = [this]()
    {
        updateLFOControlsEnabled();
        if (onLFOEnabledChanged)
            onLFOEnabledChanged(lfoEnableButton.getToggleState());
    };
    
    lfoRateSlider.onValueChange = [this]()
    {
        if (onLFORateChanged)
            onLFORateChanged(static_cast<float>(lfoRateSlider.getValue()));
    };
    
    lfoDepthSlider.onValueChange = [this]()
    {
        if (onLFODepthChanged)
            onLFODepthChanged(static_cast<float>(lfoDepthSlider.getValue() / 100.0));
    };
    
    lfoWaveShapeCombo.onChange = [this]()
    {
        if (onLFOWaveShapeChanged)
            onLFOWaveShapeChanged(lfoWaveShapeCombo.getSelectedItemIndex());
    };
    
    lfoTargetCombo.onChange = [this]()
    {
        if (onLFOTargetChanged)
            onLFOTargetChanged(lfoTargetCombo.getSelectedItemIndex());
    };
    
    soundpackCombo.onChange = [this]()
    {
        if (onSoundPackChanged && soundpackCombo.getSelectedId() > 0)
        {
            juce::String packName = soundpackCombo.getText();
            onSoundPackChanged(packName);
        }
        updateControlsEnabled();
    };
    
    closeButton.onClick = [this]()
    {
        if (onRemoveClicked)
            onRemoveClicked();
    };
    
    // Initialize controls as disabled until soundpack is selected
    updateControlsEnabled();
    updateLFOControlsEnabled();
}

void EmitterCard::setupSlider(juce::Slider& slider, juce::Label& label, const juce::String& labelText,
                               double min, double max, double defaultValue, CustomKnobLookAndFeel* lookAndFeel)
{
    slider.setRange(min, max);
    slider.setValue(defaultValue);
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 
                           UIConstants::dialTextBoxWidth, UIConstants::dialTextBoxHeight);
    slider.setTextValueSuffix("");
    slider.setLookAndFeel(lookAndFeel);
    addAndMakeVisible(slider);
    
    label.setText(labelText, juce::dontSendNotification);
    label.setFont(juce::Font(UIConstants::tinyFontSize));
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}

void EmitterCard::paint(juce::Graphics& g)
{
    g.fillAll(UIColors::cardBackground);
    
    g.setColour(UIColors::cardBackgroundLight);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 
                          static_cast<float>(UIConstants::emitterCardBorderRadius));
    
    g.setColour(UIColors::jewelCyan.withAlpha(0.3f));
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(1.0f), 
                          static_cast<float>(UIConstants::emitterCardBorderRadius), 
                          static_cast<float>(UIConstants::emitterCardBorderWidth));
    
    // Draw progress bar when playing
    if (isCurrentlyPlaying && playbackProgress > 0.0f)
    {
        auto bounds = getLocalBounds().reduced(UIConstants::smallMargin);
        auto soundpackArea = bounds.removeFromLeft(UIConstants::emitterCardSoundpackButtonWidth);
        
        // Position below timer labels
        soundpackArea.removeFromTop(20);  // Label
        soundpackArea.removeFromTop(24);  // Combo
        soundpackArea.removeFromTop(20);  // Timer
        soundpackArea.removeFromTop(2);   // Gap
        
        auto progressArea = soundpackArea.removeFromTop(3).reduced(UIConstants::tinyMargin, 0);
        
        // Background track
        g.setColour(UIColors::cardBackground.brighter(0.1f));
        g.fillRoundedRectangle(progressArea.toFloat(), 1.5f);
        
        // Progress fill
        auto fillWidth = progressArea.getWidth() * playbackProgress;
        auto fillArea = progressArea.removeFromLeft(static_cast<int>(fillWidth));
        
        g.setColour(UIColors::jewelTopaz.withAlpha(0.8f));
        g.fillRoundedRectangle(fillArea.toFloat(), 1.5f);
    }
}

void EmitterCard::resized()
{
    auto bounds = getLocalBounds().reduced(UIConstants::smallMargin);
    
    const int dialSize = UIConstants::dialSize;
    const int labelHeight = UIConstants::dialLabelHeight;
    const int spacing = UIConstants::mediumSpacing;
    
    // Left side: Soundpack selector (label + combobox + countdown)
    auto soundpackArea = bounds.removeFromLeft(UIConstants::emitterCardSoundpackButtonWidth);
    
    // Close button in top-right corner of soundpack area
    auto closeArea = soundpackArea.removeFromTop(labelHeight);
    closeButton.setBounds(closeArea.removeFromRight(20).reduced(2));
    soundpackLabel.setBounds(closeArea);
    
    soundpackCombo.setBounds(soundpackArea.removeFromTop(24).reduced(UIConstants::tinyMargin, 0));
    
    // Timer area: playing label (left) and countdown (right)
    auto timerArea = soundpackArea.removeFromTop(20).reduced(UIConstants::tinyMargin, 2);
    playingLabel.setBounds(timerArea.removeFromLeft(timerArea.getWidth() / 2));
    countdownLabel.setBounds(timerArea);
    
    // Progress bar area (thin bar below timer)
    soundpackArea.removeFromTop(2);  // Small gap
    
    bounds.removeFromLeft(spacing);
    
    // Right side: Horizontal row of controls
    // Each control is: label on top, dial below
    auto layoutControl = [&](juce::Slider& slider, juce::Label& label) {
        auto area = bounds.removeFromLeft(dialSize);
        label.setBounds(area.removeFromTop(labelHeight));
        slider.setBounds(area.removeFromTop(dialSize));
        bounds.removeFromLeft(spacing);
    };
    
    // Main parameters
    layoutControl(activitySlider, activityLabel);
    layoutControl(pitchDriftSlider, pitchDriftLabel);
    layoutControl(volumeSlider, volumeLabel);
    
    // LFO enable toggle (align with label row, not vertically centered)
    auto lfoToggleArea = bounds.removeFromLeft(50);
    auto lfoToggleTopArea = lfoToggleArea.removeFromTop(labelHeight);
    lfoEnableButton.setBounds(lfoToggleTopArea.withSizeKeepingCentre(45, 20));
    bounds.removeFromLeft(spacing);
    
    // LFO controls
    layoutControl(lfoRateSlider, lfoRateLabel);
    layoutControl(lfoDepthSlider, lfoDepthLabel);
    
    // LFO wave shape dropdown (vertical layout)
    auto waveArea = bounds.removeFromLeft(90);
    lfoWaveShapeLabel.setBounds(waveArea.removeFromTop(labelHeight));
    lfoWaveShapeCombo.setBounds(waveArea.removeFromTop(24));
    
    // LFO target dropdown (vertical layout)
    auto targetArea = bounds.removeFromLeft(90);
    lfoTargetLabel.setBounds(targetArea.removeFromTop(labelHeight));
    lfoTargetCombo.setBounds(targetArea.removeFromTop(24));
}

void EmitterCard::setActivity(float value)
{
    // Convert from 0.0-1.0 to 0-100%
    activitySlider.setValue(value * 100.0, juce::dontSendNotification);
}

void EmitterCard::setPitchDrift(float value)
{
    pitchDriftSlider.setValue(value, juce::dontSendNotification);
}

void EmitterCard::setVolume(float value)
{
    // Convert from linear gain to dB
    float dbValue = 20.0f * std::log10(std::max(0.001f, value));
    volumeSlider.setValue(dbValue, juce::dontSendNotification);
}

void EmitterCard::setLFOEnabled(bool enabled)
{
    lfoEnableButton.setToggleState(enabled, juce::dontSendNotification);
}

void EmitterCard::setLFORate(float rate)
{
    lfoRateSlider.setValue(rate, juce::dontSendNotification);
}

void EmitterCard::setLFODepth(float depth)
{
    // Convert from 0.0-1.0 to 0-100%
    lfoDepthSlider.setValue(depth * 100.0, juce::dontSendNotification);
}

void EmitterCard::setLFOWaveShape(int shapeIndex)
{
    lfoWaveShapeCombo.setSelectedItemIndex(shapeIndex, juce::dontSendNotification);
}

void EmitterCard::setLFOTarget(int targetIndex)
{
    lfoTargetCombo.setSelectedItemIndex(targetIndex, juce::dontSendNotification);
}

void EmitterCard::updateLFOControlsEnabled()
{
    bool enabled = lfoEnableButton.getToggleState();
    
    lfoRateSlider.setEnabled(enabled);
    lfoRateLabel.setEnabled(enabled);
    lfoDepthSlider.setEnabled(enabled);
    lfoDepthLabel.setEnabled(enabled);
    lfoWaveShapeCombo.setEnabled(enabled);
    lfoWaveShapeLabel.setEnabled(enabled);
    lfoTargetCombo.setEnabled(enabled);
    lfoTargetLabel.setEnabled(enabled);
}

void EmitterCard::setEmitterName(const juce::String& name)
{
    // Name is now shown in the soundpack combo selection
}

void EmitterCard::setSoundPackList(const juce::StringArray& packs)
{
    soundpackCombo.clear();
    for (int i = 0; i < packs.size(); ++i)
    {
        soundpackCombo.addItem(packs[i], i + 1);
    }
}

void EmitterCard::setSelectedSoundPack(const juce::String& packName)
{
    for (int i = 0; i < soundpackCombo.getNumItems(); ++i)
    {
        if (soundpackCombo.getItemText(i) == packName)
        {
            soundpackCombo.setSelectedId(i + 1, juce::dontSendNotification);
            break;
        }
    }
}

void EmitterCard::updateCountdown(double secondsUntilNext, bool isPlaying)
{
    isCurrentlyPlaying = isPlaying;
    
    // Don't show countdown if no soundpack selected
    if (soundpackCombo.getSelectedId() <= 0)
    {
        playingLabel.setText("", juce::dontSendNotification);
        countdownLabel.setText("", juce::dontSendNotification);
        return;
    }
    
    // Update playing state label
    if (isPlaying)
    {
        // TODO: Get actual playback time remaining from emitter
        // For now just show playing indicator
        playingLabel.setText("▶ Playing", juce::dontSendNotification);
    }
    else
    {
        playingLabel.setText("", juce::dontSendNotification);
    }
    
    // Update countdown label
    if (secondsUntilNext < 0.0 || secondsUntilNext > 999.0)
    {
        countdownLabel.setText("Next: --:--", juce::dontSendNotification);
        return;
    }
    
    int minutes = static_cast<int>(secondsUntilNext / 60.0);
    int seconds = static_cast<int>(secondsUntilNext) % 60;
    
    juce::String timeText = juce::String::formatted("Next: %d:%02d", minutes, seconds);
    countdownLabel.setText(timeText, juce::dontSendNotification);
    
    // Trigger repaint to update progress bar
    repaint();
}

void EmitterCard::updateControlsEnabled()
{
    bool hasSoundPack = soundpackCombo.getSelectedId() > 0;
    
    // Enable/disable all parameter controls based on soundpack selection
    activitySlider.setEnabled(hasSoundPack);
    activityLabel.setEnabled(hasSoundPack);
    pitchDriftSlider.setEnabled(hasSoundPack);
    pitchDriftLabel.setEnabled(hasSoundPack);
    volumeSlider.setEnabled(hasSoundPack);
    volumeLabel.setEnabled(hasSoundPack);
    
    // LFO controls
    lfoEnableButton.setEnabled(hasSoundPack);
    
    // LFO sub-controls will be handled by updateLFOControlsEnabled
    // which checks both soundpack AND LFO enable state
    if (hasSoundPack)
    {
        updateLFOControlsEnabled();
    }
    else
    {
        // Disable all LFO controls if no soundpack
        lfoRateSlider.setEnabled(false);
        lfoRateLabel.setEnabled(false);
        lfoDepthSlider.setEnabled(false);
        lfoDepthLabel.setEnabled(false);
        lfoWaveShapeCombo.setEnabled(false);
        lfoWaveShapeLabel.setEnabled(false);
        lfoTargetCombo.setEnabled(false);
        lfoTargetLabel.setEnabled(false);
    }
}

} // namespace ui
