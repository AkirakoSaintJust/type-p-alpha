/* SPDX-License-Identifier: GPL-3.0-only */
#include "Pages.h"

#include <array>

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
                  }),
          osc_(editor, op, part_),
          amp_(editor, op, part_),
          fb_(editor, op, part_)
    {
        addAndMakeVisible(header_);
        addAndMakeVisible(scope_);
        addAndMakeVisible(spectro_);
        addAndMakeVisible(osc_);
        addAndMakeVisible(amp_);
        addAndMakeVisible(fb_);
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
        area.removeFromTop(4);
        osc_.setBounds(area.removeFromTop(140));
        area.removeFromTop(4);
        amp_.setBounds(area.removeFromTop(200));
        area.removeFromTop(4);
        fb_.setBounds(area.removeFromTop(214));
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
        amp_.tick();
        fb_.tick();
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
    OscillatorCard osc_;
    AmplitudeEnvelopeCard amp_;
    FeedbackEnvelopeCard fb_;
    std::array<float, 512> scratch_{};
};

class OpLfoColumn : public juce::Component {
public:
    OpLfoColumn(PluginEditor& editor, int op, std::function<void(int)> onCopy,
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
                  }),
          osc_(editor, op, part_),
          ampLfo_(editor, "amp lfo", params::opTremRate(op, part_),
                  params::opTremDepth(op, part_), params::opTremWave(op, part_),
                  params::opTremMode(op, part_), op, part_,
                  PluginEditor::OperatorSection::amplitudeLfo),
          feedbackLfo_(editor, "fb lfo",
                       params::opFeedbackLfoRate(op, part_),
                       params::opFeedbackLfoDepth(op, part_),
                       params::opFeedbackLfoWave(op, part_),
                       params::opFeedbackLfoMode(op, part_), op, part_,
                       PluginEditor::OperatorSection::feedbackLfo),
          detuneLfo_(editor, "detune lfo", params::opDetuneLfoRate(op, part_),
                     params::opDetuneLfoDepth(op, part_),
                     params::opDetuneLfoWave(op, part_),
                     params::opDetuneLfoMode(op, part_), op, part_,
                     PluginEditor::OperatorSection::detuneLfo)
    {
        addAndMakeVisible(header_);
        addAndMakeVisible(scope_);
        addAndMakeVisible(spectro_);
        addAndMakeVisible(osc_);
        addAndMakeVisible(ampLfo_);
        addAndMakeVisible(feedbackLfo_);
        addAndMakeVisible(detuneLfo_);
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
        area.removeFromTop(4);
        osc_.setBounds(area.removeFromTop(140));
        area.removeFromTop(4);
        ampLfo_.setBounds(area.removeFromTop(140));
        area.removeFromTop(4);
        feedbackLfo_.setBounds(area.removeFromTop(140));
        area.removeFromTop(4);
        detuneLfo_.setBounds(area.removeFromTop(140));
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
    OscillatorCard osc_;
    LfoCard ampLfo_;
    LfoCard feedbackLfo_;
    LfoCard detuneLfo_;
    std::array<float, 512> scratch_{};
};

class OperatorsPage : public PageBase {
public:
    explicit OperatorsPage(PluginEditor& editor) : editor_(editor)
    {
        for (int i = 0; i < 4; ++i) {
            columns_[static_cast<size_t>(i)] = std::make_unique<OpColumn>(
                editor, i, [this](int op) { editor_.copyOperator(op); },
                [this](int op) { editor_.pasteOperator(op); });
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
    PluginEditor& editor_;
    std::array<std::unique_ptr<OpColumn>, 4> columns_{};
};

class OperatorLfosPage : public PageBase {
public:
    explicit OperatorLfosPage(PluginEditor& editor) : editor_(editor)
    {
        for (int i = 0; i < 4; ++i) {
            columns_[static_cast<size_t>(i)] = std::make_unique<OpLfoColumn>(
                editor, i, [this](int op) { editor_.copyOperator(op); },
                [this](int op) { editor_.pasteOperator(op); });
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
    PluginEditor& editor_;
    std::array<std::unique_ptr<OpLfoColumn>, 4> columns_{};
};

} // namespace

std::unique_ptr<PageBase> makeOperatorsPage(PluginEditor& editor)
{
    return std::make_unique<OperatorsPage>(editor);
}

std::unique_ptr<PageBase> makeOperatorLfosPage(PluginEditor& editor)
{
    return std::make_unique<OperatorLfosPage>(editor);
}

} // namespace p1::ui
