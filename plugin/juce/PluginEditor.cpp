/* SPDX-License-Identifier: GPL-3.0-only */
#include "PluginEditor.h"

#include <algorithm>
#include <cmath>

#include "Pages.h"
#include "ParamIDs.h"
#include "PluginProcessor.h"

namespace {

constexpr int kWidth = 1240;
constexpr int kHeight = 985;

class PatchButton : public juce::Button {
public:
    PatchButton() : juce::Button("multi-patch library...")
    {
        setMouseCursor(juce::MouseCursor::PointingHandCursor);
    }

    void paintButton(juce::Graphics& g, bool highlighted, bool) override
    {
        g.setColour(p1::ui::colour(p1::tokens::bg));
        g.fillRect(getLocalBounds());
        g.setColour(p1::ui::colour(highlighted ? p1::tokens::pink
                                               : p1::tokens::lineStrong));
        g.drawRect(getLocalBounds(), 1);
        g.setFont(p1::ui::uiFont(p1::tokens::textButton));
        g.setColour(p1::ui::colour(p1::tokens::white));
        g.drawText(getButtonText(), getLocalBounds().reduced(8, 0),
                   juce::Justification::centredLeft, true);
    }
};

class SegmentButton : public juce::Button {
public:
    explicit SegmentButton(const juce::String& label) : juce::Button(label)
    {
        setMouseCursor(juce::MouseCursor::PointingHandCursor);
    }

    void setActive(bool active)
    {
        if (active_ != active) {
            active_ = active;
            repaint();
        }
    }

    void paintButton(juce::Graphics& g, bool highlighted, bool) override
    {
        g.setColour(p1::ui::colour(p1::tokens::bg));
        g.fillRect(getLocalBounds());
        g.setColour(p1::ui::colour(active_ ? p1::tokens::purple
                                           : highlighted ? p1::tokens::pink
                                                         : p1::tokens::lineStrong));
        g.drawRect(getLocalBounds(), 1);
        g.setFont(p1::ui::uiFont(8.0f));
        g.setColour(p1::ui::colour(active_ ? p1::tokens::purple
                                           : p1::tokens::dim));
        g.drawText(getButtonText(), getLocalBounds(),
                   juce::Justification::centred, false);
    }

private:
    bool active_ = false;
};

class PartChoiceButton : public juce::Button {
public:
    explicit PartChoiceButton(const juce::String& label) : juce::Button(label)
    {
        setMouseCursor(juce::MouseCursor::PointingHandCursor);
    }

    void setActive(bool active)
    {
        if (active_ != active) {
            active_ = active;
            repaint();
        }
    }

    void paintButton(juce::Graphics& g, bool highlighted, bool) override
    {
        g.setColour(p1::ui::colour(p1::tokens::bg));
        g.fillRect(getLocalBounds());
        g.setColour(p1::ui::colour(active_ ? p1::tokens::green
                                           : highlighted ? p1::tokens::pink
                                                         : p1::tokens::lineStrong));
        g.drawRect(getLocalBounds(), 1);
        g.setFont(p1::ui::uiFont(p1::tokens::textButton));
        g.setColour(p1::ui::colour(active_ ? p1::tokens::green
                                           : p1::tokens::dim));
        g.drawText(getButtonText(), getLocalBounds().reduced(4, 0),
                   juce::Justification::centred, true);
    }

private:
    bool active_ = false;
};

} // namespace

class Header : public juce::Component {
public:
    explicit Header(PluginEditor& editor) : editor_(editor)
    {
        alg_.onClick = [this] { editor_.showAlgorithmPicker(); };
        alg_.onWheel = [this](int direction) {
            const int current = static_cast<int>(std::lround(p1::ui::getParam(
                editor_, p1::params::algorithm(editor_.activePart()))));
            editor_.applyAlgorithm((current + direction + 8) % 8);
        };
        addAndMakeVisible(alg_);

        patch_.onClick = [this] { editor_.showPatchBrowser(); };
        addAndMakeVisible(patch_);
        rename_.onClick = [this] { editor_.showRenameDialog(false, 0); };
        addAndMakeVisible(rename_);

        addAndMakeVisible(scopeView_);
        addAndMakeVisible(spectroView_);
        scopeSeg_.onClick = [this] { setSpectroMode(false); };
        spectroSeg_.onClick = [this] { setSpectroMode(true); };
        addAndMakeVisible(scopeSeg_);
        addAndMakeVisible(spectroSeg_);
        setPatchName(editor_.processor().getPatchName());
        setSpectroMode(false);
    }

    void resized() override
    {
        alg_.setBounds(300, 15, 330, 130);
        patch_.setBounds(1000, 54, 150, 35);
        rename_.setBounds(1156, 54, 64, 35);
        scopeSeg_.setBounds(876, 22, 44, 18);
        spectroSeg_.setBounds(920, 22, 56, 18);
        scopeView_.setBounds(650, 44, 330, 50);
        spectroView_.setBounds(650, 44, 330, 50);
    }

    void paint(juce::Graphics& g) override
    {
        drawWordmark(g);
        g.setFont(p1::ui::uiFont(p1::tokens::textBrandLine));
        g.setColour(p1::ui::colour(p1::tokens::dim));
        g.drawText(juce::String::fromUTF8(
                       "      p1onken // パンク派第１音響合成研究班"),
                   16, 80, 320, 18, juce::Justification::centredLeft, false);
        g.setFont(p1::ui::displayFont(p1::tokens::textToolbarLabel));
        g.setColour(p1::ui::colour(p1::tokens::dim));
        g.drawText("loaded multi-patch:", 998, 18, 236, 14,
                   juce::Justification::centredLeft, false);
        g.setFont(p1::ui::uiFont(p1::tokens::textButton));
        g.setColour(p1::ui::colour(p1::tokens::white));
        g.drawText(multiName_, 998, 32, 236, 16,
                   juce::Justification::centredLeft, true);
        g.setFont(p1::ui::displayFont(p1::tokens::textAlgorithmLabel));
        g.setColour(p1::ui::colour(p1::tokens::green));
        g.drawText("output", 650, 22, 120, 16, juce::Justification::centredLeft,
                   false);
    }

    void tick()
    {
        const int algorithm = static_cast<int>(std::lround(p1::ui::getParam(
            editor_, p1::params::algorithm(editor_.activePart()))));
        alg_.setAlgorithm(algorithm);
        setPatchName(editor_.processor().getPatchName());
        editor_.processor().readOutputScope(scratch_.data(),
                                            static_cast<int>(scratch_.size()));
        scopeView_.setSamples(scratch_.data(), static_cast<int>(scratch_.size()));
        spectroView_.setSamples(scratch_.data(), static_cast<int>(scratch_.size()));
    }

    void setPatchName(const juce::String& name)
    {
        if (name == multiName_)
            return;
        multiName_ = name;
        repaint();
    }

