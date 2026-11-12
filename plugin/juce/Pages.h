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
        const auto track = juce::Rectangle<float>(
            0.0f, (static_cast<float>(getHeight()) - 12.0f) * 0.5f, 26.0f,
            12.0f);
        g.setColour(colour(tokens::bg));
        g.fillRect(track);
        g.setColour(colour(muted ? tokens::lineStrong : tokens::pink));
        g.drawRect(track, 1.0f);
        g.setColour(colour(muted ? tokens::lineStrong : tokens::pink));
        g.fillRect(juce::Rectangle<float>(muted ? track.getRight() - 11.0f
                                                : track.getX() + 1.0f,
                                          track.getY() + 1.0f, 10.0f, 8.0f));
        g.setFont(uiFont(tokens::textLegend));
        g.setColour(colour(tokens::pink));
        g.drawText(muted ? "mute" : "active", 32, 0, getWidth() - 32,
                   getHeight(), juce::Justification::centredLeft, true);
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

class CardHeader : public juce::Component {
public:
    CardHeader(bool partLayout, bool withRename,
               std::function<void()> onRename, std::function<void()> onCopy,
               std::function<void()> onPaste,
               std::function<bool()> getMuted,
               std::function<void(bool)> setMuted)
        : partLayout_(partLayout), withRename_(withRename)
    {
        rename_.onClick = std::move(onRename);
        copy_.onClick = std::move(onCopy);
        paste_.onClick = std::move(onPaste);
        addAndMakeVisible(rename_);
        addAndMakeVisible(copy_);
        addAndMakeVisible(paste_);
        rename_.setVisible(withRename_);
        mute_ = std::make_unique<InlineMuteSwitch>(std::move(getMuted),
                                                   std::move(setMuted));
        addAndMakeVisible(*mute_);
    }

    void resized() override
    {
        auto bar = getLocalBounds();
        if (partLayout_) {
            mute_->setBounds(bar.removeFromRight(108).reduced(0, 3));
            auto actions = bar.removeFromRight(172);
            actions.removeFromRight(16);
            rename_.setBounds(actions.removeFromLeft(52).reduced(2, 3));
            copy_.setBounds(actions.removeFromLeft(52).reduced(2, 3));
            paste_.setBounds(actions.removeFromLeft(52).reduced(2, 3));
        } else {
            bar.removeFromLeft(56);
            copy_.setBounds(bar.removeFromLeft(52).reduced(2, 3));
            paste_.setBounds(bar.removeFromLeft(52).reduced(2, 3));
            mute_->setBounds(bar.removeFromRight(104).reduced(0, 3));
        }
    }

    void tick() { mute_->repaint(); }

private:
    bool partLayout_ = true;
    bool withRename_ = true;
    P1Button rename_{"rename", true};
    P1Button copy_{"copy", true};
    P1Button paste_{"paste", true};
    std::unique_ptr<InlineMuteSwitch> mute_;
};

std::unique_ptr<PageBase> makePatchPage(PluginEditor& editor);
std::unique_ptr<PageBase> makeOperatorsPage(PluginEditor& editor);
std::unique_ptr<PageBase> makeFxModPage(PluginEditor& editor);
std::unique_ptr<PageBase> makeFeedbackPage(PluginEditor& editor);

} // namespace p1::ui
