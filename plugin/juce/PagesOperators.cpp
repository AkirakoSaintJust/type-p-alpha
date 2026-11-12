/* SPDX-License-Identifier: GPL-3.0-only */
#include "Pages.h"

#include <array>
#include <map>

#include "ParamIDs.h"

namespace p1::ui {
namespace {

bool isCarrier(int algorithm, int op)
{
    static const int cars[8][4] = {
        {3, -1, -1, -1}, {3, -1, -1, -1}, {3, -1, -1, -1}, {3, -1, -1, -1},
        {1, 3, -1, -1},  {1, 2, 3, -1},   {1, 2, 3, -1},   {0, 1, 2, 3},
    };
    algorithm = juce::jlimit(0, 7, algorithm);
    for (int value : cars[algorithm]) {
        if (value == op)
            return true;
    }
    return false;
}

class OpColumn : public juce::Component {
public:
    OpColumn(PluginEditor& editor, int op, std::function<void(int)> onCopy,
             std::function<void(int)> onPaste)
        : editor_(editor),
          op_(op),
          part_(editor.activePart()),
          header_(false, false, {},
                  [this, onCopy] { onCopy(op_); },
                  [this, onPaste] { onPaste(op_); },
                  [this] {
                      return getParam(editor_, params::opMute(op_, part_)) > 0.5;
                  },
                  [this](bool muted) {
                      setParam(editor_, params::opMute(op_, part_),
                               muted ? 1 : 0);
                  })
    {
        addAndMakeVisible(header_);
        addAndMakeVisible(scope_);
        addAndMakeVisible(spectro_);

        osc_.setSize(278, 157);
        env_.setSize(278, 270);
        lfo_.setSize(278, 157);
        addAndMakeVisible(osc_);
        addAndMakeVisible(env_);
        addAndMakeVisible(lfo_);

        oscKnobs_[0] = std::make_unique<ParamKnob>(
            editor.state(), params::opRatio(op_, part_), "ratio", 34.0f);
        oscKnobs_[1] = std::make_unique<ParamKnob>(
            editor.state(), params::opDetune(op_, part_), "detune", 34.0f);
        oscKnobs_[2] = std::make_unique<ParamKnob>(
            editor.state(), params::opLevel(op_, part_), "level", 34.0f);
        oscKnobs_[3] = std::make_unique<ParamKnob>(
            editor.state(), params::opFeedback(op_, part_), "feedback", 34.0f);
        for (auto& knob : oscKnobs_)
            osc_.addAndMakeVisible(*knob);
        wave_ = std::make_unique<ParamChoice>(
            editor.state(), params::opWaveform(op_, part_), params::waveNames());
        osc_.addAndMakeVisible(*wave_);

        juce::StringArray rateIDs;
        juce::StringArray levelIDs;
        for (int i = 0; i < 4; ++i)
            rateIDs.add(params::opEnvRate(op_, i, part_));
        for (int i = 0; i < 3; ++i)
            levelIDs.add(params::opEnvLevel(op_, i, part_));
        envView_ = std::make_unique<P1Envelope>(
            editor.state(), rateIDs, levelIDs, colour(tokens::purple), 0.0, 99.0,
            false);
        env_.addAndMakeVisible(*envView_);
        for (int i = 0; i < 4; ++i) {
            rateKnobs_[static_cast<size_t>(i)] = std::make_unique<ParamKnob>(
                editor.state(), rateIDs[i], "r" + juce::String(i + 1), 34.0f);
            env_.addAndMakeVisible(*rateKnobs_[static_cast<size_t>(i)]);
        }
        for (int i = 0; i < 3; ++i) {
            levelKnobs_[static_cast<size_t>(i)] = std::make_unique<ParamKnob>(
                editor.state(), levelIDs[i], "l" + juce::String(i + 1), 34.0f);
            env_.addAndMakeVisible(*levelKnobs_[static_cast<size_t>(i)]);
        }

        lfoRate_ = std::make_unique<ParamKnob>(
            editor.state(), params::opTremRate(op_, part_), "rate", 34.0f);
        lfoDepth_ = std::make_unique<ParamKnob>(
            editor.state(), params::opTremDepth(op_, part_), "depth", 34.0f);
        lfo_.addAndMakeVisible(*lfoRate_);
        lfo_.addAndMakeVisible(*lfoDepth_);
        lfoWave_ = std::make_unique<ParamChoice>(
            editor.state(), params::opTremWave(op_, part_),
            params::lfoWaveNames());
        lfo_.addAndMakeVisible(*lfoWave_);
        lfoMode_ = std::make_unique<P1Toggle>(
            editor.state(), params::opTremMode(op_, part_), "bipolar");
        lfo_.addAndMakeVisible(*lfoMode_);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(8);
        header_.setBounds(area.removeFromTop(24));
        auto visual = area.removeFromTop(40);
        const int half = (visual.getWidth() - 6) / 2;
        scope_.setBounds(visual.removeFromLeft(half));
        visual.removeFromLeft(6);
        spectro_.setBounds(visual);
        area.removeFromTop(6);
        osc_.setBounds(area.removeFromTop(157));
        area.removeFromTop(6);
        env_.setBounds(area.removeFromTop(270));
        area.removeFromTop(6);
        lfo_.setBounds(area.removeFromTop(157));

        auto oscContent = osc_.getLocalBounds().withTrimmedTop(22).reduced(8, 0);
        auto oscRow = oscContent.removeFromTop(84);
        const int oscCell = oscRow.getWidth() / 4;
        for (auto& knob : oscKnobs_)
            knob->setBounds(oscRow.removeFromLeft(oscCell));
        wave_->setBounds(oscContent.removeFromTop(18).withTrimmedTop(2).withWidth(150));

        auto envContent = env_.getLocalBounds().withTrimmedTop(22).reduced(8, 0);
        envView_->setBounds(envContent.removeFromTop(52));
        auto rates = envContent.removeFromTop(80);
        const int rateCell = rates.getWidth() / 4;
        for (auto& knob : rateKnobs_)
            knob->setBounds(rates.removeFromLeft(rateCell));
        auto levels = envContent.removeFromTop(80);
        const int levelCell = levels.getWidth() / 3;
        for (auto& knob : levelKnobs_)
            knob->setBounds(levels.removeFromLeft(levelCell));

        auto lfoContent = lfo_.getLocalBounds().withTrimmedTop(22).reduced(8, 0);
        auto lfoRow = lfoContent.removeFromTop(84);
        lfoRate_->setBounds(lfoRow.removeFromLeft(lfoRow.getWidth() / 2));
        lfoDepth_->setBounds(lfoRow);
        auto lfoBottom = lfoContent.removeFromTop(18).withTrimmedTop(2);
        lfoWave_->setBounds(lfoBottom.removeFromLeft(150));
        lfoMode_->setBounds(lfoBottom.removeFromLeft(110));
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(colour(tokens::bg));
        const int algorithm = static_cast<int>(
            std::lround(getParam(editor_, params::algorithm(part_))));
        g.setFont(displayFont(tokens::textOperatorName));
        g.setColour(colour(isCarrier(algorithm, op_) ? tokens::pink : tokens::white));
        g.drawText("op" + juce::String(op_ + 1), 8, 8, 64, 20,
                   juce::Justification::centredLeft, false);
    }

