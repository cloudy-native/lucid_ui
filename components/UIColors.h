#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace ui
{

// UI Color System with Jewel Tones
namespace UIColors
{
    // Base background colors
    inline const juce::Colour darkBackground { 0xff0f1115 };
    inline const juce::Colour cardBackground { 0xff1a1a1a };
    inline const juce::Colour cardBackgroundLight { 0xff2a2a2a };
    inline const juce::Colour panelBackground { 0xff1a1a1a };
    
    // Text colors
    inline const juce::Colour textPrimary { 0xffcccccc };
    inline const juce::Colour textSecondary { 0xff999999 };
    inline const juce::Colour textDark { 0xff000000 };
    
    // Jewel tone base colors for dials
    inline const juce::Colour jewelCyan { 0xff00d4c8 };      // Turquoise/Cyan
    inline const juce::Colour jewelEmerald { 0xff00c896 };   // Emerald green
    inline const juce::Colour jewelSapphire { 0xff0088ff };  // Sapphire blue
    inline const juce::Colour jewelAmethyst { 0xff9966ff };  // Amethyst purple
    inline const juce::Colour jewelRuby { 0xffff3366 };      // Ruby red
    inline const juce::Colour jewelTopaz { 0xffffaa00 };     // Topaz orange/gold
    inline const juce::Colour jewelPeridot { 0xffaadd00 };   // Peridot lime
    inline const juce::Colour jewelAquamarine { 0xff66ddff }; // Aquamarine
    
    // Array of jewel tones for easy rotation
    inline const juce::Colour jewelTones[] = {
        jewelCyan,
        jewelEmerald,
        jewelSapphire,
        jewelAmethyst,
        jewelRuby,
        jewelTopaz,
        jewelPeridot,
        jewelAquamarine
    };
    
    inline constexpr int numJewelTones = 8;
    
    // Semantic colors for specific parameter types
    // These ensure consistent colors across all emitters
    namespace Semantic
    {
        inline const juce::Colour probability = jewelAmethyst;    // Purple - represents chance/randomness
        inline const juce::Colour density = jewelCyan;            // Cyan - represents frequency/activity
        inline const juce::Colour pitchDrift = jewelSapphire;     // Blue - represents pitch/tonal changes
        inline const juce::Colour volume = jewelEmerald;          // Green - represents amplitude/level
        inline const juce::Colour masterDensity = jewelCyan;      // Cyan - same as density for consistency
    }
    
    // Derive related colors from a base jewel tone
    struct JewelTheme
    {
        juce::Colour base;
        juce::Colour light;      // Lighter version for highlights
        juce::Colour dark;       // Darker version for shadows
        juce::Colour glow;       // Transparent glow for effects
        juce::Colour border;     // Border/outline color
        juce::Colour track;      // Track/background color
        
        explicit JewelTheme(const juce::Colour& baseColor)
            : base(baseColor)
            , light(baseColor.brighter(0.3f))
            , dark(baseColor.darker(0.4f))
            , glow(baseColor.withAlpha(0.3f))
            , border(baseColor.withAlpha(0.3f))
            , track(baseColor.withAlpha(0.15f))
        {
        }
        
        // Get a jewel theme by index (rotates through available tones)
        static JewelTheme fromIndex(int index)
        {
            return JewelTheme(jewelTones[index % numJewelTones]);
        }
    };
    
    // Default theme (cyan)
    inline JewelTheme defaultTheme() { return JewelTheme(jewelCyan); }

    // Shared component colours (derived from the dark palette + jewel tones)
    namespace Component
    {
        // Buttons
        inline const juce::Colour buttonBackground { 0xff2a2a2a };
        inline const juce::Colour buttonBackgroundHover { 0xff353535 };
        inline const juce::Colour buttonBackgroundDown { 0xff1e1e1e };
        inline const juce::Colour buttonBorder { 0xff444444 };
        inline const juce::Colour buttonText { 0xffcccccc };
        inline const juce::Colour buttonTextDown { 0xffffffff };

        // ComboBox
        inline const juce::Colour comboBackground { 0xff222222 };
        inline const juce::Colour comboArrow { 0xff888888 };
        inline const juce::Colour comboPopupBackground { 0xff1e1e1e };
        inline const juce::Colour comboPopupHighlight { 0xff333333 };

        // Toggle / checkbox
        inline const juce::Colour toggleOff { 0xff444444 };
        inline const juce::Colour toggleTrack { 0xff2a2a2a };

        // Linear slider
        inline const juce::Colour sliderTrack { 0xff2a2a2a };
        inline const juce::Colour sliderTrackFill { 0xff3a3a3a };

        // Focus ring (jewel-tone glow applied to any focused component)
        inline const juce::Colour focusBorder = jewelCyan.withAlpha(0.5f);

        // Tab bar
        inline const juce::Colour tabBarBackground { 0xff242424 };
        inline const juce::Colour tabActive { 0xff4a4a4a };
        inline const juce::Colour tabInactive { 0xff363636 };
    }
}

} // namespace ui
