# lucid_ui

Modern UI components with jewel-tone styling for JUCE audio plugins.

## Features

- **UIColors**: Jewel-tone color palette with semantic color assignments
- **UIConstants**: Consistent spacing, sizing, and layout constants
- **CustomKnobLookAndFeel**: Beautiful rotary knobs with customizable colors
- **FlexBox**: Simplified flexbox layout utilities

This is a foundational UI module providing reusable styling and layout components for JUCE audio plugins.

## Installation

### Option 1: Local Module Path

1. Copy the `lucid_ui` folder to your project's `modules/` directory
2. In your CMakeLists.txt, add the module path:

```cmake
juce_add_plugin(YourPlugin
    # ... other settings ...
)

target_link_libraries(YourPlugin
    PRIVATE
        juce::juce_audio_utils
        juce::juce_gui_basics
        lucid_ui
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
```

3. Add the module to your JUCE module path:

```cmake
juce_add_module(${CMAKE_CURRENT_SOURCE_DIR}/modules/lucid_ui)
```

### Option 2: Git Submodule

```bash
cd your-project
git submodule add https://github.com/stephen/lucid_ui modules/lucid_ui
```

Then follow the same CMake steps as Option 1.

## Usage

```cpp
#include <lucid_ui/lucid_ui.h>

// Use components
auto knob = std::make_unique<juce::Slider>();
knob->setLookAndFeel(&customKnobLookAndFeel);

// Access colors
g.setColour(ui::UIColors::jewelCyan);

// Use constants
auto margin = ui::UIConstants::smallMargin;
```

## Components

### UIColors

Provides a consistent jewel-tone color palette:

- `jewelCyan`, `jewelEmerald`, `jewelSapphire`, `jewelAmethyst`
- `jewelRuby`, `jewelTopaz`, `jewelPeridot`, `jewelAquamarine`
- Semantic colors for specific parameters (probability, density, pitch, volume)

### CustomKnobLookAndFeel

Beautiful rotary knobs with:
- Customizable jewel-tone colors
- Smooth gradients and glow effects
- Consistent sizing and behavior

### FlexBox

Simplified flexbox layout utilities for easier component positioning and sizing.

## Requirements

- JUCE 7.0+
- C++17 or later
- CMake 3.15+

## License

MIT License - See LICENSE file for details
