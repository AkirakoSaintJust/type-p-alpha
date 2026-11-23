/* SPDX-License-Identifier: GPL-3.0-only */
#include "Pages.h"

#include <array>

#include "ParamIDs.h"

namespace p1::ui {
namespace {

class FilterCard : public P1Card {
public:
    FilterCard(PluginEditor& editor, juce::String title, juce::String cutoffID,
               juce::String resID, juce::String typeID, juce::String onID,
               std::function<void()> onToPart = {},
               juce::String toLabel = "to 2")
        : P1Card(std::move(title)),
          cutoff_(editor.state(), std::move(cutoffID), "cutoff", 42.0f),
          res_(editor.state(), std::move(resID), "res", 42.0f),
          type_(editor.state(), std::move(typeID), {"lpf", "bpf", "hpf"}),
          on_(editor.state(), std::move(onID), "on"),
          to_(std::move(toLabel), true)
    {
        addAndMakeVisible(cutoff_);
        addAndMakeVisible(res_);
        addAndMakeVisible(type_);
        addAndMakeVisible(on_);
        to_.onClick = std::move(onToPart);
        addAndMakeVisible(to_);
        setHeaderTrailingWidth(48);
    }

    void resized() override
    {
        auto area = contentArea();
        auto row = area.removeFromTop(84);
        const int cell = row.getWidth() / 2;
        cutoff_.setBounds(row.removeFromLeft(cell));
        res_.setBounds(row.removeFromLeft(cell));
        type_.setBounds(44, 118, 150, 18);
        const auto onBounds = headerTrailingBounds();
        on_.setBounds(onBounds);
        to_.setBounds(onBounds.getX() - 16 - 32, onBounds.getY(), 32, 18);
    }

    void paint(juce::Graphics& g) override
    {
        P1Card::paint(g);
        g.setFont(uiFont(tokens::textButton));
        g.setColour(colour(tokens::dim));
        g.drawText("type", 10, 118, 32, 18, juce::Justification::centredLeft, false);
    }

private:
    ParamKnob cutoff_;
    ParamKnob res_;
    ParamChoice type_;
    P1Toggle on_;
    P1Button to_;
};

class FilterEnvelopeCard : public P1Card {
public:
    FilterEnvelopeCard(PluginEditor& editor, int part,
                       std::function<void()> onToPart = {},
                       juce::String toLabel = "to 2")
        : P1Card("filter envelope"),
          part_(part),
          envView_(editor.state(),
                   {params::filterRate(0, part), params::filterRate(1, part),
                    params::filterRate(2, part),
                    params::filterEnvRelRate(part)},
                   {params::filterLevel(0, part), params::filterLevel(1, part),
                    params::filterLevel(2, part),
                    params::filterEnvRelLevel(part)},
                   colour(tokens::green), 0.0, 99.0, false,
                   EnvelopeShape::bipolar, params::filterEnvMode(part),
                   params::filterCutoff(part), 99.0 / 55296.0),
          on_(editor.state(), params::filterEnvOn(part), "on"),
          mode_(editor.state(), params::filterEnvMode(part),
                {"sustain off", "sustain on", "loop 1", "loop 2"}),
          to_(std::move(toLabel), true)
    {
        addAndMakeVisible(envView_);
        addAndMakeVisible(on_);
        addAndMakeVisible(mode_);
        to_.onClick = std::move(onToPart);
        addAndMakeVisible(to_);
        for (int i = 0; i < 3; ++i) {
            rates_[static_cast<size_t>(i)] = std::make_unique<ParamKnob>(
                editor.state(), params::filterRate(i, part),
                "r" + juce::String(i + 1), 42.0f);
            addAndMakeVisible(*rates_[static_cast<size_t>(i)]);
        }
        rates_[3] = std::make_unique<ParamKnob>(
            editor.state(), params::filterEnvRelRate(part), "rel rate",
            42.0f);
        addAndMakeVisible(*rates_[3]);
        for (int i = 0; i < 3; ++i) {
            levels_[static_cast<size_t>(i)] = std::make_unique<ParamKnob>(
                editor.state(), params::filterLevel(i, part),
                "l" + juce::String(i + 1), 42.0f);
            addAndMakeVisible(*levels_[static_cast<size_t>(i)]);
        }
        levels_[3] = std::make_unique<ParamKnob>(
            editor.state(), params::filterEnvRelLevel(part), "rl", 42.0f);
        addAndMakeVisible(*levels_[3]);
    }