    void tick()
    {
        editor_.processor().readTapScope(
            op_ * 2 + part_, scratch_.data(), static_cast<int>(scratch_.size()));
        scope_.setSamples(scratch_.data(), static_cast<int>(scratch_.size()));
        spectro_.setSamples(scratch_.data(), static_cast<int>(scratch_.size()));
        envView_->repaint();
        header_.tick();
        repaint();
    }

private:
    PluginEditor& editor_;
    int op_ = 0;
    int part_ = 0;
    CardHeader header_;
    P1Scope scope_;
    P1Spectro spectro_;
    P1Card osc_{"oscillator"};
    P1Card env_{"envelope"};
    P1Card lfo_{"amplitude lfo"};
    std::array<std::unique_ptr<ParamKnob>, 4> oscKnobs_{};
    std::unique_ptr<ParamChoice> wave_;
    std::unique_ptr<P1Envelope> envView_;
    std::array<std::unique_ptr<ParamKnob>, 4> rateKnobs_{};
    std::array<std::unique_ptr<ParamKnob>, 3> levelKnobs_{};
    std::unique_ptr<ParamKnob> lfoRate_;
    std::unique_ptr<ParamKnob> lfoDepth_;
    std::unique_ptr<ParamChoice> lfoWave_;
    std::unique_ptr<P1Toggle> lfoMode_;
    std::array<float, 512> scratch_{};
};

class OperatorsPage : public PageBase {
public:
    explicit OperatorsPage(PluginEditor& editor) : editor_(editor)
    {
        for (int i = 0; i < 4; ++i) {
            columns_[static_cast<size_t>(i)] = std::make_unique<OpColumn>(
                editor, i, [this](int op) { copyOperator(op); },
                [this](int op) { pasteOperator(op); });
            addAndMakeVisible(*columns_[static_cast<size_t>(i)]);
        }
    }

