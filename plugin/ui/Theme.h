/* SPDX-License-Identifier: GPL-3.0-only */
#pragma once

#include <cstdint>

#include <juce_gui_basics/juce_gui_basics.h>

#include "UiTokens.h"

namespace p1::ui {

juce::Colour colour(std::uint32_t argb);

juce::Typeface::Ptr uiTypeface();
juce::Typeface::Ptr displayTypeface();
juce::Typeface::Ptr brandTypeface();

juce::Font uiFont(float height);
juce::Font displayFont(float height);
juce::Font brandFont(float height);
float textWidth(const juce::Font& font, const juce::String& text);

void drawSectionTitle(juce::Graphics& g, juce::Rectangle<int> area,
                      const juce::String& title);
void drawLabel(juce::Graphics& g, juce::Rectangle<int> area,
               const juce::String& text, juce::Justification justification,
               juce::Colour colour, float height = tokens::textInlineLabel);

class P1LookAndFeel : public juce::LookAndFeel_V4 {
public:
    P1LookAndFeel();

    void drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
                      int buttonX, int buttonY, int buttonW, int buttonH,
                      juce::ComboBox& box) override;
    void positionComboBoxText(juce::ComboBox& box, juce::Label& label) override;
    void drawPopupMenuBackground(juce::Graphics& g, int width, int height) override;
    void drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area,
                           bool isSeparator, bool isActive, bool isHighlighted,
                           bool isTicked, bool hasSubMenu,
                           const juce::String& text,
                           const juce::String& shortcutKeyText,
                           const juce::Drawable* icon,
                           const juce::Colour* textColour) override;
    void drawScrollbar(juce::Graphics& g, juce::ScrollBar& bar, int x, int y,
                       int width, int height, bool isScrollbarVertical,
                       int thumbStartPosition, int thumbSize,
                       bool isMouseOver, bool isMouseDown) override;
};

class KnobLookAndFeel : public juce::LookAndFeel_V4 {
public:
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float rotaryStartAngle,
                          float rotaryEndAngle, juce::Slider& slider) override;
};

} // namespace p1::ui
