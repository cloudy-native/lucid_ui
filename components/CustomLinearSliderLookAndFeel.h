#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "UIColors.h"

namespace ui
{

/**
 * Jewel-tone styled linear slider.
 *
 * Rounded track with jewel-tone fill for the active portion,
 * and a small gradient thumb matching the rotary knob style.
 * Also inherits the rotary drawing from CustomKnobLookAndFeel
 * so it can be used on sliders that switch style.
 */
class CustomLinearSliderLookAndFeel : public juce::LookAndFeel_V4
{
public:
    explicit CustomLinearSliderLookAndFeel(const juce::Colour& accent = UIColors::jewelCyan)
        : theme(accent)
    {
        setColour(juce::Slider::textBoxTextColourId, UIColors::textPrimary);
        setColour(juce::Slider::textBoxBackgroundColourId, UIColors::cardBackground);
        setColour(juce::Slider::textBoxOutlineColourId, UIColors::cardBackgroundLight);
    }

    void setAccentColour(const juce::Colour& accent) { theme = UIColors::JewelTheme(accent); }

    void drawLinearSlider(juce::Graphics& g,
                          int x, int y, int width, int height,
                          float sliderPos, float minSliderPos, float maxSliderPos,
                          juce::Slider::SliderStyle style,
                          juce::Slider& slider) override
    {
        if (style == juce::Slider::LinearHorizontal)
            drawHorizontalSlider(g, x, y, width, height, sliderPos, slider);
        else if (style == juce::Slider::LinearVertical)
            drawVerticalSlider(g, x, y, width, height, sliderPos, slider);
        else
            juce::LookAndFeel_V4::drawLinearSlider(g, x, y, width, height,
                sliderPos, minSliderPos, maxSliderPos, style, slider);
    }

    juce::Label* createSliderTextBox(juce::Slider& slider) override
    {
        auto* label = juce::LookAndFeel_V4::createSliderTextBox(slider);
        label->setFont(juce::Font(12.0f));
        label->setColour(juce::Label::textColourId, UIColors::textPrimary);
        label->setColour(juce::Label::backgroundColourId, UIColors::cardBackground);
        label->setColour(juce::Label::outlineColourId, UIColors::cardBackgroundLight);
        return label;
    }

private:
    void drawHorizontalSlider(juce::Graphics& g,
                              int x, int y, int width, int height,
                              float sliderPos,
                              juce::Slider& /*slider*/)
    {
        constexpr float trackHeight = 5.0f;
        constexpr float thumbRadius = 7.0f;

        float trackY = (float) y + (float) height * 0.5f - trackHeight * 0.5f;
        auto trackBounds = juce::Rectangle<float>((float) x, trackY, (float) width, trackHeight);

        // Background track
        g.setColour(UIColors::Component::sliderTrack);
        g.fillRoundedRectangle(trackBounds, trackHeight * 0.5f);

        // Filled portion
        auto fillBounds = trackBounds.withRight(sliderPos);
        g.setColour(theme.base);
        g.fillRoundedRectangle(fillBounds, trackHeight * 0.5f);

        // Thumb
        float thumbX = sliderPos;
        float thumbY = (float) y + (float) height * 0.5f;

        // Outer circle
        juce::ColourGradient thumbGrad(
            theme.light,
            thumbX, thumbY - thumbRadius,
            theme.dark,
            thumbX, thumbY + thumbRadius,
            false);
        g.setGradientFill(thumbGrad);
        g.fillEllipse(thumbX - thumbRadius, thumbY - thumbRadius,
                      thumbRadius * 2.0f, thumbRadius * 2.0f);

        // Inner highlight
        g.setColour(juce::Colours::white.withAlpha(0.15f));
        g.fillEllipse(thumbX - thumbRadius * 0.5f, thumbY - thumbRadius * 0.7f,
                      thumbRadius, thumbRadius * 0.6f);
    }

    void drawVerticalSlider(juce::Graphics& g,
                            int x, int y, int width, int height,
                            float sliderPos,
                            juce::Slider& /*slider*/)
    {
        constexpr float trackWidth = 5.0f;
        constexpr float thumbRadius = 7.0f;

        float trackX = (float) x + (float) width * 0.5f - trackWidth * 0.5f;
        auto trackBounds = juce::Rectangle<float>(trackX, (float) y, trackWidth, (float) height);

        // Background track
        g.setColour(UIColors::Component::sliderTrack);
        g.fillRoundedRectangle(trackBounds, trackWidth * 0.5f);

        // Filled portion (from bottom up)
        auto fillBounds = trackBounds.withTop(sliderPos);
        g.setColour(theme.base);
        g.fillRoundedRectangle(fillBounds, trackWidth * 0.5f);

        // Thumb
        float thumbX = (float) x + (float) width * 0.5f;
        float thumbY = sliderPos;

        juce::ColourGradient thumbGrad(
            theme.light,
            thumbX, thumbY - thumbRadius,
            theme.dark,
            thumbX, thumbY + thumbRadius,
            false);
        g.setGradientFill(thumbGrad);
        g.fillEllipse(thumbX - thumbRadius, thumbY - thumbRadius,
                      thumbRadius * 2.0f, thumbRadius * 2.0f);

        g.setColour(juce::Colours::white.withAlpha(0.15f));
        g.fillEllipse(thumbX - thumbRadius * 0.5f, thumbY - thumbRadius * 0.7f,
                      thumbRadius, thumbRadius * 0.6f);
    }

    UIColors::JewelTheme theme;
};

} // namespace ui