    void setSpectroMode(bool spectro)
    {
        spectroMode_ = spectro;
        scopeView_.setVisible(!spectroMode_);
        spectroView_.setVisible(spectroMode_);
        scopeSeg_.setActive(!spectroMode_);
        spectroSeg_.setActive(spectroMode_);
    }

private:
    void drawWordmark(juce::Graphics& g)
    {
        const auto display = p1::ui::displayFont(p1::tokens::textWordmark);
        const auto brand = p1::ui::brandFont(p1::tokens::textWordmark);
        const auto ink = p1::ui::colour(p1::tokens::brandInk);
        const auto shadow = p1::ui::colour(p1::tokens::brandShadow);
        const auto pink = p1::ui::colour(p1::tokens::pink);

        const float baseY = 42.0f;
        const juce::String name = "type P";
        const float wordWidth = p1::ui::textWidth(display, name);
        const float dotWidth = p1::ui::textWidth(display, ".");
        const float x = 16.0f;
        auto draw = [&g, baseY](const juce::String& text, const juce::Font& font,
                                juce::Colour colour, float dx, float dy) {
            g.setFont(font);
            g.setColour(colour);
            g.drawText(text, juce::Rectangle<float>(
                                 dx, baseY + dy,
                                 p1::ui::textWidth(font, text) + 4.0f, 34.0f),
                       juce::Justification::bottomLeft, false);
        };
        draw(name, display, shadow, x + 4.0f, 4.0f);
        draw(".", display, shadow, x + wordWidth + 4.0f, 4.0f);
        draw("a", brand, shadow, x + wordWidth + dotWidth + 4.0f, 4.0f);
        draw("a", brand, shadow, x + wordWidth + dotWidth + 4.7f, 4.4f);
        draw(name, display, ink, x, 0.0f);
        draw(".", display, pink, x + wordWidth, 0.0f);
        draw("a", brand, ink, x + wordWidth + dotWidth, 0.0f);
        draw("a", brand, ink, x + wordWidth + dotWidth + 0.7f, 0.4f);
    }

    PluginEditor& editor_;
    p1::ui::P1AlgorithmBlock alg_;
    PatchButton patch_;
    p1::ui::P1Button rename_{"rename"};
    p1::ui::P1Scope scopeView_;
    p1::ui::P1Spectro spectroView_;
    SegmentButton scopeSeg_{"scope"};
    SegmentButton spectroSeg_{"spectro"};
    bool spectroMode_ = false;
    std::array<float, 512> scratch_{};
    juce::String multiName_{"init patch"};
};

class Toolbar : public juce::Component {
public:
    explicit Toolbar(PluginEditor& editor) : editor_(editor)
    {
        const int part = editor_.activePart();
        for (int i = 0; i < 4; ++i) {
            opTiles_[static_cast<size_t>(i)] = std::make_unique<p1::ui::P1MuteTile>(
                "op" + juce::String(i + 1), false);
            opAttach_[static_cast<size_t>(i)] =
                std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
                    editor_.state(), p1::params::opMute(i, part),
                    *opTiles_[static_cast<size_t>(i)]);
            addAndMakeVisible(*opTiles_[static_cast<size_t>(i)]);
        }
        partTiles_[0] = std::make_unique<p1::ui::P1MuteTile>(
            "p1", p1::ui::getParam(editor_, p1::params::level(0)) <= 0.0,
            [this](bool muted) { editor_.togglePartMute(0, muted); });
        partTiles_[1] = std::make_unique<p1::ui::P1MuteTile>(
            "p2", p1::ui::getParam(editor_, p1::params::level(1)) <= 0.0,
            [this](bool muted) { editor_.togglePartMute(1, muted); });
        addAndMakeVisible(*partTiles_[0]);
        addAndMakeVisible(*partTiles_[1]);

        partButtons_[0] =
            std::make_unique<PartChoiceButton>("part 1: init");
        partButtons_[1] =
            std::make_unique<PartChoiceButton>("part 2: init");
        partButtons_[0]->onClick = [this] { editor_.setActivePart(0); };
        partButtons_[1]->onClick = [this] { editor_.setActivePart(1); };
        addAndMakeVisible(*partButtons_[0]);
        addAndMakeVisible(*partButtons_[1]);
        edit_.setButtonText("single part library...");
        edit_.onClick = [this] { editor_.showSinglePartsBrowser(); };
        addAndMakeVisible(edit_);
        refreshPartNames();
    }

    void resized() override
    {
        int x = 160;
        for (auto& tile : opTiles_) {
            tile->setBounds(x, 11, 36, 22);
            x += 40;
        }
        x += 12;
        partTiles_[0]->setBounds(x, 11, 36, 22);
        partTiles_[1]->setBounds(x + 40, 11, 36, 22);
        const int right = getWidth() - 16;
        const int actionsWidth = 176;
        edit_.setBounds(right - actionsWidth, 10, actionsWidth, 24);
        partButtons_[1]->setBounds(right - actionsWidth - 8 - 108, 10, 108, 24);
        partButtons_[0]->setBounds(right - actionsWidth - 8 - 108 - 8 - 108, 10,
                                   108, 24);
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(p1::ui::colour(p1::tokens::bg));
        g.setColour(p1::ui::colour(p1::tokens::lineStrong));
        g.fillRect(0, 0, getWidth(), 1);
        g.fillRect(0, getHeight() - 1, getWidth(), 1);
        drawLegend(g);
        g.setFont(p1::ui::uiFont(p1::tokens::textToolbarLabel));
        g.setColour(p1::ui::colour(p1::tokens::dim));
        g.drawText("now editing part:", getWidth() - 582, 0, 150, getHeight(),
                   juce::Justification::centredRight, false);
    }

    void refreshPartNames()
    {
        for (int i = 0; i < 2; ++i) {
            const auto text = "part " + juce::String(i + 1) + ": " +
                              editor_.processor().getPartName(i);
            if (partButtons_[static_cast<size_t>(i)]->getButtonText() != text)
                partButtons_[static_cast<size_t>(i)]->setButtonText(text);
        }
    }

    void tick()
    {
        refreshPartNames();
        for (int i = 0; i < 2; ++i) {
            const bool muted =
                p1::ui::getParam(editor_, p1::params::level(i)) <= 0.0;
            if (partTiles_[static_cast<size_t>(i)]->getToggleState() != muted) {
                partTiles_[static_cast<size_t>(i)]->setToggleState(
                    muted, juce::dontSendNotification);
                partTiles_[static_cast<size_t>(i)]->repaint();
            }
            partButtons_[static_cast<size_t>(i)]->setActive(
                editor_.activePart() == i);
        }
    }

private:
    void drawLegend(juce::Graphics& g)
    {
        g.setColour(p1::ui::colour(p1::tokens::bg));
        g.fillRect(16, 14, 15, 15);
        g.setColour(p1::ui::colour(p1::tokens::pink));
        g.drawRect(16, 14, 15, 15, 1);
        g.setFont(p1::ui::uiFont(11.0f));
        g.drawText(juce::String::fromUTF8("\xe7\x8c\xab"), 16, 14, 15, 15,
                   juce::Justification::centred, false);
        g.setColour(p1::ui::colour(p1::tokens::dim));
        g.drawText("muted", 36, 14, 50, 15, juce::Justification::centredLeft, false);
        g.setColour(p1::ui::colour(p1::tokens::pink));
        g.fillRect(90, 14, 15, 15);
        g.setColour(p1::ui::colour(p1::tokens::bg));
        g.drawText(juce::String::fromUTF8("\xe7\x8c\xab"), 90, 14, 15, 15,
                   juce::Justification::centred, false);
        g.setColour(p1::ui::colour(p1::tokens::dim));
        g.drawText("active", 110, 14, 50, 15, juce::Justification::centredLeft,
                   false);
    }

    PluginEditor& editor_;
    std::array<std::unique_ptr<p1::ui::P1MuteTile>, 4> opTiles_{};
    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>,
               4>
        opAttach_{};
    std::array<std::unique_ptr<p1::ui::P1MuteTile>, 2> partTiles_{};
    std::array<std::unique_ptr<PartChoiceButton>, 2> partButtons_{};
    PartChoiceButton edit_{"part library actions..."};
};

