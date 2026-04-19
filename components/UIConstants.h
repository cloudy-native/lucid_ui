#pragma once

namespace ui
{

// UI Dimensions and Spacing Constants
namespace UIConstants
{
    // Window dimensions
    constexpr int defaultWindowWidth = 1000;
    constexpr int defaultWindowHeight = 320;
    constexpr int minWindowWidth = 800;
    constexpr int minWindowHeight = 280;
    constexpr int maxWindowWidth = 2400;
    constexpr int maxWindowHeight = 1600;
    
    // EmitterCard dimensions (full-width horizontal layout)
    constexpr int emitterCardHeight = 90;  // Compact horizontal height
    constexpr int emitterCardBorderRadius = 8;
    constexpr int emitterCardBorderWidth = 2;
    constexpr int emitterCardSoundpackButtonWidth = 140;
    constexpr int emitterCardSoundpackButtonHeight = 70;
    constexpr int emitterCardVerticalSpacing = 8;  // Space between stacked cards
    
    // Dial/Knob dimensions
    constexpr int dialSize = 55;
    constexpr int dialLabelHeight = 16;
    constexpr int dialTextBoxWidth = 70;
    constexpr int dialTextBoxHeight = 18;
    
    // Master density dial (larger)
    constexpr int masterDialSize = 120;
    constexpr int masterDialTextBoxHeight = 22;
    
    // Spacing
    constexpr int tinySpacing = 2;
    constexpr int smallSpacing = 4;
    constexpr int mediumSpacing = 8;
    constexpr int largeSpacing = 12;
    constexpr int extraLargeSpacing = 16;
    constexpr int hugeSpacing = 18;
    constexpr int giantSpacing = 20;
    
    // Margins/Padding
    constexpr int tinyMargin = 4;
    constexpr int smallMargin = 8;
    constexpr int mediumMargin = 12;
    constexpr int largeMargin = 16;
    constexpr int extraLargeMargin = 20;
    
    // Top bar
    constexpr int topBarHeight = 140;
    constexpr int topBarButtonWidth = 120;
    constexpr int topBarButtonHeight = 30;
    constexpr int topBarSmallButtonWidth = 40;
    constexpr int topBarMediumButtonWidth = 70;
    constexpr int topBarLargeButtonWidth = 80;
    
    // Side panel
    constexpr int sidePanelWidth = 400;
    
    // Font sizes
    constexpr float tinyFontSize = 11.0f;
    constexpr float smallFontSize = 12.0f;
    constexpr float mediumFontSize = 13.0f;
    constexpr float largeFontSize = 14.0f;
    constexpr float extraLargeFontSize = 16.0f;
    constexpr float hugeFontSize = 20.0f;
    constexpr float giantFontSize = 24.0f;
    
    // List/Browser dimensions
    constexpr int listRowHeight = 24;
    constexpr int sampleListRowHeight = 28;
    constexpr int checkboxSize = 16;
    constexpr int playButtonSize = 16;
    
    // Corner resizer
    constexpr int resizerSize = 16;
}

} // namespace ui
