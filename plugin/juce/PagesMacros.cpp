/* SPDX-License-Identifier: GPL-3.0-only */
#include "Pages.h"

#include <array>
#include <cmath>

#include "MacroTargets.h"
#include "ParamIDs.h"

namespace p1::ui {
namespace {

/* The compact bipolar range a macro card uses. The stored value sits at the right
 * and the fill runs from the centre, so a destination or source range reads as a
 * signed amount. */
class MacroRange : public juce::Component {
public:
    MacroRange(APVTS& state, juce::String paramID, juce::Colour fill)
        : state_(state), paramID_(std::move(paramID)), fill_(fill)
    {
        spec_ = findSpec(paramID_);
        if (auto* param = state_.getParameter(paramID_)) {
            attach_ = std::make_unique<juce::ParameterAttachment>(
                *param, [this](float value) {
                    value_ = value;
                    repaint();
                });
            attach_->sendInitialUpdate();
        }
        setMouseCursor(juce::MouseCursor::PointingHandCursor);
    }

    void resized() override
    {
        track_ = getLocalBounds().withTrimmedRight(46).reduced(0, 2);
    }

    void paint(juce::Graphics& g) override
    {
        if (spec_ == nullptr)
            return;
        g.setColour(colour(tokens::lineStrong));
        g.drawRect(track_, 1);
        const auto inner = track_.reduced(1);
        const float span = static_cast<float>(spec_->max - spec_->min);
        const float norm =
            span > 0.0f
                ? juce::jlimit(0.0f, 1.0f,
                               (value_ - static_cast<float>(spec_->min)) / span)
                : 0.0f;
        const int centre = inner.getX() + inner.getWidth() / 2;
        const int edge = inner.getX() + static_cast<int>(std::round(
                                             static_cast<float>(inner.getWidth()) *
                                             norm));
        const int x = juce::jmin(centre, edge);
        const int width = juce::jmax(centre, edge) - x;
        g.setColour(fill_);
        g.fillRect(juce::Rectangle<int>(x, inner.getY(), width,
                                        inner.getHeight()));
        g.setColour(colour(tokens::lineStrong));
        g.drawVerticalLine(centre, static_cast<float>(inner.getY()),
                           static_cast<float>(inner.getBottom()));
        g.setFont(uiFont(tokens::textInlineLabel));
        g.setColour(colour(tokens::pinkBright));
        g.drawText(formatParamValue(*spec_, value_),
                   getLocalBounds().removeFromRight(44),
                   juce::Justification::centredRight, false);
    }

    void mouseDown(const juce::MouseEvent& event) override { setFromMouse(event); }
    void mouseDrag(const juce::MouseEvent& event) override { setFromMouse(event); }

private:
    void setFromMouse(const juce::MouseEvent& event)
    {
        if (attach_ == nullptr || spec_ == nullptr || track_.getWidth() <= 0)
            return;
        const float norm = juce::jlimit(
            0.0f, 1.0f,
            (static_cast<float>(event.x) - static_cast<float>(track_.getX())) /
                static_cast<float>(track_.getWidth()));
        attach_->setValueAsCompleteGesture(
            static_cast<float>(spec_->min) +
            norm * static_cast<float>(spec_->max - spec_->min));
    }

