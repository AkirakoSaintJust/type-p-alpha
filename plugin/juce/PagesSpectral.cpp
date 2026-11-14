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

constexpr int kSpectrumGroupCount = 14;
const char* const kSpectrumGroupNames[kSpectrumGroupCount] = {
    "all", "odd", "even", "octave (P8)", "harmonic third (P5)",
    "harmonic fifth (M3)", "harmonic seventh (m7)",
    "harmonic fifteenth (M7)", "harmonic eleventh (TT)",
    "harmonic thirteenth (m6)", "harmonic ninth (M2)",
    "harmonic twenty-first (P4)", "harmonic nineteenth (m3)",
    "harmonic seventeenth (m2)"};
const char* const kSpectrumGroupLabels[kSpectrumGroupCount] = {
    "all", "odd", "even", "octave P8", "third P5", "fifth M3", "seventh m7",
    "fifteenth M7", "eleventh TT", "thirteenth m6", "ninth M2",
    "twenty-first P4", "nineteenth m3", "seventeenth m2"};
const char* const kSpectrumMaskNames[3] = {"live", "dead", "both"};

bool seriesHarmonic(int base, int harmonic)
{
    if (harmonic < base)
        return false;
    for (int value = base; value <= 128; value *= 2) {
        if (value == harmonic)
            return true;
    }
    return false;
}

bool groupHas(int group, int harmonic)
{
    switch (group) {
    case 0: return true;
    case 1: return harmonic >= 3 && harmonic % 2 == 1;
    case 2: return harmonic % 2 == 0;
    case 3: return seriesHarmonic(2, harmonic);
    case 4: return seriesHarmonic(3, harmonic);
    case 5: return seriesHarmonic(5, harmonic);
    case 6: return seriesHarmonic(7, harmonic);
    case 7: return seriesHarmonic(15, harmonic);
    case 8: return seriesHarmonic(11, harmonic);
    case 9: return seriesHarmonic(13, harmonic);
    case 10: return seriesHarmonic(9, harmonic);
    case 11: return seriesHarmonic(21, harmonic);
    case 12: return seriesHarmonic(19, harmonic);
    case 13: return seriesHarmonic(17, harmonic);
    default: return true;
    }
}

class SpringSlider : public juce::Component {
public:
    SpringSlider(juce::String label, std::function<void()> onBegin,
                 std::function<void(double)> onChange,
                 std::function<void()> onEnd = {})
        : label_(std::move(label)),
          onBegin_(std::move(onBegin)),
          onChange_(std::move(onChange)),
          onEnd_(std::move(onEnd))
    {
        setMouseCursor(juce::MouseCursor::UpDownResizeCursor);
    }

    void paint(juce::Graphics& g) override
    {
        auto area = getLocalBounds();
        g.setFont(uiFont(tokens::textLegend));
        g.setColour(colour(tokens::dim));
        g.drawText(label_, area.removeFromTop(12), juce::Justification::centred,
                   false);
        auto valueArea = area.removeFromBottom(12);
        g.setColour(colour(tokens::pink));
        g.drawText(valueText(), valueArea, juce::Justification::centred, false);
        auto track = area.reduced(0, 1).withSizeKeepingCentre(
            18, juce::jmax(12, area.getHeight() - 2));
        g.setColour(colour(tokens::bg));
        g.fillRect(track);
        g.setColour(colour(tokens::lineStrong));
        g.drawRect(track, 1);
        const float centre = static_cast<float>(track.getCentreY());
        g.drawHorizontalLine(juce::roundToInt(centre),
                             static_cast<float>(track.getX() + 2),
                             static_cast<float>(track.getRight() - 2));
        const float travel =
            static_cast<float>(track.getHeight()) * 0.5f - 4.0f;
        const float thumb = centre - static_cast<float>(amount_) * travel;
        g.setColour(colour(tokens::pink));
        g.fillRect(juce::Rectangle<float>(
            static_cast<float>(track.getX() + 2), thumb - 1.5f,
            static_cast<float>(track.getWidth() - 4), 3.0f));
    }

    void mouseDown(const juce::MouseEvent& event) override
    {
        if (event.mods.isPopupMenu())
            return;
        dragging_ = true;
        startY_ = event.position.y;
        if (onBegin_)
            onBegin_();
        setAmount(0.0);
    }

    void mouseDrag(const juce::MouseEvent& event) override
    {
        if (!dragging_ || event.mods.isPopupMenu())
            return;
        setAmount((startY_ - event.position.y) / 32.0);
    }

    void mouseUp(const juce::MouseEvent&) override
    {
        if (!dragging_)
            return;
        dragging_ = false;
        amount_ = 0.0;
        repaint();
        if (onEnd_)
            onEnd_();
    }

private:
    juce::String valueText() const
    {
        const int percent =
            static_cast<int>(std::lround(amount_ * 100.0));
        return (percent > 0 ? "+" : "") + juce::String(percent) + "%";
    }

