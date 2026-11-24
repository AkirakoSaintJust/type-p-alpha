/* SPDX-License-Identifier: GPL-3.0-only */
#pragma once

#include <array>
#include <atomic>
#include <cstdint>
#include <limits>
#include <string>
#include <vector>

#include <juce_audio_processors/juce_audio_processors.h>

#include "Engine.hpp"
#include "ChunkNoteSet.h"
#include "MidiBridge.hpp"
#include "P1Product.h"
#include "Params.h"
#include "PresetStore.hpp"
#include "RateBridge.hpp"
#include "part_defaults.h"

class ScopeRing {
public:
    ScopeRing();
    void push(const float* data, int count);
    void read(float* destination, int count);

private:
    static constexpr int kCapacity = 2048;
    juce::AbstractFifo fifo_{kCapacity};
    std::array<float, kCapacity> buffer_{};
    std::array<float, kCapacity> scratch_{};
};

/* The outcome of a library save. A conflict means another instance wrote the
 * slot first, so the disk version stayed and the local edit is still only in
 * memory. `unreadable` means the file exists but could not be parsed, so nothing
 * was written over it. */
struct LibrarySaveResult {
    bool written = false;
    bool unreadable = false;
    bool conflict = false;
    bool busy = false;
    bool multi = false;
    int slot = -1;
    std::string name;
};

class TypePAlphaAudioProcessor : public juce::AudioProcessor {
public:
    TypePAlphaAudioProcessor();
    ~TypePAlphaAudioProcessor() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    using juce::AudioProcessor::processBlock;
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return P1_PRODUCT_NAME; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 8.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    void ensureLibraryLoaded();
    void resetPhaseMaps();
    LibrarySaveResult saveLibrary();
    juce::File libraryFile() const;
    void setLibraryFileForTesting(const juce::File& file)
    {
        libraryFileOverride_ = file;
    }

    juce::AudioProcessorValueTreeState& parameters() { return apvts_; }
    p1::PresetStore& presets() { return presets_; }
    const p1::PresetStore& presets() const { return presets_; }

    void readOutputScope(float* destination, int count);
    void readTapScope(int channel, float* destination, int count);
    /* The live morph for an operator and part, 0 to 1, or -1 when the operator
     * has not read the bank since the reset. */
    double specMorph(int op, int part) const;
    bool phaseMap(int op, int part, p1::PhaseMapSnapshot& out) const;

    juce::String getPatchName() const { return patchName_; }
    void setPatchName(const juce::String& name) { patchName_ = name; }
    juce::String getPartName(int part) const;
    void setPartName(int part, const juce::String& name);
    juce::String getMacroName(int index) const;
    void setMacroName(int index, const juce::String& name);
    double macroEffectiveValue(int index) const;
    /* What a macro destination does to one parameter right now. The stored value
     * stays in the parameter; `effective` is the raw value the engine receives,
     * which differs whenever a macro with a range deviates from its knob. */
    struct MacroFeedback {
        bool targeted = false;
        bool hasEffective = false;
        double effective = 0.0;
    };
    MacroFeedback macroFeedback(int index) const;
    int getUiActivePart() const { return uiActivePart_; }
    void setUiActivePart(int part) { uiActivePart_ = juce::jlimit(0, 1, part); }
    int getUiPage() const { return uiPage_; }
    void setUiPage(int page)
    {
        uiPage_ = juce::jlimit(0, 8, page);
    }

private:
    static juce::AudioProcessorValueTreeState::ParameterLayout createLayout();
    static void feedMessage(p1::MidiBridge& midi, const juce::MidiMessage& message);
    void syncParams();
    void syncMacros();
    void renderCore(int32_t* out, int frames);
    void pushTapScopes(p1::Engine& engine, int frames);
    void publishMorph(p1::Engine& engine);
    void publishPhaseMaps(p1::Engine& engine);
    void feedScheduled(int64_t limit);
    void flushDeferred();
    static void coreChunk(void* context, p1::Engine& engine, int frames);

    struct ScheduledEvent {
        juce::MidiMessage message;
        int64_t coreFrame = 0;
    };

    p1::Engine engine_;
    p1::MidiBridge midi_;
    p1::RateBridge rateBridge_;
    juce::AudioProcessorValueTreeState apvts_;
    std::array<int32_t, kP1ParamCount> lastParams_{};
    std::array<std::atomic<float>*, kP1ParamCount> paramValues_{};
    std::array<std::atomic<float>*, 6> macroValueValues_{};
    std::array<std::atomic<float>*, 6> macroMidiValues_{};
    std::array<std::atomic<float>*, 6> macroMidiRangeValues_{};
    std::array<std::atomic<float>*, 18> macroTargetValues_{};
    std::array<std::atomic<float>*, 18> macroRangeValues_{};
    std::array<std::atomic<int>, 6> macroCc_{};
    std::atomic<int> modWheel_{0};
    std::atomic<int> noteVelocity_{0};
    std::array<std::atomic<float>*, 2> algorithmValues_{};
    std::vector<int32_t> macroMemberIds_;
    std::vector<juce::RangedAudioParameter*> macroMemberParams_;
    std::vector<uint8_t> macroMemberLog_;
    std::vector<int> macroMemberIndex_;
    std::vector<double> macroOffsets_;
    bool macroApplied_ = false;
    std::atomic<float>* outputVolume_ = nullptr;
    float volumeGain_ = 1.0f;
    std::vector<int32_t> directBuffer_;
    std::vector<float> monoFold_;
    std::atomic<bool> paramsDirty_{false};
    double hostRate_ = 48000.0;

    std::vector<ScheduledEvent> scheduled_;
    std::size_t nextScheduled_ = 0;
    p1::ChunkNoteSet chunkNotes_{};
    std::vector<juce::MidiMessage> deferredOffs_;
    uint64_t coreFrames_ = 0;
    std::array<std::atomic<int>, 8> liveMorph_{};
    std::array<std::array<p1::PhaseMapSnapshot, 8>, 3> phaseMapSnapshots_{};
    /* A three-buffer handoff. The writer owns `phaseMapBack_`, the reader owns
     * `phaseMapFront_`, and `phaseMapMiddle_` carries the index plus a new-data
     * bit. The writer never writes the buffer the reader holds. */
    mutable std::atomic<uint32_t> phaseMapMiddle_{1};
    uint32_t phaseMapBack_ = 0;
    mutable uint32_t phaseMapFront_ = 2;

    ScopeRing outputScope_;
    std::array<ScopeRing, 10> tapScopes_;
    juce::String patchName_{"init patch"};
    std::array<juce::String, 2> partNames_{{"init", "init"}};
    std::array<juce::String, 6> macroNames_{};
    int uiActivePart_ = 0;
    int uiPage_ = 0;
    bool libraryLoaded_ = false;
    juce::File libraryFileOverride_;
    p1::PresetStore presets_{kP1PartParamCount, kP1PartCount,
                             p1::partParamDefaults(), kP1PartCutoffIndex};
};