    void resized() override
    {
        auto area = contentArea();
        envView_.setBounds(area.removeFromTop(56));
        area.removeFromTop(4);
        auto row = area.removeFromTop(84);
        const int cell = row.getWidth() / 4;
        for (auto& knob : rates_)
            knob->setBounds(row.removeFromLeft(cell));
        auto levelRow = area.removeFromTop(84);
        const int levelCell = levelRow.getWidth() / 4;
        for (auto& knob : levels_)
            knob->setBounds(levelRow.removeFromLeft(levelCell));
        auto modeRow = area.removeFromTop(84);
        mode_.setBounds(modeRow.removeFromLeft(levelCell * 2)
                            .withSizeKeepingCentre(levelCell * 2 - 8, 24));
        on_.setBounds(getWidth() - 56, 1, 48, 18);
        to_.setBounds(on_.getX() - 16 - 32, 1, 32, 18);
    }

    void tick() { envView_.repaint(); }

private:
    int part_ = 0;
    P1Envelope envView_;
    P1Toggle on_;
    ParamChoice mode_;
    P1Button to_;
    std::array<std::unique_ptr<ParamKnob>, 4> rates_{};
    std::array<std::unique_ptr<ParamKnob>, 4> levels_{};
};

class PitchLfoCard : public P1Card {
public:
    explicit PitchLfoCard(PluginEditor& editor,
                          std::function<void()> onToPart = {},
                          juce::String toLabel = "to 2")
        : P1Card("pitch lfo"),
          part_(editor.activePart()),
          waveView_(editor.state(), params::pitchWave(part_), colour(tokens::green)),
          rate_(editor.state(), params::pitchRate(part_), "rate", 36.0f),
          depth_(editor.state(), params::pitchDepth(part_), "depth", 36.0f),
          rateFade_(editor.state(), params::pitchRateFade(part_), "rate fade",
                    36.0f),
          depthFade_(editor.state(), params::pitchDepthFade(part_), "depth fade",
                     36.0f),
          wave_(editor.state(), params::pitchWave(part_), params::lfoWaveNames()),
          mode_(editor.state(), params::pitchMode(part_), "bipolar"),
          to_(std::move(toLabel), true)
    {
        addAndMakeVisible(waveView_);
        addAndMakeVisible(rate_);
        addAndMakeVisible(depth_);
        addAndMakeVisible(rateFade_);
        addAndMakeVisible(depthFade_);
        addAndMakeVisible(wave_);
        addAndMakeVisible(mode_);
        to_.onClick = std::move(onToPart);
        addAndMakeVisible(to_);
    }

    void resized() override
    {
        auto area = contentArea();
        to_.setBounds(getWidth() - 8 - 32, 1, 32, 18);
        waveView_.setBounds(area.removeFromTop(88));
        area.removeFromTop(10);
        auto row = area.removeFromTop(84);
        rate_.setBounds(row.removeFromLeft(row.getWidth() / 2));
        depth_.setBounds(row);
        auto fadeRow = area.removeFromTop(80);
        rateFade_.setBounds(fadeRow.removeFromLeft(fadeRow.getWidth() / 2));
        depthFade_.setBounds(fadeRow);
        area.removeFromTop(10);
        auto bottom = area.removeFromTop(18).withTrimmedTop(2);
        wave_.setBounds(bottom.removeFromLeft(150));
        mode_.setBounds(bottom.removeFromLeft(110));
    }

