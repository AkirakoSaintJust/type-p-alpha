/* SPDX-License-Identifier: GPL-3.0-only */
#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <vector>

#include <juce_audio_processors/juce_audio_processors.h>

#include "PluginProcessor.h"
#include "Widgets.h"

class Header;
class Toolbar;

namespace p1::ui {

class P1Canvas : public juce::Component {
public:
    void paint(juce::Graphics& g) override;
};

class PageBase : public juce::Component {
public:
    ~PageBase() override = default;
    virtual void tick() = 0;
};

} // namespace p1::ui

class PluginEditor : public juce::AudioProcessorEditor, private juce::Timer {
public:
    enum class OperatorSection {
        amplitudeEnvelope,
        feedbackEnvelope,
        amplitudeLfo,
        feedbackLfo,
        detuneLfo,
    };

    explicit PluginEditor(TypePAlphaAudioProcessor& audioProcessor);
    ~PluginEditor() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;

    TypePAlphaAudioProcessor& processor() { return processor_; }
    juce::AudioProcessorValueTreeState& state() { return processor_.parameters(); }
    int activePart() const { return activePart_; }
    int activeTab() const { return activeTab_; }

    void setActivePart(int part);
    void showTab(int tab);
    void refreshNames();
    void showAlgorithmPicker();
    void applyAlgorithm(int algorithm);
    void showRenameDialog(bool part, int index);
    void showSinglePartsBrowser();
    void exportLibrary();
    void importLibrary();
    void showPatchBrowser();
    void showPatchSaveMode(int slot);
    void showPartBrowser(int part);
    void copyPart(int part);
    void pastePart(int part);
    void copyOperator(int op);
    void pasteOperator(int op);
    void copyOperatorSection(int op, int part, OperatorSection section);
    void pasteOperatorSection(int op, int part, OperatorSection section);
    void initPart(int part);
    void initMultiPatch();
    void togglePartMute(int part, bool muted);
    void savePartToSlot(int part, int slot);
    void loadPartFromSlot(int part, int slot);
    void savePatchToSlot(int slot, int mode = 0);
    void loadPatchFromSlot(int slot);
    void showConfirm(const juce::String& message, std::function<void()> onConfirm,
                     const juce::String& title = "confirm save",
                     const juce::String& confirm = "save");
    void closeDialog();
    void updateVisualizers();

private:
    void buildUi();
    void buildHeader();
    void buildToolbar();
    void buildPage();
    void setSpectroMode(bool spectro);
    void fitToHostWindow();
    std::vector<int32_t> currentPartParams(int part);
    void applyPartParams(int part, const std::vector<int32_t>& params);
    int currentRouting(int field, int part);
    void applyRouting(int field, int part, int value);

    TypePAlphaAudioProcessor& processor_;
    p1::ui::P1LookAndFeel lookAndFeel_;
    p1::ui::P1Canvas canvas_;
    std::unique_ptr<Header> header_;
    std::unique_ptr<Toolbar> toolbar_;
    p1::ui::P1TabBar tabs_;
    std::unique_ptr<p1::ui::PageBase> page_;
    std::unique_ptr<p1::ui::P1Dialog> dialog_;
    std::unique_ptr<juce::FileChooser> fileChooser_;
    int activePart_ = 0;
    int activeTab_ = 0;
    bool spectroMode_ = false;
    bool switchingTab_ = false;
    bool startupFitPending_ = true;
    int startupFitTicks_ = 4;
    int startupFitAttempts_ = 90;
    std::array<int, 2> mutedLevel_{99, 99};
    std::map<juce::String, double> partClipboard_;
    std::map<juce::String, double> operatorClipboard_;
    int operatorClipboardOp_ = -1;
    std::map<juce::String, double> operatorSectionClipboard_;
    int operatorSectionClipboardOp_ = -1;
    OperatorSection operatorSectionClipboardKind_ =
        OperatorSection::amplitudeEnvelope;
    bool operatorSectionClipboardValid_ = false;
};
