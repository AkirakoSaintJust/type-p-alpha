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
    for (int h = 1; h <= 64; ++h)
        ids.add(a ? params::opSpecA(op, h, part)
                  : params::opSpecB(op, h, part));
    return ids;
}

juce::StringArray rangeNames()
{
    return {"1-64", "65-128"};
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
                params::opSpecEnvRelRate(op, part)},
               {params::opSpecEnvLevel(op, 0, part),
                params::opSpecEnvLevel(op, 1, part),
                params::opSpecEnvLevel(op, 2, part),
                params::opSpecEnvRelLevel(op, part)},
               colour(tokens::purple), 0.0, 99.0, false,
               EnvelopeShape::bipolar, params::opSpecEnvMode(op, part)),
          on_(editor.state(), params::opSpecEnvOn(op, part), "on"),
          mode_(editor.state(), params::opSpecEnvMode(op, part),
                {"off", "sustain", "loop1", "loop2"})
    {
        addAndMakeVisible(env_);
        addAndMakeVisible(on_);
        addAndMakeVisible(mode_);
        on_.onClick = [&ed = editor, op, part] {
            ensureSpectrumMode(ed, op, part, params::opSpecEnvOn(op, part));
        };
        for (int i = 0; i < 3; ++i) {
            rates_[static_cast<std::size_t>(i)] = std::make_unique<ParamKnob>(
                editor.state(), params::opSpecEnvRate(op, i, part),
                "r" + juce::String(i + 1), 28.0f);
            addAndMakeVisible(*rates_[static_cast<std::size_t>(i)]);
        }
        rates_[3] = std::make_unique<ParamKnob>(
            editor.state(), params::opSpecEnvRelRate(op, part), "rel", 28.0f);
        addAndMakeVisible(*rates_[3]);
        for (int i = 0; i < 3; ++i) {
            levels_[static_cast<std::size_t>(i)] = std::make_unique<ParamKnob>(
                editor.state(), params::opSpecEnvLevel(op, i, part),
                "l" + juce::String(i + 1), 28.0f);
            addAndMakeVisible(*levels_[static_cast<std::size_t>(i)]);
        }
        levels_[3] = std::make_unique<ParamKnob>(
            editor.state(), params::opSpecEnvRelLevel(op, part), "rl", 28.0f);
        addAndMakeVisible(*levels_[3]);
        setHeaderTrailingWidth(48);
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
        auto modeRow = area.removeFromTop(24);
        mode_.setBounds(modeRow.removeFromLeft(96).withSizeKeepingCentre(96, 22));
        on_.setBounds(headerTrailingBounds());
    }

    void tick() { env_.repaint(); }

private:
    P1Envelope env_;
    P1Toggle on_;
    ParamChoice mode_;
    std::array<std::unique_ptr<ParamKnob>, 4> rates_{};
    std::array<std::unique_ptr<ParamKnob>, 4> levels_{};
};

