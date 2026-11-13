/* SPDX-License-Identifier: GPL-3.0-only */
#pragma once

#include <array>
#include <functional>
#include <memory>
#include <vector>

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "Params.h"
#include "Theme.h"

namespace p1::ui {

using APVTS = juce::AudioProcessorValueTreeState;

const P1ParamSpec* findSpec(const juce::String& id);
juce::String formatParamValue(const P1ParamSpec& spec, double value);
double parseParamValue(const P1ParamSpec& spec, const juce::String& text);

class ParamKnob : public juce::Component {
public:
    ParamKnob(APVTS& state, juce::String paramID, juce::String label,
              float dialSize = 42.0f);
    ~ParamKnob() override;

    void setParamID(const juce::String& paramID);
    void resized() override;
    void paint(juce::Graphics& g) override;

    /* Runs once when a user starts dragging the knob. Host automation does not
     * call it. */
    std::function<void()> onUserEdit;

private:
    /* Arrow keys step a detented ratio knob to the next detent instead of
       nudging the raw value, which would snap straight back to where it was. */
    class KnobSlider : public juce::Slider {
    public:
        bool keyPressed(const juce::KeyPress& key) override
        {
            if (onDetentKey && onDetentKey(key))
                return true;
            return juce::Slider::keyPressed(key);
        }

        void mouseWheelMove(const juce::MouseEvent& event,
                            const juce::MouseWheelDetails& wheel) override;

        std::function<bool(const juce::KeyPress&)> onDetentKey;
    };

    void updateValueText();
    void commitValue();
    void snapValue();
    bool stepRatio(const juce::KeyPress& key);
    bool isRatio() const;

    APVTS& state_;
    juce::String paramID_;
    const P1ParamSpec* spec_ = nullptr;
    KnobSlider slider_;
    juce::Label name_;
    juce::Label value_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attach_;
    float dialSize_ = 42.0f;
    bool snapping_ = false;
};

class P1Button : public juce::Button {
public:
    explicit P1Button(const juce::String& text, bool mini = false);
    void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted,
                     bool shouldDrawButtonAsDown) override;
};

class P1MuteTile : public juce::Button {
public:
    P1MuteTile(const juce::String& label, bool muted,
               std::function<void(bool)> onToggle = {});
    void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted,
                     bool shouldDrawButtonAsDown) override;
};

class P1Toggle : public juce::Button {
public:
    P1Toggle(APVTS& state, const juce::String& paramID,
             const juce::String& label = "on");
    void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted,
                     bool shouldDrawButtonAsDown) override;

private:
    juce::String label_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> attach_;
};

class P1Choice : public juce::ComboBox {
public:
    P1Choice() = default;
};

class ParamChoice : public juce::Component {
public:
    ParamChoice(APVTS& state, const juce::String& paramID,
                juce::StringArray items);
    void resized() override;

private:
    void updateFromNormalized(float normalized);

    juce::ComboBox combo_;
    juce::RangedAudioParameter* param_ = nullptr;
    int min_ = 0;
    std::unique_ptr<juce::ParameterAttachment> attach_;
};

class P1TabBar : public juce::Component {
public:
    P1TabBar();
    void setLabels(const juce::StringArray& labels);
    void setActive(int index);
    void resized() override;
    void paint(juce::Graphics& g) override;
    std::function<void(int)> onTabChanged;

private:
    class Tab : public juce::Button {
    public:
        Tab(const juce::String& text, int index, P1TabBar& owner);
        void paintButton(juce::Graphics& g, bool highlighted, bool down) override;

    private:
        int index_;
        P1TabBar& owner_;
    };

    std::vector<std::unique_ptr<Tab>> tabs_;
    int active_ = 0;
};

class P1Scope : public juce::Component {
public:
    P1Scope();
    void setSamples(const float* samples, int count);
    void paint(juce::Graphics& g) override;

private:
    std::vector<float> samples_;
};

class P1Spectro : public juce::Component {
public:
    P1Spectro();
    void setSamples(const float* samples, int count);
    void paint(juce::Graphics& g) override;

private:
    juce::dsp::FFT fft_{9};
    std::array<float, 1024> fftData_{};
    std::vector<float> samples_;
};

class P1SpectralBars : public juce::Component {
public:
    P1SpectralBars(APVTS& state, juce::StringArray parameterIDs,
                   juce::Colour barColour);
    void paint(juce::Graphics& g) override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;