    void tick() { waveView_.repaint(); }

private:
    int part_ = 0;
    P1Wave waveView_;
    ParamKnob rate_;
    ParamKnob depth_;
    ParamKnob rateFade_;
    ParamKnob depthFade_;
    ParamChoice wave_;
    P1Toggle mode_;
    P1Button to_;
};

class PitchEnvelopeCard : public P1Card {
public:
    explicit PitchEnvelopeCard(PluginEditor& editor,
                          std::function<void()> onToPart = {},
                          juce::String toLabel = "to 2")
        : P1Card("pitch envelope"),
          part_(editor.activePart()),
          envView_(editor.state(),
                   {params::pitchEnvRate(0, part_), params::pitchEnvRate(1, part_),
                    params::pitchEnvRate(2, part_),
                    params::pitchEnvRelRate(part_)},
                   {params::pitchEnvLevel(0, part_), params::pitchEnvLevel(1, part_),
                    params::pitchEnvLevel(2, part_),
                    params::pitchEnvRelLevel(part_)},
                   colour(tokens::purple), -32768.0, 32767.0, true,
                   EnvelopeShape::bipolar, params::pitchEnvMode(part_)),
          on_(editor.state(), params::pitchEnvOn(part_), "on"),
          mode_(editor.state(), params::pitchEnvMode(part_),
                {"sustain off", "sustain on", "loop 1", "loop 2"}),
          to_(std::move(toLabel), true)
    {
        addAndMakeVisible(envView_);
        addAndMakeVisible(on_);
        addAndMakeVisible(mode_);
        to_.onClick = std::move(onToPart);
        addAndMakeVisible(to_);
        for (int i = 0; i < 3; ++i) {
            rates_[static_cast<size_t>(i)] = std::make_unique<ParamKnob>(
                editor.state(), params::pitchEnvRate(i, part_),
                "r" + juce::String(i + 1), 42.0f);
            addAndMakeVisible(*rates_[static_cast<size_t>(i)]);
        }
        rates_[3] = std::make_unique<ParamKnob>(
            editor.state(), params::pitchEnvRelRate(part_), "rel rate",
            42.0f);
        addAndMakeVisible(*rates_[3]);
        for (int i = 0; i < 3; ++i) {
            levels_[static_cast<size_t>(i)] = std::make_unique<ParamKnob>(
                editor.state(), params::pitchEnvLevel(i, part_),
                "l" + juce::String(i + 1), 42.0f);
            addAndMakeVisible(*levels_[static_cast<size_t>(i)]);
        }
        levels_[3] = std::make_unique<ParamKnob>(
            editor.state(), params::pitchEnvRelLevel(part_), "rl", 42.0f);
        addAndMakeVisible(*levels_[3]);
    }

    void resized() override
    {
        auto area = contentArea();
        envView_.setBounds(area.removeFromTop(56));
        area.removeFromTop(4);
        auto row = area.removeFromTop(84);
        const int cell = row.getWidth() / 4;
        for (auto& knob : rates_)
            knob->setBounds(row.removeFromLeft(cell));
        auto levelRow = area.removeFromTop(84);
        const int levelCell = levelRow.getWidth() / 4;
        for (auto& knob : levels_)
            knob->setBounds(levelRow.removeFromLeft(levelCell));
        auto modeRow = area.removeFromTop(84);
        mode_.setBounds(modeRow.removeFromLeft(levelCell * 2)
                            .withSizeKeepingCentre(levelCell * 2 - 8, 24));
        on_.setBounds(getWidth() - 56, 1, 48, 18);
        to_.setBounds(on_.getX() - 16 - 32, 1, 32, 18);
    }

