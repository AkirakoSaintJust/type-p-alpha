/* SPDX-License-Identifier: GPL-3.0-only */
#include "Pages.h"

#include <array>

#include "ParamIDs.h"

namespace p1::ui {
namespace {

class FeedbackEnvelopeCard : public P1Card {
public:
    FeedbackEnvelopeCard(PluginEditor& editor, int part)
        : P1Card("feedback envelope"),
          envView_(editor.state(),
                   {params::feedbackRate(0, part), params::feedbackRate(1, part),
                    params::feedbackRate(2, part), params::feedbackRate(3, part)},
                   {params::feedbackLevel(0, part), params::feedbackLevel(1, part),
                    params::feedbackLevel(2, part)},
                   colour(tokens::purple), 0.0, 99.0, false)
    {
        addAndMakeVisible(envView_);
        for (int i = 0; i < 4; ++i) {
            rates_[static_cast<size_t>(i)] = std::make_unique<ParamKnob>(
                editor.state(), params::feedbackRate(i, part),
                "r" + juce::String(i + 1), 42.0f);
            addAndMakeVisible(*rates_[static_cast<size_t>(i)]);
        }
        for (int i = 0; i < 3; ++i) {
            levels_[static_cast<size_t>(i)] = std::make_unique<ParamKnob>(
                editor.state(), params::feedbackLevel(i, part),
                "l" + juce::String(i + 1), 42.0f);
            addAndMakeVisible(*levels_[static_cast<size_t>(i)]);
        }
    }

    void resized() override
    {
        auto area = contentArea();
        envView_.setBounds(area.removeFromTop(150));
        area.removeFromTop(6);
        auto row = area.removeFromTop(84);
        const int cell = row.getWidth() / 4;
        for (auto& knob : rates_)
            knob->setBounds(row.removeFromLeft(cell));
        auto levelRow = area.removeFromTop(84);
        const int levelCell = levelRow.getWidth() / 3;
        for (auto& knob : levels_)
            knob->setBounds(levelRow.removeFromLeft(levelCell));
    }

    void tick() { envView_.repaint(); }

private:
    P1Envelope envView_;
    std::array<std::unique_ptr<ParamKnob>, 4> rates_{};
    std::array<std::unique_ptr<ParamKnob>, 3> levels_{};
};

class FeedbackDepthCard : public P1Card {
public:
    FeedbackDepthCard(PluginEditor& editor, int part)
        : P1Card("operator depth")
    {
        for (int i = 0; i < 4; ++i) {
            depths_[static_cast<size_t>(i)] = std::make_unique<ParamKnob>(
                editor.state(), params::opFeedbackDepth(i, part),
                "op" + juce::String(i + 1), 42.0f);
            shapes_[static_cast<size_t>(i)] = std::make_unique<ParamChoice>(
                editor.state(), params::opFeedbackShape(i, part),
                params::feedbackShapeNames());
            filters_[static_cast<size_t>(i)] = std::make_unique<ParamChoice>(
                editor.state(), params::opFeedbackFilter(i, part),
                params::feedbackFilterNames());
            addAndMakeVisible(*depths_[static_cast<size_t>(i)]);
            addAndMakeVisible(*shapes_[static_cast<size_t>(i)]);
            addAndMakeVisible(*filters_[static_cast<size_t>(i)]);
        }
    }

    void resized() override
    {
        auto area = contentArea();
        const int width = area.getWidth() / 4;
        for (int i = 0; i < 4; ++i) {
            auto column = area.removeFromLeft(width);
            auto knob = column.removeFromTop(76);
            depths_[static_cast<size_t>(i)]->setBounds(
                knob.withSizeKeepingCentre(80, 76));
            shapes_[static_cast<size_t>(i)]->setBounds(
                column.removeFromTop(20).withTrimmedTop(4).withWidth(200));
            filters_[static_cast<size_t>(i)]->setBounds(
                column.removeFromTop(20).withTrimmedTop(4).withWidth(140));
        }
    }

private:
    std::array<std::unique_ptr<ParamKnob>, 4> depths_{};
    std::array<std::unique_ptr<ParamChoice>, 4> shapes_{};
    std::array<std::unique_ptr<ParamChoice>, 4> filters_{};
};

class FeedbackPage : public PageBase {
public:
    explicit FeedbackPage(PluginEditor& editor)
    {
        const int part = editor.activePart();
        env_ = std::make_unique<FeedbackEnvelopeCard>(editor, part);
        depth_ = std::make_unique<FeedbackDepthCard>(editor, part);
        addAndMakeVisible(*env_);
        addAndMakeVisible(*depth_);
    }

    void resized() override
    {
        auto area = getLocalBounds();
        env_->setBounds(area.removeFromTop(380));
        area.removeFromTop(10);
        depth_->setBounds(area);
    }

    void tick() override { env_->tick(); }

private:
    std::unique_ptr<FeedbackEnvelopeCard> env_;
    std::unique_ptr<FeedbackDepthCard> depth_;
};

} // namespace

std::unique_ptr<PageBase> makeFeedbackPage(PluginEditor& editor)
{
    return std::make_unique<FeedbackPage>(editor);
}

} // namespace p1::ui
