/* SPDX-License-Identifier: GPL-3.0-only */
#pragma once

#include <array>
#include <atomic>
#include <cstdint>
#include <limits>
#include <vector>

#include <juce_audio_processors/juce_audio_processors.h>

#include "Engine.hpp"
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
    void saveLibrary();
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

    juce::String getPatchName() const { return patchName_; }
    void setPatchName(const juce::String& name) { patchName_ = name; }
    juce::String getPartName(int part) const;
    void setPartName(int part, const juce::String& name);
    int getUiActivePart() const { return uiActivePart_; }
    void setUiActivePart(int part) { uiActivePart_ = juce::jlimit(0, 1, part); }
    int getUiPage() const { return uiPage_; }
    void setUiPage(int page)
    {
        uiPage_ = juce::jlimit(0, 5, page);
    }

private:
    static juce::AudioProcessorValueTreeState::ParameterLayout createLayout();
    static void feedMessage(p1::MidiBridge& midi, const juce::MidiMessage& message);
    void syncParams();
    void renderCore(int32_t* out, int frames);
    void pushTapScopes(p1::Engine& engine, int frames);

    p1::Engine engine_;
    p1::MidiBridge midi_;
    p1::RateBridge rateBridge_;
    juce::AudioProcessorValueTreeState apvts_;
    std::array<int32_t, kP1ParamCount> lastParams_{};
    std::array<std::atomic<float>*, kP1ParamCount> paramValues_{};
    std::vector<int32_t> directBuffer_;
    std::atomic<bool> paramsDirty_{false};
    double hostRate_ = 48000.0;

    ScopeRing outputScope_;
    std::array<ScopeRing, 10> tapScopes_;
    juce::String patchName_{"init patch"};
    std::array<juce::String, 2> partNames_{{"init", "init"}};
    int uiActivePart_ = 0;
    int uiPage_ = 0;
    bool libraryLoaded_ = false;
    juce::File libraryFileOverride_;
    p1::PresetStore presets_{kP1PartParamCount, kP1PartCount,
                             p1::partParamDefaults()};
};
