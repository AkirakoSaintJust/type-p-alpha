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
#include "VolumeLaw.h"

namespace p1::ui {

using APVTS = juce::AudioProcessorValueTreeState;

const P1ParamSpec* findSpec(const juce::String& id);
juce::String formatParamValue(const P1ParamSpec& spec, double value);
/* The same value without its unit, for the effective value beside a stored one. */
juce::String formatParamValuePlain(const P1ParamSpec& spec, double value);
double parseParamValue(const P1ParamSpec& spec, const juce::String& text);

/* The master volume fader position as dB text, and back. The law is in
 * VolumeLaw.h, which the processor and the header slider share. */
juce::String volumeText(int position);
int volumePositionFromText(const juce::String& text);

/* A control whose parameter can be a macro destination. The editor asks the
 * processor once per refresh and the control draws the stored value with the
 * effective value beside it while a macro deviates. */
class MacroFeedbackTarget {
public:
    virtual ~MacroFeedbackTarget() = default;
    virtual int macroParamIndex() const = 0;
    virtual void setMacroFeedback(bool targeted, bool hasEffective,
                                  double effective) = 0;
};

/* The value line of a knob. A plain value draws in its stored colour. When a
 * targeted parameter has an effective value, the stored value keeps its unit and
 * the effective value follows in smaller green type without a unit. */
class ParamValueLabel : public juce::Label {
public:
    ParamValueLabel();
    void setStoredColour(juce::Colour colour);
    void setEffectiveText(const juce::String& text, juce::Colour colour);
    void paint(juce::Graphics& g) override;

private:
    juce::String effective_;
    juce::Colour storedColour_;
    juce::Colour effectiveColour_;
};

class ParamKnob : public juce::Component, public MacroFeedbackTarget {
public:
    ParamKnob(APVTS& state, juce::String paramID, juce::String label,
              float dialSize = 42.0f, float labelSize = 0.0f);
    ~ParamKnob() override;

    void setParamID(const juce::String& paramID);
    void setModArc(double fraction);
    int macroParamIndex() const override { return specIndex_; }
    void setMacroFeedback(bool targeted, bool hasEffective,
                          double effective) override;
    void resized() override;
    void paint(juce::Graphics& g) override;

    /* Runs once when a user starts dragging the knob. Host automation does not
     * call it. */
    std::function<void()> onUserEdit;
    /* Runs once when a user edit finishes, after the value is written. Host
     * automation and preset restores do not call it. */
    std::function<void()> onUserEditEnd;

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
        std::function<void()> onUserEdit;
        std::function<void()> onUserEditEnd;
    };

    void updateValueText();
    void refreshValueDisplay();
    void commitValue();
    void snapValue();
    bool stepRatio(const juce::KeyPress& key);
    bool isRatio() const;

    APVTS& state_;
    juce::String paramID_;
    const P1ParamSpec* spec_ = nullptr;
    int specIndex_ = -1;
    KnobSlider slider_;
    juce::Label name_;
    ParamValueLabel value_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attach_;
    float dialSize_ = 42.0f;
    float labelSize_ = 0.0f;
    bool hasName_ = true;
    bool snapping_ = false;
    bool macroTargeted_ = false;
    bool macroHasEffective_ = false;
    double macroEffective_ = 0.0;
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
    void setCompact(bool compact);
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
    juce::StringArray labels_;
    int active_ = 0;
    bool forcedCompact_ = false;
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
    static constexpr int kWindow = 512;
    static constexpr int kBars = 22;
    juce::dsp::FFT fft_{9};
    std::array<float, 1024> fftData_{};
    std::array<float, kWindow> window_{};
    std::array<int, kBars> barBin_{};
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
    void setFirstHarmonic(int first);
    void setGroupTest(std::function<bool(int)> test);
    /* The selection window in absolute harmonics, and the live/dead test that
     * composes with it. Freehand editing only writes a bar both allow. */
    void setEditWindow(int first, int last);
    void setEditTest(std::function<bool(int, double)> test);
    std::function<void()> onEdit;

private:
    void setFromMouse(const juce::MouseEvent& event);
    double levelAt(int index) const;
    void setLevel(int index, double value);
    bool inWindow(int harmonic) const;

    APVTS& state_;
    juce::StringArray parameterIDs_;
    juce::Colour colour_;
    int firstHarmonic_ = 1;
    int windowFirst_ = 1;
    int windowLast_ = 128;
    std::function<bool(int)> groupTest_;
    std::function<bool(int, double)> editTest_;
};

