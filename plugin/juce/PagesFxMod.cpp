/* SPDX-License-Identifier: GPL-3.0-only */
#include "Pages.h"

#include <array>

#include "MacroTargets.h"
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
                            .withSizeKeepingCentre(levelCell * 2 - 8,
                                                   tokens::choiceHeight));
        on_.setBounds(getWidth() - 56, 1, 48, 18);
        to_.setBounds(on_.getX() - 16 - 32, 1, 32, 18);
    }

    void tick() { envView_.tick(); }

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
        auto bottom = area.removeFromTop(tokens::choiceHeight);
        wave_.setBounds(bottom.removeFromLeft(150));
        mode_.setBounds(bottom.removeFromLeft(110));
    }

    void tick() { waveView_.tick(); }

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
                            .withSizeKeepingCentre(levelCell * 2 - 8,
                                                   tokens::choiceHeight));
        on_.setBounds(getWidth() - 56, 1, 48, 18);
        to_.setBounds(on_.getX() - 16 - 32, 1, 32, 18);
    }

    void tick() { envView_.tick(); }

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
                            .withSizeKeepingCentre(cell * 2 - 8,
                                                   tokens::choiceHeight));
        on_.setBounds(getWidth() - 56, 1, 48, 18);
        to_.setBounds(on_.getX() - 16 - 32, 1, 32, 18);
    }

    void tick() { envView_.tick(); }

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
          on_(editor.state(),
              params::withPart(key ? "key_sens_on" : "vel_sens_on", part_),
              "on"),
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
                editor.state(), params::withPart("breakpoint", part_),
                "breakpoint", 42.0f);
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
        const bool enabled = getParam(
            editor_, params::withPart(key_ ? "key_sens_on" : "vel_sens_on",
                                      part_)) > 0.5;
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

class RingGraph : public juce::Component {
public:
    void setPair(int a, int b)
    {
        a_ = a;
        b_ = b;
        repaint();
    }

    void setDirect(bool a, bool b)
    {
        if (directA_ == a && directB_ == b)
            return;
        directA_ = a;
        directB_ = b;
        repaint();
    }

    void paint(juce::Graphics& g) override
    {
        constexpr int boxW = 24;
        constexpr int boxH = 16;
        const int ax = 0, ay = 4;
        const int bx = 0, by = 34;
        const int rx = 46, ry = 16, rw = 22, rh = 22;
        const auto drawBox = [&](int x, int y, int op) {
            g.setColour(colour(tokens::pink));
            g.fillRect(x, y, boxW, boxH);
            g.setColour(juce::Colours::black);
            g.setFont(uiFont(9.0f));
            g.drawText("op" + juce::String(op + 1), x, y, boxW, boxH,
                       juce::Justification::centred, false);
        };
        const auto arrow = [&](int x1, int y1, int x2, int y2) {
            g.setColour(colour(tokens::dim));
            g.drawLine(static_cast<float>(x1), static_cast<float>(y1),
                       static_cast<float>(x2 - 5), static_cast<float>(y2), 1.2f);
            juce::Path head;
            head.addTriangle(static_cast<float>(x2), static_cast<float>(y2),
                             static_cast<float>(x2 - 6), static_cast<float>(y2 - 3),
                             static_cast<float>(x2 - 6), static_cast<float>(y2 + 3));
            g.fillPath(head);
        };
        drawBox(ax, ay, a_);
        drawBox(bx, by, b_);
        arrow(ax + boxW, ay + boxH / 2, rx, ry + 5);
        arrow(bx + boxW, by + boxH / 2, rx, ry + rh - 5);
        g.setColour(colour(tokens::lineStrong));
        g.drawRect(rx, ry, rw, rh, 1);
        g.setColour(colour(tokens::pink));
        drawBoldText(g, displayFont(7.0f), "R",
                     juce::Rectangle<float>(static_cast<float>(rx),
                                            static_cast<float>(ry),
                                            static_cast<float>(rw),
                                            static_cast<float>(rh)),
                     1.0f);
        const int outX = getWidth() - 2;
        const int outY = ry + rh / 2;
        g.setColour(colour(tokens::purple));
        g.drawLine(static_cast<float>(rx + rw), static_cast<float>(outY),
                   static_cast<float>(outX - 5), static_cast<float>(outY), 1.4f);
        juce::Path head;
        head.addTriangle(static_cast<float>(outX), static_cast<float>(outY),
                         static_cast<float>(outX - 6), static_cast<float>(outY - 3),
                         static_cast<float>(outX - 6), static_cast<float>(outY + 3));
        g.fillPath(head);
        const auto directArrow = [&](int y) {
            g.setColour(colour(tokens::purple));
            g.drawLine(static_cast<float>(boxW + 2), static_cast<float>(y),
                       static_cast<float>(outX - 5), static_cast<float>(y), 1.4f);
            juce::Path directHead;
            directHead.addTriangle(static_cast<float>(outX), static_cast<float>(y),
                                   static_cast<float>(outX - 6),
                                   static_cast<float>(y - 3),
                                   static_cast<float>(outX - 6),
                                   static_cast<float>(y + 3));
            g.fillPath(directHead);
        };
        if (directA_)
            directArrow(ay + boxH / 2);
        if (directB_)
            directArrow(by + boxH / 2);
    }

private:
    int a_ = 0;
    int b_ = 0;
    bool directA_ = false;
    bool directB_ = false;
};

