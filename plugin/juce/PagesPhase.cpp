/* SPDX-License-Identifier: GPL-3.0-only */
#include "Pages.h"

#include <array>
#include <cmath>

#include "ParamIDs.h"

namespace p1::ui {
namespace {

float rawValue(APVTS& state, const juce::String& id)
{
    if (auto* value = state.getRawParameterValue(id))
        return value->load();
    return 0.0f;
}

bool isCarrier(int algorithm, int op)
{
    static const int cars[8][4] = {
        {3, -1, -1, -1}, {3, -1, -1, -1}, {3, -1, -1, -1}, {3, -1, -1, -1},
        {1, 3, -1, -1},  {1, 2, 3, -1},   {1, 2, 3, -1},   {0, 1, 2, 3},
    };
    algorithm = juce::jlimit(0, 7, algorithm);
    for (int value : cars[algorithm])
        if (value == op)
            return true;
    return false;
}

/* The static parameter preview is the fallback until the part's map has been
 * seeded. Once the engine publishes one, the same plot uses the real composed
 * geometry and the newest voice's mapped phase. */
class PhaseMapView : public juce::Component {
public:
    PhaseMapView(PluginEditor& editor, int op, int part)
        : state_(editor.state()),
          processor_(editor.processor()),
          op_(op),
          part_(part)
    {
        setInterceptsMouseClicks(false, false);
    }

