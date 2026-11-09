/* SPDX-License-Identifier: GPL-3.0-only */
#pragma once

#include <memory>

#include "PluginEditor.h"

namespace p1::ui {

inline void setParam(PluginEditor& editor, const juce::String& id, double value)
{
    if (auto* parameter = editor.state().getParameter(id))
        parameter->setValueNotifyingHost(
            parameter->convertTo0to1(static_cast<float>(value)));
}

inline double getParam(PluginEditor& editor, const juce::String& id)
{
    if (auto* value = editor.state().getRawParameterValue(id))
        return value->load();
    return 0.0;
}

class InlineMuteSwitch : public juce::Component {
public:
    InlineMuteSwitch(std::function<bool()> getMuted,
                     std::function<void(bool)> setMuted)
        : getMuted_(std::move(getMuted)), setMuted_(std::move(setMuted))
    {
        setMouseCursor(juce::MouseCursor::PointingHandCursor);
    }

    void paint(juce::Graphics& g) override
    {
        const bool muted = getMuted_ ? getMuted_() : false;
        g.setFont(displayFont(9.0f));
        g.setColour(colour(muted ? tokens::faint : tokens::pink));
        g.drawText("active", 0, 0, 34, getHeight(),
                   juce::Justification::centredLeft, false);
        const auto track = juce::Rectangle<float>(37.0f, (getHeight() - 12.0f) * 0.5f,
                                                  26.0f, 12.0f);
        g.setColour(colour(tokens::bg));
        g.fillRect(track);
        g.setColour(colour(tokens::lineStrong));
        g.drawRect(track, 1.0f);
        g.setColour(colour(muted ? tokens::faint : tokens::pink));
        g.fillRect(juce::Rectangle<float>(muted ? track.getRight() - 11.0f
                                                : track.getX() + 1.0f,
                                          track.getY() + 1.0f, 10.0f, 8.0f));
        g.setColour(colour(muted ? tokens::pink : tokens::faint));
        g.drawText("mute", 68, 0, 34, getHeight(),
                   juce::Justification::centredLeft, false);
    }

    void mouseUp(const juce::MouseEvent&) override
    {
        if (setMuted_)
            setMuted_(!(getMuted_ && getMuted_()));
        repaint();
    }

private:
    std::function<bool()> getMuted_;
    std::function<void(bool)> setMuted_;
};

std::unique_ptr<PageBase> makePatchPage(PluginEditor& editor);
std::unique_ptr<PageBase> makeOperatorsPage(PluginEditor& editor);
std::unique_ptr<PageBase> makeFxModPage(PluginEditor& editor);

} // namespace p1::ui
