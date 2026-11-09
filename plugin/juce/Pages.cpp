/* SPDX-License-Identifier: GPL-3.0-only */
#include "Pages.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <map>

#include "ParamIDs.h"

namespace p1::ui {
namespace {

constexpr int kPageWidth = 1206;
constexpr int kPageHeight = 685;

void setParam(PluginEditor& editor, const juce::String& id, double value)
{
    if (auto* parameter = editor.state().getParameter(id))
        parameter->setValueNotifyingHost(parameter->convertTo0to1(
            static_cast<float>(value)));
}

double getParam(PluginEditor& editor, const juce::String& id)
{
    if (auto* value = editor.state().getRawParameterValue(id))
        return value->load();
    return 0.0;
}

class ModeButton : public juce::Button {
public:
    explicit ModeButton(const juce::String& label) : juce::Button(label)
    {
        setMouseCursor(juce::MouseCursor::PointingHandCursor);
    }

    void setActive(bool active)
    {
        if (active_ != active) {
            active_ = active;
            repaint();
        }
    }

    void paintButton(juce::Graphics& g, bool highlighted, bool) override
    {
        g.setColour(colour(tokens::bg));
        g.fillRect(getLocalBounds());
        g.setColour(colour(active_ ? tokens::green
                                   : highlighted ? tokens::pink
                                                 : tokens::lineStrong));
        g.drawRect(getLocalBounds(), 1);
        g.setFont(uiFont(tokens::textButton));
        g.setColour(colour(active_ ? tokens::green
                                   : highlighted ? tokens::white
                                                 : tokens::dim));
        g.drawText(getButtonText(), getLocalBounds(),
                   juce::Justification::centred, false);
    }

private:
    bool active_ = false;
};

class PartCard : public juce::Component {
public:
    PartCard(PluginEditor& editor, int part, bool active)
        : editor_(editor), part_(part), active_(active)
    {
        addAndMakeVisible(rename_);
        addAndMakeVisible(copy_);
        addAndMakeVisible(paste_);
        rename_.onClick = [this] { editor_.showRenameDialog(true, part_); };
        copy_.onClick = [this] { editor_.copyPart(part_); };
        paste_.onClick = [this] { editor_.pastePart(part_); };
        mute_ = std::make_unique<InlineMuteSwitch>(
            [this] {
                return getParam(editor_, params::level(part_)) <= 0.0;
            },
            [this](bool muted) { editor_.togglePartMute(part_, muted); });
        addAndMakeVisible(*mute_);

        const std::array<juce::String, 6> labels = {
            "midi channel", "keyzone start", "keyzone end", "tune", "transpose",
            "delay"};
        const std::array<juce::String, 6> ids = {
            params::channel(part_), params::keyLow(part_), params::keyHigh(part_),
            params::detune(part_), params::transpose(part_), params::delay(part_)};
        for (size_t i = 0; i < knobs_.size(); ++i) {
            knobs_[i] = std::make_unique<ParamKnob>(editor_.state(), ids[i],
                                                    labels[i], 38.0f);
            addAndMakeVisible(*knobs_[i]);
        }
        level_ = std::make_unique<P1LevelSlider>(editor_.state(),
                                                 params::level(part_));
        addAndMakeVisible(*level_);
        addAndMakeVisible(scope_);
        addAndMakeVisible(spectro_);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(10);
        auto header = area.removeFromTop(24);
        mute_->setBounds(header.removeFromRight(108).reduced(0, 3));
        auto actions = header.removeFromRight(172);
        actions.removeFromRight(16);
        rename_.setBounds(actions.removeFromLeft(52).reduced(2, 3));
        copy_.setBounds(actions.removeFromLeft(52).reduced(2, 3));
        paste_.setBounds(actions.removeFromLeft(52).reduced(2, 3));
        auto knobRow = area.removeFromTop(92);
        const int cell = knobRow.getWidth() / static_cast<int>(knobs_.size());
        for (auto& knob : knobs_)
            knob->setBounds(knobRow.removeFromLeft(cell));
        level_->setBounds(area.removeFromTop(38).reduced(4, 0));
        area.removeFromTop(6);
        const int visualWidth = (area.getWidth() - 8) / 2;
        scope_.setBounds(area.removeFromLeft(visualWidth));
        area.removeFromLeft(8);
        spectro_.setBounds(area);
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(colour(tokens::bg));
        if (active_) {
            g.setColour(colour(tokens::green));
            g.drawRect(getLocalBounds(), 1);
        }
        g.setFont(displayFont(tokens::textPartName));
        g.setColour(colour(active_ ? tokens::green : tokens::text));
        g.drawText("part " + juce::String(part_ + 1) + ": " +
                       editor_.processor().getPartName(part_),
                   getLocalBounds().reduced(12).removeFromTop(20),
                   juce::Justification::centredLeft, false);
    }