    APVTS& state_;
    juce::String paramID_;
    juce::Colour fill_;
    const P1ParamSpec* spec_ = nullptr;
    float value_ = 0.0f;
    juce::Rectangle<int> track_;
    std::unique_ptr<juce::ParameterAttachment> attach_;
};

/* The target menu with its eight section headings. The item id is the target enum
 * plus one, so `none` is 1 and the parameter value is the id minus one. */
class MacroTargetChoice : public juce::Component {
public:
    MacroTargetChoice(APVTS& state, juce::String paramID)
        : state_(state), paramID_(std::move(paramID))
    {
        combo_.addItem("none", 1);
        juce::String lastSection;
        for (int i = 0; i < p1::macros::kTargetCount; ++i) {
            const auto& target = p1::macros::kTargets[static_cast<std::size_t>(i)];
            const juce::String section(target.section);
            if (section != lastSection) {
                combo_.addSectionHeading(section);
                lastSection = section;
            }
            combo_.addItem(target.name, i + 2);
        }
        combo_.onChange = [this] {
            if (updating_)
                return;
            const int id = combo_.getSelectedId();
            if (id <= 0)
                return;
            if (auto* parameter = state_.getParameter(paramID_))
                parameter->setValueNotifyingHost(
                    parameter->convertTo0to1(static_cast<float>(id - 1)));
        };
        addAndMakeVisible(combo_);
        if (auto* parameter = state_.getParameter(paramID_)) {
            attach_ = std::make_unique<juce::ParameterAttachment>(
                *parameter, [this](float value) {
                    const juce::ScopedValueSetter<bool> guard(updating_, true);
                    combo_.setSelectedId(
                        juce::jlimit(0, p1::macros::kTargetCount,
                                     static_cast<int>(std::lround(value))) +
                            1,
                        juce::dontSendNotification);
                });
            attach_->sendInitialUpdate();
        }
    }

    void resized() override { combo_.setBounds(getLocalBounds()); }

private:
    APVTS& state_;
    juce::String paramID_;
    juce::ComboBox combo_;
    std::unique_ptr<juce::ParameterAttachment> attach_;
    bool updating_ = false;
};

class MacroCard : public juce::Component {
public:
    MacroCard(PluginEditor& editor, int index)
        : editor_(editor),
          index_(index),
          knob_(editor.state(), params::macroValue(index), "", 92.0f),
          source_(editor.state(), params::macroMidi(index),
                  {"off", "cc " + juce::String(72 + index), "mod wheel",
                   "velocity"}),
          midiRange_(editor.state(), params::macroMidiRange(index),
                     colour(tokens::green)),
          rename_("rename", true)
    {
        addAndMakeVisible(knob_);
        addAndMakeVisible(source_);
        addAndMakeVisible(midiRange_);
        for (int destination = 0; destination < 3; ++destination) {
            targets_[static_cast<std::size_t>(destination)] =
                std::make_unique<MacroTargetChoice>(
                    editor.state(), params::macroTarget(index, destination));
            ranges_[static_cast<std::size_t>(destination)] =
                std::make_unique<MacroRange>(
                    editor.state(), params::macroRange(index, destination),
                    colour(tokens::purple));
            addAndMakeVisible(*targets_[static_cast<std::size_t>(destination)]);
            addAndMakeVisible(*ranges_[static_cast<std::size_t>(destination)]);
        }
        rename_.onClick = [this] { editor_.showRenameMacro(index_); };
        addAndMakeVisible(rename_);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(9);
        auto head = area.removeFromTop(22);
        rename_.setBounds(head.removeFromRight(60).withSizeKeepingCentre(58, 18));
        auto left = area.removeFromLeft(120);
        knob_.setBounds(left.withSizeKeepingCentre(116, 116));
        auto right = area;
        constexpr int rowHeight = 26;
        const int spacing =
            juce::jmax(4, (right.getHeight() - rowHeight * 4) / 5);
        int y = right.getY() + spacing;
        midiRow_ = juce::Rectangle<int>(right.getX(), y, right.getWidth(),
                                        rowHeight);
        const int selectWidth = juce::jlimit(120, 176, right.getWidth() - 50 - 190 - 14);
        source_.setBounds(midiRow_.getX() + 52, midiRow_.getY() + 3, selectWidth,
                          20);
        midiRange_.setBounds(midiRow_.getRight() - 190, midiRow_.getY(), 190,
                             rowHeight);
        y += rowHeight + spacing;
        for (int destination = 0; destination < 3; ++destination) {
            auto row = juce::Rectangle<int>(right.getX(), y, right.getWidth(),
                                            rowHeight);
            destRows_[static_cast<std::size_t>(destination)] = row;
            targets_[static_cast<std::size_t>(destination)]->setBounds(
                row.getX() + 18, row.getY() + 3,
                juce::jmax(80, row.getWidth() - 18 - 190 - 8), 20);
            ranges_[static_cast<std::size_t>(destination)]->setBounds(
                row.getRight() - 190, row.getY(), 190, rowHeight);
            y += rowHeight + spacing;
        }
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(colour(tokens::panel));
        drawSectionTitle(g, getLocalBounds().removeFromTop(20).reduced(8, 0),
                         title());
        g.setFont(uiFont(tokens::textInlineLabel));
        g.setColour(colour(tokens::dim));
        g.drawText("midi in", midiRow_.getX(), midiRow_.getY() + 4, 50, 16,
                   juce::Justification::centredLeft, false);
        for (int destination = 0; destination < 3; ++destination) {
            const auto& row = destRows_[static_cast<std::size_t>(destination)];
            g.setColour(destinationOn_[static_cast<std::size_t>(destination)]
                            ? colour(tokens::faint)
                            : colour(tokens::lineStrong));
            g.drawText(juce::String(destination + 1), row.getX(),
                       row.getY() + 4, 14, 16,
                       juce::Justification::centredRight, false);
        }
    }