    void paint(juce::Graphics& g) override
    {
        auto area = getLocalBounds().toFloat().reduced(2.0f);
        auto captionArea = area.toNearestInt().removeFromBottom(13);
        g.fillAll(colour(tokens::bg));

        p1::PhaseMapSnapshot live;
        const bool hasLive = processor_.phaseMap(op_, part_, live);
        const int on = static_cast<int>(
            std::lround(rawValue(state_, params::opPhase(op_, "on", part_))));
        const int fallbackSegments = juce::jlimit(
            1, 32, static_cast<int>(std::lround(
                       rawValue(state_, params::opPhase(op_, "segments", part_)))));
        /* Off is the neutral map, not the last live state. The engine also
         * clears the pool on the switch edge and on a patch load. */
        const bool liveMap = hasLive && on != 0;
        const int segments = liveMap ? live.segments : fallbackSegments;
        const float bp = liveMap
            ? static_cast<float>(live.bp) / 1000.0f
            : rawValue(state_, params::opPhase(op_, "bp", part_)) / 1000.0f;

        const auto alpha = on ? 1.0f : 0.45f;
        g.setColour(colour(tokens::lineStrong).withAlpha(0.28f));
        for (int i = 1; i < 4; ++i) {
            const float x = area.getX() +
                area.getWidth() * static_cast<float>(i) / 4.0f;
            const float y = area.getY() +
                area.getHeight() * static_cast<float>(i) / 4.0f;
            g.drawVerticalLine(juce::roundToInt(x), area.getY(), area.getBottom());
            g.drawHorizontalLine(juce::roundToInt(y), area.getX(), area.getRight());
        }

        const auto point = [&area](float x, float y) {
            return juce::Point<float>(area.getX() + x * area.getWidth(),
                                      area.getBottom() - y * area.getHeight());
        };

        /* F: 0 -> 0.5 at the folded breakpoint, then 0.5 -> 1. */
        const float xb = juce::jlimit(0.001f, 0.999f, bp);
        juce::Path f;
        f.startNewSubPath(point(0.0f, 0.0f));
        f.lineTo(point(xb, 0.5f));
        f.lineTo(point(1.0f, 1.0f));
        g.setColour(colour(tokens::lineStrong).withAlpha(0.55f * alpha));
        g.strokePath(f, juce::PathStrokeType(1.0f));

        if (liveMap) {
            const float dashes[] = {2.0f, 2.0f};
            for (int i = 1; i < live.segments; ++i) {
                const float y = static_cast<float>(i) /
                    static_cast<float>(live.segments);
                g.setColour(colour(tokens::lineStrong).withAlpha(0.10f * alpha));
                g.drawHorizontalLine(juce::roundToInt(area.getBottom() -
                                                       y * area.getHeight()),
                                     area.getX(), area.getRight());
            }
            for (std::size_t k = 0;
                 k < static_cast<std::size_t>(live.active); ++k) {
                const float x0 = static_cast<float>(live.x[k]) / 4096.0f;
                const float x1 = static_cast<float>(live.x[k + 1]) / 4096.0f;
                const float y0 = static_cast<float>(live.y0[k]) / 4096.0f;
                const int32_t dy = static_cast<int32_t>((
                    static_cast<int64_t>(live.x[k + 1] - live.x[k]) *
                    live.slope[k]) >> 16);
                const float y1 = static_cast<float>(juce::jlimit(
                    0, 4095, static_cast<int>(live.y0[k]) + dy)) / 4096.0f;
                const auto segmentColour = live.flip[k]
                    ? tokens::purple
                    : tokens::green;
                g.setColour(colour(segmentColour).withAlpha(
                    alpha * (live.locked[k] ? 0.35f : 1.0f)));
                g.drawLine(juce::Line<float>(point(x0, y0), point(x1, y1)),
                           1.4f);
                if (live.order[k] != static_cast<uint8_t>(k)) {
                    g.setColour(colour(tokens::pink).withAlpha(alpha));
                    g.drawDashedLine(
                        juce::Line<float>(point(x0, 0.0f), point(x0, 1.0f)),
                        dashes, 2, 1.0f);
                }
            }
            if (live.voices > 0) {
                const auto dot = point(static_cast<float>(live.phaseQ) / 4096.0f,
                                       static_cast<float>(live.phaseY) / 4096.0f);
                g.setColour(colour(tokens::white));
                g.fillEllipse(dot.x - 2.0f, dot.y - 2.0f, 4.0f, 4.0f);
            }
        } else {
            juce::Path map;
            map.startNewSubPath(point(0.0f, 0.0f));
            for (int i = 1; i <= segments; ++i) {
                const float x = static_cast<float>(i) /
                    static_cast<float>(segments);
                const float y = static_cast<float>(i) /
                    static_cast<float>(segments);
                map.lineTo(point(x, y));
            }
            g.setColour(colour(tokens::green).withAlpha(alpha));
            g.strokePath(map, juce::PathStrokeType(1.4f));
        }

        g.setColour(colour(tokens::pink).withAlpha(alpha));
        g.drawVerticalLine(juce::roundToInt(area.getX() + xb * area.getWidth()),
                           area.getY(), area.getBottom());

        int locked = 0;
        if (liveMap)
            for (std::size_t k = 0;
                 k < static_cast<std::size_t>(live.active); ++k)
                locked += live.locked[k] ? 1 : 0;
        g.setFont(uiFont(tokens::textLegend));
        g.setColour(colour(tokens::dim));
        const auto caption = "bp " + juce::String(bp, 3) + " · π";
        g.drawText(caption, captionArea,
                   juce::Justification::centredLeft, false);
        auto count = juce::String(liveMap ? live.active : segments) + "/" +
                     juce::String(segments) + " seg";
        if (locked > 0)
            count += " " + juce::String(locked) + " locked";
        g.drawText(count, captionArea, juce::Justification::centredRight,
                   false);
    }

private:
    APVTS& state_;
    TypePAlphaAudioProcessor& processor_;
    int op_ = 0;
    int part_ = 0;
};

class PhaseCard : public P1Card {
public:
    PhaseCard(PluginEditor& editor, int op, int part)
        : P1Card("phase",
                 [&ed = editor, op, part] {
                     ed.copyOperatorSection(
                         op, part, PluginEditor::OperatorSection::phase);
                 },
                 [&ed = editor, op, part] {
                     ed.pasteOperatorSection(
                         op, part, PluginEditor::OperatorSection::phase);
                 }),
          on_(editor.state(), params::opPhase(op, "on", part), "on")
    {
        static constexpr const char* fields[9] = {
            "bp", "segments", "flip", "flip_rate", "reorder",
            "reorder_rate", "jump", "drift", "drift_rate"};
        static constexpr const char* labels[9] = {
            "distort", "segmts", "flip", "fl rate", "reorder",
            "ro rate", "jump", "drift", "dr rate"};
        for (std::size_t i = 0; i < knobs_.size(); ++i) {
            knobs_[i] = std::make_unique<ParamKnob>(
                editor.state(), params::opPhase(op, fields[i], part), labels[i],
                28.0f);
            addAndMakeVisible(*knobs_[i]);
        }
        addAndMakeVisible(on_);
        setHeaderTrailingWidth(48);
    }

