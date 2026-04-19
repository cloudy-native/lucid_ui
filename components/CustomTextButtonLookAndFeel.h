#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "UIColors.h"

namespace ui
{

/**
 * Jewel-tone styled TextButton.
 *
 * Two modes:
 *   - Primary: normal state is a dark, barely-tinted button (reads as "ready").
 *     Highlighted/dirty state is a vivid, saturated jewel-tone with glow
 *     (reads as "action needed").
 *   - Secondary: dark fill with accent-tinted border and accent text.
 */
class CustomTextButtonLookAndFeel : public juce::LookAndFeel_V4
{
public:
    explicit CustomTextButtonLookAndFeel(
        const juce::Colour& accent = UIColors::jewelCyan,
        bool secondary = false)
        : theme(accent)
        , isSecondary(secondary)
    {
    }

    void setAccentColour(const juce::Colour& accent) { theme = UIColors::JewelTheme(accent); }
    void setSecondary(bool s) { isSecondary = s; }
    void setHighlighted(bool h) { isHighlighted = h; }
    bool getHighlighted() const { return isHighlighted; }

    void drawButtonBackground(juce::Graphics& g,
                              juce::Button& button,
                              const juce::Colour& /*unused*/,
                              bool isMouseOver,
                              bool isButtonDown) override
    {
        auto bounds = button.getLocalBounds().toFloat().reduced(0.5f);
        constexpr float cr = 6.0f;
        const bool enabled = button.isEnabled();

        if (isSecondary)
            drawSecondary(g, bounds, cr, enabled, isMouseOver, isButtonDown);
        else if (isHighlighted && enabled)
            drawPrimaryHighlighted(g, bounds, cr, isMouseOver, isButtonDown);
        else
            drawPrimaryNormal(g, bounds, cr, enabled, isMouseOver, isButtonDown);
    }

    void drawButtonText(juce::Graphics& g,
                        juce::TextButton& button,
                        bool /*isMouseOver*/,
                        bool isButtonDown) override
    {
        g.setFont(getTextButtonFont(button, button.getHeight()));

        juce::Colour col;
        if (isSecondary)
            col = isButtonDown ? juce::Colours::white : theme.light.withAlpha(0.85f);
        else if (isHighlighted)
            col = juce::Colours::white;
        else
            col = isButtonDown ? juce::Colours::white : UIColors::textPrimary;

        if (!button.isEnabled())
            col = col.withAlpha(0.3f);

        g.setColour(col);
        g.drawFittedText(button.getButtonText(),
                         button.getLocalBounds().reduced(4, 2),
                         juce::Justification::centred, 1);
    }

    juce::Font getTextButtonFont(juce::TextButton& /*button*/, int buttonHeight) override
    {
        return juce::FontOptions(juce::jmin(15.0f, buttonHeight * 0.55f)).withStyle("Bold");
    }

private:
    // ── Primary NORMAL: dark, barely tinted — "ready but idle" ──────────
    void drawPrimaryNormal(juce::Graphics& g,
                           juce::Rectangle<float> b, float cr,
                           bool enabled, bool hover, bool down)
    {
        // Very dark base with just a hint of the accent hue
        juce::Colour base(0xff2e2e2e);
        if (!enabled)     base = base.withAlpha(0.5f);
        else if (down)    base = base.darker(0.15f);
        else if (hover)   base = base.brighter(0.06f);

        g.setColour(base);
        g.fillRoundedRectangle(b, cr);

        // Top edge sheen
        g.setColour(juce::Colours::white.withAlpha(0.04f));
        g.fillRoundedRectangle(b.withHeight(b.getHeight() * 0.35f), cr);

        // Accent-tinted border so it's not just a grey box
        juce::Colour border = enabled
            ? (hover ? theme.base.withAlpha(0.45f) : theme.base.withAlpha(0.2f))
            : UIColors::Component::buttonBorder;
        g.setColour(border);
        g.drawRoundedRectangle(b, cr, 1.0f);
    }

    // ── Primary HIGHLIGHTED: vivid accent — "settings changed, click me" ─
    void drawPrimaryHighlighted(juce::Graphics& g,
                                juce::Rectangle<float> b, float cr,
                                bool hover, bool down)
    {
        juce::Colour base = theme.base;
        if (down)       base = theme.dark;
        else if (hover) base = theme.light;

        // Full gradient fill
        juce::ColourGradient grad(
            base, b.getX(), b.getY(),
            base.darker(0.25f), b.getX(), b.getBottom(),
            false);
        g.setGradientFill(grad);
        g.fillRoundedRectangle(b, cr);

        // Strong top sheen
        g.setColour(juce::Colours::white.withAlpha(0.2f));
        g.fillRoundedRectangle(b.withHeight(b.getHeight() * 0.38f), cr);

        // Bright glow ring
        g.setColour(theme.base.withAlpha(0.55f));
        g.drawRoundedRectangle(b, cr, 2.0f);
    }

    // ── Secondary: dark with accent border + accent text ────────────────
    void drawSecondary(juce::Graphics& g,
                       juce::Rectangle<float> b, float cr,
                       bool enabled, bool hover, bool down)
    {
        juce::Colour bg(0xff323232);
        if (down)       bg = bg.darker(0.15f);
        else if (hover) bg = bg.brighter(0.08f);

        g.setColour(enabled ? bg : bg.withAlpha(0.5f));
        g.fillRoundedRectangle(b, cr);

        // Top edge sheen
        g.setColour(juce::Colours::white.withAlpha(0.04f));
        g.fillRoundedRectangle(b.withHeight(b.getHeight() * 0.35f), cr);

        // Accent border
        juce::Colour border = enabled
            ? (hover ? theme.base.withAlpha(0.6f) : theme.base.withAlpha(0.35f))
            : UIColors::Component::buttonBorder;
        g.setColour(border);
        g.drawRoundedRectangle(b, cr, 1.2f);
    }

    UIColors::JewelTheme theme;
    bool isSecondary;
    bool isHighlighted = false;
};

} // namespace ui