namespace {

using namespace p1;
using namespace p1::ui;

class ListBody : public juce::Component, private juce::ListBoxModel {
public:
    ListBody(int count, int rowHeight, int selected, const juce::String& name)
        : count_(juce::jmax(0, count)),
          selected_(juce::jlimit(0, juce::jmax(0, count_ - 1), selected)),
          list_(name, this)
    {
        addAndMakeVisible(list_);
        list_.setRowHeight(rowHeight);
        list_.setColour(juce::ListBox::backgroundColourId,
                        colour(tokens::popup));
        list_.setColour(juce::ListBox::outlineColourId,
                        juce::Colours::transparentBlack);
        list_.setColour(juce::ListBox::textColourId, colour(tokens::text));
        list_.selectRow(selected_, true, true);
    }

    int selected() const { return selected_; }

    void resized() override
    {
        list_.setBounds(getLocalBounds());
        list_.scrollToEnsureRowIsOnscreen(selected_);
    }

    int getNumRows() override { return count_; }

    void paintListBoxItem(int row, juce::Graphics& g, int width, int height,
                          bool selected) override
    {
        const auto area = juce::Rectangle<int>(0, 0, width, height);
        if (selected) {
            g.setColour(colour(tokens::purple).withAlpha(0.08f));
            g.fillRect(area);
            g.setColour(colour(tokens::purple));
            g.drawRect(area, 1);
        } else {
            g.setColour(colour(tokens::popup));
            g.fillRect(area);
        }
        g.setColour(colour(tokens::line));
        g.fillRect(8, height - 1, width - 16, 1);
        paintRow(g, row, area, selected);
    }

    void listBoxItemClicked(int row, const juce::MouseEvent&) override
    {
        selected_ = juce::jlimit(0, juce::jmax(0, count_ - 1), row);
        list_.selectRow(selected_, true, true);
        repaint();
    }

protected:
    virtual void paintRow(juce::Graphics& g, int row,
                          juce::Rectangle<int> area, bool selected) = 0;

private:
    int count_ = 0;
    int selected_ = 0;
    juce::ListBox list_;
};

class SlotListBody : public ListBody {
public:
    SlotListBody(int count, std::function<juce::String(int)> nameFor,
                 std::function<bool(int)> usedFor, int selected)
        : ListBody(count, 24, selected, "slots"),
          nameFor_(std::move(nameFor)),
          usedFor_(std::move(usedFor))
    {
    }

protected:
    void paintRow(juce::Graphics& g, int row, juce::Rectangle<int> area,
                  bool selected) override
    {
        const bool used = usedFor_ && usedFor_(row);
        g.setFont(uiFont(tokens::textSlotRow));
        g.setColour(colour(selected ? tokens::purple
                                    : used ? tokens::dim : tokens::faint));
        g.drawText(juce::String(row + 1).paddedLeft('0', 2), area.reduced(8, 0),
                   juce::Justification::centredLeft, false);
        juce::String name = used && nameFor_ ? nameFor_(row) : "empty";
        if (name.isEmpty())
            name = "empty";
        g.setColour(colour(selected ? tokens::purple
                                    : used ? tokens::text : tokens::faint));
        g.drawText(name, area.withTrimmedLeft(42).withTrimmedRight(8),
                   juce::Justification::centredLeft, true);
    }

private:
    std::function<juce::String(int)> nameFor_;
    std::function<bool(int)> usedFor_;
};

class ModeListBody : public ListBody {
public:
    ModeListBody(juce::StringArray modes, int selected)
        : ListBody(modes.size(), 28, selected, "modes"),
          modes_(std::move(modes))
    {
    }

protected:
    void paintRow(juce::Graphics& g, int row, juce::Rectangle<int> area,
                  bool selected) override
    {
        g.setFont(uiFont(tokens::textSlotRow));
        g.setColour(colour(selected ? tokens::purple : tokens::text));
        g.drawText(modes_[row], area.reduced(8, 0),
                   juce::Justification::centredLeft, true);
    }

private:
    juce::StringArray modes_;
};

class AlgTile : public juce::Button {
public:
    AlgTile(int index, std::function<void(int)> onSelect)
        : juce::Button("alg"), index_(index), onSelect_(std::move(onSelect))
    {
        setMouseCursor(juce::MouseCursor::PointingHandCursor);
    }

    void setSelected(bool selected)
    {
        if (selected_ != selected) {
            selected_ = selected;
            repaint();
        }
    }

    void paintButton(juce::Graphics& g, bool highlighted, bool) override
    {
        g.setColour(p1::ui::colour(p1::tokens::bg));
        g.fillRect(getLocalBounds());
        g.setColour(p1::ui::colour(selected_ ? p1::tokens::green
                                             : highlighted ? p1::tokens::pink
                                                           : p1::tokens::lineStrong));
        g.drawRect(getLocalBounds(), 1);
        auto area = getLocalBounds().reduced(6, 4);
        auto caption = area.removeFromBottom(18);
        p1::ui::drawAlgorithmGraph(g, area.toFloat(), index_);
        g.setFont(p1::ui::uiFont(11.0f));
        g.setColour(p1::ui::colour(selected_ ? p1::tokens::green
                                             : p1::tokens::dim));
        g.drawText("alg " + juce::String(index_), caption,
                   juce::Justification::centred, false);
    }

    void mouseUp(const juce::MouseEvent&) override
    {
        if (onSelect_)
            onSelect_(index_);
    }

private:
    int index_ = 0;
    bool selected_ = false;
    std::function<void(int)> onSelect_;
};

class AlgPickerBody : public juce::Component {
public:
    explicit AlgPickerBody(int selected)
        : selected_(juce::jlimit(0, 7, selected))
    {
        for (int i = 0; i < 8; ++i) {
            tiles_[static_cast<size_t>(i)] = std::make_unique<AlgTile>(
                i, [this](int index) { select(index); });
            tiles_[static_cast<size_t>(i)]->setSelected(i == selected_);
            addAndMakeVisible(*tiles_[static_cast<size_t>(i)]);
        }
        note_.setText("pink operators are carriers.",
                      juce::dontSendNotification);
        note_.setFont(p1::ui::uiFont(13.0f));
        note_.setColour(juce::Label::textColourId,
                        p1::ui::colour(p1::tokens::dim));
        addAndMakeVisible(note_);
    }

    int selected() const { return selected_; }

