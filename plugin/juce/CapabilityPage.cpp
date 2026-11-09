/* SPDX-License-Identifier: GPL-3.0-only */
#include "CapabilityPage.h"

#include <memory>
#include <vector>

#include "CapabilityPages.h"
#include "Theme.h"
#include "Widgets.h"

namespace p1::ui {
namespace {

struct Control {
    int kind = 0;
    std::unique_ptr<ParamKnob> knob;
    std::unique_ptr<ParamChoice> choice;
    std::unique_ptr<juce::Label> label;
};

class CapabilitySchemaPage : public PageBase {
public:
    CapabilitySchemaPage(int index, PluginEditor& editor)
    {
        const auto& page = kP1CapabilityPages[index];
        card_ = std::make_unique<P1Card>(page.title);
        addAndMakeVisible(*card_);
        note_ = std::make_unique<juce::Label>();
        note_->setText(page.note != nullptr ? page.note : "",
                       juce::dontSendNotification);
        note_->setFont(uiFont(tokens::textNote));
        note_->setColour(juce::Label::textColourId, colour(tokens::dim));
        if (note_->getText().isNotEmpty())
            card_->addAndMakeVisible(*note_);
        for (int i = 0; i < page.n_controls; ++i) {
            const auto& spec = page.controls[i];
            Control control;
            control.kind = spec.kind;
            if (spec.kind == 0) {
                control.knob = std::make_unique<ParamKnob>(
                    editor.state(), spec.param, spec.label, 56.0f);
                card_->addAndMakeVisible(*control.knob);
            } else {
                juce::StringArray choices;
                for (int c = 0; c < spec.n_choices; ++c)
                    choices.add(spec.choices[c]);
                control.choice = std::make_unique<ParamChoice>(
                    editor.state(), spec.param, choices);
                card_->addAndMakeVisible(*control.choice);
                control.label = std::make_unique<juce::Label>();
                control.label->setText(spec.label, juce::dontSendNotification);
                control.label->setFont(uiFont(tokens::textInlineLabel));
                control.label->setColour(juce::Label::textColourId,
                                         colour(tokens::dim));
                card_->addAndMakeVisible(*control.label);
            }
            controls_.push_back(std::move(control));
        }
    }

    void resized() override
    {
        card_->setBounds(getLocalBounds());
        auto area = card_->contentArea().reduced(12);
        if (note_ != nullptr && note_->getText().isNotEmpty())
            note_->setBounds(area.removeFromTop(22));
        const int count = static_cast<int>(controls_.size());
        if (count <= 0)
            return;
        const int columns = juce::jmin(4, count);
        const int rows = (count + columns - 1) / columns;
        const int width = area.getWidth() / columns;
        const int height = juce::jmin(190, area.getHeight() / juce::jmax(1, rows));
        for (int i = 0; i < count; ++i) {
            const int column = i % columns;
            const int row = i / columns;
            auto cell = juce::Rectangle<int>(
                area.getX() + column * width, area.getY() + row * height,
                width, height);
            auto& control = controls_[static_cast<size_t>(i)];
            if (control.kind == 0) {
                control.knob->setBounds(
                    cell.withSizeKeepingCentre(104, 124));
            } else {
                auto inner = cell.reduced(8, 18);
                control.label->setBounds(inner.removeFromTop(20));
                control.choice->setBounds(inner.removeFromTop(28));
            }
        }
    }

    void tick() override {}

private:
    std::unique_ptr<P1Card> card_;
    std::unique_ptr<juce::Label> note_;
    std::vector<Control> controls_;
};

} // namespace

std::unique_ptr<PageBase> makeCapabilitySchemaPage(int index,
                                                   PluginEditor& editor)
{
    return std::make_unique<CapabilitySchemaPage>(index, editor);
}

} // namespace p1::ui