class SpectrumRow : public P1Card {
public:
    SpectrumRow(PluginEditor& editor, int op, int part, bool a)
        : P1Card("op" + juce::String(op + 1) + " spectrum " + (a ? "a" : "b"),
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
        maskLabel_.setText("mask", juce::dontSendNotification);
        maskLabel_.setFont(uiFont(tokens::textLegend));
        maskLabel_.setColour(juce::Label::textColourId, colour(tokens::dim));
        addAndMakeVisible(maskLabel_);
        addAndMakeVisible(mask_);
        selectionLabel_.setText("selection", juce::dontSendNotification);
        selectionLabel_.setFont(uiFont(tokens::textLegend));
        selectionLabel_.setColour(juce::Label::textColourId,
                                  colour(tokens::dim));
        addAndMakeVisible(selectionLabel_);
        addAndMakeVisible(selection_);
        groupLabel_.setText("group", juce::dontSendNotification);
        groupLabel_.setFont(uiFont(tokens::textLegend));
        groupLabel_.setColour(juce::Label::textColourId, colour(tokens::dim));
        addAndMakeVisible(groupLabel_);
        addAndMakeVisible(group_);
        template_.onClick = [this] { showTemplateMenu(); };
        addAndMakeVisible(template_);
        addAndMakeVisible(norm_);
        addAndMakeVisible(toOther_);
        addAndMakeVisible(tilt_);
        addAndMakeVisible(level_);
        addAndMakeVisible(formant_);
        addAndMakeVisible(centre_);

        legend_.setFont(uiFont(tokens::textLegend));
        legend_.setColour(juce::Label::textColourId, colour(tokens::dim));
        legend_.setMinimumHorizontalScale(0.6f);
        legend_.setInterceptsMouseClicks(false, false);
        addAndMakeVisible(legend_);

        group_.addItemList(
            juce::StringArray(kSpectrumGroupLabels, kSpectrumGroupCount), 1);
        group_.setSelectedItemIndex(0, juce::dontSendNotification);
        group_.onChange = [this] {
            refreshGroupTest();
            refreshLegend();
        };
        mask_.onChange = [this](int) {
            refreshGroupTest();
            refreshLegend();
        };
        selection_.onChange = [this] { applySelection(); };
        centre_.addItemList(
            juce::StringArray([] {
                juce::StringArray names;
                for (int h = 1; h <= 64; ++h)
                    names.add("h" + juce::String(h));
                return names;
            }()),
            1);
        centre_.setSelectedItemIndex(4, juce::dontSendNotification);
        norm_.onClick = [this] { normalise(); };
        toOther_.onClick = [this] { copyToOther(); };
        toOther_.setButtonText(isA_ ? "to b" : "to a");
        refreshSelectionItems();
        applySelection();
        refreshGroupTest();
        refreshLegend();
    }

    void tick()
    {
        const int first = firstHarmonic();
        bars_.setFirstHarmonic(first);
        if (first != lastFirst_) {
            lastFirst_ = first;
            refreshSelectionItems();
            applySelection();
        }
        refreshLegend();
        bars_.repaint();
    }

    void resized() override
    {
        P1Card::resized();
        /* Header controls sit to the left of the card's copy and paste pair. */
        auto header = getLocalBounds().removeFromTop(20).reduced(4, 1);
        header.removeFromRight(112);
        toOther_.setBounds(header.removeFromRight(44));
        header.removeFromRight(4);
        norm_.setBounds(header.removeFromRight(44));
        header.removeFromRight(6);
        template_.setBounds(header.removeFromRight(150));
        header.removeFromRight(6);
        group_.setBounds(header.removeFromRight(120));
        header.removeFromRight(3);
        groupLabel_.setBounds(header.removeFromRight(30));
        header.removeFromRight(6);
        selection_.setBounds(header.removeFromRight(104));
        header.removeFromRight(3);
        selectionLabel_.setBounds(header.removeFromRight(48));
        header.removeFromRight(6);
        mask_.setBounds(header.removeFromRight(112));
        header.removeFromRight(3);
        maskLabel_.setBounds(header.removeFromRight(26));
        header.removeFromRight(6);
        range_.setBounds(header.removeFromRight(64));
        header.removeFromRight(3);
        rangeLabel_.setBounds(header.removeFromRight(30));

        auto area = contentArea();
        auto controls = area.removeFromRight(280);
        area.removeFromRight(8);
        bars_.setBounds(area.removeFromTop(82));
        legend_.setBounds(area.removeFromTop(14));
        controls.removeFromTop(6);
        tilt_.setBounds(controls.removeFromLeft(84));
        controls.removeFromLeft(4);
        level_.setBounds(controls.removeFromLeft(84));
        controls.removeFromLeft(4);
        formant_.setBounds(controls.removeFromLeft(50));
        centre_.setBounds(
            controls.removeFromLeft(58).withTrimmedBottom(78));
    }

private:
    int firstHarmonic() const
    {
        const int range = juce::jlimit(
            0, 1,
            static_cast<int>(std::lround(getParam(
                editor_, isA_ ? params::opSpecRangeA(op_, part_)
                              : params::opSpecRangeB(op_, part_)))));
        return range * 64 + 1;
    }