    void setAmount(double amount)
    {
        amount_ = juce::jlimit(-1.0, 1.0, amount);
        repaint();
        if (onChange_)
            onChange_(amount_);
    }

    juce::String label_;
    double amount_ = 0.0;
    float startY_ = 0.0f;
    bool dragging_ = false;
    std::function<void()> onBegin_;
    std::function<void(double)> onChange_;
    std::function<void()> onEnd_;
};

class MaskRow : public juce::Component {
public:
    explicit MaskRow(int initial) : index_(juce::jlimit(0, 2, initial))
    {
        setMouseCursor(juce::MouseCursor::PointingHandCursor);
    }

    int index() const { return index_; }

    void paint(juce::Graphics& g) override
    {
        auto area = getLocalBounds();
        const int cell = area.getWidth() / 3;
        for (int i = 0; i < 3; ++i) {
            auto r = (i == 2 ? area : area.removeFromLeft(cell)).reduced(1);
            g.setColour(colour(tokens::bg));
            g.fillRect(r);
            g.setColour(i == index_ ? colour(tokens::green)
                                    : colour(tokens::lineStrong));
            g.drawRect(r, 1);
            g.setFont(uiFont(10.0f));
            g.setColour(i == index_ ? colour(tokens::green)
                                    : colour(tokens::dim));
            g.drawText(kSpectrumMaskNames[i], r,
                       juce::Justification::centred, false);
            if (i == 2)
                break;
        }
    }

    void mouseDown(const juce::MouseEvent& event) override
    {
        if (event.mods.isPopupMenu())
            return;
        const int next = juce::jlimit(
            0, 2, static_cast<int>(event.position.x * 3.0f /
                                   static_cast<float>(
                                       juce::jmax(1, getWidth()))));
        if (next == index_)
            return;
        index_ = next;
        repaint();
        if (onChange)
            onChange(index_);
    }

