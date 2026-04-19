#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <vector>
#include <memory>

namespace ui
{

enum class FlexDirection
{
    Row,
    Column
};

enum class JustifyContent
{
    FlexStart,
    FlexEnd,
    Center,
    SpaceBetween,
    SpaceAround,
    SpaceEvenly
};

enum class AlignItems
{
    FlexStart,
    FlexEnd,
    Center,
    Stretch
};

class FlexItem
{
public:
    FlexItem() = default;
    explicit FlexItem(juce::Component* comp) : component(comp) {}
    
    FlexItem& withFlex(float flexGrow, float flexShrink = 1.0f, float flexBasis = 0.0f)
    {
        this->flexGrow = flexGrow;
        this->flexShrink = flexShrink;
        this->flexBasis = flexBasis;
        return *this;
    }
    
    FlexItem& withWidth(float w) { width = w; return *this; }
    FlexItem& withHeight(float h) { height = h; return *this; }
    FlexItem& withMargin(float m) { margin = juce::BorderSize<float>(m); return *this; }
    FlexItem& withMargin(float top, float right, float bottom, float left)
    {
        margin = juce::BorderSize<float>(top, left, bottom, right);
        return *this;
    }
    
    juce::Component* component { nullptr };
    float flexGrow { 0.0f };
    float flexShrink { 1.0f };
    float flexBasis { 0.0f };
    float width { -1.0f };
    float height { -1.0f };
    juce::BorderSize<float> margin { 0.0f };
};

class FlexBox : public juce::Component
{
public:
    FlexBox();
    ~FlexBox() override = default;
    
    void setDirection(FlexDirection dir) { direction = dir; }
    void setJustifyContent(JustifyContent justify) { justifyContent = justify; }
    void setAlignItems(AlignItems align) { alignItems = align; }
    void setGap(float g) { gap = g; }
    void setPadding(float p) { padding = juce::BorderSize<float>(p); }
    void setPadding(float top, float right, float bottom, float left)
    {
        padding = juce::BorderSize<float>(top, left, bottom, right);
    }
    
    void addItem(const FlexItem& item);
    void clearItems();
    
    void resized() override;
    
private:
    FlexDirection direction { FlexDirection::Row };
    JustifyContent justifyContent { JustifyContent::FlexStart };
    AlignItems alignItems { AlignItems::Stretch };
    float gap { 0.0f };
    juce::BorderSize<float> padding { 0.0f };
    
    std::vector<FlexItem> items;
    
    void performLayout();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FlexBox)
};

} // namespace ui
