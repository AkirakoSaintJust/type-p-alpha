/* SPDX-License-Identifier: GPL-3.0-only */
#include "Pages.h"

#include <array>
#include <cmath>
#include <functional>
#include <vector>

#include "ParamIDs.h"
#include "SpectrumAnalysis.h"

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

juce::StringArray spectrumIDs(int op, int part, bool a)
{
    juce::StringArray ids;
    for (int h = 1; h <= 16; ++h)
        ids.add(a ? params::opSpecA(op, h, part)
                  : params::opSpecB(op, h, part));
    return ids;
}

juce::StringArray rangeNames()
{
    return {"1-16", "17-32", "33-48", "49-64",
            "65-80", "81-96", "97-112", "113-128"};
}

class MacroSlider : public juce::Component {
public:
    MacroSlider(juce::String label, double min, double max,
                std::function<void()> onBegin,
                std::function<void(double)> onChange,
                std::function<void()> onEnd = {})
        : label_(std::move(label)),
          min_(min),
          max_(max),
          onBegin_(std::move(onBegin)),
          onChange_(std::move(onChange)),
          onEnd_(std::move(onEnd))
    {
        setMouseCursor(juce::MouseCursor::PointingHandCursor);
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(colour(tokens::bg));
        g.setColour(colour(tokens::line));
        g.drawRect(getLocalBounds(), 1);
        auto area = getLocalBounds().reduced(4, 0);
        g.setFont(uiFont(tokens::textLegend));
        g.setColour(colour(tokens::dim));
        g.drawText(label_, area, juce::Justification::centredLeft, false);
        const float trackLeft = static_cast<float>(area.getX()) + 34.0f;
        const float trackRight = static_cast<float>(area.getRight());
        if (trackRight <= trackLeft)
            return;
        const float centre = (trackLeft + trackRight) * 0.5f;
        float start = centre;
        float fill = 0.0f;
        if (min_ < 0.0) {
            const float t = static_cast<float>(amount_ / (max_ - min_));
            fill = centre + t * (trackRight - trackLeft);
            g.setColour(colour(tokens::lineStrong));
            g.drawVerticalLine(juce::roundToInt(centre), 3.0f,
                               static_cast<float>(getHeight()) - 3.0f);
        } else {
            const float t = static_cast<float>((amount_ - min_) / (max_ - min_));
            start = trackLeft;
            fill = trackLeft + t * (trackRight - trackLeft);
        }
        g.setColour(colour(tokens::purple));
        g.fillRect(juce::Rectangle<float>(
            juce::jmin(start, fill), 5.0f, std::abs(fill - start),
            static_cast<float>(getHeight()) - 10.0f));
    }

    void mouseDown(const juce::MouseEvent& event) override
    {
        if (event.mods.isPopupMenu())
            return;
        dragging_ = true;
        if (onBegin_)
            onBegin_();
        setFromMouse(event);
    }

    void mouseDrag(const juce::MouseEvent& event) override
    {
        if (dragging_ && !event.mods.isPopupMenu())
            setFromMouse(event);
    }

    void mouseUp(const juce::MouseEvent&) override
    {
        if (!dragging_)
            return;
        dragging_ = false;
        amount_ = 0.0;
        if (onEnd_)
            onEnd_();
        repaint();
    }

private:
    void setFromMouse(const juce::MouseEvent& event)
    {
        auto area = getLocalBounds().reduced(4, 0);
        const float trackLeft = static_cast<float>(area.getX()) + 34.0f;
        const float trackRight = static_cast<float>(area.getRight());
        const float width = juce::jmax(1.0f, trackRight - trackLeft);
        const double t = juce::jlimit(
            0.0, 1.0,
            static_cast<double>(event.position.x - trackLeft) / width);
        amount_ = min_ + t * (max_ - min_);
        if (onChange_)
            onChange_(amount_);
        repaint();
    }

    juce::String label_;
    double min_ = 0.0;
    double max_ = 1.0;
    double amount_ = 0.0;
    bool dragging_ = false;
    std::function<void()> onBegin_;
    std::function<void(double)> onChange_;
    std::function<void()> onEnd_;
};