    void resized() override
    {
        P1Card::resized();
        auto area = contentArea();
        area.removeFromTop(3);
        for (int row = 0; row < 2; ++row) {
            auto line = area.removeFromTop(63);
            const int count = row == 0 ? 5 : 4;
            const int cell = line.getWidth() / count;
            for (int col = 0; col < count; ++col) {
                const int index = row == 0 ? col : 5 + col;
                knobs_[static_cast<std::size_t>(index)]->setBounds(
                    line.removeFromLeft(cell));
            }
        }
        on_.setBounds(headerTrailingBounds());
    }

private:
    P1Toggle on_;
    std::array<std::unique_ptr<ParamKnob>, 9> knobs_{};
};

class PhaseEnvelopeCard : public P1Card {
public:
    PhaseEnvelopeCard(PluginEditor& editor, int op, int part)
        : P1Card("phase env",
                 [&ed = editor, op, part] {
                     ed.copyOperatorSection(
                         op, part,
                         PluginEditor::OperatorSection::phaseEnvelope);
                 },
                 [&ed = editor, op, part] {
                     ed.pasteOperatorSection(
                         op, part,
                         PluginEditor::OperatorSection::phaseEnvelope);
                 }),
          envView_(editor.state(),
                   {params::opPhaseEnv(op, "r1", part),
                    params::opPhaseEnv(op, "r2", part),
                    params::opPhaseEnv(op, "r3", part),
                    params::opPhaseEnv(op, "relr", part)},
                   {params::opPhaseEnv(op, "l1", part),
                    params::opPhaseEnv(op, "l2", part),
                    params::opPhaseEnv(op, "l3", part),
                    params::opPhaseEnv(op, "rell", part)},
                   colour(tokens::purple), -100.0, 100.0, true,
                   EnvelopeShape::bipolar,
                   params::opPhaseEnv(op, "mode", part)),
          on_(editor.state(), params::opPhaseEnv(op, "on", part), "on"),
          mode_(editor.state(), params::opPhaseEnv(op, "mode", part),
                {"sustain off", "sustain on", "loop 1", "loop 2"})
    {
        addAndMakeVisible(envView_);
        addAndMakeVisible(on_);
        addAndMakeVisible(mode_);
        for (int i = 0; i < 3; ++i) {
            rates_[static_cast<std::size_t>(i)] = std::make_unique<ParamKnob>(
                editor.state(), params::opPhaseEnv(
                                    op, i == 0 ? "r1" : i == 1 ? "r2" : "r3",
                                    part),
                "r" + juce::String(i + 1), 18.0f);
            addAndMakeVisible(*rates_[static_cast<std::size_t>(i)]);
        }
        rates_[3] = std::make_unique<ParamKnob>(
            editor.state(), params::opPhaseEnv(op, "relr", part), "rel", 18.0f);
        addAndMakeVisible(*rates_[3]);
        for (int i = 0; i < 3; ++i) {
            levels_[static_cast<std::size_t>(i)] = std::make_unique<ParamKnob>(
                editor.state(), params::opPhaseEnv(
                                    op, i == 0 ? "l1" : i == 1 ? "l2" : "l3",
                                    part),
                "l" + juce::String(i + 1), 18.0f);
            addAndMakeVisible(*levels_[static_cast<std::size_t>(i)]);
        }
        levels_[3] = std::make_unique<ParamKnob>(
            editor.state(), params::opPhaseEnv(op, "rell", part), "rl", 18.0f);
        addAndMakeVisible(*levels_[3]);
        setHeaderTrailingWidth(48);
    }

