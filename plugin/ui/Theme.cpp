/* SPDX-License-Identifier: GPL-3.0-only */
#include "Theme.h"

#include <cmath>

#include "BinaryData.h"

namespace p1::ui {
namespace {

juce::Typeface::Ptr makeTypeface(const void* data, int size)
{
    return juce::Typeface::createSystemTypefaceFor(data, static_cast<size_t>(size));
}

} // namespace

juce::Colour colour(std::uint32_t argb)
{
    return juce::Colour(argb);
}

juce::Typeface::Ptr uiTypeface()
{
    static const auto typeface = makeTypeface(BinaryData::DotGothic16Regular_ttf,
                                              BinaryData::DotGothic16Regular_ttfSize);
    return typeface;
}

juce::Typeface::Ptr displayTypeface()
{
    static const auto typeface =
        makeTypeface(BinaryData::ZenDotsRegular_ttf, BinaryData::ZenDotsRegular_ttfSize);
    return typeface;
}

juce::Typeface::Ptr brandTypeface()
{
    static const auto typeface = makeTypeface(BinaryData::UnboundedRegular_ttf,
                                              BinaryData::UnboundedRegular_ttfSize);
    return typeface;
}

juce::Font uiFont(float height)
{
    return juce::Font(juce::FontOptions(uiTypeface())).withHeight(height + 4.0f);
}

juce::Font displayFont(float height)
{
    return juce::Font(juce::FontOptions(displayTypeface())).withHeight(height + 4.0f);
}

juce::Font brandFont(float height)
{
    return juce::Font(juce::FontOptions(brandTypeface())).withHeight(height + 4.0f);
}

float textWidth(const juce::Font& font, const juce::String& text)
{
    juce::GlyphArrangement glyphs;
    glyphs.addLineOfText(font, text, 0.0f, 0.0f);
    if (glyphs.getNumGlyphs() == 0)
        return 0.0f;
    return glyphs.getBoundingBox(0, glyphs.getNumGlyphs(), true).getWidth();
}

void drawSectionTitle(juce::Graphics& g, juce::Rectangle<int> area,
                      const juce::String& title)
{
    g.setFont(displayFont(tokens::textSectionTitle));
    g.setColour(colour(tokens::faint));
    g.drawText("//", area, juce::Justification::centredLeft, false);
    g.setColour(colour(tokens::pink));
    g.drawText(title, area.withTrimmedLeft(19), juce::Justification::centredLeft,
               false);
}

P1LookAndFeel::P1LookAndFeel()
{
    setColour(juce::PopupMenu::backgroundColourId, colour(tokens::popup));
    setColour(juce::PopupMenu::textColourId, colour(tokens::text));
    setColour(juce::PopupMenu::highlightedBackgroundColourId,
              colour(tokens::purple).withAlpha(0.18f));
    setColour(juce::PopupMenu::highlightedTextColourId, colour(tokens::purple));
    setColour(juce::Label::textColourId, colour(tokens::dim));
    setColour(juce::TextEditor::backgroundColourId, colour(tokens::bg));
    setColour(juce::TextEditor::textColourId, colour(tokens::green));
    setColour(juce::TextEditor::highlightColourId,
              colour(tokens::purple).withAlpha(0.35f));
    setColour(juce::TextEditor::outlineColourId, colour(tokens::lineStrong));
    setColour(juce::TextEditor::focusedOutlineColourId, colour(tokens::green));
    setColour(juce::CaretComponent::caretColourId, colour(tokens::green));
    setColour(juce::ScrollBar::thumbColourId, colour(tokens::lineStrong));
    setColour(juce::ScrollBar::trackColourId, colour(tokens::bg));
}

void P1LookAndFeel::drawComboBox(juce::Graphics& g, int width, int height,
                                 bool, int, int, int, int, juce::ComboBox&)
{
    g.fillAll(colour(tokens::bg));
    g.setColour(colour(tokens::lineStrong));
    g.drawRect(0, 0, width, height, tokens::width);
    const float arrowX = static_cast<float>(width - 10);
    const float arrowY = static_cast<float>(height) * 0.5f;
    juce::Path arrow;
    arrow.startNewSubPath(arrowX - 3.0f, arrowY - 2.0f);
    arrow.lineTo(arrowX, arrowY + 2.0f);
    arrow.lineTo(arrowX + 3.0f, arrowY - 2.0f);
    g.setColour(colour(tokens::faint));
    g.strokePath(arrow, juce::PathStrokeType(1.0f));
}

void P1LookAndFeel::positionComboBoxText(juce::ComboBox& box, juce::Label& label)
{
    label.setFont(uiFont(tokens::textSelect));
    label.setColour(juce::Label::textColourId, colour(tokens::dim));
    label.setJustificationType(juce::Justification::centredLeft);
    label.setBounds(5, 0, box.getWidth() - 12, box.getHeight());
}

void P1LookAndFeel::drawPopupMenuBackground(juce::Graphics& g, int width,
                                            int height)
{
    g.fillAll(colour(tokens::popup));
    g.setColour(colour(tokens::lineStrong));
    g.drawRect(0, 0, width, height, tokens::width);
}

void P1LookAndFeel::drawPopupMenuItem(
    juce::Graphics& g, const juce::Rectangle<int>& area, bool isSeparator,
    bool isActive, bool isHighlighted, bool isTicked, bool, const juce::String& text,
    const juce::String&, const juce::Drawable*, const juce::Colour*)
{
    if (isSeparator) {
        g.setColour(colour(tokens::lineStrong));
        g.fillRect(area.reduced(5, 0).withHeight(1));
        return;
    }
    if (isHighlighted)
        g.fillAll(colour(tokens::purple).withAlpha(0.16f));
    g.setFont(uiFont(tokens::textButton));
    g.setColour(isActive ? (isHighlighted ? colour(tokens::purple)
                                          : colour(tokens::text))
                         : colour(tokens::faint));
    auto textArea = area.withTrimmedLeft(8);
    if (isTicked)
        textArea.removeFromLeft(12);
    g.drawText(text, textArea, juce::Justification::centredLeft, true);
    if (isTicked) {
        g.setColour(colour(tokens::green));
        g.fillRect(area.getX() + 5, area.getCentreY() - 2, 4, 4);
    }
}

void P1LookAndFeel::drawScrollbar(juce::Graphics& g, juce::ScrollBar&, int,
                                  int, int width, int height, bool isScrollbarVertical,
                                  int thumbStartPosition, int thumbSize, bool,
                                  bool)
{
    g.fillAll(colour(tokens::bg));
    g.setColour(colour(tokens::lineStrong));
    if (isScrollbarVertical)
        g.fillRect(width / 2, thumbStartPosition, 2, thumbSize);
    else
        g.fillRect(thumbStartPosition, height / 2, thumbSize, 2);
}

void KnobLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width,
                                       int height, float sliderPos,
                                       float rotaryStartAngle,
                                       float rotaryEndAngle, juce::Slider& slider)
{
    const auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat();
    const auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.5f - 3.0f;
    const auto centre = bounds.getCentre();
    const auto endAngle = rotaryStartAngle +
                          sliderPos * (rotaryEndAngle - rotaryStartAngle);

    juce::Path track;
    track.addCentredArc(centre.x, centre.y, radius, radius, 0.0f, rotaryStartAngle,
                        rotaryEndAngle, true);
    g.setColour(colour(tokens::lineStrong));
    g.strokePath(track, juce::PathStrokeType(3.0f));

    if (sliderPos > 0.001f) {
        juce::Path valueArc;
        valueArc.addCentredArc(centre.x, centre.y, radius, radius, 0.0f,
                               rotaryStartAngle, endAngle, true);
        g.setColour(colour(tokens::pink));
        g.strokePath(valueArc, juce::PathStrokeType(3.0f));
    }

    const auto pointerLength = radius - 4.0f;
    const auto pointerX = centre.x + pointerLength * std::sin(endAngle);
    const auto pointerY = centre.y - pointerLength * std::cos(endAngle);
    g.setColour(colour(tokens::white));
    g.drawLine(centre.x, centre.y, pointerX, pointerY, 2.0f);

    if (!slider.isEnabled()) {
        g.setColour(colour(tokens::bg).withAlpha(0.55f));
        g.fillEllipse(bounds);
    }
}

} // namespace p1::ui