class MorphEnvelopeCard : public P1Card {
public:
    MorphEnvelopeCard(PluginEditor& editor, int op, int part)
        : P1Card("morph env",
                 [&ed = editor, op, part] {
                     ed.copyOperatorSection(
                         op, part,
                         PluginEditor::OperatorSection::spectrumEnvelope);
                 },
                 [&ed = editor, op, part] {
                     ed.pasteOperatorSection(
                         op, part,
                         PluginEditor::OperatorSection::spectrumEnvelope);
                 }),
          env_(editor.state(),
               {params::opSpecEnvRate(op, 0, part),
                params::opSpecEnvRate(op, 1, part),
                params::opSpecEnvRate(op, 2, part),
                params::opSpecEnvRate(op, 3, part)},
               {params::opSpecEnvLevel(op, 0, part),
                params::opSpecEnvLevel(op, 1, part),
                params::opSpecEnvLevel(op, 2, part)},
               colour(tokens::purple), 0.0, 99.0, false),
          depth_(editor.state(), params::opSpecEnvDepth(op, part), "depth",
                 28.0f)
    {
        addAndMakeVisible(env_);
        addAndMakeVisible(depth_);
        for (int i = 0; i < 4; ++i) {
            rates_[static_cast<std::size_t>(i)] = std::make_unique<ParamKnob>(
                editor.state(), params::opSpecEnvRate(op, i, part),
                "r" + juce::String(i + 1), 28.0f);
            addAndMakeVisible(*rates_[static_cast<std::size_t>(i)]);
        }
        for (int i = 0; i < 3; ++i) {
            levels_[static_cast<std::size_t>(i)] = std::make_unique<ParamKnob>(
                editor.state(), params::opSpecEnvLevel(op, i, part),
                "l" + juce::String(i + 1), 28.0f);
            addAndMakeVisible(*levels_[static_cast<std::size_t>(i)]);
        }
    }

    void resized() override
    {
        P1Card::resized();
        auto area = contentArea();
        area.removeFromTop(8);
        env_.setBounds(area.removeFromTop(38));
        auto rates = area.removeFromTop(60);
        const int rateCell = rates.getWidth() / 4;
        for (auto& knob : rates_)
            knob->setBounds(rates.removeFromLeft(rateCell));
        auto levels = area.removeFromTop(60);
        const int levelCell = levels.getWidth() / 4;
        for (auto& knob : levels_)
            knob->setBounds(levels.removeFromLeft(levelCell));
        depth_.setBounds(levels.removeFromLeft(levelCell));
    }

    void tick() { env_.repaint(); }

private:
    P1Envelope env_;
    ParamKnob depth_;
    std::array<std::unique_ptr<ParamKnob>, 4> rates_{};
    std::array<std::unique_ptr<ParamKnob>, 3> levels_{};
};

