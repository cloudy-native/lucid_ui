#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "UIColors.h"
#include "UIConstants.h"

namespace ui
{

class CustomKnobLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomKnobLookAndFeel(const juce::Colour& baseColor = UIColors::jewelCyan)
        : theme(baseColor)
    {
        setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
        setColour(juce::Slider::textBoxBackgroundColourId, UIColors::cardBackground);
        setColour(juce::Slider::textBoxOutlineColourId, UIColors::cardBackgroundLight);
    }
    
    void setBaseColor(const juce::Colour& baseColor)
    {
        theme = UIColors::JewelTheme(baseColor);
    }
    
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                         float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                         juce::Slider& slider) override
    {
        auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat().reduced(4.0f);
        auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
        auto centreX = bounds.getCentreX();
        auto centreY = bounds.getCentreY();
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        
        // Draw outer circle with jewel tone gradient (using theme colors)
        juce::ColourGradient gradient(
            theme.base,
            centreX, centreY - radius,
            theme.dark,
            centreX, centreY + radius,
            false
        );
        
        g.setGradientFill(gradient);
        g.fillEllipse(centreX - radius, centreY - radius, radius * 2.0f, radius * 2.0f);
        
        // Draw subtle inner shadow
        g.setColour(juce::Colours::black.withAlpha(0.3f));
        g.fillEllipse(centreX - radius * 0.85f, centreY - radius * 0.85f, 
                     radius * 1.7f, radius * 1.7f);
        
        // Draw inner circle (using derived darker color)
        juce::ColourGradient innerGradient(
            theme.base.darker(0.2f),
            centreX, centreY - radius * 0.8f,
            theme.dark,
            centreX, centreY + radius * 0.8f,
            false
        );
        
        g.setGradientFill(innerGradient);
        g.fillEllipse(centreX - radius * 0.8f, centreY - radius * 0.8f, 
                     radius * 1.6f, radius * 1.6f);
        
        // Draw thin white indicator line
        juce::Path indicatorLine;
        auto lineLength = radius * 0.7f;
        auto lineThickness = 2.0f;
        
        indicatorLine.addRectangle(-lineThickness / 2.0f, -radius * 0.75f, 
                                   lineThickness, lineLength);
        
        g.setColour(juce::Colours::white);
        g.fillPath(indicatorLine, juce::AffineTransform::rotation(angle)
                                                        .translated(centreX, centreY));
        
        // Draw subtle highlight on top
        g.setColour(juce::Colours::white.withAlpha(0.15f));
        g.fillEllipse(centreX - radius * 0.6f, centreY - radius * 0.8f, 
                     radius * 1.0f, radius * 0.6f);
    }
    
    juce::Label* createSliderTextBox(juce::Slider& slider) override
    {
        auto* label = juce::LookAndFeel_V4::createSliderTextBox(slider);
        label->setFont(juce::Font(UIConstants::smallFontSize));
        label->setJustificationType(juce::Justification::centred);
        return label;
    }
    
private:
    UIColors::JewelTheme theme;
};

} // namespace ui