    void tick() { envView_.repaint(); }

private:
    int part_ = 0;
    P1Envelope envView_;
    P1Toggle on_;
    ParamChoice mode_;
    P1Button to_;
    std::array<std::unique_ptr<ParamKnob>, 4> rates_{};
    std::array<std::unique_ptr<ParamKnob>, 4> levels_{};
};

class PanEnvelopeCard : public P1Card {
public:
    explicit PanEnvelopeCard(PluginEditor& editor,
                          std::function<void()> onToPart = {},
                          juce::String toLabel = "to 2")
        : P1Card("pan envelope"),
          part_(editor.activePart()),
          envView_(editor.state(),
                   {params::panEnvRate(0, part_), params::panEnvRate(1, part_),
                    params::panEnvRate(2, part_),
                    params::panEnvRelRate(part_)},
                   {params::panEnvLevel(0, part_), params::panEnvLevel(1, part_),
                    params::panEnvLevel(2, part_),
                    params::panEnvRelLevel(part_)},
                   colour(tokens::purple), -99.0, 99.0, true,
                   EnvelopeShape::bipolar, params::panEnvMode(part_),
                   params::pan(part_)),
          on_(editor.state(), params::panEnvOn(part_), "on"),
          mode_(editor.state(), params::panEnvMode(part_),
                {"sustain off", "sustain on", "loop 1", "loop 2"}),
          to_(std::move(toLabel), true)
    {
        addAndMakeVisible(envView_);
        addAndMakeVisible(on_);
        addAndMakeVisible(mode_);
        to_.onClick = std::move(onToPart);
        addAndMakeVisible(to_);
        for (int i = 0; i < 4; ++i) {
            const juce::String id = i < 3 ? params::panEnvRate(i, part_)
                                          : params::panEnvRelRate(part_);
            rates_[static_cast<size_t>(i)] = std::make_unique<ParamKnob>(
                editor.state(), id,
                i < 3 ? "r" + juce::String(i + 1) : "rel rate", 42.0f);
            addAndMakeVisible(*rates_[static_cast<size_t>(i)]);
        }
        for (int i = 0; i < 4; ++i) {
            const juce::String id = i < 3 ? params::panEnvLevel(i, part_)
                                          : params::panEnvRelLevel(part_);
            levels_[static_cast<size_t>(i)] = std::make_unique<ParamKnob>(
                editor.state(), id, i < 3 ? "l" + juce::String(i + 1) : "rl",
                42.0f);
            addAndMakeVisible(*levels_[static_cast<size_t>(i)]);
        }
    }

    void resized() override
    {
        auto area = contentArea();
        envView_.setBounds(area.removeFromTop(56));
        area.removeFromTop(4);
        auto row = area.removeFromTop(84);
        const int cell = row.getWidth() / 4;
        for (auto& knob : rates_)
            knob->setBounds(row.removeFromLeft(cell));
        auto levelRow = area.removeFromTop(84);
        const int levelCell = levelRow.getWidth() / 4;
        for (auto& knob : levels_)
            knob->setBounds(levelRow.removeFromLeft(levelCell));
        auto modeRow = area.removeFromTop(84);
        mode_.setBounds(modeRow.removeFromLeft(cell * 2)
                            .withSizeKeepingCentre(cell * 2 - 8, 24));
        on_.setBounds(getWidth() - 56, 1, 48, 18);
        to_.setBounds(on_.getX() - 16 - 32, 1, 32, 18);
    }