class SpectrumCard : public P1Card {
public:
    SpectrumCard(PluginEditor& editor, int op, int part, bool a)
        : P1Card(a ? "spectrum a" : "spectrum b",
                 [&ed = editor, op, part, a] {
                     ed.copyOperatorSection(
                         op, part,
                         a ? PluginEditor::OperatorSection::spectrumA
                           : PluginEditor::OperatorSection::spectrumB);
                 },
                 [&ed = editor, op, part, a] {
                     ed.pasteOperatorSection(
                         op, part,
                         a ? PluginEditor::OperatorSection::spectrumA
                           : PluginEditor::OperatorSection::spectrumB);
                 }),
          editor_(editor),
          op_(op),
          part_(part),
          isA_(a),
          bars_(editor.state(), spectrumIDs(op, part, a),
                colour(a ? tokens::purple : tokens::pink)),
          range_(editor.state(),
                 a ? params::opSpecRangeA(op, part)
                   : params::opSpecRangeB(op, part),
                 rangeNames()),
          norm_("norm", true),
          toOther_(a ? "to b" : "to a", true),
          odd_("odd", 0.0, 1.0, [this] { beginMacro(); },
               [this](double value) { applyMacro(0, value); },
               [this] { endMacro(); }),
          even_("even", 0.0, 1.0, [this] { beginMacro(); },
                [this](double value) { applyMacro(1, value); },
                [this] { endMacro(); }),
          tilt_("tilt", -1.0, 1.0, [this] { beginMacro(); },
                [this](double value) { applyMacro(2, value); },
                [this] { endMacro(); }),
          formant_("formant", 0.0, 1.0, [this] { beginMacro(); },
                   [this](double value) { applyMacro(3, value); },
                   [this] { endMacro(); })
    {
        addAndMakeVisible(bars_);
        addAndMakeVisible(range_);
        rangeLabel_.setText("range", juce::dontSendNotification);
        rangeLabel_.setFont(uiFont(tokens::textLegend));
        rangeLabel_.setColour(juce::Label::textColourId, colour(tokens::dim));
        addAndMakeVisible(rangeLabel_);
        addAndMakeVisible(norm_);
        addAndMakeVisible(toOther_);
        addAndMakeVisible(odd_);
        addAndMakeVisible(even_);
        addAndMakeVisible(tilt_);
        addAndMakeVisible(formant_);
        addAndMakeVisible(centre_);
        centre_.addItemList(
            juce::StringArray{"h1", "h2", "h3", "h4", "h5", "h6", "h7", "h8",
                              "h9", "h10", "h11", "h12", "h13", "h14", "h15",
                              "h16"},
            1);
        centre_.setSelectedItemIndex(4, juce::dontSendNotification);
        bars_.onEdit = [this] { commitPreview(); };
        norm_.onClick = [this] { normalise(); };
        toOther_.onClick = [this] {
            commitPreview();
            if (copyToOther_)
                copyToOther_(bars_.levels());
        };
    }

    void setCopyToOther(
        std::function<void(const std::vector<double>&)> callback)
    {
        copyToOther_ = std::move(callback);
    }

    void setLevels(const std::vector<double>& levels)
    {
        bars_.setLevels(levels);
    }

    void commitWavePreview() { commitPreview(); }

    void tick()
    {
        updatePreview();
        bars_.repaint();
    }

    void paint(juce::Graphics& g) override
    {
        P1Card::paint(g);
        if (isA_ && bars_.hasPreview()) {
            g.setFont(uiFont(tokens::textLegend));
            g.setColour(colour(tokens::green));
            g.drawText("from wave",
                       getLocalBounds()
                           .removeFromTop(20)
                           .withTrimmedLeft(124)
                           .withTrimmedRight(100),
                       juce::Justification::centredLeft, false);
        }
    }

    void resized() override
    {
        P1Card::resized();
        auto area = contentArea();
        area.removeFromTop(4);
        auto rangeRow = area.removeFromTop(20);
        rangeLabel_.setBounds(rangeRow.removeFromLeft(38));
        range_.setBounds(rangeRow.removeFromLeft(92).reduced(1));
        area.removeFromTop(2);
        bars_.setBounds(area.removeFromTop(82));
        auto row = area.removeFromTop(22);
        odd_.setBounds(row.removeFromLeft(row.getWidth() / 3));
        even_.setBounds(row.removeFromLeft(row.getWidth() / 2));
        tilt_.setBounds(row);
        auto bottom = area.removeFromTop(22);
        formant_.setBounds(bottom.removeFromLeft(bottom.getWidth() / 2));
        centre_.setBounds(bottom.removeFromLeft(58).reduced(2, 1));
        norm_.setBounds(bottom.removeFromLeft(48).reduced(2, 1));
        toOther_.setBounds(bottom.removeFromLeft(48).reduced(2, 1));
    }

private:
    void beginMacro()
    {
        commitPreview();
        snapshot_ = bars_.levels();
    }

    void endMacro() { snapshot_.clear(); }

    void normalise()
    {
        commitPreview();
        auto levels = bars_.levels();
        double peak = 0.0;
        for (double level : levels)
            peak = juce::jmax(peak, level);
        if (peak <= 0.0)
            return;
        for (double& level : levels)
            level = level * 99.0 / peak;
        bars_.setLevels(levels);
    }

    std::vector<double> analyseWave(int wave, int first) const
    {
        const auto values = analyseWaveTable(wave, first);
        return std::vector<double>(values.begin(), values.end());
    }

