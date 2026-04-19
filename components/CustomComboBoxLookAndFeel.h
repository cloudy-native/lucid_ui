#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "UIColors.h"

namespace ui
{

/**
 * Jewel-tone styled ComboBox + PopupMenu.
 *
 * The combo box itself has a subtle gradient background, accent-tinted
 * border, and a chevron arrow. The popup menu is dark with accent
 * highlight bars and a checkmark for the selected item.
 */
class CustomComboBoxLookAndFeel : public juce::LookAndFeel_V4
{
public:
    explicit CustomComboBoxLookAndFeel(const juce::Colour& accent = UIColors::jewelCyan)
        : theme(accent)
    {
        // Force dark popup colours so JUCE's internal rendering never
        // falls back to a light theme
        setColour(juce::PopupMenu::backgroundColourId,            juce::Colour(0xff1c1c1c));
        setColour(juce::PopupMenu::textColourId,                  UIColors::textPrimary);
        setColour(juce::PopupMenu::headerTextColourId,            UIColors::textPrimary);
        setColour(juce::PopupMenu::highlightedBackgroundColourId, accent.withAlpha(0.18f));
        setColour(juce::PopupMenu::highlightedTextColourId,       juce::Colours::white);

        // ComboBox colours (used by JUCE internals we don't override)
        setColour(juce::ComboBox::backgroundColourId,  juce::Colour(0xff222222));
        setColour(juce::ComboBox::textColourId,        UIColors::textPrimary);
        setColour(juce::ComboBox::outlineColourId,     juce::Colours::transparentBlack);
        setColour(juce::ComboBox::arrowColourId,       UIColors::Component::comboArrow);
        setColour(juce::ComboBox::focusedOutlineColourId, accent.withAlpha(0.5f));
    }

    void setAccentColour(const juce::Colour& accent) { theme = UIColors::JewelTheme(accent); }

    // Force popup menus to be children of the editor component so they
    // inherit this LookAndFeel instead of using the desktop default.
    juce::Component* getParentComponentForMenuOptions(const juce::PopupMenu::Options& options) override
    {
        if (auto* target = options.getTargetComponent())
            return target->getTopLevelComponent();
        return nullptr;
    }

    // ── ComboBox ────────────────────────────────────────────────────────

    void drawComboBox(juce::Graphics& g, int width, int height,
                      bool isButtonDown,
                      int /*buttonX*/, int /*buttonY*/,
                      int /*buttonW*/, int /*buttonH*/,
                      juce::ComboBox& box) override
    {
        auto bounds = juce::Rectangle<float>(0.0f, 0.0f, (float) width, (float) height);
        constexpr float cr = 5.0f;
        const bool focused = box.hasKeyboardFocus(true);
        const bool hovered = box.isMouseOver(true);

        // Background — subtle vertical gradient for depth
        {
            juce::Colour top(0xff282828);
            juce::Colour bot(0xff1e1e1e);
            if (isButtonDown) { top = top.darker(0.1f); bot = bot.darker(0.1f); }
            else if (hovered) { top = top.brighter(0.04f); bot = bot.brighter(0.02f); }

            juce::ColourGradient grad(top, 0, 0, bot, 0, (float) height, false);
            g.setGradientFill(grad);
            g.fillRoundedRectangle(bounds, cr);
        }

        // Top edge sheen
        g.setColour(juce::Colours::white.withAlpha(0.04f));
        g.fillRoundedRectangle(bounds.withHeight(bounds.getHeight() * 0.4f), cr);

        // Border
        juce::Colour border;
        if (focused)       border = theme.base.withAlpha(0.6f);
        else if (hovered)  border = theme.base.withAlpha(0.3f);
        else               border = juce::Colour(0xff3a3a3a);

        g.setColour(border);
        g.drawRoundedRectangle(bounds.reduced(0.5f), cr, 1.0f);

        // Chevron arrow (▾) — right side
        {
            const float arrowW = 8.0f;
            const float arrowH = 5.0f;
            const float ax = (float) width - 16.0f;
            const float ay = (float) height * 0.5f - arrowH * 0.5f;

            juce::Path chevron;
            chevron.startNewSubPath(ax, ay);
            chevron.lineTo(ax + arrowW * 0.5f, ay + arrowH);
            chevron.lineTo(ax + arrowW, ay);

            g.setColour(focused || hovered ? theme.light : UIColors::Component::comboArrow);
            g.strokePath(chevron, juce::PathStrokeType(1.5f,
                juce::PathStrokeType::curved,
                juce::PathStrokeType::rounded));
        }
    }

