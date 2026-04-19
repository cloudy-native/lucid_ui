#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "FlexBox.h"
#include "CustomKnobLookAndFeel.h"
#include "UIConstants.h"
#include "UIColors.h"

namespace ui
{

class EmitterCard : public juce::Component
{
public:
    EmitterCard(int colorIndex = 0);
    ~EmitterCard() override = default;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    // Parameter accessors
    void setActivity(float value);
    void setPitchDrift(float value);
    void setVolume(float value);
    void setLFOEnabled(bool enabled);
    void setLFORate(float rate);
    void setLFODepth(float depth);
    void setLFOWaveShape(int shapeIndex);
    void setLFOTarget(int targetIndex);
    
    float getActivity() const { return static_cast<float>(activitySlider.getValue()); }
    float getPitchDrift() const { return static_cast<float>(pitchDriftSlider.getValue()); }
    float getVolume() const { return static_cast<float>(volumeSlider.getValue()); }
    bool getLFOEnabled() const { return lfoEnableButton.getToggleState(); }
    float getLFORate() const { return static_cast<float>(lfoRateSlider.getValue()); }
    float getLFODepth() const { return static_cast<float>(lfoDepthSlider.getValue()); }
    int getLFOWaveShape() const { return lfoWaveShapeCombo.getSelectedItemIndex(); }
    int getLFOTarget() const { return lfoTargetCombo.getSelectedItemIndex(); }
    
    // Callbacks
    std::function<void(float)> onActivityChanged;
    std::function<void(float)> onPitchDriftChanged;
    std::function<void(float)> onVolumeChanged;
    std::function<void(bool)> onLFOEnabledChanged;
    std::function<void(float)> onLFORateChanged;
    std::function<void(float)> onLFODepthChanged;
    std::function<void(int)> onLFOWaveShapeChanged;
    std::function<void(int)> onLFOTargetChanged;
    std::function<void(const juce::String&)> onSoundPackChanged;
    
    void setEmitterName(const juce::String& name);
    void setSoundPackList(const juce::StringArray& packs);
    void setSelectedSoundPack(const juce::String& packName);
    void updateCountdown(double secondsUntilNext, bool isPlaying);
    void updateControlsEnabled();
    
    std::function<void()> onRemoveClicked;
    
private:
    // Soundpack selector (left side)
    juce::Label soundpackLabel;
    juce::ComboBox soundpackCombo;
    juce::Label playingLabel;    // Shows "▶ 23s" when playing
    juce::Label countdownLabel;  // Shows "Next: 1:47"
    juce::TextButton closeButton;  // Remove emitter button
    
    // Playback progress
    float playbackProgress = 0.0f;  // 0.0 to 1.0
    bool isCurrentlyPlaying = false;
    
    // Main parameter controls
    juce::Label activityLabel;
    juce::Slider activitySlider;
    
    juce::Label pitchDriftLabel;
    juce::Slider pitchDriftSlider;
    
    juce::Label volumeLabel;
    juce::Slider volumeSlider;
    
    // LFO controls
    juce::ToggleButton lfoEnableButton;
    juce::Label lfoRateLabel;
    juce::Slider lfoRateSlider;
    juce::Label lfoDepthLabel;
    juce::Slider lfoDepthSlider;
    juce::Label lfoWaveShapeLabel;
    juce::ComboBox lfoWaveShapeCombo;
    juce::Label lfoTargetLabel;
    juce::ComboBox lfoTargetCombo;
    
    FlexBox mainLayout;
    FlexBox controlsLayout;
    
    void setupSlider(juce::Slider& slider, juce::Label& label, const juce::String& labelText,
                     double min, double max, double defaultValue, CustomKnobLookAndFeel* lookAndFeel);
    
    void updateLFOControlsEnabled();
    
    // Separate LookAndFeel for each dial with different jewel tones
    CustomKnobLookAndFeel activityLookAndFeel;
    CustomKnobLookAndFeel pitchDriftLookAndFeel;
    CustomKnobLookAndFeel volumeLookAndFeel;
    CustomKnobLookAndFeel lfoRateLookAndFeel;
    CustomKnobLookAndFeel lfoDepthLookAndFeel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EmitterCard)
};

} // namespace ui