    void tick() { envView_.repaint(); }

private:
    int part_ = 0;
    P1Envelope envView_;
    P1Toggle on_;
    ParamChoice mode_;
    P1Button to_;
    std::array<std::unique_ptr<ParamKnob>, 4> rates_{};
    std::array<std::unique_ptr<ParamKnob>, 4> levels_{};
};

class FollowCard : public P1Card {
public:
    FollowCard(PluginEditor& editor, bool key, juce::String title,
               std::function<void()> onToPart = {},
               juce::String toLabel = "to 2")
        : P1Card(std::move(title)),
          editor_(editor),
          key_(key),
          part_(editor.activePart()),
          targets_(params::followTargets(part_)),
          on_(editor.state(), key ? "key_sens_on" : "vel_sens_on", "on"),
          to_(std::move(toLabel), true)
    {
        wipe_.setButtonText("wipe all");
        target_.addItemList(targetItems(), 1);
        target_.setSelectedItemIndex(0, juce::dontSendNotification);
        target_.onChange = [this] {
            selected_ = target_.getSelectedItemIndex();
            updateTarget();
        };
        addAndMakeVisible(target_);
        addAndMakeVisible(on_);
        wipe_.onClick = [this] { wipe(); };
        addAndMakeVisible(wipe_);
        to_.onClick = std::move(onToPart);
        addAndMakeVisible(to_);
        if (key_) {
            breakpoint_ = std::make_unique<ParamKnob>(
                editor.state(), "breakpoint", "breakpoint", 42.0f);
            addAndMakeVisible(*breakpoint_);
        }
        below_ = std::make_unique<ParamKnob>(
            editor.state(), targets_[0].keyLo, "below", 42.0f);
        above_ = std::make_unique<ParamKnob>(
            editor.state(), targets_[0].keyHi, "above", 42.0f);
        amount_ = std::make_unique<ParamKnob>(
            editor.state(), targets_[0].velocity, "amount", 42.0f);
        if (key_) {
            addAndMakeVisible(*below_);
            addAndMakeVisible(*above_);
            amount_->setVisible(false);
        } else {
            addAndMakeVisible(*amount_);
            below_->setVisible(false);
            above_->setVisible(false);
        }
        updateTarget();
    }

    void resized() override
    {
        const int width = getWidth();
        on_.setBounds(width - 56, 1, 48, 18);
        wipe_.setBounds(width - 120, 1, 56, 18);
        to_.setBounds(wipe_.getX() - 4 - 30, 1, 30, 18);
        if (key_) {
            breakpoint_->setBounds(8, 26, 80, 82);
            target_.setBounds(96, 50, 180, 18);
            below_->setBounds(8, 112, 80, 82);
            above_->setBounds(96, 112, 80, 82);
        } else {
            target_.setBounds(8, 50, 270, 18);
            amount_->setBounds(8, 76, 80, 82);
        }
    }

    void paint(juce::Graphics& g) override
    {
        P1Card::paint(g);
        g.setFont(uiFont(tokens::textButton));
        g.setColour(colour(tokens::dim));
        if (key_)
            g.drawText("target", 96, 30, 60, 16, juce::Justification::centredLeft,
                       false);
        else
            g.drawText("target", 8, 30, 60, 16, juce::Justification::centredLeft,
                       false);
    }

    void refresh()
    {
        const bool enabled = getParam(editor_, key_ ? "key_sens_on" : "vel_sens_on") >
                             0.5;
        const float alpha = enabled ? 1.0f : 0.45f;
        target_.setAlpha(alpha);
        if (breakpoint_ != nullptr)
            breakpoint_->setAlpha(alpha);
        below_->setAlpha(alpha);
        above_->setAlpha(alpha);
        amount_->setAlpha(alpha);
        repaint();
    }

private:
    juce::StringArray targetItems() const
    {
        juce::StringArray items;
        for (const auto& target : targets_)
            items.add(target.name);
        return items;
    }

    void updateTarget()
    {
        const auto& target = targets_[static_cast<size_t>(juce::jlimit(
            0, static_cast<int>(targets_.size()) - 1, selected_))];
        below_->setParamID(target.keyLo);
        above_->setParamID(target.keyHi);
        amount_->setParamID(target.velocity);
    }

    void wipe()
    {
        for (const auto& target : targets_) {
            if (key_) {
                setParam(editor_, target.keyLo, 0.0);
                setParam(editor_, target.keyHi, 0.0);
            } else {
                setParam(editor_, target.velocity, 0.0);
            }
        }
    }

