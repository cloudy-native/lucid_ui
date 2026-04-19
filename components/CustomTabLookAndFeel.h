#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "UIColors.h"

namespace ui
{

/**
 * Jewel-tone styled TabbedComponent.
 *
 * Active tab gets a subtle accent-tinted background and a bright underline.
 * Inactive tabs are dark with a faint border. The whole bar blends with
 * the dark background rather than floating as grey pills.
 */
class CustomTabLookAndFeel : public juce::LookAndFeel_V4
{
public:
    explicit CustomTabLookAndFeel(const juce::Colour& accent = UIColors::jewelTopaz)
        : theme(accent)
    {
    }

    void setAccentColour(const juce::Colour& accent) { theme = UIColors::JewelTheme(accent); }

    void drawTabbedButtonBarBackground(juce::TabbedButtonBar& bar,
                                       juce::Graphics& g) override
    {
        g.setColour(UIColors::Component::tabBarBackground);
        g.fillRect(bar.getLocalBounds());

        // Bottom separator line
        auto r = bar.getLocalBounds().toFloat();
        g.setColour(juce::Colours::black.withAlpha(0.4f));
        g.fillRect(r.withHeight(1.0f).withY(r.getBottom() - 1.0f));
    }

    void drawTabAreaBehindFrontButton(juce::TabbedButtonBar& /*bar*/,
                                      juce::Graphics& /*g*/,
                                      int /*w*/, int /*h*/) override
    {
        // Intentionally empty — the bar background + per-tab paint handles everything
    }

    void drawTabButton(juce::TabBarButton& button,
                       juce::Graphics& g,
                       bool isMouseOver,
                       bool isMouseDown) override
    {
        auto area = button.getLocalBounds().toFloat();
        const bool isActive = button.isFrontTab();
        auto pill = area.reduced(2.0f, 4.0f);
        constexpr float cr = 6.0f;

        if (isActive)
        {
            // Active: accent-tinted dark fill
            juce::Colour bg = theme.base.withAlpha(0.12f);
            if (isMouseDown)
                bg = theme.base.withAlpha(0.18f);
            else if (isMouseOver)
                bg = theme.base.withAlpha(0.15f);

            g.setColour(bg);
            g.fillRoundedRectangle(pill, cr);

            // Accent border
            g.setColour(theme.base.withAlpha(0.35f));
            g.drawRoundedRectangle(pill, cr, 1.2f);

            // Bright underline
            auto underline = area.withY(area.getBottom() - 3.0f)
                                 .withHeight(3.0f)
                                 .reduced(8.0f, 0.0f);
            g.setColour(theme.base);
            g.fillRoundedRectangle(underline, 1.5f);
        }
        else
        {
            // Inactive: very subtle
            juce::Colour bg(0xff2a2a2a);
            if (isMouseDown)
                bg = bg.brighter(0.06f);
            else if (isMouseOver)
                bg = bg.brighter(0.03f);

            g.setColour(bg);
            g.fillRoundedRectangle(pill, cr);

            g.setColour(juce::Colours::white.withAlpha(0.06f));
            g.drawRoundedRectangle(pill, cr, 1.0f);
        }

        // Text
        float fontSize = isActive ? 14.5f : 13.5f;
        g.setFont(juce::FontOptions(fontSize).withStyle(isActive ? "Bold" : "Regular"));
        g.setColour(isActive ? juce::Colours::white
                             : juce::Colours::white.withAlpha(0.55f));
        g.drawFittedText(button.getButtonText(),
                         button.getLocalBounds().reduced(10, 3),
                         juce::Justification::centred, 1);
    }

    int getTabButtonBestWidth(juce::TabBarButton& button, int tabDepth) override
    {
        juce::GlyphArrangement ga;
        ga.addLineOfText(getTabButtonFont(button, tabDepth),
                         button.getButtonText(), 0.0f, 0.0f);
        int textWidth = (int) std::ceil(
            ga.getBoundingBox(0, -1, true).getWidth());
        return juce::jlimit(90, 220, 48 + textWidth);
    }

private:
    UIColors::JewelTheme theme;
};

} // namespace ui