    void resized() override
    {
        auto area = getLocalBounds();
        auto grid = area.removeFromTop(2 * 140 + 8);
        const int width = (grid.getWidth() - 24) / 4;
        for (int i = 0; i < 8; ++i) {
            const int column = i % 4;
            const int row = i / 4;
            tiles_[static_cast<size_t>(i)]->setBounds(
                column * (width + 8), row * 148, width, 140);
        }
        note_.setBounds(area.removeFromTop(22));
    }

private:
    void select(int index)
    {
        selected_ = juce::jlimit(0, 7, index);
        for (int i = 0; i < 8; ++i)
            tiles_[static_cast<size_t>(i)]->setSelected(i == selected_);
    }

    int selected_ = 4;
    std::array<std::unique_ptr<AlgTile>, 8> tiles_{};
    juce::Label note_;
};

class RenameBody : public juce::Component {
public:
    explicit RenameBody(const juce::String& initial)
    {
        editor_.setText(initial, false);
        editor_.setFont(p1::ui::uiFont(14.0f));
        editor_.setColour(juce::TextEditor::textColourId,
                          p1::ui::colour(p1::tokens::green));
        editor_.setInputRestrictions(
            32, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 _-.");
        editor_.onReturnKey = [] {};
        addAndMakeVisible(editor_);
        note_.setText("max 32 characters.", juce::dontSendNotification);
        note_.setFont(p1::ui::uiFont(12.0f));
        note_.setColour(juce::Label::textColourId,
                        p1::ui::colour(p1::tokens::dim));
        addAndMakeVisible(note_);
    }

    juce::String text() const { return editor_.getText().trim(); }
    void focus() { editor_.grabKeyboardFocus(); }

    void resized() override
    {
        auto area = getLocalBounds();
        editor_.setBounds(area.removeFromTop(30));
        area.removeFromTop(6);
        note_.setBounds(area.removeFromTop(18));
    }

private:
    juce::TextEditor editor_;
    juce::Label note_;
};

} // namespace

PluginEditor::PluginEditor(TypePAlphaAudioProcessor& audioProcessor)
    : AudioProcessorEditor(&audioProcessor), processor_(audioProcessor)
{
    activePart_ = processor_.getUiActivePart();
    activeTab_ = processor_.getUiPage();
    setLookAndFeel(&lookAndFeel_);
    setOpaque(true);
    setResizable(true, true);
    setResizeLimits(620, 493, 2480, 1970);
    if (auto* boundsConstrainer = getConstrainer())
        boundsConstrainer->setFixedAspectRatio(static_cast<double>(kWidth) / kHeight);
    addAndMakeVisible(canvas_);
    buildUi();
    int initialWidth = kWidth;
    int initialHeight = kHeight;
    if (auto* display =
            juce::Desktop::getInstance().getDisplays().getPrimaryDisplay()) {
        /* userArea excludes the taskbar or dock. The 40 px vertical reserve is
         * the system title bar and a margin, and fitToHostWindow corrects it once
         * the peer exists and the real chrome can be measured. */
        const auto area = display->userArea.toNearestInt().reduced(8, 48);
        const float scale = juce::jlimit(
            0.5f, 2.0f,
            juce::jmin(static_cast<float>(area.getWidth()) / static_cast<float>(kWidth),
                       static_cast<float>(area.getHeight()) / static_cast<float>(kHeight)));
        initialWidth = juce::roundToInt(static_cast<float>(kWidth) * scale);
        initialHeight = juce::roundToInt(static_cast<float>(kHeight) * scale);
    }
    setSize(initialWidth, initialHeight);
    startTimerHz(30);
}

PluginEditor::~PluginEditor()
{
    stopTimer();
    dialog_.reset();
    setLookAndFeel(nullptr);
}

void PluginEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void PluginEditor::resized()
{
    const float scale =
        juce::jmin(static_cast<float>(getWidth()) / static_cast<float>(kWidth),
                   static_cast<float>(getHeight()) / static_cast<float>(kHeight));
    const float x =
        (static_cast<float>(getWidth()) - static_cast<float>(kWidth) * scale) *
        0.5f;
    const float y =
        (static_cast<float>(getHeight()) - static_cast<float>(kHeight) * scale) *
        0.5f;
    canvas_.setBounds(0, 0, kWidth, kHeight);
    canvas_.setTransform(juce::AffineTransform::scale(scale).translated(x, y));
}

void PluginEditor::timerCallback()
{
    if (startupFitPending_) {
        if (getPeer() != nullptr) {
            if (startupFitTicks_ > 0)
                --startupFitTicks_;
            else
                fitToHostWindow();
        }
        /* The host can set its own size after the editor first attaches, so keep
           applying the cap for a moment rather than trusting a single attempt. */
        if (--startupFitAttempts_ <= 0)
            startupFitPending_ = false;
    }
    updateVisualizers();
}

void PluginEditor::fitToHostWindow()
{
    auto* peer = getPeer();
    if (peer == nullptr || getHeight() <= 0)
        return;
    const auto peerBounds = peer->getBounds();
    if (peerBounds.isEmpty())
        return;
    auto* display =
        juce::Desktop::getInstance().getDisplays().getDisplayForRect(peerBounds);
    if (display == nullptr)
        return;
    /* Size to the display work area, which already excludes the taskbar or dock,
       minus the chrome the window actually adds (the system title bar and any
       host menu). The difference between the peer bounds and the editor bounds is
       measured rather than guessed, so the editor does not open too small or push
       its title bar past the work area. */
    auto area = display->userArea.toNearestInt().reduced(8);
    const auto editorBounds = getScreenBounds();
    if (editorBounds.getWidth() > 0 && editorBounds.getHeight() > 0 &&
        peerBounds.getWidth() >= editorBounds.getWidth() &&
        peerBounds.getHeight() >= editorBounds.getHeight()) {
        area.setWidth(area.getWidth() -
                      (peerBounds.getWidth() - editorBounds.getWidth()));
        area.setHeight(area.getHeight() -
                       (peerBounds.getHeight() - editorBounds.getHeight()));
    }
    if (area.getWidth() <= 0 || area.getHeight() <= 0)
        return;
    const float scale = juce::jlimit(
        0.5f, 2.0f,
        juce::jmin(static_cast<float>(area.getWidth()) / static_cast<float>(kWidth),
                   static_cast<float>(area.getHeight()) / static_cast<float>(kHeight)));
    const int width = juce::roundToInt(static_cast<float>(kWidth) * scale);
    const int height = juce::roundToInt(static_cast<float>(kHeight) * scale);
    if (width != getWidth() || height != getHeight())
        setSize(width, height);
}

void PluginEditor::updateVisualizers()
{
    const int part = processor_.getUiActivePart();
    if (part != activePart_) {
        setActivePart(part);
        return;
    }
    const int page = processor_.getUiPage();
    if (page != activeTab_) {
        showTab(page);
        return;
    }
    if (header_ != nullptr)
        header_->tick();
    if (toolbar_ != nullptr)
        toolbar_->tick();
    if (page_ != nullptr)
        page_->tick();
}

void PluginEditor::buildUi()
{
    dialog_.reset();
    canvas_.removeAllChildren();
    header_ = std::make_unique<Header>(*this);
    canvas_.addAndMakeVisible(*header_);
    header_->setBounds(1, 1, 1238, 159);
    toolbar_ = std::make_unique<Toolbar>(*this);
    canvas_.addAndMakeVisible(*toolbar_);
    toolbar_->setBounds(1, 160, 1238, 44);
    tabs_.setBounds(1, 204, 1238, 37);
    activeTab_ = juce::jlimit(0, 6, activeTab_);
    juce::StringArray labels;
    labels.add("patch settings");
    labels.add("part fx/mod");
    labels.add("operator envs");
    labels.add("operator lfos");
    labels.add("op spectra a");
    labels.add("op spectra b");
    labels.add("op spectra mod");
    tabs_.setLabels(labels);
    switchingTab_ = true;
    tabs_.setActive(activeTab_);
    switchingTab_ = false;
    tabs_.onTabChanged = [this](int tab) {
        if (!switchingTab_)
            showTab(tab);
    };
    canvas_.addAndMakeVisible(tabs_);
    buildPage();
}

void PluginEditor::buildPage()
{
    if (page_ != nullptr) {
        canvas_.removeChildComponent(page_.get());
        page_.reset();
    }
    switch (activeTab_) {
    case 1:
        page_ = p1::ui::makeFxModPage(*this);
        break;
    case 2:
        page_ = p1::ui::makeOperatorsPage(*this);
        break;
    case 3:
        page_ = p1::ui::makeOperatorLfosPage(*this);
        break;
    case 4:
        page_ = p1::ui::makeOperatorSpectraPage(*this, true);
        break;
    case 5:
        page_ = p1::ui::makeOperatorSpectraPage(*this, false);
        break;
    case 6:
        page_ = p1::ui::makeOperatorSpectraModPage(*this);
        break;
    default:
        page_ = p1::ui::makePatchPage(*this);
        break;
    }
    page_->setBounds(17, 255, 1206, 685);
    canvas_.addAndMakeVisible(*page_);
}

void PluginEditor::setActivePart(int part)
{
    part = juce::jlimit(0, 1, part);
    if (part == activePart_)
        return;
    activePart_ = part;
    processor_.setUiActivePart(part);
    buildUi();
}

void PluginEditor::showTab(int tab)
{
    tab = juce::jlimit(0, 6, tab);
    if (tab == activeTab_)
        return;
    activeTab_ = tab;
    processor_.setUiPage(tab);
    switchingTab_ = true;
    tabs_.setActive(tab);
    switchingTab_ = false;
    buildPage();
}

void PluginEditor::refreshNames()
{
    if (header_ != nullptr)
        header_->setPatchName(processor_.getPatchName());
    buildPage();
}

void PluginEditor::showAlgorithmPicker()
{
    const int current = static_cast<int>(std::lround(
        p1::ui::getParam(*this, p1::params::algorithm(activePart_))));
    auto body = std::make_unique<AlgPickerBody>(current);
    auto* bodyPtr = body.get();
    dialog_ = std::make_unique<p1::ui::P1Dialog>(
        "algorithm", std::move(body), 780, 490,
        juce::StringArray{"load", "cancel"},
        [this, bodyPtr](int index) {
            if (index == 0)
                applyAlgorithm(bodyPtr->selected());
            closeDialog();
        },
        [this] { closeDialog(); });
    canvas_.addAndMakeVisible(*dialog_);
    dialog_->setBounds(0, 0, kWidth, kHeight);
    dialog_->toFront(true);
}

void PluginEditor::applyAlgorithm(int algorithm)
{
    p1::ui::setParam(*this, p1::params::algorithm(activePart_),
                     juce::jlimit(0, 7, algorithm));
    if (header_ != nullptr)
        header_->tick();
}

void PluginEditor::showRenameDialog(bool part, int index)
{
    const juce::String initial =
        part ? processor_.getPartName(index) : processor_.getPatchName();
    auto body = std::make_unique<RenameBody>(initial);
    auto* bodyPtr = body.get();
    dialog_ = std::make_unique<p1::ui::P1Dialog>(
        part ? "rename part " + juce::String(index + 1) : "rename patch",
        std::move(body), 420, 190, juce::StringArray{"save", "cancel"},
        [this, bodyPtr, part, index](int button) {
            if (button == 0) {
                const juce::String text = bodyPtr->text();
                if (part)
                    processor_.setPartName(index, text);
                else
                    processor_.setPatchName(text);
                refreshNames();
            }
            closeDialog();
        },
        [this] { closeDialog(); });
    canvas_.addAndMakeVisible(*dialog_);
    dialog_->setBounds(0, 0, kWidth, kHeight);
    dialog_->toFront(true);
    bodyPtr->focus();
}

void PluginEditor::showSinglePartsBrowser()
{
    auto& store = processor_.presets();
    const int part = activePart();
    const int source = store.partSource(part);
    auto body = std::make_unique<SlotListBody>(
        p1::PresetStore::kPartSlots,
        [this](int slot) {
            return juce::String(processor_.presets().partName(slot));
        },
        [this](int slot) { return processor_.presets().partUsed(slot); },
        source >= 0 ? source : 0);
    auto* bodyPtr = body.get();
    auto save = [this, bodyPtr](int which) {
        const int slot = bodyPtr->selected();
        const bool used = processor_.presets().partUsed(slot);
        closeDialog();
        if (used) {
            showConfirm(
                "overwrite part slot " +
                    juce::String(slot + 1).paddedLeft('0', 2) + "?",
                [this, which, slot] { savePartToSlot(which, slot); });
        } else {
            savePartToSlot(which, slot);
        }
    };
    auto load = [this, bodyPtr](int which) {
        const int slot = bodyPtr->selected();
        closeDialog();
        if (processor_.presets().partUsed(slot))
            loadPartFromSlot(which, slot);
    };
    dialog_ = std::make_unique<p1::ui::P1Dialog>(
        "single parts", std::move(body), 640, 500,
        juce::StringArray{"export bank",
                          "import bank",
                          "init single part 1",
                          "init single part 2",
                          "save single part 1",
                          "save single part 2",
                          "load single part 1",
                          "load single part 2",
                          "cancel"},
        [this, save, load](int button) {
            if (button == 0) {
                closeDialog();
                exportLibrary();
            } else if (button == 1) {
                closeDialog();
                importLibrary();
            } else if (button == 2) {
                closeDialog();
                showConfirm("init part 1? this wipes the current part.",
                            [this] { initPart(0); }, "init single part 1",
                            "init");
            } else if (button == 3) {
                closeDialog();
                showConfirm("init part 2? this wipes the current part.",
                            [this] { initPart(1); }, "init single part 2",
                            "init");
            } else if (button == 4) {
                save(0);
            } else if (button == 5) {
                save(1);
            } else if (button == 6) {
                load(0);
            } else if (button == 7) {
                load(1);
            } else {
                closeDialog();
            }
        },
        [this] { closeDialog(); });
    canvas_.addAndMakeVisible(*dialog_);
    dialog_->setBounds(0, 0, kWidth, kHeight);
    dialog_->toFront(true);
}

void PluginEditor::exportLibrary()
{
    const auto initial = juce::File::getSpecialLocation(
                             juce::File::userDocumentsDirectory)
                             .getChildFile("type-p-alpha.p1lib");
    fileChooser_ = std::make_unique<juce::FileChooser>(
        "export part and patch library", initial, "*.p1lib");
    fileChooser_->launchAsync(
        juce::FileBrowserComponent::saveMode |
            juce::FileBrowserComponent::canSelectFiles |
            juce::FileBrowserComponent::warnAboutOverwriting,
        [this](const juce::FileChooser& chooser) {
            const auto file = chooser.getResult();
            if (file == juce::File{})
                return;
            const auto bytes = processor_.presets().packLibrary();
            file.replaceWithData(bytes.data(), bytes.size());
        });
}

void PluginEditor::importLibrary()
{
    const auto initial =
        juce::File::getSpecialLocation(juce::File::userDocumentsDirectory);
    fileChooser_ = std::make_unique<juce::FileChooser>(
        "import part and patch library", initial, "*.p1lib");
    fileChooser_->launchAsync(
        juce::FileBrowserComponent::openMode |
            juce::FileBrowserComponent::canSelectFiles,
        [this](const juce::FileChooser& chooser) {
            const auto file = chooser.getResult();
            if (file == juce::File{})
                return;
            juce::MemoryBlock data;
            if (!file.loadFileAsData(data))
                return;
            if (processor_.presets().loadLibrary(
                    static_cast<const uint8_t*>(data.getData()), data.getSize())) {
                processor_.saveLibrary();
                refreshNames();
            }
        });
}

void PluginEditor::copyPart(int part)
{
    partClipboard_.clear();
    const auto suffix = ".p" + juce::String(juce::jlimit(0, 1, part));
    for (const auto& spec : kP1ParamSpecs) {
        const juce::String id(spec.id);
        if (!id.endsWith(suffix))
            continue;
        if (auto* value = state().getRawParameterValue(id))
            partClipboard_[id.dropLastCharacters(3)] = value->load();
    }
}

void PluginEditor::pastePart(int part)
{
    const auto suffix = ".p" + juce::String(juce::jlimit(0, 1, part));
    for (const auto& [base, value] : partClipboard_) {
        if (auto* parameter = state().getParameter(base + suffix))
            parameter->setValueNotifyingHost(
                parameter->convertTo0to1(static_cast<float>(value)));
    }
    buildPage();
}

void PluginEditor::copyOperator(int op)
{
    operatorClipboard_.clear();
    operatorClipboardOp_ = op;
    const int part = activePart();
    const juce::String suffix = ".p" + juce::String(part);
    for (const auto& spec : kP1ParamSpecs) {
        const juce::String id(spec.id);
        if (!id.endsWith(suffix))
            continue;
        const juce::String prefix = "op" + juce::String(op + 1);
        const bool opMatch = id.startsWith(prefix + "_") ||
                             id.startsWith(prefix + ".");
        const bool op1Edge =
            op == 0 && (id.startsWith("attack.") || id.startsWith("release."));
        if (!opMatch && !op1Edge)
            continue;
        if (auto* value = state().getRawParameterValue(id))
            operatorClipboard_[id.dropLastCharacters(3)] = value->load();
    }
}

void PluginEditor::pasteOperator(int op)
{
    if (operatorClipboard_.empty())
        return;
    const auto suffix = ".p" + juce::String(activePart());
    const juce::String source = "op" + juce::String(operatorClipboardOp_ + 1);
    const juce::String destination = "op" + juce::String(op + 1);
    for (const auto& [storedBase, value] : operatorClipboard_) {
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
        if (auto* parameter = state().getParameter(base + suffix))
            parameter->setValueNotifyingHost(
                parameter->convertTo0to1(static_cast<float>(value)));
    }
    buildPage();
}

namespace {

/* The parameters each operator card owns, in its display order. The list is the
 * card's contract: copy fills the clipboard from it and paste writes back through
 * it, so a card cannot copy some of its controls and miss others. The amplitude
 * envelope's first and last rates are the op1-only `attack` and `release` names,
 * and opEnvRate maps them for the source and destination op. */
std::vector<juce::String> sectionParamIDs(int op, int part,
                                          PluginEditor::OperatorSection section)
{
    std::vector<juce::String> ids;
    const auto add = [&ids](const juce::String& id) { ids.push_back(id); };
    switch (section) {
    case PluginEditor::OperatorSection::oscillator:
        add(p1::params::opRatio(op, part));
        add(p1::params::opDetune(op, part));
        add(p1::params::opWaveform(op, part));
        add(p1::params::opLevel(op, part));
        add(p1::params::opFeedback(op, part));
        add(p1::params::opSpecMorph(op, part));
        break;
    case PluginEditor::OperatorSection::amplitudeEnvelope:
        for (int i = 0; i < 4; ++i)
            add(p1::params::opEnvRate(op, i, part));
        for (int i = 0; i < 3; ++i)
            add(p1::params::opEnvLevel(op, i, part));
        break;
    case PluginEditor::OperatorSection::feedbackEnvelope:
        add(p1::params::opFeedbackOn(op, part));
        for (int i = 0; i < 4; ++i)
            add(p1::params::opFeedbackRate(op, i, part));
        for (int i = 0; i < 4; ++i)
            add(p1::params::opFeedbackLevel(op, i, part));
        break;
    case PluginEditor::OperatorSection::amplitudeLfo:
        add(p1::params::opTremRate(op, part));
        add(p1::params::opTremDepth(op, part));
        add(p1::params::opTremWave(op, part));
        add(p1::params::opTremMode(op, part));
        break;
    case PluginEditor::OperatorSection::feedbackLfo:
        add(p1::params::opFeedbackLfoRate(op, part));
        add(p1::params::opFeedbackLfoDepth(op, part));
        add(p1::params::opFeedbackLfoWave(op, part));
        add(p1::params::opFeedbackLfoMode(op, part));
        break;
    case PluginEditor::OperatorSection::detuneLfo:
        add(p1::params::opDetuneLfoRate(op, part));
        add(p1::params::opDetuneLfoDepth(op, part));
        add(p1::params::opDetuneLfoWave(op, part));
        add(p1::params::opDetuneLfoMode(op, part));
        break;
    case PluginEditor::OperatorSection::spectrumA:
        add(p1::params::opSpecRangeA(op, part));
        for (int h = 1; h <= 64; ++h)
            add(p1::params::opSpecA(op, h, part));
        break;
    case PluginEditor::OperatorSection::spectrumB:
        add(p1::params::opSpecRangeB(op, part));
        for (int h = 1; h <= 64; ++h)
            add(p1::params::opSpecB(op, h, part));
        break;
    case PluginEditor::OperatorSection::spectrumEnvelope:
        add(p1::params::opSpecEnvDepth(op, part));
        for (int i = 0; i < 4; ++i)
            add(p1::params::opSpecEnvRate(op, i, part));
        for (int i = 0; i < 3; ++i)
            add(p1::params::opSpecEnvLevel(op, i, part));
        break;
    case PluginEditor::OperatorSection::spectrumLfo:
        add(p1::params::opSpecLfoRate(op, part));
        add(p1::params::opSpecLfoDepth(op, part));
        add(p1::params::opSpecLfoWave(op, part));
        add(p1::params::opSpecLfoMode(op, part));
        break;
    }
    return ids;
}

} // namespace

void PluginEditor::copyOperatorSection(int op, int part,
                                       OperatorSection section)
{
    operatorSectionClipboard_.clear();
    operatorSectionClipboardOp_ = op;
    operatorSectionClipboardKind_ = section;
    operatorSectionClipboardValid_ = true;
    for (const auto& id : sectionParamIDs(op, part, section)) {
        if (auto* value = state().getRawParameterValue(id))
            operatorSectionClipboard_.push_back(value->load());
    }
}

void PluginEditor::pasteOperatorSection(int op, int part,
                                        OperatorSection section)
{
    if (!operatorSectionClipboardValid_ ||
        operatorSectionClipboardKind_ != section ||
        operatorSectionClipboard_.empty())
        return;
    const auto ids = sectionParamIDs(op, part, section);
    const int count = juce::jmin(static_cast<int>(ids.size()),
                                 static_cast<int>(
                                     operatorSectionClipboard_.size()));
    for (int i = 0; i < count; ++i) {
        if (auto* parameter = state().getParameter(ids[static_cast<std::size_t>(i)]))
            parameter->setValueNotifyingHost(parameter->convertTo0to1(
                static_cast<float>(
                    operatorSectionClipboard_[static_cast<std::size_t>(i)])));
    }
    if (section == OperatorSection::spectrumA ||
        section == OperatorSection::spectrumB) {
        if (auto* parameter = state().getParameter(
                p1::params::opWaveform(op, part))) {
            parameter->setValueNotifyingHost(
                parameter->convertTo0to1(13.0f));
        }
    }
    buildPage();
}

void PluginEditor::initPart(int part)
{
    const int index = juce::jlimit(0, 1, part);
    const auto suffix = ".p" + juce::String(index);
    for (const auto& spec : kP1ParamSpecs) {
        const juce::String id(spec.id);
        if (!id.endsWith(suffix))
            continue;
        if (auto* parameter = state().getParameter(id))
            parameter->setValueNotifyingHost(
                parameter->convertTo0to1(static_cast<float>(spec.def)));
    }
    processor_.setPartName(index, "init");
    buildPage();
}

void PluginEditor::initMultiPatch()
{
    initPart(0);
    initPart(1);
    processor_.setPatchName("init multi");
    refreshNames();
}

void PluginEditor::togglePartMute(int part, bool muted)
{
    part = juce::jlimit(0, 1, part);
    const auto id = p1::params::level(part);
    const int current =
        static_cast<int>(std::lround(p1::ui::getParam(*this, id)));
    if (muted) {
        if (current > 0)
            mutedLevel_[static_cast<size_t>(part)] = current;
        p1::ui::setParam(*this, id, 0);
    } else {
        const int restore = mutedLevel_[static_cast<size_t>(part)];
        p1::ui::setParam(*this, id, restore > 0 ? restore : 99);
    }
}

std::vector<int32_t> PluginEditor::currentPartParams(int part)
{
    part = juce::jlimit(0, kP1PartCount - 1, part);
    std::vector<int32_t> values(static_cast<std::size_t>(kP1PartParamCount), 0);
    for (int i = 0; i < kP1PartParamCount; ++i) {
        const auto& spec =
            kP1ParamSpecs[static_cast<std::size_t>(part * kP1PartParamCount + i)];
        if (auto* value = state().getRawParameterValue(spec.id))
            values[static_cast<std::size_t>(i)] =
                static_cast<int32_t>(std::lround(value->load()));
    }
    return values;
}

void PluginEditor::applyPartParams(int part, const std::vector<int32_t>& params)
{
    part = juce::jlimit(0, kP1PartCount - 1, part);
    const int count = juce::jmin(kP1PartParamCount, static_cast<int>(params.size()));
    for (int i = 0; i < count; ++i) {
        const auto& spec =
            kP1ParamSpecs[static_cast<std::size_t>(part * kP1PartParamCount + i)];
        if (auto* parameter = state().getParameter(spec.id)) {
            parameter->setValueNotifyingHost(
                parameter->convertTo0to1(static_cast<float>(
                    params[static_cast<std::size_t>(i)])));
        }
    }
}

int PluginEditor::currentRouting(int field, int part)
{
    const int id = kP1RoutingBase + field * kP1PartCount + juce::jlimit(0, 1, part);
    if (id < 0 || id >= kP1ParamCount)
        return 0;
    if (auto* value = state().getRawParameterValue(
            kP1ParamSpecs[static_cast<std::size_t>(id)].id))
        return static_cast<int>(std::lround(value->load()));
    return 0;
}

void PluginEditor::applyRouting(int field, int part, int value)
{
    const int id = kP1RoutingBase + field * kP1PartCount + juce::jlimit(0, 1, part);
    if (id < 0 || id >= kP1ParamCount)
        return;
    if (auto* parameter = state().getParameter(
            kP1ParamSpecs[static_cast<std::size_t>(id)].id))
        parameter->setValueNotifyingHost(
            parameter->convertTo0to1(static_cast<float>(value)));
}

void PluginEditor::savePartToSlot(int part, int slot)
{
    auto& store = processor_.presets();
    if (!store.savePart(slot, processor_.getPartName(part).toStdString(),
                        currentPartParams(part)))
        return;
    store.setPartSource(part, slot);
    processor_.saveLibrary();
}

void PluginEditor::loadPartFromSlot(int part, int slot)
{
    auto& store = processor_.presets();
    std::string name;
    std::vector<int32_t> params;
    if (!store.loadPart(slot, name, params))
        return;
    applyPartParams(part, params);
    processor_.setPartName(part, juce::String(name));
    store.setPartSource(part, slot);
    refreshNames();
}

void PluginEditor::savePatchToSlot(int slot, int mode)
{
    auto& store = processor_.presets();
    std::vector<uint8_t> partSlots(static_cast<std::size_t>(kP1PartCount), 0xFF);
    std::vector<int32_t> level(static_cast<std::size_t>(kP1PartCount), 99);
    std::vector<int32_t> delay(static_cast<std::size_t>(kP1PartCount), 0);
    std::vector<int32_t> channel(static_cast<std::size_t>(kP1PartCount), 0);
    std::vector<int32_t> low(static_cast<std::size_t>(kP1PartCount), 0);
    std::vector<int32_t> high(static_cast<std::size_t>(kP1PartCount), 127);
    for (int part = 0; part < kP1PartCount; ++part) {
        int source = store.partSource(part);
        const bool missing = source < 0 || !store.partUsed(source);
        if (mode == 1 || missing) {
            source = store.findFreePartSlot();
            if (source < 0)
                return;
            if (!store.savePart(source,
                                processor_.getPartName(part).toStdString(),
                                currentPartParams(part)))
                return;
            store.setPartSource(part, source);
        } else if (mode == 2) {
            if (!store.savePart(source,
                                processor_.getPartName(part).toStdString(),
                                currentPartParams(part)))
                return;
        }
        partSlots[static_cast<std::size_t>(part)] =
            source >= 0 && source < p1::PresetStore::kPartSlots
                ? static_cast<uint8_t>(source)
                : 0xFF;
        channel[static_cast<std::size_t>(part)] = currentRouting(0, part);
        low[static_cast<std::size_t>(part)] = currentRouting(1, part);
        high[static_cast<std::size_t>(part)] = currentRouting(2, part);
        level[static_cast<std::size_t>(part)] = currentRouting(3, part);
        delay[static_cast<std::size_t>(part)] = currentRouting(4, part);
    }
    if (!store.saveMulti(slot, processor_.getPatchName().toStdString(), partSlots,
                         level, delay, channel, low, high))
        return;
    store.setCurrentMulti(slot);
    processor_.saveLibrary();
}

void PluginEditor::loadPatchFromSlot(int slot)
{
    auto& store = processor_.presets();
    p1::PresetStore::Multi multi;
    if (!store.loadMulti(slot, multi))
        return;
    processor_.setPatchName(juce::String(multi.name));
    for (int part = 0; part < kP1PartCount; ++part) {
        const int sourceSlot =
            multi.partSlot[static_cast<std::size_t>(part)] <
                    p1::PresetStore::kPartSlots
                ? multi.partSlot[static_cast<std::size_t>(part)]
                : -1;
        store.setPartSource(part, sourceSlot);
        if (sourceSlot >= 0) {
            std::string name;
            std::vector<int32_t> params;
            if (store.loadPart(sourceSlot, name, params)) {
                applyPartParams(part, params);
                processor_.setPartName(part, juce::String(name));
            }
        }
        applyRouting(0, part, multi.channel[static_cast<std::size_t>(part)]);
        applyRouting(1, part, multi.low[static_cast<std::size_t>(part)]);
        applyRouting(2, part, multi.high[static_cast<std::size_t>(part)]);
        applyRouting(3, part, multi.level[static_cast<std::size_t>(part)]);
        applyRouting(4, part, multi.delay[static_cast<std::size_t>(part)]);
    }
    store.setCurrentMulti(slot);
    refreshNames();
}

void PluginEditor::showConfirm(const juce::String& message,
                               std::function<void()> onConfirm,
                               const juce::String& title,
                               const juce::String& confirm)
{
    auto body = std::make_unique<juce::Label>();
    body->setText(message, juce::dontSendNotification);
    body->setFont(uiFont(tokens::textDialogBody));
    body->setColour(juce::Label::textColourId, colour(tokens::text));
    body->setJustificationType(juce::Justification::centred);
    dialog_ = std::make_unique<p1::ui::P1Dialog>(
        title, std::move(body), 420, 160,
        juce::StringArray{confirm, "cancel"},
        [this, confirmAction = std::move(onConfirm)](int button) {
            if (button == 0)
                confirmAction();
            closeDialog();
        },
        [this] { closeDialog(); });
    canvas_.addAndMakeVisible(*dialog_);
    dialog_->setBounds(0, 0, kWidth, kHeight);
    dialog_->toFront(true);
}

void PluginEditor::showPatchBrowser()
{
    auto& store = processor_.presets();
    auto body = std::make_unique<SlotListBody>(
        p1::PresetStore::kMultiSlots,
        [this](int slot) {
            return juce::String(processor_.presets().multiName(slot));
        },
        [this](int slot) { return processor_.presets().multiUsed(slot); },
        store.currentMulti());
    auto* bodyPtr = body.get();
    dialog_ = std::make_unique<p1::ui::P1Dialog>(
        "multi-patches", std::move(body), 640, 500,
        juce::StringArray{"export bank", "import bank", "init multi-patch",
                          "save", "load", "cancel"},
        [this, bodyPtr](int button) {
            if (button == 0) {
                closeDialog();
                exportLibrary();
            } else if (button == 1) {
                closeDialog();
                importLibrary();
            } else if (button == 2) {
                closeDialog();
                showConfirm("init multi-patch? this wipes the current patch.",
                            [this] { initMultiPatch(); }, "init multi-patch", "init");
            } else if (button == 3) {
                const int slot = bodyPtr->selected();
                closeDialog();
                showPatchSaveMode(slot);
            } else if (button == 4) {
                loadPatchFromSlot(bodyPtr->selected());
                closeDialog();
            } else {
                closeDialog();
            }
        },
        [this] { closeDialog(); });
    canvas_.addAndMakeVisible(*dialog_);
    dialog_->setBounds(0, 0, kWidth, kHeight);
    dialog_->toFront(true);
}

void PluginEditor::showPatchSaveMode(int slot)
{
    auto body = std::make_unique<ModeListBody>(
        juce::StringArray{"discard edits to parts", "save as new parts",
                          "mutate parts in place"},
        0);
    auto* bodyPtr = body.get();
    dialog_ = std::make_unique<p1::ui::P1Dialog>(
        "save multi-patch", std::move(body), 420, 280,
        juce::StringArray{"save", "cancel"},
        [this, bodyPtr, slot](int button) {
            if (button == 0) {
                const int mode = bodyPtr->selected();
                closeDialog();
                if (processor_.presets().multiUsed(slot)) {
                    showConfirm(
                        "overwrite multi-patch slot " +
                            juce::String(slot + 1).paddedLeft('0', 2) + "?",
                        [this, slot, mode] { savePatchToSlot(slot, mode); });
                } else {
                    savePatchToSlot(slot, mode);
                }
            } else {
                closeDialog();
            }
        },
        [this] { closeDialog(); });
    canvas_.addAndMakeVisible(*dialog_);
    dialog_->setBounds(0, 0, kWidth, kHeight);
    dialog_->toFront(true);
}

void PluginEditor::closeDialog()
{
    if (dialog_ != nullptr) {
        canvas_.removeChildComponent(dialog_.get());
        dialog_.reset();
    }
}

void p1::ui::P1Canvas::paint(juce::Graphics& g)
{
    g.fillAll(p1::ui::colour(p1::tokens::bg));
    g.setColour(p1::ui::colour(p1::tokens::lineStrong));
    g.drawRect(0, 0, getWidth(), getHeight(), 1);
    g.setColour(p1::ui::colour(p1::tokens::pink));
    g.fillRect(0, 0, p1::tokens::accentCorner, 2);
    g.fillRect(0, 0, 2, p1::tokens::accentCorner);
    g.fillRect(getWidth() - p1::tokens::accentCorner, getHeight() - 2,
               p1::tokens::accentCorner, 2);
    g.fillRect(getWidth() - 2, getHeight() - p1::tokens::accentCorner, 2,
               p1::tokens::accentCorner);
    g.setColour(p1::ui::colour(p1::tokens::lineStrong));
    g.fillRect(0, 956, getWidth(), 1);
    g.setColour(p1::ui::colour(p1::tokens::green));
    g.fillRect(16, 966, 6, 6);
    g.fillRect(80, 966, 6, 6);
    g.setFont(p1::ui::displayFont(p1::tokens::textStatus));
    g.setColour(p1::ui::colour(p1::tokens::dim));
    g.drawText("midi", 28, 960, 44, 18, juce::Justification::centredLeft, false);
    g.drawText("sync", 92, 960, 44, 18, juce::Justification::centredLeft, false);
}
