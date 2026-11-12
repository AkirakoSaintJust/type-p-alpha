/* SPDX-License-Identifier: GPL-3.0-only */
#pragma once

#include <memory>

#include "ParamIDs.h"
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

class OscillatorCard : public P1Card {
public:
    OscillatorCard(PluginEditor& editor, int op, int part)
        : P1Card("oscillator",
                 [&ed = editor, op, part] {
                     ed.copyOperatorSection(
                         op, part, PluginEditor::OperatorSection::oscillator);
                 },
                 [&ed = editor, op, part] {
                     ed.pasteOperatorSection(
                         op, part, PluginEditor::OperatorSection::oscillator);
                 }),
          ratio_(editor.state(), params::opRatio(op, part), "ratio", 28.0f),
          detune_(editor.state(), params::opDetune(op, part), "detune", 28.0f),
          level_(editor.state(), params::opLevel(op, part), "level", 28.0f),
          feedback_(editor.state(), params::opFeedback(op, part), "feedback",
                    28.0f),
          wave_(editor.state(), params::opWaveform(op, part),
                params::waveNames())
    {
        addAndMakeVisible(ratio_);
        addAndMakeVisible(detune_);
        addAndMakeVisible(level_);
        addAndMakeVisible(feedback_);
        addAndMakeVisible(wave_);
    }

    void resized() override
    {
        P1Card::resized();
        auto area = contentArea();
        area.removeFromTop(8);
        auto row = area.removeFromTop(70);
        const int cell = row.getWidth() / 4;
        ratio_.setBounds(row.removeFromLeft(cell));
        detune_.setBounds(row.removeFromLeft(cell));
        level_.setBounds(row.removeFromLeft(cell));
        feedback_.setBounds(row);
        wave_.setBounds(area.removeFromTop(18).withTrimmedTop(2).withWidth(150));
    }

private:
    ParamKnob ratio_;
    ParamKnob detune_;
    ParamKnob level_;
    ParamKnob feedback_;
    ParamChoice wave_;
};

class AmplitudeEnvelopeCard : public P1Card {
public:
    AmplitudeEnvelopeCard(PluginEditor& editor, int op, int part)
        : P1Card("amp env",
                 [&ed = editor, op, part] {
                     ed.copyOperatorSection(
                         op, part,
                         PluginEditor::OperatorSection::amplitudeEnvelope);
                 },
                 [&ed = editor, op, part] {
                     ed.pasteOperatorSection(
                         op, part,
                         PluginEditor::OperatorSection::amplitudeEnvelope);
                 }),
          envView_(editor.state(),
                   {params::opEnvRate(op, 0, part), params::opEnvRate(op, 1, part),
                    params::opEnvRate(op, 2, part), params::opEnvRate(op, 3, part)},
                   {params::opEnvLevel(op, 0, part), params::opEnvLevel(op, 1, part),
                    params::opEnvLevel(op, 2, part)},
                   colour(tokens::purple), 0.0, 99.0, false)
    {
        addAndMakeVisible(envView_);
        for (int i = 0; i < 4; ++i) {
            rates_[static_cast<std::size_t>(i)] = std::make_unique<ParamKnob>(
                editor.state(), params::opEnvRate(op, i, part),
                "r" + juce::String(i + 1), 28.0f);
            addAndMakeVisible(*rates_[static_cast<std::size_t>(i)]);
        }
        for (int i = 0; i < 3; ++i) {
            levels_[static_cast<std::size_t>(i)] = std::make_unique<ParamKnob>(
                editor.state(), params::opEnvLevel(op, i, part),
                "l" + juce::String(i + 1), 28.0f);
            addAndMakeVisible(*levels_[static_cast<std::size_t>(i)]);
        }
    }

    void resized() override
    {
        P1Card::resized();
        auto area = contentArea();
        area.removeFromTop(8);
        envView_.setBounds(area.removeFromTop(40));
        auto rates = area.removeFromTop(64);
        const int rateCell = rates.getWidth() / 4;
        for (auto& knob : rates_)
            knob->setBounds(rates.removeFromLeft(rateCell));
        auto levels = area.removeFromTop(64);
        const int levelCell = levels.getWidth() / 3;
        for (auto& knob : levels_)
            knob->setBounds(levels.removeFromLeft(levelCell));
    }

    void tick() { envView_.repaint(); }

private:
    P1Envelope envView_;
    std::array<std::unique_ptr<ParamKnob>, 4> rates_{};
    std::array<std::unique_ptr<ParamKnob>, 3> levels_{};
};