class RingmodCard : public P1Card {
public:
    RingmodCard(PluginEditor& editor, int part)
        : P1Card("ringmod"), editor_(editor), part_(part)
    {
        for (int i = 0; i < 3; ++i) {
            addAndMakeVisible(graph_[static_cast<std::size_t>(i)]);
            on_[static_cast<std::size_t>(i)] = std::make_unique<P1Toggle>(
                editor.state(), params::ringOn(i, part_), "on");
            addAndMakeVisible(*on_[static_cast<std::size_t>(i)]);
            level_[static_cast<std::size_t>(i)] = std::make_unique<ParamKnob>(
                editor.state(), params::ringLevel(i, part_), "level", 42.0f);
            addAndMakeVisible(*level_[static_cast<std::size_t>(i)]);
        }
        for (int op = 0; op < 4; ++op) {
            out_[static_cast<std::size_t>(op)] = std::make_unique<P1Toggle>(
                editor.state(), params::carrierOut(op, part_),
                "op" + juce::String(op + 1));
            addAndMakeVisible(*out_[static_cast<std::size_t>(op)]);
        }
        noRing_.setText("no ringmod", juce::dontSendNotification);
        noRing_.setFont(uiFont(tokens::textInlineLabel));
        noRing_.setColour(juce::Label::textColourId, colour(tokens::faint));
        noRing_.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(noRing_);
        original_.setText("original", juce::dontSendNotification);
        original_.setFont(uiFont(10.0f));
        original_.setColour(juce::Label::textColourId, colour(tokens::dim));
        original_.setJustificationType(juce::Justification::centredLeft);
        addAndMakeVisible(original_);
        updateCarriers();
    }

    void resized() override { layout(); }

    void tick()
    {
        const int algorithm = static_cast<int>(
            std::lround(getParam(editor_, params::algorithm(part_))));
        const auto& mask = p1::macros::kCarrierMasks[static_cast<std::size_t>(
            juce::jlimit(0, 9, algorithm))];
        int next[4] = {0, 0, 0, 0};
        int count = 0;
        for (int op = 0; op < 4; ++op)
            if (mask[static_cast<std::size_t>(op)] != 0)
                next[count++] = op;
        bool changed = count != carrierCount_;
        for (int i = 0; i < count && !changed; ++i)
            changed = next[i] != carriers_[static_cast<std::size_t>(i)];
        if (changed) {
            carrierCount_ = count;
            for (int i = 0; i < 4; ++i)
                carriers_[static_cast<std::size_t>(i)] = next[i];
            updateCarriers();
        }
        for (int i = 0; i < 3; ++i) {
            const bool on = std::lround(
                                getParam(editor_, params::ringOn(i, part_))) != 0;
            auto& level = *level_[static_cast<std::size_t>(i)];
            level.setEnabled(on);
            level.setAlpha(on ? 1.0f : 0.35f);
            if (i + 1 < carrierCount_)
                graph_[static_cast<std::size_t>(i)].setDirect(
                    on && carrierOutOn(carriers_[static_cast<std::size_t>(i)]),
                    on && carrierOutOn(
                              carriers_[static_cast<std::size_t>(i + 1)]));
        }
        bool anyOn = false;
        const int pairs = juce::jmax(0, carrierCount_ - 1);
        for (int i = 0; i < pairs; ++i)
            anyOn = anyOn || std::lround(
                                   getParam(editor_, params::ringOn(i, part_))) != 0;
        original_.setAlpha(anyOn ? 1.0f : 0.35f);
        for (auto& toggle : out_) {
            toggle->setEnabled(anyOn);
            toggle->setAlpha(anyOn ? 1.0f : 0.35f);
        }
    }

private:
    bool carrierOutOn(int op) const
    {
        return std::lround(getParam(editor_, params::carrierOut(op, part_))) != 0;
    }

