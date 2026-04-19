#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "UIColors.h"

namespace ui
{

/**
 * Jewel-tone styled ToggleButton.
 *
 * Draws a rounded-rect checkbox that fills with the jewel accent
 * colour when toggled on, with a white checkmark.
 * Off state shows a dark outline matching the card palette.
 */
class CustomToggleButtonLookAndFeel : public juce::LookAndFeel_V4
{
public:
    explicit CustomToggleButtonLookAndFeel(const juce::Colour& accent = UIColors::jewelCyan)
        : theme(accent)
    {
    }

    void setAccentColour(const juce::Colour& accent) { theme = UIColors::JewelTheme(accent); }

    void drawToggleButton(juce::Graphics& g,
                          juce::ToggleButton& button,
                          bool shouldDrawButtonAsHighlighted,
                          bool shouldDrawButtonAsDown) override
    {
        constexpr float boxSize = 16.0f;
        constexpr float cornerRadius = 3.5f;

        auto bounds = button.getLocalBounds().toFloat();
        float boxY = bounds.getCentreY() - boxSize * 0.5f;
        auto boxBounds = juce::Rectangle<float>(2.0f, boxY, boxSize, boxSize);

        bool isOn = button.getToggleState();

        if (isOn)
        {
            // Filled jewel-tone box
            juce::Colour fill = theme.base;
            if (shouldDrawButtonAsDown)
                fill = theme.dark;
            else if (shouldDrawButtonAsHighlighted)
                fill = theme.light;

            g.setColour(fill);
            g.fillRoundedRectangle(boxBounds, cornerRadius);

            // Checkmark
            juce::Path tick;
            float x = boxBounds.getX();
            float y = boxBounds.getY();
            tick.startNewSubPath(x + 4.0f, y + boxSize * 0.52f);
            tick.lineTo(x + boxSize * 0.42f, y + boxSize - 4.5f);
            tick.lineTo(x + boxSize - 3.5f, y + 4.5f);

            g.setColour(juce::Colours::white);
            g.strokePath(tick, juce::PathStrokeType(2.0f,
                juce::PathStrokeType::mitered,
                juce::PathStrokeType::rounded));
        }
        else
        {
            // Empty box with border
            juce::Colour border = UIColors::Component::toggleOff;
            if (shouldDrawButtonAsHighlighted)
                border = theme.border;

            g.setColour(UIColors::Component::toggleTrack);
            g.fillRoundedRectangle(boxBounds, cornerRadius);

            g.setColour(border);
            g.drawRoundedRectangle(boxBounds, cornerRadius, 1.2f);
        }

        // Label text
        float textX = boxBounds.getRight() + 8.0f;
        auto textBounds = bounds.withLeft(textX).withTrimmedRight(2.0f);

        juce::Colour textCol = button.isEnabled()
            ? UIColors::textPrimary
            : UIColors::textSecondary;

        g.setColour(textCol);
        g.setFont(juce::FontOptions(juce::jmin(14.0f, bounds.getHeight() * 0.5f)));
        g.drawFittedText(button.getButtonText(),
                         textBounds.toNearestInt(),
                         juce::Justification::centredLeft, 1);
    }

private:
    UIColors::JewelTheme theme;
};

} // namespace ui