class FeedbackEnvelopeCard : public P1Card {
public:
    FeedbackEnvelopeCard(PluginEditor& editor, int op, int part)
        : P1Card("fb env",
                 [&ed = editor, op, part] {
                     ed.copyOperatorSection(
                         op, part,
                         PluginEditor::OperatorSection::feedbackEnvelope);
                 },
                 [&ed = editor, op, part] {
                     ed.pasteOperatorSection(
                         op, part,
                         PluginEditor::OperatorSection::feedbackEnvelope);
                 }),
          envView_(editor.state(),
                   {params::opFeedbackRate(op, 0, part),
                    params::opFeedbackRate(op, 1, part),
                    params::opFeedbackRate(op, 2, part),
                    params::opFeedbackRate(op, 3, part)},
                   {params::opFeedbackLevel(op, 0, part),
                    params::opFeedbackLevel(op, 1, part),
                    params::opFeedbackLevel(op, 2, part),
                    params::opFeedbackLevel(op, 3, part)},
                   colour(tokens::purple), -32768.0, 32767.0, true),
          on_(editor.state(), params::opFeedbackOn(op, part), "on")
    {
        addAndMakeVisible(envView_);
        addAndMakeVisible(on_);
        for (int i = 0; i < 4; ++i) {
            rates_[static_cast<std::size_t>(i)] = std::make_unique<ParamKnob>(
                editor.state(), params::opFeedbackRate(op, i, part),
                "r" + juce::String(i + 1), 28.0f);
            addAndMakeVisible(*rates_[static_cast<std::size_t>(i)]);
        }
        for (int i = 0; i < 4; ++i) {
            levels_[static_cast<std::size_t>(i)] = std::make_unique<ParamKnob>(
                editor.state(), params::opFeedbackLevel(op, i, part),
                "l" + juce::String(i + 1), 28.0f);
            addAndMakeVisible(*levels_[static_cast<std::size_t>(i)]);
        }
        setHeaderTrailingWidth(48);
    }

    void resized() override
    {
        P1Card::resized();
        auto area = contentArea();
        area.removeFromTop(8);
        auto top = area.removeFromTop(64);
        envView_.setBounds(top);
        auto rates = area.removeFromTop(64);
        const int rateCell = rates.getWidth() / 4;
        for (auto& knob : rates_)
            knob->setBounds(rates.removeFromLeft(rateCell));
        auto levels = area.removeFromTop(64);
        const int levelCell = levels.getWidth() / 4;
        for (auto& knob : levels_)
            knob->setBounds(levels.removeFromLeft(levelCell));
        on_.setBounds(headerTrailingBounds());
    }

    void tick() { envView_.repaint(); }

private:
    P1Envelope envView_;
    P1Toggle on_;
    std::array<std::unique_ptr<ParamKnob>, 4> rates_{};
    std::array<std::unique_ptr<ParamKnob>, 4> levels_{};
};

class LfoCard : public P1Card {
public:
    LfoCard(PluginEditor& editor, juce::String title, juce::String rateID,
            juce::String depthID, juce::String waveID, juce::String modeID,
            int op, int part, PluginEditor::OperatorSection section)
        : P1Card(std::move(title),
                 [&ed = editor, op, part, section] {
                     ed.copyOperatorSection(op, part, section);
                 },
                 [&ed = editor, op, part, section] {
                     ed.pasteOperatorSection(op, part, section);
                 }),
          rate_(editor.state(), std::move(rateID), "rate", 28.0f),
          depth_(editor.state(), std::move(depthID), "depth", 28.0f),
          wave_(editor.state(), std::move(waveID), params::lfoWaveNames()),
          mode_(editor.state(), std::move(modeID), "bipolar")
    {
        addAndMakeVisible(rate_);
        addAndMakeVisible(depth_);
        addAndMakeVisible(wave_);
        addAndMakeVisible(mode_);
    }

    void resized() override
    {
        P1Card::resized();
        auto area = contentArea();
        area.removeFromTop(8);
        auto row = area.removeFromTop(70);
        rate_.setBounds(row.removeFromLeft(row.getWidth() / 2));
        depth_.setBounds(row);
        auto bottom = area.removeFromTop(18).withTrimmedTop(2);
        wave_.setBounds(bottom.removeFromLeft(140));
        bottom.removeFromLeft(10);
        mode_.setBounds(bottom.removeFromLeft(110));
    }

private:
    ParamKnob rate_;
    ParamKnob depth_;
    ParamChoice wave_;
    P1Toggle mode_;
};

std::unique_ptr<PageBase> makePatchPage(PluginEditor& editor);
std::unique_ptr<PageBase> makeOperatorsPage(PluginEditor& editor);
std::unique_ptr<PageBase> makeFxModPage(PluginEditor& editor);
std::unique_ptr<PageBase> makeOperatorLfosPage(PluginEditor& editor);

} // namespace p1::ui
