#include "FlexBox.h"

namespace ui
{

FlexBox::FlexBox()
{
}

void FlexBox::addItem(const FlexItem& item)
{
    items.push_back(item);
    if (item.component)
        addAndMakeVisible(item.component);
}

void FlexBox::clearItems()
{
    items.clear();
}

void FlexBox::resized()
{
    performLayout();
}

void FlexBox::performLayout()
{
    auto bounds = getLocalBounds().toFloat();
    
    // Apply padding
    bounds = padding.subtractedFrom(bounds);
    
    if (items.empty())
        return;
    
    const bool isRow = (direction == FlexDirection::Row);
    // Snap float bounds to integer layout units at the boundary (setBounds is int).
    const int mainSize  = juce::roundToInt (isRow ? bounds.getWidth()  : bounds.getHeight());
    const int crossSize = juce::roundToInt (isRow ? bounds.getHeight() : bounds.getWidth());
    
    // Calculate total flex basis and flexible space
    float totalFlexBasis = 0.0f;
    float totalFlexGrow = 0.0f;
    int fixedSize = 0;
    const int numGaps = juce::jmax (0, static_cast<int> (items.size()) - 1);
    
    for (const auto& item : items)
    {
        if (item.flexGrow > 0.0f)
        {
            totalFlexGrow += item.flexGrow;
            totalFlexBasis += item.flexBasis;
        }
        else
        {
            if (isRow)
                fixedSize += juce::roundToInt (item.width > 0.0f ? item.width : 0.0f);
            else
                fixedSize += juce::roundToInt (item.height > 0.0f ? item.height : 0.0f);
        }
    }
    
    const int availableSpace = mainSize - fixedSize - juce::roundToInt (gap * static_cast<float> (numGaps));
    
    // Calculate positions based on justify content
    int mainPos = juce::roundToInt (isRow ? bounds.getX() : bounds.getY());
    int extraSpace = 0;
    int spaceBetween = 0;
    
    switch (justifyContent)
    {
        case JustifyContent::FlexStart:
            // Default: no adjustment needed
            break;
        case JustifyContent::FlexEnd:
            mainPos += availableSpace;
            break;
        case JustifyContent::Center:
            mainPos += availableSpace / 2;
            break;
        case JustifyContent::SpaceBetween:
            if (items.size() > 1)
                spaceBetween = availableSpace / (static_cast<int>(items.size()) - 1);
            break;
        case JustifyContent::SpaceAround:
            extraSpace = availableSpace / static_cast<int>(items.size());
            mainPos += extraSpace / 2;
            break;
        case JustifyContent::SpaceEvenly:
            extraSpace = availableSpace / (static_cast<int>(items.size()) + 1);
            mainPos += extraSpace;
            break;
    }
    
    // Layout items
    for (const auto& item : items)
    {
        if (!item.component)
            continue;
        
        // Calculate main axis size
        int itemMainSize;
        if (item.flexGrow > 0.0f && totalFlexGrow > 0.0f)
        {
            const float freeSpace = static_cast<float> (availableSpace) - totalFlexBasis;
            const float flexSpace = freeSpace * (item.flexGrow / totalFlexGrow);
            itemMainSize = juce::roundToInt (item.flexBasis + flexSpace);
        }
        else
        {
            itemMainSize = juce::roundToInt (isRow ? item.width : item.height);
        }
        
        // Calculate cross axis size and position
        int itemCrossSize = juce::roundToInt (isRow ? item.height : item.width);
        if (itemCrossSize < 0)
            itemCrossSize = crossSize;
        
        int crossPos = juce::roundToInt (isRow ? bounds.getY() : bounds.getX());
        
        switch (alignItems)
        {
            case AlignItems::FlexStart:
                // Default: no adjustment needed
                break;
            case AlignItems::FlexEnd:
                crossPos += crossSize - itemCrossSize;
                break;
            case AlignItems::Center:
                crossPos += (crossSize - itemCrossSize) / 2;
                break;
            case AlignItems::Stretch:
                itemCrossSize = crossSize;
                break;
        }
        
        // Apply margin
        auto marginBounds = isRow
            ? juce::Rectangle<float>(static_cast<float>(mainPos), static_cast<float>(crossPos), 
                                      static_cast<float>(itemMainSize), static_cast<float>(itemCrossSize))
            : juce::Rectangle<float>(static_cast<float>(crossPos), static_cast<float>(mainPos), 
                                      static_cast<float>(itemCrossSize), static_cast<float>(itemMainSize));
        
        marginBounds = item.margin.subtractedFrom(marginBounds);
        
        item.component->setBounds(marginBounds.toNearestInt());
        
        // Advance main position
        mainPos += itemMainSize + juce::roundToInt (gap);
        
        if (justifyContent == JustifyContent::SpaceBetween)
            mainPos += spaceBetween;
        else if (justifyContent == JustifyContent::SpaceAround || justifyContent == JustifyContent::SpaceEvenly)
            mainPos += extraSpace;
    }
}

} // namespace ui