    void refreshGroupTest()
    {
        bars_.setGroupTest([this](int harmonic) {
            return group_.getSelectedItemIndex() != 0 &&
                   groupHas(group_.getSelectedItemIndex(), harmonic);
        });
    }

    std::pair<int, int> windowFor(int index) const
    {
        static constexpr int kOffsets[7][2] = {
            {0, 63}, {0, 31}, {32, 63}, {0, 15},
            {16, 31}, {32, 47}, {48, 63}};
        const int choice = juce::jlimit(0, 6, index);
        const int first = firstHarmonic();
        return {first + kOffsets[choice][0], first + kOffsets[choice][1]};
    }

    void refreshSelectionItems()
    {
        const int choice = juce::jlimit(0, 6, selection_.getSelectedItemIndex());
        juce::StringArray labels;
        for (int i = 0; i < 7; ++i) {
            const auto [first, last] = windowFor(i);
            labels.add("edit " + juce::String(first) + "-" + juce::String(last));
        }
        selection_.clear(juce::dontSendNotification);
        selection_.addItemList(labels, 1);
        selection_.setSelectedItemIndex(choice, juce::dontSendNotification);
    }

    void applySelection()
    {
        const int choice = juce::jlimit(0, 6, selection_.getSelectedItemIndex());
        const auto [first, last] = windowFor(choice);
        bars_.setEditWindow(first, last);
        bars_.setEditTest(
            [this](int harmonic, double level) { return editable(harmonic, level); });
    }

    bool editable(int harmonic, double level) const
    {
        const auto [first, last] = windowFor(selection_.getSelectedItemIndex());
        if (harmonic < first || harmonic > last)
            return false;
        const int mask = mask_.index();
        if (mask == 2)
            return true;
        const bool live = level > 0.0;
        return mask == 0 ? live : !live;
    }

    bool selected(int index) const
    {
        if (snapshot_.empty())
            return false;
        const int harmonic = firstHarmonic() + index;
        const auto [first, last] =
            windowFor(selection_.getSelectedItemIndex());
        if (harmonic < first || harmonic > last)
            return false;
        if (!groupHas(group_.getSelectedItemIndex(), harmonic))
            return false;
        const int mask = mask_.index();
        if (mask == 2)
            return true;
        const bool live =
            snapshot_[static_cast<std::size_t>(index)] > 0.0;
        return mask == 0 ? live : !live;
    }

    void beginMacro() { snapshot_ = bars_.levels(); }

    void endMacro() { snapshot_.clear(); }