    PluginEditor& editor_;
    bool key_ = true;
    int part_ = 0;
    std::array<params::FollowTarget, 19> targets_{};
    P1Choice target_;
    P1Toggle on_;
    P1Button wipe_{"wipe all", true};
    P1Button to_;
    std::unique_ptr<ParamKnob> breakpoint_;
    std::unique_ptr<ParamKnob> below_;
    std::unique_ptr<ParamKnob> above_;
    std::unique_ptr<ParamKnob> amount_;
    int selected_ = 0;
};

class FxModPage : public PageBase {
public:
    explicit FxModPage(PluginEditor& editor)
    {
        const int part = editor.activePart();
        PluginEditor* ed = &editor;
        const juce::String toLabel = part == 0 ? "to 2" : "to 1";
        const auto copy = [ed](PluginEditor::PartFxSection section) {
            return [ed, section] { ed->copyPartFxSection(section); };
        };
        filter1_ = std::make_unique<FilterCard>(
            editor, "filter", params::filterCutoff(part), params::filterRes(part),
            params::filterType(part), params::filterOn(part),
            copy(PluginEditor::PartFxSection::filter), toLabel);
        filterEnv_ = std::make_unique<FilterEnvelopeCard>(
            editor, part, copy(PluginEditor::PartFxSection::filterEnvelope),
            toLabel);
        pitchLfo_ = std::make_unique<PitchLfoCard>(
            editor, copy(PluginEditor::PartFxSection::pitchLfo), toLabel);
        pitchEnv_ = std::make_unique<PitchEnvelopeCard>(
            editor, copy(PluginEditor::PartFxSection::pitchEnvelope), toLabel);
        panEnv_ = std::make_unique<PanEnvelopeCard>(
            editor, copy(PluginEditor::PartFxSection::panEnvelope), toLabel);
        keyFollow_ = std::make_unique<FollowCard>(
            editor, true, "key follow",
            copy(PluginEditor::PartFxSection::keyFollow), toLabel);
        velFollow_ = std::make_unique<FollowCard>(
            editor, false, "velocity follow",
            copy(PluginEditor::PartFxSection::velocityFollow), toLabel);
        addAndMakeVisible(*filter1_);
        addAndMakeVisible(*filterEnv_);
        addAndMakeVisible(*pitchLfo_);
        addAndMakeVisible(*pitchEnv_);
        addAndMakeVisible(*panEnv_);
        addAndMakeVisible(*keyFollow_);
        addAndMakeVisible(*velFollow_);
    }

    void resized() override
    {
        auto area = getLocalBounds();
        const int columns = 4;
        const int width = (area.getWidth() - (columns - 1) * 10) / columns;
        const int height = (area.getHeight() - 10) / 2;
        std::array<P1Card*, 8> cards = {
            filter1_.get(),  filterEnv_.get(), pitchLfo_.get(),
            pitchEnv_.get(), panEnv_.get(),   keyFollow_.get(),
            velFollow_.get(), nullptr};
        for (int i = 0; i < 8; ++i) {
            if (cards[static_cast<size_t>(i)] == nullptr)
                continue;
            const int column = i % columns;
            const int row = i / columns;
            cards[static_cast<size_t>(i)]->setBounds(
                column * (width + 10), row * (height + 10), width, height);
        }
    }

    void tick() override
    {
        filterEnv_->tick();
        pitchLfo_->tick();
        pitchEnv_->tick();
        panEnv_->tick();
        keyFollow_->refresh();
        velFollow_->refresh();
    }

private:
    std::unique_ptr<FilterCard> filter1_;
    std::unique_ptr<FilterEnvelopeCard> filterEnv_;
    std::unique_ptr<PitchLfoCard> pitchLfo_;
    std::unique_ptr<PitchEnvelopeCard> pitchEnv_;
    std::unique_ptr<PanEnvelopeCard> panEnv_;
    std::unique_ptr<FollowCard> keyFollow_;
    std::unique_ptr<FollowCard> velFollow_;
};

} // namespace

std::unique_ptr<PageBase> makeFxModPage(PluginEditor& editor)
{
    return std::make_unique<FxModPage>(editor);
}

} // namespace p1::ui