    void resized() override
    {
        auto area = getLocalBounds();
        const int width = (area.getWidth() - 30) / 4;
        for (auto& column : columns_) {
            column->setBounds(area.removeFromLeft(width));
            area.removeFromLeft(10);
        }
    }

    void tick() override
    {
        for (auto& column : columns_)
            column->tick();
    }

private:
    void copyOperator(int op)
    {
        clipboard_.clear();
        clipboardOp_ = op;
        const int part = editor_.activePart();
        for (const auto& spec : kP1ParamSpecs) {
            const juce::String id(spec.id);
            if (!id.endsWith(".p" + juce::String(part)))
                continue;
            const bool opMatch =
                id.startsWith("op" + juce::String(op + 1) + "_") ||
                id.startsWith("op" + juce::String(op + 1) + ".");
            const bool op1Edge = op == 0 && (id.startsWith("attack.") ||
                                             id.startsWith("release."));
            if (opMatch || op1Edge)
                clipboard_[id.dropLastCharacters(3)] =
                    editor_.state().getRawParameterValue(id)->load();
        }
        hasClipboard_ = !clipboard_.empty();
    }

    void pasteOperator(int op)
    {
        if (!hasClipboard_)
            return;
        const auto suffix = ".p" + juce::String(editor_.activePart());
        const juce::String source = "op" + juce::String(clipboardOp_ + 1);
        const juce::String destination = "op" + juce::String(op + 1);
        for (const auto& [storedBase, value] : clipboard_) {
            juce::String base = storedBase;
            if (base.startsWith(source + "_") || base.startsWith(source + "."))
                base = destination + base.substring(source.length());
            if (op == 0 && base == "op1.eg.r1")
                base = "attack";
            else if (op == 0 && base == "op1.eg.r4")
                base = "release";
            else if (op != 0 && base == "attack")
                base = destination + ".eg.r1";
            else if (op != 0 && base == "release")
                base = destination + ".eg.r4";
            if (auto* parameter = editor_.state().getParameter(base + suffix))
                parameter->setValueNotifyingHost(
                    parameter->convertTo0to1(static_cast<float>(value)));
        }
    }

    PluginEditor& editor_;
    std::array<std::unique_ptr<OpColumn>, 4> columns_{};
    std::map<juce::String, double> clipboard_;
    int clipboardOp_ = 0;
    bool hasClipboard_ = false;
};

} // namespace

std::unique_ptr<PageBase> makeOperatorsPage(PluginEditor& editor)
{
    return std::make_unique<OperatorsPage>(editor);
}

} // namespace p1::ui