    void tick()
    {
        editor_.processor().readTapScope(8 + part_, scratch_.data(),
                                         static_cast<int>(scratch_.size()));
        scope_.setSamples(scratch_.data(), static_cast<int>(scratch_.size()));
        spectro_.setSamples(scratch_.data(), static_cast<int>(scratch_.size()));
        mute_->repaint();
        repaint();
    }

private:
    PluginEditor& editor_;
    int part_ = 0;
    bool active_ = false;
    P1Button rename_{"rename", true};
    P1Button copy_{"copy", true};
    P1Button paste_{"paste", true};
    std::unique_ptr<InlineMuteSwitch> mute_;
    std::array<std::unique_ptr<ParamKnob>, 6> knobs_{};
    std::unique_ptr<P1LevelSlider> level_;
    P1Scope scope_;
    P1Spectro spectro_;
    std::array<float, 512> scratch_{};
};

class VoiceMode : public juce::Component {
public:
    explicit VoiceMode(PluginEditor& editor)
        : editor_(editor), part_(editor.activePart())
    {
        for (int i = 0; i < 4; ++i) {
            modes_[static_cast<size_t>(i)] =
                std::make_unique<ModeButton>(modeNames_[static_cast<size_t>(i)]);
            addAndMakeVisible(*modes_[static_cast<size_t>(i)]);
        }
        modes_[0]->onClick = [this] { setParam(editor_, params::mode(part_), 0); };
        modes_[1]->onClick = [this] { setParam(editor_, params::mode(part_), 1); };
        modes_[2]->onClick = [this] {
            setParam(editor_, params::legato(part_),
                     getParam(editor_, params::legato(part_)) > 0.5 ? 0 : 1);
        };
        modes_[3]->onClick = [this] {
            setParam(editor_, params::unisonCount(part_),
                     getParam(editor_, params::unisonCount(part_)) > 1.5 ? 1 : 2);
        };
        unison_ = std::make_unique<ParamKnob>(
            editor_.state(), params::unisonCount(part_), "unison", 42.0f);
        detune_ = std::make_unique<ParamKnob>(
            editor_.state(), params::unisonDetune(part_), "detune", 42.0f);
        porta_ = std::make_unique<ParamKnob>(
            editor_.state(), params::portamento(part_), "porta", 42.0f);
        addAndMakeVisible(*unison_);
        addAndMakeVisible(*detune_);
        addAndMakeVisible(*porta_);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(10);
        area.removeFromTop(20);
        auto row = area.removeFromTop(28);
        for (auto& mode : modes_)
            mode->setBounds(row.removeFromLeft(58).reduced(1, 2));
        /* Sit the three knobs next to the mode buttons rather than at the card
           edge, with a breathing gap so the groups stay separate. */
        const int x = 10 + 4 * 58 + 32;
        unison_->setBounds(x, 34, 74, 76);
        detune_->setBounds(x + 78, 34, 74, 76);
        porta_->setBounds(x + 156, 34, 74, 76);
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(colour(tokens::bg));
        g.setFont(displayFont(tokens::textSectionTitle));
        g.setColour(colour(tokens::pink));
        g.drawText("voice mode", 10, 4, 160, 18,
                   juce::Justification::centredLeft, false);
    }

    void tick()
    {
        const bool mono = getParam(editor_, params::mode(part_)) > 0.5;
        modes_[0]->setActive(!mono);
        modes_[1]->setActive(mono);
        modes_[2]->setActive(mono &&
                             getParam(editor_, params::legato(part_)) > 0.5);
        modes_[3]->setActive(mono &&
                             getParam(editor_, params::unisonCount(part_)) > 1.5);
        for (int i = 2; i < 4; ++i) {
            modes_[static_cast<size_t>(i)]->setEnabled(mono);
            modes_[static_cast<size_t>(i)]->setAlpha(mono ? 1.0f : 0.35f);
        }
        unison_->setEnabled(mono);
        detune_->setEnabled(mono);
        porta_->setEnabled(mono);
        unison_->setAlpha(mono ? 1.0f : 0.35f);
        detune_->setAlpha(mono ? 1.0f : 0.35f);
        porta_->setAlpha(mono ? 1.0f : 0.35f);
    }

private:
    PluginEditor& editor_;
    int part_ = 0;
    std::array<juce::String, 4> modeNames_{{"poly", "mono", "legato", "unison"}};
    std::array<std::unique_ptr<ModeButton>, 4> modes_{};
    std::unique_ptr<ParamKnob> unison_;
    std::unique_ptr<ParamKnob> detune_;
    std::unique_ptr<ParamKnob> porta_;
};

class PatchPage : public PageBase {
public:
    explicit PatchPage(PluginEditor& editor) : editor_(editor)
    {
        for (int i = 0; i < 2; ++i) {
            parts_[static_cast<size_t>(i)] = std::make_unique<PartCard>(
                editor, i, editor.activePart() == i);
            addAndMakeVisible(*parts_[static_cast<size_t>(i)]);
        }
        voice_ = std::make_unique<VoiceMode>(editor);
        addAndMakeVisible(*voice_);
    }

    void resized() override
    {
        auto area = getLocalBounds();
        auto parts = area.removeFromTop(550);
        const int width = (parts.getWidth() - 10) / 2;
        parts_[0]->setBounds(parts.removeFromLeft(width));
        parts.removeFromLeft(10);
        parts_[1]->setBounds(parts);
        area.removeFromTop(10);
        voice_->setBounds(area);
    }

    void tick() override
    {
        for (auto& part : parts_)
            part->tick();
        voice_->tick();
    }

private:
    PluginEditor& editor_;
    std::array<std::unique_ptr<PartCard>, 2> parts_{};
    std::unique_ptr<VoiceMode> voice_;
};

} // namespace

std::unique_ptr<PageBase> makePatchPage(PluginEditor& editor)
{
    return std::make_unique<PatchPage>(editor);
}

} // namespace p1::ui