    std::function<void(int)> onChange;

private:
    int index_ = 2;
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
          mask_(2),
          tilt_(
              "tilt", [this] { beginMacro(); },
              [this](double value) { applyTilt(value); },
              [this] { endMacro(); }),
          level_(
              "level", [this] { beginMacro(); },
              [this](double value) { applyLevel(value); },
              [this] { endMacro(); }),
          formant_(
              "formant", [this] { beginMacro(); },
              [this](double value) { applyFormant(value); },
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
        addAndMakeVisible(group_);
        addAndMakeVisible(mask_);
        addAndMakeVisible(tilt_);
        addAndMakeVisible(level_);
        addAndMakeVisible(formant_);
        addAndMakeVisible(centre_);
        addAndMakeVisible(groupLabel_);
        addAndMakeVisible(maskLabel_);
        addAndMakeVisible(legend_);
        groupLabel_.setText("group", juce::dontSendNotification);
        groupLabel_.setFont(uiFont(tokens::textLegend));
        groupLabel_.setColour(juce::Label::textColourId, colour(tokens::dim));
        maskLabel_.setText("mask", juce::dontSendNotification);
        maskLabel_.setFont(uiFont(tokens::textLegend));
        maskLabel_.setColour(juce::Label::textColourId, colour(tokens::dim));
        legend_.setFont(uiFont(tokens::textLegend));
        legend_.setColour(juce::Label::textColourId, colour(tokens::dim));
        legend_.setMinimumHorizontalScale(0.6f);
        legend_.setInterceptsMouseClicks(false, false);
        group_.addItemList(
            juce::StringArray(kSpectrumGroupLabels, kSpectrumGroupCount), 1);
        group_.setSelectedItemIndex(0, juce::dontSendNotification);
        group_.onChange = [this] {
            bars_.setGroupTest(
                [this](int harmonic) {
                    return groupHas(group_.getSelectedItemIndex(), harmonic);
                });
            refreshLegend();
        };
        mask_.onChange = [this](int) { refreshLegend(); };
        bars_.setGroupTest([this](int harmonic) {
            return groupHas(group_.getSelectedItemIndex(), harmonic);
        });
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
        refreshLegend();
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
        area.removeFromTop(2);
        auto rangeRow = area.removeFromTop(18);
        rangeLabel_.setBounds(rangeRow.removeFromLeft(34));
        range_.setBounds(rangeRow.removeFromLeft(88).reduced(1));
        area.removeFromTop(2);
        bars_.setBounds(area.removeFromTop(62));
        legend_.setBounds(area.removeFromTop(12));
        auto groupRow = area.removeFromTop(20);
        groupLabel_.setBounds(groupRow.removeFromLeft(32));
        group_.setBounds(groupRow.removeFromLeft(100).reduced(1));
        groupRow.removeFromLeft(4);
        maskLabel_.setBounds(groupRow.removeFromLeft(28));
        mask_.setBounds(groupRow);
        auto springs = area.removeFromTop(64);
        const int springWidth = springs.getWidth() / 3;
        tilt_.setBounds(springs.removeFromLeft(springWidth).reduced(1));
        level_.setBounds(springs.removeFromLeft(springWidth).reduced(1));
        auto formantCell = springs;
        formant_.setBounds(formantCell.removeFromTop(44).reduced(1));
        centre_.setBounds(
            formantCell.removeFromTop(18).withSizeKeepingCentre(58, 18));
        auto foot = area.removeFromTop(20);
        norm_.setBounds(foot.removeFromLeft(48).reduced(1));
        toOther_.setBounds(foot.removeFromRight(48).reduced(1));
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
        refreshLegend();
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

    int firstHarmonic() const
    {
        const int range = juce::jlimit(
            0, 7,
            static_cast<int>(std::lround(getParam(
                editor_, isA_ ? params::opSpecRangeA(op_, part_)
                              : params::opSpecRangeB(op_, part_)))));
        return range * 16 + 1;
    }

    bool selected(int index) const
    {
        if (snapshot_.empty())
            return false;
        const int harmonic = firstHarmonic() + index;
        if (!groupHas(group_.getSelectedItemIndex(), harmonic))
            return false;
        const int mask = mask_.index();
        if (mask == 2)
            return true;
        const bool live =
            snapshot_[static_cast<std::size_t>(index)] > 0.0;
        return mask == 0 ? live : !live;
    }

    void applyTilt(double amount)
    {
        if (snapshot_.empty())
            return;
        auto levels = snapshot_;
        for (int i = 0; i < static_cast<int>(levels.size()); ++i) {
            if (!selected(i))
                continue;
            levels[static_cast<std::size_t>(i)] = juce::jlimit(
                0.0, 99.0,
                levels[static_cast<std::size_t>(i)] *
                    (1.0 + amount * i / 15.0));
        }
        bars_.setLevels(levels);
    }

    void applyLevel(double amount)
    {
        if (snapshot_.empty())
            return;
        auto levels = snapshot_;
        for (int i = 0; i < static_cast<int>(levels.size()); ++i) {
            if (!selected(i))
                continue;
            levels[static_cast<std::size_t>(i)] = juce::jlimit(
                0.0, 99.0,
                levels[static_cast<std::size_t>(i)] + amount * 99.0);
        }
        bars_.setLevels(levels);
    }

    void applyFormant(double amount)
    {
        if (snapshot_.empty())
            return;
        auto levels = snapshot_;
        const int centre = centre_.getSelectedItemIndex() + 1;
        for (int i = 0; i < static_cast<int>(levels.size()); ++i) {
            const int h = i + 1;
            levels[static_cast<std::size_t>(i)] = juce::jlimit(
                0.0, 99.0,
                levels[static_cast<std::size_t>(i)] +
                    amount * 99.0 *
                        std::exp(-((h - centre) * (h - centre)) / 4.5));
        }
        bars_.setLevels(levels);
    }

    void refreshLegend()
    {
        const int first = firstHarmonic();
        const int group = group_.getSelectedItemIndex();
        juce::String text(kSpectrumGroupNames[group]);
        text << " \xc2\xb7 " << kSpectrumMaskNames[mask_.index()];
        bool any = false;
        for (int i = 0; i < 16; ++i) {
            const int harmonic = first + i;
            if (!groupHas(group, harmonic))
                continue;
            text << " " << juce::String(harmonic);
            any = true;
        }
        if (!any)
            text << " none in range";
        if (text != legend_.getText())
            legend_.setText(text, juce::dontSendNotification);
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
    P1Choice group_;
    MaskRow mask_;
    juce::Label groupLabel_;
    juce::Label maskLabel_;
    juce::Label legend_;
    SpringSlider tilt_;
    SpringSlider level_;
    SpringSlider formant_;
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

void commitWaveToSpectrum(PluginEditor& editor, int op, int part)
{
    const auto waveID = params::opWaveform(op, part);
    int wave = 13;
    if (auto* value = editor.state().getRawParameterValue(waveID))
        wave = static_cast<int>(std::lround(value->load()));
    if (wave < 0 || wave > 12)
        return;
    int range = 0;
    if (auto* value = editor.state().getRawParameterValue(
            params::opSpecRangeA(op, part))) {
        range = static_cast<int>(std::lround(value->load()));
    }
    range = juce::jlimit(0, 7, range);
    const auto levels = analyseWaveTable(wave, range * 16 + 1);
    for (int h = 0; h < 16; ++h) {
        if (auto* parameter =
                editor.state().getParameter(params::opSpecA(op, h + 1, part))) {
            parameter->setValueNotifyingHost(parameter->convertTo0to1(
                static_cast<float>(levels[static_cast<std::size_t>(h)])));
        }
    }
    if (auto* parameter = editor.state().getParameter(waveID)) {
        parameter->setValueNotifyingHost(
            parameter->convertTo0to1(13.0f));
    }
}

std::unique_ptr<PageBase> makeOperatorSpectraPage(PluginEditor& editor)
{
    return std::make_unique<OperatorSpectraPage>(editor);
}

std::unique_ptr<PageBase> makeOperatorSpectraModPage(PluginEditor& editor)
{
    return std::make_unique<OperatorSpectraModPage>(editor);
}

} // namespace p1::ui