    void positionComboBoxText(juce::ComboBox& box, juce::Label& label) override
    {
        label.setBounds(10, 0, box.getWidth() - 30, box.getHeight());
        label.setFont(getComboBoxFont(box));
        label.setColour(juce::Label::textColourId, UIColors::textPrimary);
    }

    juce::Font getComboBoxFont(juce::ComboBox& box) override
    {
        return juce::FontOptions(juce::jmin(14.0f, (float) box.getHeight() * 0.52f));
    }

    // ── PopupMenu ───────────────────────────────────────────────────────

    void drawPopupMenuBackground(juce::Graphics& g, int width, int height) override
    {
        auto bounds = juce::Rectangle<float>(0.0f, 0.0f, (float) width, (float) height);

        // Drop shadow
        g.setColour(juce::Colours::black.withAlpha(0.4f));
        g.fillRoundedRectangle(bounds.expanded(1.0f), 6.0f);

        // Background
        g.setColour(juce::Colour(0xff1c1c1c));
        g.fillRoundedRectangle(bounds, 5.0f);

        // Border
        g.setColour(juce::Colour(0xff3a3a3a));
        g.drawRoundedRectangle(bounds.reduced(0.5f), 5.0f, 1.0f);
    }

    void drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area,
                           bool isSeparator, bool isActive, bool isHighlighted,
                           bool isTicked, bool hasSubMenu,
                           const juce::String& text, const juce::String& shortcutKeyText,
                           const juce::Drawable* icon, const juce::Colour* textColourToUse) override
    {
        juce::ignoreUnused(shortcutKeyText, icon, hasSubMenu);

        if (isSeparator)
        {
            auto r = area.reduced(8, 0).toFloat();
            g.setColour(juce::Colour(0xff333333));
            g.fillRect(r.withHeight(1.0f).withY(r.getCentreY()));
            return;
        }

        auto r = area.reduced(4, 1);

        // Highlight bar
        if (isHighlighted && isActive)
        {
            g.setColour(theme.base.withAlpha(0.18f));
            g.fillRoundedRectangle(r.toFloat(), 4.0f);
        }

        // Text colour
        juce::Colour textCol;
        if (textColourToUse != nullptr)
            textCol = *textColourToUse;
        else if (!isActive)
            textCol = UIColors::textSecondary;
        else if (isHighlighted)
            textCol = juce::Colours::white;
        else if (isTicked)
            textCol = theme.light;
        else
            textCol = UIColors::textPrimary;

        auto textArea = r.reduced(8, 0);

        // Tick indicator — small checkmark, not a dot
        if (isTicked)
        {
            float cx = (float) textArea.getX() + 2.0f;
            float cy = (float) textArea.getCentreY();

            juce::Path tick;
            tick.startNewSubPath(cx, cy);
            tick.lineTo(cx + 3.5f, cy + 3.5f);
            tick.lineTo(cx + 9.0f, cy - 3.0f);

            g.setColour(theme.base);
            g.strokePath(tick, juce::PathStrokeType(1.8f,
                juce::PathStrokeType::mitered,
                juce::PathStrokeType::rounded));

            textArea.removeFromLeft(18);
        }

        g.setColour(textCol);
        g.setFont(juce::FontOptions(juce::jmin(13.5f, (float) area.getHeight() * 0.55f)));
        g.drawFittedText(text, textArea, juce::Justification::centredLeft, 1);
    }

    // Ensure the popup menu window itself uses our colours
    void getIdealPopupMenuItemSize(const juce::String& text, bool isSeparator,
                                   int standardMenuItemHeight,
                                   int& idealWidth, int& idealHeight) override
    {
        if (isSeparator)
        {
            idealWidth = 50;
            idealHeight = 8;
            return;
        }

        juce::Font font(juce::FontOptions(13.5f));
        idealWidth = (int) std::ceil(juce::GlyphArrangement::getStringWidth(font, text)) + 44;
        idealHeight = juce::jmax(standardMenuItemHeight, 28);
    }

private:
    UIColors::JewelTheme theme;
};

} // namespace ui
