/* SPDX-License-Identifier: GPL-3.0-only */
#include "ui.h"

#include <algorithm>
#include <cmath>

#include "Theme.h"
#include "Widgets.h"

namespace p1::ui {
namespace {

constexpr float kPi = 3.14159265358979323846f;

float tone_cutoff(float tone)
{
    const float t = juce::jlimit(0.0f, 100.0f, tone) / 100.0f;
    return 200.0f + t * t * 19800.0f;
}

float tone_coeff(float tone)
{
    return juce::jmin(1.0f, 2.0f * kPi * tone_cutoff(tone) / 44100.0f);
}

class ToneGraph : public juce::Component {
public:
    explicit ToneGraph(PluginEditor& editor) : editor_(editor) {}

    void paint(juce::Graphics& g) override
    {
        auto area = getLocalBounds().toFloat().reduced(2.0f);
        auto labels = area.removeFromBottom(18.0f);
        const auto grid = colour(tokens::line);
        const auto faint = colour(tokens::faint);
        const bool on = getParam(editor_, "cap_tone_on") > 0.5;
        const float tone = static_cast<float>(getParam(editor_, "cap_tone"));
        const float a = tone_coeff(tone);

        g.setColour(grid);
        for (int i = 0; i <= 6; ++i) {
            const float y = area.getY() + area.getHeight() * (float)i / 6.0f;
            g.drawHorizontalLine(juce::roundToInt(y), area.getX(), area.getRight());
        }
        for (int i = 0; i <= 5; ++i) {
            const float x = area.getX() + area.getWidth() * (float)i / 5.0f;
            g.drawVerticalLine(juce::roundToInt(x), area.getY(), area.getBottom());
        }

        g.setFont(uiFont(tokens::textNote));
        g.setColour(faint);
        g.drawText("0 dB", area.withHeight(16), juce::Justification::topLeft, false);
        g.drawText("-18", area.withHeight(16).withTrimmedTop(area.getHeight() * 0.5f - 8.0f),
                   juce::Justification::topLeft, false);
        g.drawText("-36 dB",
                   area.withHeight(16).withTrimmedTop(area.getHeight() - 18.0f),
                   juce::Justification::bottomLeft, false);
        g.drawText("20", labels, juce::Justification::centredLeft, false);
        g.drawText("1k", labels.withTrimmedLeft(labels.getWidth() * 0.45f),
                   juce::Justification::centredLeft, false);
        g.drawText("20k", labels, juce::Justification::centredRight, false);

        juce::Path curve;
        const int points = juce::jmax(2, juce::roundToInt(area.getWidth()));
        for (int i = 0; i < points; ++i) {
            const float x = area.getX() + area.getWidth() * (float)i / (float)(points - 1);
            const float hz = 20.0f * std::pow(1000.0f, (float)i / (float)(points - 1));
            const float w = 2.0f * kPi * hz / 44100.0f;
            const float re = 1.0f - (1.0f - a) * std::cos(w);
            const float im = (1.0f - a) * std::sin(w);
            const float mag = a / std::sqrt(re * re + im * im);
            const float db = 20.0f * std::log10(juce::jmax(1.0e-6f, mag));
            const float y = area.getBottom() -
                            area.getHeight() * juce::jlimit(0.0f, 1.0f, (db + 36.0f) / 42.0f);
            if (i == 0)
                curve.startNewSubPath(x, y);
            else
                curve.lineTo(x, y);
        }
        g.setColour(colour(on ? tokens::green : tokens::dim));
        g.strokePath(curve, juce::PathStrokeType(2.0f));
    }

private:
    PluginEditor& editor_;
};

class ToneCurvePage : public PageBase {
public:
    explicit ToneCurvePage(PluginEditor& editor)
        : graph_(editor),
          on_(editor.state(), "cap_tone_on",
              juce::StringArray{"off", "on"}),
          tone_(editor.state(), "cap_tone", "tone", 72.0f)
    {
        addAndMakeVisible(card_);
        addAndMakeVisible(graph_);
        addAndMakeVisible(on_);
        addAndMakeVisible(tone_);
        onLabel_.setText("on", juce::dontSendNotification);
        onLabel_.setFont(uiFont(tokens::textInlineLabel));
        onLabel_.setColour(juce::Label::textColourId, colour(tokens::dim));
        addAndMakeVisible(onLabel_);
    }

    void resized() override
    {
        card_.setBounds(getLocalBounds());
        auto area = card_.contentArea().reduced(12);
        auto controls = area.removeFromRight(240);
        graph_.setBounds(area.reduced(0, 8));
        onLabel_.setBounds(controls.removeFromTop(20));
        on_.setBounds(controls.removeFromTop(28));
        controls.removeFromTop(18);
        tone_.setBounds(controls.removeFromTop(112).withSizeKeepingCentre(112, 112));
    }

    void tick() override { graph_.repaint(); }

private:
    P1Card card_{"tone curve"};
    ToneGraph graph_;
    ParamChoice on_;
    ParamKnob tone_;
    juce::Label onLabel_;
};

} // namespace

std::unique_ptr<PageBase> makeToneCurvePage(PluginEditor& editor)
{
    return std::make_unique<ToneCurvePage>(editor);
}

} // namespace p1::ui