    bool isCarrier(int op) const
    {
        for (int i = 0; i < carrierCount_; ++i)
            if (carriers_[static_cast<std::size_t>(i)] == op)
                return true;
        return false;
    }

    void updateCarriers()
    {
        const int pairs = juce::jmax(0, carrierCount_ - 1);
        for (int i = 0; i < 3; ++i) {
            const auto row = static_cast<std::size_t>(i);
            const bool show = i < pairs;
            graph_[row].setVisible(show);
            on_[row]->setVisible(show);
            level_[row]->setVisible(show);
            if (show) {
                const bool on = std::lround(
                                    getParam(editor_, params::ringOn(i, part_))) != 0;
                graph_[row].setPair(carriers_[row],
                                    carriers_[static_cast<std::size_t>(i + 1)]);
                graph_[row].setDirect(
                    on && carrierOutOn(carriers_[row]),
                    on && carrierOutOn(
                              carriers_[static_cast<std::size_t>(i + 1)]));
            }
        }
        const bool controls = carrierCount_ > 1;
        noRing_.setVisible(!controls);
        original_.setVisible(controls);
        for (int op = 0; op < 4; ++op)
            out_[static_cast<std::size_t>(op)]->setVisible(controls &&
                                                           isCarrier(op));
        layout();
    }

    void layout()
    {
        auto area = contentArea();
        if (carrierCount_ < 2) {
            noRing_.setBounds(area);
            return;
        }
        auto outRow = area.removeFromBottom(22);
        original_.setBounds(outRow.removeFromLeft(44));
        int x = outRow.getX();
        for (int op = 0; op < 4; ++op) {
            auto& toggle = *out_[static_cast<std::size_t>(op)];
            if (!toggle.isVisible())
                continue;
            toggle.setBounds(x, outRow.getY() + 2, 52, 18);
            x += 54;
        }
        const int pairs = carrierCount_ - 1;
        for (int i = 0; i < pairs; ++i) {
            auto row = area.removeFromTop(82);
            graph_[static_cast<std::size_t>(i)].setBounds(
                row.getX(), row.getY() + 13, 112, 56);
            on_[static_cast<std::size_t>(i)]->setBounds(
                row.getX() + 118, row.getY() + 32, 52, 18);
            level_[static_cast<std::size_t>(i)]->setBounds(
                row.getRight() - 56, row.getY(), 56, 82);
        }
    }

    PluginEditor& editor_;
    int part_ = 0;
    std::array<RingGraph, 3> graph_{};
    std::array<std::unique_ptr<P1Toggle>, 3> on_{};
    std::array<std::unique_ptr<ParamKnob>, 3> level_{};
    std::array<std::unique_ptr<P1Toggle>, 4> out_{};
    juce::Label noRing_;
    juce::Label original_;
    std::array<int, 4> carriers_{};
    int carrierCount_ = 0;
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
            editor, false, "velocity",
            copy(PluginEditor::PartFxSection::velocityFollow), toLabel);
        ringmod_ = std::make_unique<RingmodCard>(editor, part);
        addAndMakeVisible(*filter1_);
        addAndMakeVisible(*filterEnv_);
        addAndMakeVisible(*pitchLfo_);
        addAndMakeVisible(*pitchEnv_);
        addAndMakeVisible(*panEnv_);
        addAndMakeVisible(*keyFollow_);
        addAndMakeVisible(*velFollow_);
        addAndMakeVisible(*ringmod_);
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
            velFollow_.get(), ringmod_.get()};
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
        ringmod_->tick();
    }

private:
    std::unique_ptr<FilterCard> filter1_;
    std::unique_ptr<FilterEnvelopeCard> filterEnv_;
    std::unique_ptr<PitchLfoCard> pitchLfo_;
    std::unique_ptr<PitchEnvelopeCard> pitchEnv_;
    std::unique_ptr<PanEnvelopeCard> panEnv_;
    std::unique_ptr<FollowCard> keyFollow_;
    std::unique_ptr<FollowCard> velFollow_;
    std::unique_ptr<RingmodCard> ringmod_;
};

} // namespace

std::unique_ptr<PageBase> makeFxModPage(PluginEditor& editor)
{
    return std::make_unique<FxModPage>(editor);
}

} // namespace p1::ui