    void updatePreview()
    {
        const int range = juce::jlimit(
            0, 7,
            static_cast<int>(std::lround(getParam(
                editor_, isA_ ? params::opSpecRangeA(op_, part_)
                              : params::opSpecRangeB(op_, part_)))));
        const int first = range * 16 + 1;
        bars_.setFirstHarmonic(first);
        if (!isA_)
            return;
        const int wave = static_cast<int>(
            std::lround(getParam(editor_, params::opWaveform(op_, part_))));
        if (wave == 13) {
            if (bars_.hasPreview())
                bars_.clearPreview();
            lastWave_ = 13;
            return;
        }
        if (wave < 0 || wave > 12)
            return;
        if (wave == lastWave_ && first == lastFirst_ && bars_.hasPreview())
            return;
        lastWave_ = wave;
        lastFirst_ = first;
        bars_.setPreviewLevels(analyseWave(wave, first));
    }

    void commitPreview()
    {
        if (!isA_ || !bars_.hasPreview())
            return;
        const auto levels = bars_.levels();
        bars_.clearPreview();
        bars_.setLevels(levels);
        setParam(editor_, params::opWaveform(op_, part_), 13);
        lastWave_ = 13;
    }

    void applyMacro(int kind, double amount)
    {
        if (snapshot_.empty())
            return;
        auto levels = snapshot_;
        const int centre = centre_.getSelectedItemIndex() + 1;
        for (int i = 0; i < static_cast<int>(levels.size()); ++i) {
            const int h = i + 1;
            switch (kind) {
            case 0:
                if (h % 2 == 0)
                    levels[static_cast<std::size_t>(i)] *= (1.0 - amount);
                break;
            case 1:
                if (h > 1 && h % 2 == 1)
                    levels[static_cast<std::size_t>(i)] *= (1.0 - amount);
                break;
            case 2:
                levels[static_cast<std::size_t>(i)] *= juce::jlimit(
                    0.0, 1.0, 1.0 + amount * (h - 1) / 15.0);
                break;
            case 3:
                levels[static_cast<std::size_t>(i)] = juce::jlimit(
                    0.0, 99.0,
                    levels[static_cast<std::size_t>(i)] +
                        amount * 99.0 *
                            std::exp(-((h - centre) * (h - centre)) / 4.5));
                break;
            default:
                break;
            }
            levels[static_cast<std::size_t>(i)] =
                juce::jlimit(0.0, 99.0, levels[static_cast<std::size_t>(i)]);
        }
        bars_.setLevels(levels);
    }

    PluginEditor& editor_;
    int op_ = 0;
    int part_ = 0;
    bool isA_ = true;
    int lastWave_ = -1;
    int lastFirst_ = 1;
    P1SpectralBars bars_;
    ParamChoice range_;
    juce::Label rangeLabel_;
    P1Button norm_;
    P1Button toOther_;
    MacroSlider odd_;
    MacroSlider even_;
    MacroSlider tilt_;
    MacroSlider formant_;
    juce::ComboBox centre_;
    std::vector<double> snapshot_;
    std::function<void(const std::vector<double>&)> copyToOther_;
};