    void tick()
    {
        const int source = static_cast<int>(std::lround(
            getParam(editor_, params::macroMidi(index_))));
        const bool midiOn = source != 0;
        midiRange_.setEnabled(midiOn);
        midiRange_.setAlpha(midiOn ? 1.0f : 0.35f);
        bool changed = midiOn != midiOn_;
        midiOn_ = midiOn;
        for (int destination = 0; destination < 3; ++destination) {
            const auto& row = static_cast<std::size_t>(destination);
            const bool on =
                std::lround(getParam(
                    editor_, params::macroTarget(index_, destination))) != 0;
            ranges_[row]->setEnabled(on);
            ranges_[row]->setAlpha(on ? 1.0f : 0.35f);
            changed = changed || on != destinationOn_[row];
            destinationOn_[row] = on;
        }
        const double effective = editor_.processor().macroEffectiveValue(index_);
        knob_.setModArc((effective + 100.0) / 200.0);
        if (changed || name_ != title()) {
            name_ = title();
            repaint();
        }
    }

private:
    juce::String title() const
    {
        const auto name = editor_.processor().getMacroName(index_);
        return name.isNotEmpty() ? name : "macro " + juce::String(index_ + 1);
    }

    PluginEditor& editor_;
    int index_ = 0;
    ParamKnob knob_;
    ParamChoice source_;
    MacroRange midiRange_;
    std::array<std::unique_ptr<MacroTargetChoice>, 3> targets_{};
    std::array<std::unique_ptr<MacroRange>, 3> ranges_{};
    P1Button rename_{"rename", true};
    juce::Rectangle<int> midiRow_;
    std::array<juce::Rectangle<int>, 3> destRows_{};
    std::array<bool, 3> destinationOn_{};
    bool midiOn_ = false;
    juce::String name_;
};

class MacrosPage : public PageBase {
public:
    explicit MacrosPage(PluginEditor& editor) : editor_(editor)
    {
        for (int i = 0; i < 6; ++i) {
            cards_[static_cast<std::size_t>(i)] =
                std::make_unique<MacroCard>(editor, i);
            addAndMakeVisible(*cards_[static_cast<std::size_t>(i)]);
        }
    }

    void resized() override
    {
        const auto area = getLocalBounds();
        constexpr int gap = 10;
        const int columnWidth = (area.getWidth() - gap) / 2;
        const int rowHeight = (area.getHeight() - 2 * gap) / 3;
        for (int i = 0; i < 6; ++i) {
            const int column = i % 2;
            const int row = i / 2;
            cards_[static_cast<std::size_t>(i)]->setBounds(
                area.getX() + column * (columnWidth + gap),
                area.getY() + row * (rowHeight + gap), columnWidth, rowHeight);
        }
    }

    void paint(juce::Graphics& g) override { g.fillAll(colour(tokens::bg)); }

    void tick() override
    {
        for (auto& card : cards_)
            card->tick();
    }

private:
    PluginEditor& editor_;
    std::array<std::unique_ptr<MacroCard>, 6> cards_{};
};

} // namespace

std::unique_ptr<PageBase> makeMacrosPage(PluginEditor& editor)
{
    return std::make_unique<MacrosPage>(editor);
}

} // namespace p1::ui