class P1SpectrumDisplay : public juce::Component {
public:
    P1SpectrumDisplay(APVTS& state, juce::StringArray aIDs,
                      juce::StringArray bIDs, juce::String morphID,
                      juce::String rangeAID, juce::String rangeBID,
                      juce::Colour aColour, juce::Colour bColour);
    void paint(juce::Graphics& g) override;
    /* The morph the engine last read, 0 to 1, or -1 for the base parameter. */
    void setLiveMorph(double value) { liveMorph_ = value; }
    void tick();

private:
    double levelAt(const juce::StringArray& ids, int index) const;

    APVTS& state_;
    juce::StringArray aIDs_;
    juce::StringArray bIDs_;
    juce::String morphID_;
    juce::String rangeAID_;
    juce::String rangeBID_;
    juce::Colour aColour_;
    juce::Colour bColour_;
    double liveMorph_ = -1.0;
    uint64_t lastSignature_ = 0;
    bool haveSignature_ = false;
};

enum class EnvelopeShape {
    legacyFourStage,
    operatorAmp,
    bipolar,
};

class P1Envelope : public juce::Component {
public:
    P1Envelope(APVTS& state, juce::StringArray rateIDs,
               juce::StringArray levelIDs, juce::Colour curveColour,
               double levelMin, double levelMax, bool zeroLine,
               EnvelopeShape shape = EnvelopeShape::legacyFourStage,
               juce::String modeID = {}, juce::String startID = {},
               double startScale = 1.0);
    void paint(juce::Graphics& g) override;
    void refresh() { repaint(); }
    /* Repaint only when the drawn parameter state changed. */
    void tick();

private:
    APVTS& state_;
    juce::StringArray rateIDs_;
    juce::StringArray levelIDs_;
    juce::Colour colour_;
    double levelMin_ = 0.0;
    double levelMax_ = 99.0;
    bool zeroLine_ = false;
    EnvelopeShape shape_ = EnvelopeShape::legacyFourStage;
    juce::String modeID_;
    juce::String startID_;
    double startScale_ = 1.0;
    uint64_t lastSignature_ = 0;
    bool haveSignature_ = false;
};

class P1Wave : public juce::Component {
public:
    P1Wave(APVTS& state, juce::String waveID, juce::Colour curveColour);
    void paint(juce::Graphics& g) override;
    void tick();

private:
    APVTS& state_;
    juce::String waveID_;
    juce::Colour colour_;
    int lastWave_ = -1;
    bool haveWave_ = false;
};

class P1LevelSlider : public juce::Component, public MacroFeedbackTarget {
public:
    P1LevelSlider(APVTS& state, const juce::String& paramID,
                  juce::String caption = "level", bool bipolar = false);
    void resized() override;
    void paint(juce::Graphics& g) override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    int macroParamIndex() const override { return specIndex_; }
    void setMacroFeedback(bool targeted, bool hasEffective,
                          double effective) override;

private:
    void setFromMouse(const juce::MouseEvent& event);

    APVTS& state_;
    juce::String paramID_;
    juce::String caption_;
    bool bipolar_ = false;
    const P1ParamSpec* spec_ = nullptr;
    int specIndex_ = -1;
    float value_ = 0.0f;
    juce::Rectangle<int> track_;
    std::unique_ptr<juce::ParameterAttachment> attach_;
    bool macroTargeted_ = false;
    bool macroHasEffective_ = false;
    double macroEffective_ = 0.0;
};

/* The master output fader. The parameter is the fader position, so the fill is
 * linear in position whilst the gain follows the law in VolumeLaw.h, and the
 * unity mark sits at 75 percent of the track. */
class P1VolumeSlider : public juce::Component {
public:
    P1VolumeSlider(APVTS& state, const juce::String& paramID);
    void resized() override;
    void paint(juce::Graphics& g) override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;

private:
    void setFromMouse(const juce::MouseEvent& event);

    APVTS& state_;
    juce::String paramID_;
    int position_ = p1::kVolumeUnity;
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
    void setRingMask(uint8_t mask);
    void setDirectMask(uint8_t mask);
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
    uint8_t ringMask_ = 0;
    uint8_t directMask_ = 0;
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
                        int algorithm, uint8_t ringMask = 0,
                        uint8_t directMask = 0);

} // namespace p1::ui