class SpectraColumn : public juce::Component {
public:
    SpectraColumn(PluginEditor& editor, int op, std::function<void(int)> onCopy,
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
          osc_(editor, op, part_, true),
          a_(editor, op, part_, true),
          b_(editor, op, part_, false)
    {
        addAndMakeVisible(header_);
        addAndMakeVisible(scope_);
        addAndMakeVisible(spectro_);
        addAndMakeVisible(osc_);
        addAndMakeVisible(a_);
        addAndMakeVisible(b_);
        a_.setCopyToOther(
            [this](const std::vector<double>& levels) { b_.setLevels(levels); });
        b_.setCopyToOther([this](const std::vector<double>& levels) {
            a_.commitWavePreview();
            a_.setLevels(levels);
        });
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
        a_.setBounds(area.removeFromTop(220));
        area.removeFromTop(4);
        b_.setBounds(area.removeFromTop(220));
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(colour(tokens::bg));
        const int algorithm = static_cast<int>(
            std::lround(getParam(editor_, params::algorithm(part_))));
        g.setFont(displayFont(tokens::textOperatorName));
        g.setColour(colour(isCarrier(algorithm, op_) ? tokens::pink
                                                      : tokens::white));
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
        a_.tick();
        b_.tick();
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
    SpectrumCard a_;
    SpectrumCard b_;
    std::array<float, 512> scratch_{};
};

class SpectraModColumn : public juce::Component {
public:
    SpectraModColumn(PluginEditor& editor, int op,
                     std::function<void(int)> onCopy,
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
          osc_(editor, op, part_, true),
          display_(editor.state(), spectrumIDs(op, part_, true),
                   spectrumIDs(op, part_, false),
                   params::opSpecMorph(op, part_),
                   params::opWaveform(op, part_),
                   params::opSpecRangeA(op, part_),
                   params::opSpecRangeB(op, part_), colour(tokens::purple),
                   colour(tokens::pink)),
          env_(editor, op, part_),
          lfo_(editor, "morph lfo", params::opSpecLfoRate(op, part_),
               params::opSpecLfoDepth(op, part_),
               params::opSpecLfoWave(op, part_),
               params::opSpecLfoMode(op, part_), op, part_,
               PluginEditor::OperatorSection::spectrumLfo)
    {
        addAndMakeVisible(header_);
        addAndMakeVisible(osc_);
        addAndMakeVisible(display_);
        addAndMakeVisible(env_);
        addAndMakeVisible(lfo_);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(8);
        header_.setBounds(area.removeFromTop(24));
        osc_.setBounds(area.removeFromTop(140));
        area.removeFromTop(4);
        display_.setBounds(area.removeFromTop(120));
        area.removeFromTop(4);
        env_.setBounds(area.removeFromTop(220));
        area.removeFromTop(4);
        lfo_.setBounds(area.removeFromTop(140));
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(colour(tokens::bg));
        const int algorithm = static_cast<int>(
            std::lround(getParam(editor_, params::algorithm(part_))));
        g.setFont(displayFont(tokens::textOperatorName));
        g.setColour(colour(isCarrier(algorithm, op_) ? tokens::pink
                                                      : tokens::white));
        g.drawText("op" + juce::String(op_ + 1), 8, 8, 64, 20,
                   juce::Justification::centredLeft, false);
    }

    void tick()
    {
        header_.tick();
        display_.repaint();
        env_.tick();
        repaint();
    }

private:
    PluginEditor& editor_;
    int op_ = 0;
    int part_ = 0;
    CardHeader header_;
    OscillatorCard osc_;
    P1SpectrumDisplay display_;
    MorphEnvelopeCard env_;
    LfoCard lfo_;
};

class OperatorSpectraPage : public PageBase {
public:
    explicit OperatorSpectraPage(PluginEditor& editor) : editor_(editor)
    {
        for (int i = 0; i < 4; ++i) {
            columns_[static_cast<std::size_t>(i)] =
                std::make_unique<SpectraColumn>(
                    editor, i, [this](int op) { editor_.copyOperator(op); },
                    [this](int op) { editor_.pasteOperator(op); });
            addAndMakeVisible(*columns_[static_cast<std::size_t>(i)]);
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
    std::array<std::unique_ptr<SpectraColumn>, 4> columns_{};
};

class OperatorSpectraModPage : public PageBase {
public:
    explicit OperatorSpectraModPage(PluginEditor& editor) : editor_(editor)
    {
        for (int i = 0; i < 4; ++i) {
            columns_[static_cast<std::size_t>(i)] =
                std::make_unique<SpectraModColumn>(
                    editor, i, [this](int op) { editor_.copyOperator(op); },
                    [this](int op) { editor_.pasteOperator(op); });
            addAndMakeVisible(*columns_[static_cast<std::size_t>(i)]);
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
    std::array<std::unique_ptr<SpectraModColumn>, 4> columns_{};
};

} // namespace

std::unique_ptr<PageBase> makeOperatorSpectraPage(PluginEditor& editor)
{
    return std::make_unique<OperatorSpectraPage>(editor);
}

std::unique_ptr<PageBase> makeOperatorSpectraModPage(PluginEditor& editor)
{
    return std::make_unique<OperatorSpectraModPage>(editor);
}

} // namespace p1::ui