    std::vector<double> levels() const;
    void setLevels(const std::vector<double>& levels);
    void setPreviewLevels(const std::vector<double>& levels);
    void clearPreview();
    void setFirstHarmonic(int first);
    bool hasPreview() const { return !preview_.empty(); }
    std::function<void()> onEdit;

private:
    void setFromMouse(const juce::MouseEvent& event);
    double levelAt(int index) const;
    double displayLevelAt(int index) const;
    void setLevel(int index, double value);

    APVTS& state_;
    juce::StringArray parameterIDs_;
    juce::Colour colour_;
    std::vector<double> preview_;
    int firstHarmonic_ = 1;
};

class P1SpectrumDisplay : public juce::Component {
public:
    P1SpectrumDisplay(APVTS& state, juce::StringArray aIDs,
                      juce::StringArray bIDs, juce::String morphID,
                      juce::String waveformID, juce::String rangeAID,
                      juce::String rangeBID, juce::Colour aColour,
                      juce::Colour bColour);
    void paint(juce::Graphics& g) override;

private:
    double levelAt(const juce::StringArray& ids, int index) const;

    APVTS& state_;
    juce::StringArray aIDs_;
    juce::StringArray bIDs_;
    juce::String morphID_;
    juce::String waveformID_;
    juce::String rangeAID_;
    juce::String rangeBID_;
    juce::Colour aColour_;
    juce::Colour bColour_;
};

class P1Envelope : public juce::Component {
public:
    P1Envelope(APVTS& state, juce::StringArray rateIDs,
               juce::StringArray levelIDs, juce::Colour curveColour,
               double levelMin, double levelMax, bool zeroLine);
    void paint(juce::Graphics& g) override;
    void refresh() { repaint(); }

private:
    APVTS& state_;
    juce::StringArray rateIDs_;
    juce::StringArray levelIDs_;
    juce::Colour colour_;
    double levelMin_ = 0.0;
    double levelMax_ = 99.0;
    bool zeroLine_ = false;
};

class P1Wave : public juce::Component {
public:
    P1Wave(APVTS& state, juce::String waveID, juce::Colour curveColour);
    void paint(juce::Graphics& g) override;

private:
    APVTS& state_;
    juce::String waveID_;
    juce::Colour colour_;
};

class P1LevelSlider : public juce::Component {
public:
    P1LevelSlider(APVTS& state, const juce::String& paramID);
    void resized() override;
    void paint(juce::Graphics& g) override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;

private:
    void setFromMouse(const juce::MouseEvent& event);

    APVTS& state_;
    juce::String paramID_;
    const P1ParamSpec* spec_ = nullptr;
    float value_ = 0.0f;
    juce::Rectangle<int> track_;
    std::unique_ptr<juce::ParameterAttachment> attach_;
};

class P1Card : public juce::Component {
public:
    explicit P1Card(juce::String title, std::function<void()> onCopy = {},
                    std::function<void()> onPaste = {});
    void paint(juce::Graphics& g) override;
    void resized() override;

    void setHeaderTrailingWidth(int width);
    juce::Rectangle<int> headerTrailingBounds() const;
    juce::Rectangle<int> contentArea() const;

private:
    juce::String title_;
    bool hasActions_ = false;
    int trailingWidth_ = 0;
    P1Button copy_{"copy", true};
    P1Button paste_{"paste", true};
};

class P1AlgorithmBlock : public juce::Component {
public:
    P1AlgorithmBlock();
    void setAlgorithm(int algorithm);
    void paint(juce::Graphics& g) override;
    void mouseUp(const juce::MouseEvent& event) override;
    void mouseEnter(const juce::MouseEvent& event) override;
    void mouseExit(const juce::MouseEvent& event) override;
    void mouseWheelMove(const juce::MouseEvent& event,
                        const juce::MouseWheelDetails& wheel) override;

    std::function<void()> onClick;
    std::function<void(int)> onWheel;

private:
    int algorithm_ = 4;
    bool hover_ = false;
};

class P1Dialog : public juce::Component {
public:
    P1Dialog(juce::String title, std::unique_ptr<juce::Component> body,
             int width, int height, juce::StringArray buttons,
             std::function<void(int)> onButton = {},
             std::function<void()> onClose = {});
    void paint(juce::Graphics& g) override;
    void resized() override;
    void mouseUp(const juce::MouseEvent& event) override;

private:
    juce::String title_;
    std::unique_ptr<juce::Component> body_;
    juce::StringArray buttons_;
    std::vector<std::unique_ptr<P1Button>> buttonComponents_;
    std::function<void(int)> onButton_;
    std::function<void()> onClose_;
    juce::Rectangle<int> panel_;
    juce::Rectangle<int> closeArea_;
};

void drawAlgorithmGraph(juce::Graphics& g, juce::Rectangle<float> area,
                        int algorithm);

} // namespace p1::ui