    void resized() override
    {
        P1Card::resized();
        auto area = contentArea();
        envView_.setBounds(area.removeFromTop(48));
        area.removeFromTop(2);
        auto rates = area.removeFromTop(54);
        const int rateCell = rates.getWidth() / 4;
        for (auto& knob : rates_)
            knob->setBounds(rates.removeFromLeft(rateCell));
        auto levels = area.removeFromTop(54);
        const int levelCell = levels.getWidth() / 4;
        for (auto& knob : levels_)
            knob->setBounds(levels.removeFromLeft(levelCell));
        auto modeRow = area.removeFromTop(22);
        mode_.setBounds(modeRow.removeFromLeft(levelCell * 2));
        on_.setBounds(headerTrailingBounds());
    }

    void tick() { envView_.repaint(); }

private:
    P1Envelope envView_;
    P1Toggle on_;
    ParamChoice mode_;
    std::array<std::unique_ptr<ParamKnob>, 4> rates_{};
    std::array<std::unique_ptr<ParamKnob>, 4> levels_{};
};

class PhaseColumn : public juce::Component {
public:
    PhaseColumn(PluginEditor& editor, int op, std::function<void(int)> onCopy,
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
          map_(editor, op, part_),
          phase_(editor, op, part_),
          lfo_(editor, "phase lfo",
               params::opPhaseLfo(op, "rate", part_),
               params::opPhaseLfo(op, "depth", part_),
               params::opPhaseLfo(op, "wave", part_),
               params::opPhaseLfo(op, "mode", part_),
               params::opPhaseLfo(op, "rate_fade", part_),
               params::opPhaseLfo(op, "depth_fade", part_), op, part_,
               PluginEditor::OperatorSection::phaseLfo, {}, false, "on"),
          env_(editor, op, part_)
    {
        addAndMakeVisible(header_);
        addAndMakeVisible(map_);
        addAndMakeVisible(phase_);
        addAndMakeVisible(lfo_);
        addAndMakeVisible(env_);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(8);
        header_.setBounds(area.removeFromTop(26));
        area.removeFromTop(3);
        map_.setBounds(area.removeFromTop(99));
        area.removeFromTop(3);
        phase_.setBounds(area.removeFromTop(159));
        area.removeFromTop(3);
        lfo_.setBounds(area.removeFromTop(147));
        area.removeFromTop(3);
        env_.setBounds(area.removeFromTop(210));
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
        map_.repaint();
        env_.tick();
    }

private:
    PluginEditor& editor_;
    int op_ = 0;
    int part_ = 0;
    CardHeader header_;
    PhaseMapView map_;
    PhaseCard phase_;
    LfoCard lfo_;
    PhaseEnvelopeCard env_;
};

class PhaseMapPage : public PageBase {
public:
    explicit PhaseMapPage(PluginEditor& editor) : editor_(editor)
    {
        for (int i = 0; i < 4; ++i) {
            columns_[static_cast<std::size_t>(i)] =
                std::make_unique<PhaseColumn>(
                    editor, i, [this](int op) { editor_.copyOperator(op); },
                    [this](int op) { editor_.pasteOperator(op); });
            addAndMakeVisible(*columns_[static_cast<std::size_t>(i)]);
        }
    }

    void resized() override
    {
        auto area = getLocalBounds();
        constexpr int gap = 10;
        const int width = (area.getWidth() - gap * 3) / 4;
        for (int i = 0; i < 4; ++i) {
            columns_[static_cast<std::size_t>(i)]->setBounds(
                area.removeFromLeft(width));
            if (i < 3)
                area.removeFromLeft(gap);
        }
    }

    void tick() override
    {
        for (auto& column : columns_)
            column->tick();
    }

private:
    PluginEditor& editor_;
    std::array<std::unique_ptr<PhaseColumn>, 4> columns_{};
};

} // namespace

std::unique_ptr<PageBase> makePhaseMapPage(PluginEditor& editor)
{
    return std::make_unique<PhaseMapPage>(editor);
}

} // namespace p1::ui