    void applyTilt(double amount)
    {
        if (snapshot_.empty())
            return;
        auto levels = snapshot_;
        const double centre =
            (static_cast<double>(levels.size()) - 1.0) * 0.5;
        for (int i = 0; i < static_cast<int>(levels.size()); ++i) {
            if (!selected(i))
                continue;
            levels[static_cast<std::size_t>(i)] = juce::jlimit(
                0.0, 99.0,
                levels[static_cast<std::size_t>(i)] *
                    (1.0 + amount *
                               (static_cast<double>(i) - centre) / centre));
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
        const int formantCentre = centre_.getSelectedItemIndex() + 1;
        const auto [winFirst, winLast] =
            windowFor(selection_.getSelectedItemIndex());
        for (int i = 0; i < static_cast<int>(levels.size()); ++i) {
            const int h = firstHarmonic() + i;
            if (h < winFirst || h > winLast)
                continue;
            levels[static_cast<std::size_t>(i)] = juce::jlimit(
                0.0, 99.0,
                levels[static_cast<std::size_t>(i)] +
                    amount * 99.0 *
                        std::exp(-((h - formantCentre) * (h - formantCentre)) /
                                 4.5));
        }
        bars_.setLevels(levels);
    }

    void normalise()
    {
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

    void copyToOther()
    {
        const auto levels = bars_.levels();
        const auto other = spectrumIDs(op_, part_, !isA_);
        for (int i = 0; i < other.size() && i < static_cast<int>(levels.size());
             ++i) {
            if (auto* parameter = editor_.state().getParameter(other[i])) {
                parameter->setValueNotifyingHost(parameter->convertTo0to1(
                    static_cast<float>(levels[static_cast<std::size_t>(i)])));
            }
        }
    }

    void showTemplateMenu()
    {
        juce::PopupMenu menu;
        const auto names = params::waveNames();
        for (int i = 0; i < 13 && i < names.size(); ++i)
            menu.addItem(i + 1, names[i]);
        menu.showMenuAsync(
            juce::PopupMenu::Options().withTargetComponent(&template_)
                .withMinimumWidth(150),
            [this](int result) {
                if (result > 0)
                    loadTemplate(result - 1);
            });
    }

    void loadTemplate(int wave)
    {
        const auto values = analyseWaveTable(wave, firstHarmonic());
        const auto ids = spectrumIDs(op_, part_, isA_);
        for (int i = 0; i < ids.size() && i < 64; ++i)
            setParam(editor_, ids[i],
                     static_cast<float>(
                         std::lround(values[static_cast<std::size_t>(i)])));
        bars_.repaint();
    }

    void refreshLegend()
    {
        const int first = firstHarmonic();
        const int group = group_.getSelectedItemIndex();
        juce::String text(kSpectrumGroupNames[group]);
        text << " \xc2\xb7 " << kSpectrumMaskNames[mask_.index()];
        const auto [winFirst, winLast] = windowFor(selection_.getSelectedItemIndex());
        text << " \xc2\xb7 edit " << winFirst << "-" << winLast;
        int count = 0;
        for (int i = 0; i < 64; ++i) {
            const int harmonic = first + i;
            if (!groupHas(group, harmonic))
                continue;
            ++count;
            if (count <= 24)
                text << " " << juce::String(harmonic);
        }
        if (count == 0)
            text << " none in range";
        else if (count > 24)
            text << " " << juce::String(count) << " members";
        if (legend_.getText() != text)
            legend_.setText(text, juce::dontSendNotification);
    }

    PluginEditor& editor_;
    int op_ = 0;
    int part_ = 0;
    bool isA_ = true;
    P1SpectralBars bars_;
    ParamChoice range_;
    juce::Label rangeLabel_;
    MaskRow mask_;
    juce::Label maskLabel_;
    P1Choice selection_;
    juce::Label selectionLabel_;
    P1Choice group_;
    juce::Label groupLabel_;
    P1Button template_{"load spectrum template...", true};
    P1Button norm_{"norm", true};
    P1Button toOther_{"to b", true};
    SpringSlider tilt_;
    SpringSlider level_;
    SpringSlider formant_;
    juce::ComboBox centre_;
    juce::Label legend_;
    std::vector<double> snapshot_;
    int lastFirst_ = -1;
};

class SpectraOperatorCard : public juce::Component {
public:
    SpectraOperatorCard(PluginEditor& editor, int op)
        : editor_(editor),
          op_(op),
          part_(editor.activePart()),
          header_(
              false, false, {},
              [&ed = editor, op] { ed.copyOperator(op); },
              [&ed = editor, op] { ed.pasteOperator(op); },
              [&ed = editor, op] {
                  return getParam(ed, params::opMute(op, ed.activePart())) >
                         0.5;
              },
              [&ed = editor, op](bool muted) {
                  setParam(ed, params::opMute(op, ed.activePart()),
                           muted ? 1 : 0);
              }),
          osc_(editor, op, editor.activePart(), true)
    {
        addAndMakeVisible(header_);
        addAndMakeVisible(scope_);
        addAndMakeVisible(spectro_);
        addAndMakeVisible(osc_);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(8, 0);
        header_.setBounds(area.removeFromTop(24));
        auto visual = area.removeFromTop(30);
        const int half = (visual.getWidth() - 6) / 2;
        scope_.setBounds(visual.removeFromLeft(half));
        visual.removeFromLeft(6);
        spectro_.setBounds(visual);
        area.removeFromTop(3);
        osc_.setBounds(area.removeFromTop(112));
    }

    void tick()
    {
        editor_.processor().readTapScope(
            op_ * 2 + part_, scratch_.data(),
            static_cast<int>(scratch_.size()));
        scope_.setSamples(scratch_.data(), static_cast<int>(scratch_.size()));
        spectro_.setSamples(scratch_.data(), static_cast<int>(scratch_.size()));
        header_.tick();
        repaint();
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

private:
    PluginEditor& editor_;
    int op_ = 0;
    int part_ = 0;
    CardHeader header_;
    P1Scope scope_;
    P1Spectro spectro_;
    OscillatorCard osc_;
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
                   params::opSpecRangeA(op, part_),
                   params::opSpecRangeB(op, part_), colour(tokens::purple),
                   colour(tokens::pink)),
          env_(editor, op, part_),
          lfo_(editor, "morph lfo", params::opSpecLfoRate(op, part_),
               params::opSpecLfoDepth(op, part_),
               params::opSpecLfoWave(op, part_),
               params::opSpecLfoMode(op, part_), op, part_,
               PluginEditor::OperatorSection::spectrumLfo,
               [&ed = editor, op] {
                   ensureSpectrumMode(ed, op, ed.activePart(),
                                      params::opSpecLfoDepth(op, ed.activePart()));
               })
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
        area.removeFromTop(6);
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
        display_.setLiveMorph(editor_.processor().specMorph(op_, part_));
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
    OperatorSpectraPage(PluginEditor& editor, bool a)
    {
        for (int i = 0; i < 4; ++i) {
            opcards_[static_cast<std::size_t>(i)] =
                std::make_unique<SpectraOperatorCard>(editor, i);
            addAndMakeVisible(*opcards_[static_cast<std::size_t>(i)]);
            rows_[static_cast<std::size_t>(i)] =
                std::make_unique<SpectrumRow>(editor, i,
                                              editor.activePart(), a);
            content_.addAndMakeVisible(*rows_[static_cast<std::size_t>(i)]);
        }
        viewport_.setViewedComponent(&content_, false);
        viewport_.setScrollBarsShown(false, false);
        addAndMakeVisible(viewport_);
    }

    void resized() override
    {
        auto area = getLocalBounds();
        auto top = area.removeFromTop(170);
        const int width = (top.getWidth() - 30) / 4;
        for (auto& card : opcards_) {
            card->setBounds(top.removeFromLeft(width));
            top.removeFromLeft(10);
        }
        viewport_.setBounds(area);
        const int contentWidth = juce::jmax(320, viewport_.getWidth());
        constexpr int pairHeight = 124;
        constexpr int pairGap = 4;
        content_.setSize(contentWidth, 4 * pairHeight + 3 * pairGap + 6);
        int y = 6;
        for (auto& row : rows_) {
            row->setBounds(0, y, contentWidth, pairHeight);
            y += pairHeight + pairGap;
        }
    }

    void tick() override
    {
        for (auto& card : opcards_)
            card->tick();
        for (auto& row : rows_)
            row->tick();
    }

private:
    std::array<std::unique_ptr<SpectraOperatorCard>, 4> opcards_{};
    std::array<std::unique_ptr<SpectrumRow>, 4> rows_{};
    juce::Component content_;
    juce::Viewport viewport_;
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

std::unique_ptr<PageBase> makeOperatorSpectraPage(PluginEditor& editor, bool a)
{
    return std::make_unique<OperatorSpectraPage>(editor, a);
}

std::unique_ptr<PageBase> makeOperatorSpectraModPage(PluginEditor& editor)
{
    return std::make_unique<OperatorSpectraModPage>(editor);
}

} // namespace p1::ui
