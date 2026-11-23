/* SPDX-License-Identifier: GPL-3.0-only */
#include "Widgets.h"

#include <algorithm>
#include <cmath>
#include <vector>

#include "CutoffLaw.h"
#include "VolumeLaw.h"
#include "math_tables.h"
#include "SpectrumAnalysis.h"

namespace p1::ui {
namespace {

KnobLookAndFeel& knobLookAndFeel()
{
    static KnobLookAndFeel lnf;
    return lnf;
}

const char* const waveNames[14] = {
    "sine",      "triangle", "half sine", "half tri", "2x sine", "2x tri",
    "2x half sine", "2x half tri", "rect sine", "rect tri", "pulse", "square",
    "saw", "spectrum"};

juce::String signedNumber(int value)
{
    return (value > 0 ? "+" : "") + juce::String(value);
}

int32_t snapRatio(int32_t value)
{
    if (kP1RatioDetents.empty())
        return value;
    const auto it = std::lower_bound(kP1RatioDetents.begin(),
                                     kP1RatioDetents.end(), value);
    if (it == kP1RatioDetents.begin())
        return *it;
    if (it == kP1RatioDetents.end())
        return kP1RatioDetents.back();
    const auto high = *it;
    const auto low = *(it - 1);
    return (value - low) <= (high - value) ? low : high;
}

/* The detent one step above (dir > 0) or one step below the current value.
   An off-detent value moves to the neighbour on that side, so a typed 1.37
   followed by an up-arrow lands on 1.41 rather than 1.50. */
int32_t stepRatioDetent(int32_t value, int dir)
{
    if (kP1RatioDetents.empty())
        return value;
    if (dir > 0) {
        const auto it = std::upper_bound(kP1RatioDetents.begin(),
                                         kP1RatioDetents.end(), value);
        return it == kP1RatioDetents.end() ? kP1RatioDetents.back() : *it;
    }
    const auto it = std::lower_bound(kP1RatioDetents.begin(),
                                     kP1RatioDetents.end(), value);
    if (it == kP1RatioDetents.begin())
        return *it;
    return *(it - 1);
}

juce::String notePair(int note)
{
    note = juce::jlimit(0, 127, note);
    return juce::String(note) + "/" +
           juce::MidiMessage::getMidiNoteName(note, true, true, 4).toLowerCase();
}

juce::String percentage(const P1ParamSpec& spec, double value, bool inverse)
{
    const double range = static_cast<double>(spec.max - spec.min);
    int pct = range > 0.0
                  ? static_cast<int>(
                        std::lround((value - spec.min) * 100.0 / range))
                  : 0;
    if (inverse)
        pct = 100 - pct;
    return juce::String(juce::jlimit(0, 100, pct)) + "%";
}

} // namespace

const P1ParamSpec* findSpec(const juce::String& id)
{
    for (const auto& spec : kP1ParamSpecs) {
        if (id == spec.id)
            return &spec;
    }
    return nullptr;
}

juce::String volumeText(int position)
{
    if (position <= 0)
        return "silent";
    const double db = p1::volumeDbFromPosition(position);
    const juce::String sign = db > 0.05 ? "+" : "";
    return sign + juce::String(db, 1) + " dB";
}

int volumePositionFromText(const juce::String& text)
{
    const auto lower = text.trim().toLowerCase();
    if (lower == "silent" || lower.startsWith("-inf"))
        return 0;
    juce::String numeric;
    for (auto c : text) {
        if ((c >= '0' && c <= '9') || c == '-' || c == '+' || c == '.')
            numeric += juce::String::charToString(c);
    }
    if (numeric.isEmpty() || numeric == "-" || numeric == "+")
        return p1::kVolumeUnity;
    return p1::volumePositionFromDb(numeric.getDoubleValue());
}

juce::String formatParamValue(const P1ParamSpec& spec, double value)
{
    const auto display = juce::String(spec.display);
    const int v = static_cast<int>(std::lround(value));
    if (juce::String(spec.id).startsWith("channel."))
        return v < 0 ? "omni" : juce::String(v + 1);
    if (display == "ratio")
        return juce::String(value / 65536.0, 2);
    if (display == "cent")
        return signedNumber(v) + "c";
    if (display == "semitones")
        return signedNumber(v) + "st";
    if (display == "wave") {
        if (v >= 0 && v < 14)
            return waveNames[v];
        return juce::String(v);
    }
    if (display == "lfo_wave") {
        static const char* const names[6] = {"sine", "triangle", "saw",
                                              "inverse saw", "square", "random"};
        return v >= 0 && v < 6 ? names[v] : juce::String(v);
    }
    if (display == "frac3")
        return juce::String(value / 1000.0, 3);
    if (display == "off_pct")
        return v <= 0 ? "off" : percentage(spec, value, false);
    if (display == "pct_inv")
        return percentage(spec, value, true);
    if (display == "pct")
        return percentage(spec, value, false);
    if (display == "cutoff") {
        const double hz = p1::cutoffHzFromPosition(v);
        if (hz < 100.0)
            return juce::String(hz, 1) + "Hz";
        if (hz < 1000.0)
            return juce::String(static_cast<int>(std::lround(hz))) + "Hz";
        return juce::String(hz / 1000.0, 2) + "kHz";
    }
    if (display == "oct")
        return juce::String(static_cast<double>(v) / 65535.0 * 8.0, 2) + " oct";
    if (display == "pct_exp") {
        const double range = static_cast<double>(spec.max - spec.min);
        const double x = range > 0.0 ? (value - spec.min) / range : 0.0;
        constexpr double k = 3.0;
        const double den = 1.0 - std::exp(-k);
        const double p = 100.0 * (1.0 - std::exp(-k * x)) / den;
        return juce::String(p, 2) + "%";
    }
    if (display == "pct_bip") {
        const double range = static_cast<double>(spec.max - spec.min);
        const double percent =
            range > 0.0 ? 200.0 * (value - spec.min) / range - 100.0 : 0.0;
        return signedNumber(static_cast<int>(std::lround(percent))) + "%";
    }
    if (display == "fade") {
        /* The fade is a rate: 1 is the slowest, 99 is the fastest. Show the
         * rate itself so the value rises with the dial; the two-line graph
         * carries the time view. */
        const int rate = juce::jlimit(0, 99, v);
        if (rate <= 0)
            return "off";
        return juce::String(rate);
    }
    if (display == "filter") {
        static const char* const names[3] = {"lpf", "bpf", "hpf"};
        return v >= 0 && v < 3 ? names[v] : juce::String(v);
    }
    if (display == "onoff")
        return v > 0 ? "on" : "off";
    if (display == "onoff_inv")
        return v > 0 ? "off" : "on";
    if (display == "note")
        return notePair(v);
    if (display == "mode")
        return v == 0 ? "poly" : "mono";
    if (display == "priority") {
        static const char* const names[3] = {"last", "highest", "lowest"};
        return v >= 0 && v < 3 ? names[v] : juce::String(v);
    }
    if (display == "porta")
        return v <= 0 ? "off" : juce::String(v) + "ms";
    if (display == "fold")
        return "fold";
    if (display == "semi" || display == "semi_fine" || display == "semi_lfo") {
        const double st = value * 12.0 / 32768.0;
        return (st > 0.0 ? "+" : "") + juce::String(st, 2) + "st";
    }
    if (display == "hz") {
        const double hz = value * 48000.0 / 4294967296.0;
        return juce::String(hz, hz < 1.0 ? 2 : 1) + "Hz";
    }
    if (display == "range") {
        const int index = juce::jlimit(0, 1, v);
        const int first = index * 64 + 1;
        return juce::String(first) + "-" + juce::String(first + 63);
    }
    if (juce::String(spec.unit) == "cents")
        return juce::String(v) + "c";
    return juce::String(v) + (spec.unit[0] != '\0' ? juce::String(spec.unit) : "");
}

juce::String formatParamValuePlain(const P1ParamSpec& spec, double value)
{
    auto text = formatParamValue(spec, value);
    /* The prefix stays, so a jump from 900 Hz to 2.0 kHz reads as 2.0k rather
     * than a bare 2.0. Longest suffix first, and the unit words only follow a
     * number. */
    static const char* const units[] = {"Hz", "ms", " oct", "st", "c", "%"};
    for (const char* unit : units) {
        const juce::String suffix(unit);
        if (text.endsWith(suffix) && text.length() > suffix.length())
            return text.dropLastCharacters(suffix.length()).trimEnd();
    }
    return text;
}

double parseParamValue(const P1ParamSpec& spec, const juce::String& text)
{
    const auto display = juce::String(spec.display);
    juce::String numeric;
    for (auto c : text) {
        if ((c >= '0' && c <= '9') || c == '-' || c == '+' || c == '.')
            numeric += juce::String::charToString(c);
        else if (c == '/' && display == "note")
            break;
    }
    const double v = numeric.getDoubleValue();
    if (display == "fade") {
        const auto lower = text.trim().toLowerCase();
        if (lower == "off")
            return 0.0;
        if (lower == "instant")
            return 99.0;
        return juce::jlimit(0.0, 99.0, v);
    }
    if (display == "onoff" || display == "onoff_inv") {
        const auto lower = text.trim().toLowerCase();
        if (lower == "on" || lower == "active")
            return display == "onoff_inv" ? spec.min : spec.max;
        if (lower == "off" || lower == "inactive")
            return display == "onoff_inv" ? spec.max : spec.min;
    }
    if (juce::String(spec.id).startsWith("channel."))
        return juce::jlimit(static_cast<double>(spec.min),
                            static_cast<double>(spec.max),
                            text.trim().toLowerCase() == "omni" ? -1.0 : v - 1.0);
    if (display == "ratio")
        return juce::jlimit(static_cast<double>(spec.min),
                            static_cast<double>(spec.max), v * 65536.0);
    if (display == "semi" || display == "semi_fine" || display == "semi_lfo")
        return juce::jlimit(static_cast<double>(spec.min),
                            static_cast<double>(spec.max), v * 32768.0 / 12.0);
    if (display == "hz")
        return juce::jlimit(static_cast<double>(spec.min),
                            static_cast<double>(spec.max),
                            v * 4294967296.0 / 48000.0);
    if (display == "range") {
        if (v >= 0.0 && v <= 1.0)
            return v;
        const int first = (static_cast<int>(v) - 1) / 64;
        return juce::jlimit(0, 1, first);
    }
    if (display == "pct" || display == "off_pct") {
        const double range = static_cast<double>(spec.max - spec.min);
        return juce::jlimit(static_cast<double>(spec.min),
                            static_cast<double>(spec.max),
                            spec.min + v * range / 100.0);
    }
    if (display == "cutoff") {
        const double hz = text.containsIgnoreCase("khz") ? v * 1000.0 : v;
        return juce::jlimit(static_cast<double>(spec.min),
                            static_cast<double>(spec.max),
                            static_cast<double>(p1::cutoffPositionFromHz(hz)));
    }
    if (display == "oct")
        return juce::jlimit(static_cast<double>(spec.min),
                            static_cast<double>(spec.max),
                            v / 8.0 * 65535.0);
    if (display == "pct_exp") {
        const double range = static_cast<double>(spec.max - spec.min);
        constexpr double k = 3.0;
        const double den = 1.0 - std::exp(-k);
        const double p = juce::jlimit(0.0, 100.0, v) / 100.0;
        const double x = -std::log(1.0 - p * den) / k;
        return juce::jlimit(static_cast<double>(spec.min),
                            static_cast<double>(spec.max),
                            spec.min + x * range);
    }
    if (display == "pct_inv") {
        const double range = static_cast<double>(spec.max - spec.min);
        return juce::jlimit(static_cast<double>(spec.min),
                            static_cast<double>(spec.max),
                            spec.max - v * range / 100.0);
    }
    if (display == "pct_bip") {
        const double range = static_cast<double>(spec.max - spec.min);
        return juce::jlimit(static_cast<double>(spec.min),
                            static_cast<double>(spec.max),
                            spec.min + (v + 100.0) * range / 200.0);
    }
    if (display == "frac3")
        return juce::jlimit(static_cast<double>(spec.min),
                            static_cast<double>(spec.max), v * 1000.0);
    if (display == "lfo_wave") {
        static const char* const names[6] = {"sine", "triangle", "saw",
                                              "inverse saw", "square", "random"};
        const auto lower = text.trim().toLowerCase();
        for (int i = 0; i < 6; ++i)
            if (lower == names[i])
                return i;
        return juce::jlimit(static_cast<double>(spec.min),
                            static_cast<double>(spec.max), v);
    }
    return juce::jlimit(static_cast<double>(spec.min),
                        static_cast<double>(spec.max), v);
}

ParamValueLabel::ParamValueLabel()
    : storedColour_(juce::Colours::white), effectiveColour_(juce::Colours::white)
{
}

void ParamValueLabel::setStoredColour(juce::Colour colour)
{
    if (storedColour_ == colour)
        return;
    storedColour_ = colour;
    setColour(juce::Label::textColourId, colour);
    repaint();
}

void ParamValueLabel::setEffectiveText(const juce::String& text,
                                       juce::Colour colour)
{
    if (effective_ == text && effectiveColour_ == colour)
        return;
    effective_ = text;
    effectiveColour_ = colour;
    repaint();
}

void ParamValueLabel::paint(juce::Graphics& g)
{
    const auto area = getLocalBounds();
    const auto font = getFont();
    const auto stored = getText();
    if (effective_.isEmpty()) {
        juce::Label::paint(g);
        return;
    }
    const float storedWidth = textWidth(font, stored);
    float smallHeight = font.getHeight() * 0.62f;
    auto small = font.withHeight(smallHeight);
    float effectiveWidth = textWidth(small, effective_);
    constexpr float gap = 6.0f;
    const float available = static_cast<float>(area.getWidth()) - 2.0f;
    while (storedWidth + gap + effectiveWidth > available && smallHeight > 8.0f) {
        smallHeight -= 0.5f;
        small = font.withHeight(smallHeight);
        effectiveWidth = textWidth(small, effective_);
    }
    const float total = storedWidth + gap + effectiveWidth;
    const float x = static_cast<float>(area.getX()) +
                    (static_cast<float>(area.getWidth()) - total) * 0.5f;
    g.setFont(font);
    g.setColour(storedColour_);
    g.drawText(stored,
               juce::Rectangle<float>(x, static_cast<float>(area.getY()),
                                      storedWidth, static_cast<float>(area.getHeight()))
                   .toNearestInt(),
               juce::Justification::centredLeft, false);
    g.setFont(small);
    g.setColour(effectiveColour_);
    g.drawText(effective_,
               juce::Rectangle<float>(
                   x + storedWidth + gap, static_cast<float>(area.getY()),
                   effectiveWidth, static_cast<float>(area.getHeight()))
                   .toNearestInt(),
               juce::Justification::centredLeft, false);
}

ParamKnob::ParamKnob(APVTS& state, juce::String paramID, juce::String label,
                     float dialSize, float labelSize)
    : state_(state), paramID_(std::move(paramID)), dialSize_(dialSize),
      labelSize_(labelSize)
{
    spec_ = findSpec(paramID_);
    specIndex_ = spec_ != nullptr
                     ? static_cast<int>(spec_ - kP1ParamSpecs.data())
                     : -1;
    value_.setStoredColour(colour(tokens::pinkBright));
    value_.setEffectiveText({}, colour(tokens::green));
    slider_.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider_.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    slider_.setRotaryParameters(juce::MathConstants<float>::pi * 1.25f,
                                juce::MathConstants<float>::pi * 2.75f, true);
    slider_.setLookAndFeel(&knobLookAndFeel());
    slider_.setMouseDragSensitivity(220);
    slider_.onDetentKey = [this](const juce::KeyPress& key) {
        return stepRatio(key);
    };
    slider_.onUserEdit = [this] {
        if (onUserEdit)
            onUserEdit();
    };
    slider_.onDragStart = [this] { slider_.onUserEdit(); };
    slider_.onDragEnd = [this] {
        if (onUserEditEnd)
            onUserEditEnd();
    };
    slider_.onUserEditEnd = [this] {
        if (onUserEditEnd)
            onUserEditEnd();
    };
    slider_.onValueChange = [this] {
        snapValue();
        updateValueText();
    };
    addAndMakeVisible(slider_);

    name_.setText(label, juce::dontSendNotification);
    hasName_ = label.isNotEmpty();
    name_.setVisible(hasName_);
    name_.setFont(uiFont(labelSize_ > 0.0f ? labelSize_ : tokens::textKnobName));
    name_.setColour(juce::Label::textColourId, colour(tokens::dim));
    name_.setJustificationType(juce::Justification::centred);
    name_.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(name_);

    value_.setFont(uiFont(tokens::textKnobValue));
    value_.setColour(juce::Label::textColourId, colour(tokens::pinkBright));
    value_.setJustificationType(juce::Justification::centred);
    value_.setEditable(true, false, false);
    value_.onTextChange = [this] { commitValue(); };
    addAndMakeVisible(value_);

    attach_ = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        state_, paramID_, slider_);
    updateValueText();
}

ParamKnob::~ParamKnob()
{
    slider_.setLookAndFeel(nullptr);
}

void ParamKnob::KnobSlider::mouseWheelMove(const juce::MouseEvent& event,
                                           const juce::MouseWheelDetails& wheel)
{
    if (!isEnabled() || !isScrollWheelEnabled()) {
        juce::Slider::mouseWheelMove(event, wheel);
        return;
    }
    if (event.mods.isAnyMouseButtonDown())
        return;
    float amount = std::abs(wheel.deltaX) > std::abs(wheel.deltaY) ? -wheel.deltaX
                                                                  : wheel.deltaY;
    if (wheel.isReversed)
        amount = -amount;
    if (std::abs(amount) < 1.0e-6f)
        return;
    if (onUserEdit)
        onUserEdit();
    /* The default JUCE wheel step is 15% of the range, which is far too coarse
       for a raw level or a Q15 rate. Use 2% per step, and 0.5% with shift. */
    const double proportion = 0.02 * (event.mods.isShiftDown() ? 0.25 : 1.0);
    const double current = valueToProportionOfLength(getValue());
    const double next = juce::jlimit(0.0, 1.0, current + amount * proportion);
    double value = proportionOfLengthToValue(next);
    if (juce::approximatelyEqual(value, getValue())) {
        const double interval =
            getInterval() > 0.0 ? getInterval() : (getMaximum() - getMinimum()) * 0.001;
        value += amount > 0.0f ? interval : -interval;
    }
    setValue(juce::jlimit(getMinimum(), getMaximum(), value),
             juce::sendNotificationSync);
    if (onUserEditEnd)
        onUserEditEnd();
}

void ParamKnob::setParamID(const juce::String& paramID)
{
    paramID_ = paramID;
    spec_ = findSpec(paramID_);
    specIndex_ = spec_ != nullptr
                     ? static_cast<int>(spec_ - kP1ParamSpecs.data())
                     : -1;
    macroTargeted_ = false;
    macroHasEffective_ = false;
    value_.setStoredColour(colour(tokens::pinkBright));
    value_.setEffectiveText({}, colour(tokens::green));
    attach_.reset();
    attach_ = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        state_, paramID_, slider_);
    updateValueText();
    repaint();
}

void ParamKnob::setMacroFeedback(bool targeted, bool hasEffective,
                                 double effective)
{
    if (macroTargeted_ == targeted && macroHasEffective_ == hasEffective &&
        (!hasEffective || std::abs(macroEffective_ - effective) < 0.5))
        return;
    macroTargeted_ = targeted;
    macroHasEffective_ = hasEffective;
    macroEffective_ = effective;
    refreshValueDisplay();
}

void ParamKnob::setModArc(double fraction)
{
    fraction = juce::jlimit(0.0, 1.0, fraction);
    const double current = static_cast<double>(
        slider_.getProperties().getWithDefault("modArc", -1.0));
    if (current >= 0.0 && std::abs(current - fraction) < 0.0005)
        return;
    slider_.getProperties().set("modArc", fraction);
    slider_.repaint();
}

void ParamKnob::resized()
{
    auto area = getLocalBounds();
    const int dial = static_cast<int>(std::round(dialSize_));
    auto dialArea = area.removeFromTop(dial).withSizeKeepingCentre(
        juce::jmin(dial, getWidth()), dial);
    slider_.setBounds(dialArea);
    name_.setBounds(0, dialArea.getBottom() + 1, getWidth(),
                    hasName_ ? 16 : 0);
    value_.setBounds(0, dialArea.getBottom() + (hasName_ ? 15 : 1), getWidth(),
                     20);
}

void ParamKnob::paint(juce::Graphics&) {}

void ParamKnob::updateValueText()
{
    if (spec_ != nullptr)
        value_.setText(formatParamValue(*spec_, slider_.getValue()),
                       juce::dontSendNotification);
    refreshValueDisplay();
}

void ParamKnob::refreshValueDisplay()
{
    if (spec_ == nullptr)
        return;
    const bool atRest = macroTargeted_ && !macroHasEffective_;
    value_.setStoredColour(colour(atRest ? tokens::purple : tokens::pinkBright));
    if (macroHasEffective_)
        value_.setEffectiveText(formatParamValuePlain(*spec_, macroEffective_),
                                colour(tokens::green));
    else
        value_.setEffectiveText({}, colour(tokens::green));
}

bool ParamKnob::isRatio() const
{
    return spec_ != nullptr && juce::String(spec_->display) == "ratio";
}

void ParamKnob::snapValue()
{
    if (!isRatio() || snapping_)
        return;
    const auto current = static_cast<int32_t>(std::lround(slider_.getValue()));
    const auto snapped = snapRatio(current);
    if (snapped == current)
        return;
    const juce::ScopedValueSetter<bool> guard(snapping_, true);
    slider_.setValue(static_cast<double>(snapped), juce::sendNotificationSync);
}

bool ParamKnob::stepRatio(const juce::KeyPress& key)
{
    if (!isRatio())
        return false;
    const int code = key.getKeyCode();
    int dir = 0;
    if (code == juce::KeyPress::upKey || code == juce::KeyPress::rightKey)
        dir = 1;
    else if (code == juce::KeyPress::downKey || code == juce::KeyPress::leftKey)
        dir = -1;
    if (dir == 0)
        return false;
    const auto current = static_cast<int32_t>(std::lround(slider_.getValue()));
    slider_.setValue(static_cast<double>(stepRatioDetent(current, dir)),
                     juce::sendNotificationSync);
    return true;
}

void ParamKnob::commitValue()
{
    if (spec_ == nullptr)
        return;
    const double value = parseParamValue(*spec_, value_.getText());
    slider_.setValue(value, juce::sendNotificationSync);
    snapValue();
    updateValueText();
    if (onUserEditEnd)
        onUserEditEnd();
}

P1Button::P1Button(const juce::String& label, bool mini)
    : juce::Button(label)
{
    setButtonText(label);
    setMouseCursor(juce::MouseCursor::PointingHandCursor);
    setSize(mini ? 52 : 72, mini ? 18 : 24);
}

void P1Button::paintButton(juce::Graphics& g, bool highlighted, bool down)
{
    const auto bounds = getLocalBounds().toFloat();
    g.setColour(colour(tokens::bg));
    g.fillRect(bounds);
    g.setColour(highlighted || down ? colour(tokens::pink)
                                    : colour(tokens::lineStrong));
    g.drawRect(bounds, 1.0f);
    g.setFont(uiFont(getHeight() <= 20 ? 10.0f : tokens::textButton));
    g.setColour(down ? colour(tokens::pink)
                     : highlighted ? colour(tokens::white)
                                   : colour(tokens::dim));
    g.drawText(getButtonText(), getLocalBounds(),
               juce::Justification::centred, false);
}

P1MuteTile::P1MuteTile(const juce::String& label, bool muted,
                       std::function<void(bool)> onToggle)
    : juce::Button(label)
{
    setButtonText(label);
    setClickingTogglesState(true);
    setToggleState(muted, juce::dontSendNotification);
    setMouseCursor(juce::MouseCursor::PointingHandCursor);
    onClick = [this, onToggle] {
        if (onToggle)
            onToggle(getToggleState());
        repaint();
    };
}

void P1MuteTile::paintButton(juce::Graphics& g, bool highlighted, bool)
{
    const auto bounds = getLocalBounds().toFloat();
    const bool muted = getToggleState();
    g.setColour(muted ? colour(tokens::bg) : colour(tokens::pink));
    g.fillRect(bounds);
    g.setColour(muted ? colour(tokens::pink)
                      : highlighted ? colour(tokens::white)
                                    : colour(tokens::pink));
    g.drawRect(bounds, 1.0f);
    g.setFont(uiFont(tokens::textMuteLabel));
    g.setColour(muted ? colour(tokens::pink) : colour(tokens::bg));
    g.drawText(getButtonText(), getLocalBounds(),
               juce::Justification::centred, false);
}

P1Toggle::P1Toggle(APVTS& state, const juce::String& paramID,
                   const juce::String& label)
    : juce::Button(label), label_(label)
{
    setClickingTogglesState(true);
    setMouseCursor(juce::MouseCursor::PointingHandCursor);
    attach_ = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        state, paramID, *this);
}

void P1Toggle::paintButton(juce::Graphics& g, bool highlighted, bool)
{
    const bool on = getToggleState();
    const auto track = juce::Rectangle<float>(
        0.0f, (static_cast<float>(getHeight()) - 12.0f) * 0.5f, 26.0f, 12.0f);
    g.setColour(colour(tokens::bg));
    g.fillRect(track);
    g.setColour(on ? colour(tokens::green)
                   : highlighted ? colour(tokens::pink)
                                 : colour(tokens::lineStrong));
    g.drawRect(track, 1.0f);
    const float thumbX = on ? track.getX() + 1.0f : track.getRight() - 11.0f;
    g.setColour(on ? colour(tokens::pink) : colour(tokens::faint));
    g.fillRect(juce::Rectangle<float>(thumbX, track.getY() + 1.0f, 10.0f, 8.0f));
    g.setFont(uiFont(tokens::textLegend));
    g.setColour(on ? colour(tokens::green) : colour(tokens::dim));
    const juce::String state = on ? "on" : "off";
    const juce::String caption =
        (label_ == "on" || label_.isEmpty()) ? state : label_ + " " + state;
    g.drawText(caption, juce::Rectangle<int>(32, 0, getWidth() - 32, getHeight()),
               juce::Justification::centredLeft, true);
}

ParamChoice::ParamChoice(APVTS& state, const juce::String& paramID,
                         juce::StringArray items)
{
    const auto* spec = findSpec(paramID);
    min_ = spec != nullptr ? spec->min : 0;
    param_ = state.getParameter(paramID);
    combo_.addItemList(items, 1);
    addAndMakeVisible(combo_);
    if (param_ != nullptr) {
        attach_ = std::make_unique<juce::ParameterAttachment>(
            *param_, [this](float normalized) { updateFromNormalized(normalized); });
        attach_->sendInitialUpdate();
    }
    combo_.onChange = [this] {
        if (attach_ != nullptr)
            attach_->setValueAsCompleteGesture(
                static_cast<float>(combo_.getSelectedItemIndex() + min_));
    };
}

void ParamChoice::resized()
{
    combo_.setBounds(getLocalBounds());
}

void ParamChoice::updateFromNormalized(float normalized)
{
    if (param_ == nullptr)
        return;
    const int value =
        static_cast<int>(std::lround(normalized));
    combo_.setSelectedItemIndex(value - min_, juce::dontSendNotification);
}

P1TabBar::Tab::Tab(const juce::String& label, int index, P1TabBar& owner)
    : juce::Button(label), index_(index), owner_(owner)
{
    setMouseCursor(juce::MouseCursor::PointingHandCursor);
    /* The active tab tail reaches into the tab to its left. */

    setPaintingIsUnclipped(true);
}

void P1TabBar::Tab::paintButton(juce::Graphics& g, bool highlighted, bool)
{
    const bool active = owner_.active_ == index_;
    auto bounds = getLocalBounds().toFloat();
    if (active) {
        /* The first tab keeps the original tail. Later tabs run 10px further
           left so the pink shape reads as one continuous tab, not a detached
           label. */
        const float shift = index_ == 0 ? 0.0f : -10.0f;
        juce::Path cut;
        cut.startNewSubPath(5.0f + shift, 0.0f);
        cut.lineTo(bounds.getWidth() - 12.0f, 0.0f);
        cut.lineTo(bounds.getWidth(), bounds.getHeight() * 0.5f);
        cut.lineTo(bounds.getWidth() - 7.0f, bounds.getHeight());
        cut.lineTo(0.0f + shift, bounds.getHeight());
        cut.lineTo(5.0f + shift, bounds.getHeight() * 0.5f);
        cut.closeSubPath();
        g.setColour(colour(tokens::pink));
        g.fillPath(cut);
        g.setColour(colour(tokens::bg));
    } else {
        g.setColour(highlighted ? colour(tokens::pink) : colour(tokens::dim));
    }
    g.setFont(displayFont(tokens::textTab));
    g.drawText(getButtonText(), getLocalBounds().withTrimmedLeft(active ? 8 : 10),
               juce::Justification::centredLeft, false);
}

P1TabBar::P1TabBar() = default;

void P1TabBar::setLabels(const juce::StringArray& labels)
{
    labels_ = labels;
    tabs_.clear();
    for (int i = 0; i < labels.size(); ++i) {
        auto tab = std::make_unique<Tab>(labels[i], i, *this);
        tab->onClick = [this, i] { setActive(i); };
        addAndMakeVisible(*tab);
        tabs_.push_back(std::move(tab));
    }
    active_ = juce::jlimit(0, juce::jmax(0, static_cast<int>(tabs_.size()) - 1),
                           active_);
    resized();
    repaint();
}

void P1TabBar::setCompact(bool compact)
{
    if (forcedCompact_ == compact)
        return;
    forcedCompact_ = compact;
    resized();
    repaint();
}

void P1TabBar::setActive(int index)
{
    if (tabs_.empty())
        return;
    active_ = juce::jlimit(0, static_cast<int>(tabs_.size()) - 1, index);
    for (auto& tab : tabs_) {
        if (tab != nullptr)
            tab->repaint();
    }
    if (onTabChanged)
        onTabChanged(active_);
}

void P1TabBar::resized()
{
    const int count = static_cast<int>(tabs_.size());
    if (count == 0)
        return;

    int naturalWidth = 0;
    for (auto& tab : tabs_) {
        if (tab == nullptr)
            continue;
        naturalWidth += static_cast<int>(
            std::ceil(textWidth(displayFont(tokens::textTab), tab->getButtonText()))) +
            32;
    }

    /* The eight-tab editor is wider than a half-size window. Keep the full
     * labels when they fit, otherwise use the same tab order with compact
     * labels and equal widths so no label runs into its neighbour. */
    static constexpr const char* compactLabels[8] = {
        "patch", "params", "op envs", "op lfos", "phase", "spec a", "spec b",
        "spec mod"};
    const auto* top = getTopLevelComponent();
    const int visibleWidth =
        top != nullptr && top != this ? top->getWidth() : getWidth();
    const bool compact =
        count == 8 && (forcedCompact_ || naturalWidth > visibleWidth);

    int x = 0;
    for (int i = 0; i < count; ++i) {
        auto& tab = tabs_[static_cast<std::size_t>(i)];
        if (tab == nullptr)
            continue;
        if (compact)
            tab->setButtonText(compactLabels[i]);
        else if (i < labels_.size())
            tab->setButtonText(labels_[i]);

        const int width = compact
            ? (i == count - 1 ? getWidth() - x : getWidth() / count)
            : static_cast<int>(std::ceil(textWidth(
                  displayFont(tokens::textTab), tab->getButtonText()))) + 32;
        tab->setBounds(x, 0, width, getHeight());
        x += width;
    }
}

void P1TabBar::paint(juce::Graphics& g)
{
    g.fillAll(colour(tokens::bg));
    g.setColour(colour(tokens::lineStrong));
    g.fillRect(0, getHeight() - 1, getWidth(), 1);
}

P1Scope::P1Scope()
{
    setInterceptsMouseClicks(false, false);
}

void P1Scope::setSamples(const float* samples, int count)
{
    samples_.assign(samples, samples + juce::jmax(0, count));
    repaint();
}

void P1Scope::paint(juce::Graphics& g)
{
    g.fillAll(colour(tokens::bg));
    const auto area = getLocalBounds().toFloat().reduced(1.0f, 2.0f);
    if (area.isEmpty())
        return;
    juce::Path path;
    const int n = static_cast<int>(samples_.size());
    /* The operator level is a Yamaha total-level curve, so a normal patch sits
       well below full scale, and a 4x soft-knee gain restores the old trace
       height whilst the tanh keeps a loud trace inside the widget. That stops
       it squaring off at the top. */
    const float gain = 4.0f;
    if (n < 2) {
        path.startNewSubPath(area.getX(), area.getCentreY());
        path.lineTo(area.getRight(), area.getCentreY());
    } else {
        for (int i = 0; i < n; ++i) {
            const float x = area.getX() +
                            area.getWidth() * static_cast<float>(i) /
                                static_cast<float>(n - 1);
            const float y = area.getCentreY() -
                            std::tanh(samples_[static_cast<size_t>(i)] * gain) *
                                area.getHeight() * 0.46f;
            if (i == 0)
                path.startNewSubPath(x, y);
            else
                path.lineTo(x, y);
        }
    }
    g.setColour(colour(tokens::green));
    g.strokePath(path, juce::PathStrokeType(1.3f));
}

P1Spectro::P1Spectro()
{
    setInterceptsMouseClicks(false, false);
}

void P1Spectro::setSamples(const float* samples, int count)
{
    samples_.assign(samples, samples + juce::jmax(0, count));
    repaint();
}

void P1Spectro::paint(juce::Graphics& g)
{
    g.fillAll(colour(tokens::bg));
    fftData_.fill(0.0f);
    const int n = juce::jmin(512, static_cast<int>(samples_.size()));
    for (int i = 0; i < n; ++i) {
        const float window = 0.5f - 0.5f * std::cos(
            juce::MathConstants<float>::twoPi * static_cast<float>(i) / 511.0f);
        fftData_[static_cast<size_t>(i)] = samples_[static_cast<size_t>(i)] * window;
    }
    fft_.performFrequencyOnlyForwardTransform(fftData_.data());
    const int bars = 22;
    const float barWidth = static_cast<float>(getWidth()) / bars;
    for (int i = 0; i < bars; ++i) {
        const int bin = juce::jlimit(1, 220, 2 + static_cast<int>(
            std::pow(static_cast<double>(i) / bars, 2.0) * 210.0));
        float magnitude = 0.0f;
        for (int k = bin; k < juce::jmin(256, bin + 3); ++k)
            magnitude = juce::jmax(magnitude, fftData_[static_cast<size_t>(k)]);
        magnitude = magnitude / 512.0f;
        /* The same 4x as the scope, because the operator level curve leaves
           normal patches well below full scale. Lift the log argument to keep
           the bars readable. The clamp keeps a loud spectrum at full height. */
        const float level = juce::jlimit(
            0.04f, 1.0f, std::log10(1.0f + magnitude * 360.0f));
        const float height =
            level * (static_cast<float>(getHeight()) - 2.0f);
        g.setColour(colour(i % 5 == 0 ? tokens::purple : tokens::pink));
        g.fillRect(static_cast<float>(i) * barWidth + 1.0f,
                   static_cast<float>(getHeight()) - height, barWidth - 2.0f,
                   height);
    }
}

P1SpectralBars::P1SpectralBars(APVTS& state, juce::StringArray parameterIDs,
                               juce::Colour barColour)
    : state_(state), parameterIDs_(std::move(parameterIDs)), colour_(barColour)
{
    setMouseCursor(juce::MouseCursor::PointingHandCursor);
}

double P1SpectralBars::levelAt(int index) const
{
    if (index < 0 || index >= parameterIDs_.size())
        return 0.0;
    if (auto* value = state_.getRawParameterValue(parameterIDs_[index]))
        return juce::jlimit(0.0, 99.0, static_cast<double>(value->load()));
    return 0.0;
}

std::vector<double> P1SpectralBars::levels() const
{
    std::vector<double> result;
    result.reserve(static_cast<std::size_t>(parameterIDs_.size()));
    for (int i = 0; i < parameterIDs_.size(); ++i)
        result.push_back(levelAt(i));
    return result;
}

void P1SpectralBars::setFirstHarmonic(int first)
{
    if (first == firstHarmonic_)
        return;
    firstHarmonic_ = first;
    repaint();
}

void P1SpectralBars::setEditWindow(int first, int last)
{
    windowFirst_ = first;
    windowLast_ = last;
    repaint();
}

void P1SpectralBars::setEditTest(std::function<bool(int, double)> test)
{
    editTest_ = std::move(test);
}

bool P1SpectralBars::inWindow(int harmonic) const
{
    return harmonic >= windowFirst_ && harmonic <= windowLast_;
}

void P1SpectralBars::setGroupTest(std::function<bool(int)> test)
{
    groupTest_ = std::move(test);
    repaint();
}

void P1SpectralBars::setLevel(int index, double value)
{
    if (index < 0 || index >= parameterIDs_.size())
        return;
    if (auto* parameter = state_.getParameter(parameterIDs_[index]))
        parameter->setValueNotifyingHost(parameter->convertTo0to1(
            static_cast<float>(juce::jlimit(0.0, 99.0, value))));
}

void P1SpectralBars::setLevels(const std::vector<double>& levels)
{
    const int count = juce::jmin(static_cast<int>(levels.size()),
                                 parameterIDs_.size());
    for (int i = 0; i < count; ++i)
        setLevel(i, levels[static_cast<std::size_t>(i)]);
    repaint();
}

void P1SpectralBars::setFromMouse(const juce::MouseEvent& event)
{
    auto area = getLocalBounds().reduced(4, 4);
    auto bars = area.withTrimmedBottom(12);
    if (bars.isEmpty())
        return;
    const float barsWidth = static_cast<float>(bars.getWidth());
    const float barsHeight = static_cast<float>(bars.getHeight());
    const float barsLeft = static_cast<float>(bars.getX());
    const float barsTop = static_cast<float>(bars.getY());
    const int count = juce::jmax(1, parameterIDs_.size());
    const int index = juce::jlimit(
        0, count - 1, static_cast<int>((event.position.x - barsLeft) /
                                       (barsWidth / static_cast<float>(count))));
    const int harmonic = firstHarmonic_ + index;
    if (!inWindow(harmonic))
        return;
    if (editTest_ && !editTest_(harmonic, levelAt(index)))
        return;
    const double level = juce::jlimit(
        0.0, 99.0, 99.0 * (barsTop + barsHeight - event.position.y) /
                        static_cast<double>(barsHeight));
    setLevel(index, level);
    repaint();
}

void P1SpectralBars::mouseDown(const juce::MouseEvent& event)
{
    if (event.mods.isPopupMenu())
        return;
    setFromMouse(event);
}

void P1SpectralBars::mouseDrag(const juce::MouseEvent& event)
{
    if (event.mods.isPopupMenu())
        return;
    setFromMouse(event);
}

void P1SpectralBars::paint(juce::Graphics& g)
{
    g.fillAll(colour(tokens::bg));
    g.setColour(colour(tokens::line));
    g.drawRect(getLocalBounds(), 1);
    auto area = getLocalBounds().reduced(4, 4);
    auto bars = area.withTrimmedBottom(12);
    if (bars.isEmpty())
        return;
    const int count = juce::jmax(1, parameterIDs_.size());
    const float slot = static_cast<float>(bars.getWidth()) /
                       static_cast<float>(count);
    /* The editable window is a faintly lighter band, so a narrowed selection is
     * visible even when its bars are empty. */
    if (windowFirst_ > firstHarmonic_ ||
        windowLast_ < firstHarmonic_ + count - 1) {
        const int start = juce::jmax(0, windowFirst_ - firstHarmonic_);
        const int end = juce::jmin(count - 1, windowLast_ - firstHarmonic_);
        g.setColour(juce::Colours::white.withAlpha(0.02f));
        g.fillRect(static_cast<float>(bars.getX()) +
                       static_cast<float>(start) * slot,
                   static_cast<float>(bars.getY()),
                   static_cast<float>(end - start + 1) * slot,
                   static_cast<float>(bars.getHeight()));
    }
    for (int i = 0; i < count; ++i) {
        const float level = static_cast<float>(levelAt(i)) / 99.0f;
        const float height = level * static_cast<float>(bars.getHeight());
        const bool outside = !inWindow(firstHarmonic_ + i);
        g.setColour(outside
                        ? colour(tokens::lineStrong)
                        : colour(i % 2 == 0 ? tokens::purple : tokens::pink));
        g.fillRect(static_cast<float>(bars.getX()) + static_cast<float>(i) * slot + 1.0f,
                   static_cast<float>(bars.getBottom()) - height,
                   juce::jmax(1.0f, slot - 2.0f), height);
    }
    g.setColour(colour(tokens::lineStrong));
    g.drawHorizontalLine(bars.getBottom(), static_cast<float>(bars.getX()),
                         static_cast<float>(bars.getRight()));
    if (groupTest_) {
        g.setColour(colour(tokens::green));
        for (int i = 0; i < count; ++i) {
            if (!groupTest_(firstHarmonic_ + i))
                continue;
            g.drawRect(juce::Rectangle<float>(
                           static_cast<float>(bars.getX()) +
                               static_cast<float>(i) * slot + 1.0f,
                           static_cast<float>(bars.getY()),
                           juce::jmax(1.0f, slot - 2.0f),
                           static_cast<float>(bars.getHeight())),
                       1.0f);
        }
    }
    g.setFont(uiFont(tokens::textLegend));
    g.setColour(colour(tokens::dim));
    for (int k = 0; k < count; ++k) {
        const int h = firstHarmonic_ + k;
        const bool member = groupTest_ && groupTest_(h);
        if (k % 8 != 0 && !member)
            continue;
        const float x = static_cast<float>(bars.getX()) +
                        (static_cast<float>(k) + 0.5f) * slot;
        g.drawText(juce::String(h), juce::Rectangle<float>(
                                        x - slot * 0.5f,
                                        static_cast<float>(bars.getBottom()) + 1.0f,
                                        slot, 11.0f),
                   juce::Justification::centred, false);
    }
}

P1SpectrumDisplay::P1SpectrumDisplay(APVTS& state, juce::StringArray aIDs,
                                     juce::StringArray bIDs,
                                     juce::String morphID,
                                     juce::String rangeAID,
                                     juce::String rangeBID,
                                     juce::Colour aColour,
                                     juce::Colour bColour)
    : state_(state),
      aIDs_(std::move(aIDs)),
      bIDs_(std::move(bIDs)),
      morphID_(std::move(morphID)),
      rangeAID_(std::move(rangeAID)),
      rangeBID_(std::move(rangeBID)),
      aColour_(aColour),
      bColour_(bColour)
{
    setInterceptsMouseClicks(false, false);
}

double P1SpectrumDisplay::levelAt(const juce::StringArray& ids, int index) const
{
    if (index < 0 || index >= ids.size())
        return 0.0;
    if (auto* value = state_.getRawParameterValue(ids[index]))
        return juce::jlimit(0.0, 99.0, static_cast<double>(value->load()));
    return 0.0;
}

void P1SpectrumDisplay::paint(juce::Graphics& g)
{
    g.fillAll(colour(tokens::bg));
    g.setColour(colour(tokens::line));
    g.drawRect(getLocalBounds(), 1);

    auto area = getLocalBounds().toFloat().reduced(6.0f, 6.0f);
    auto legend = area.removeFromTop(10.0f);
    auto graph = area.withTrimmedBottom(12.0f);
    if (graph.isEmpty())
        return;

    const int rangeA = juce::jlimit(
        0, 1, static_cast<int>(std::lround(
                  state_.getRawParameterValue(rangeAID_) != nullptr
                      ? static_cast<double>(
                            state_.getRawParameterValue(rangeAID_)->load())
                      : 0.0)));
    const int rangeB = juce::jlimit(
        0, 1, static_cast<int>(std::lround(
                  state_.getRawParameterValue(rangeBID_) != nullptr
                      ? static_cast<double>(
                            state_.getRawParameterValue(rangeBID_)->load())
                      : 0.0)));
    const int firstA = rangeA * 64 + 1;
    const int firstB = rangeB * 64 + 1;
    const int minFirst = juce::jmin(firstA, firstB);
    const int maxLast = juce::jmax(firstA + 63, firstB + 63);
    const int span = maxLast - minFirst + 1;
    auto xAt = [&](int harmonic) {
        return graph.getX() +
               (static_cast<float>(harmonic - minFirst) + 0.5f) /
                   static_cast<float>(span) * graph.getWidth();
    };

    const juce::String aText = "spectrum a (" + juce::String(firstA) + "-" +
                               juce::String(firstA + 63) + ")";
    const juce::String bText = "spectrum b (" + juce::String(firstB) + "-" +
                               juce::String(firstB + 63) + ")";
    const juce::String waveText = "waveform";
    juce::Font legendFont = uiFont(tokens::textLegend);
    auto legendWidths = [&] {
        return std::array<float, 3>{
            textWidth(legendFont, aText) + 8.0f,
            textWidth(legendFont, bText) + 8.0f,
            textWidth(legendFont, waveText) + 4.0f};
    };
    auto widths = legendWidths();
    if (widths[0] + widths[1] + widths[2] > legend.getWidth()) {
        legendFont = uiFont(tokens::textLegend - 1.0f);
        widths = legendWidths();
    }
    g.setFont(legendFont);
    g.setColour(aColour_);
    g.drawText(aText, legend.removeFromLeft(widths[0]),
               juce::Justification::centredLeft, false);
    g.setColour(bColour_);
    g.drawText(bText, legend.removeFromLeft(widths[1]),
               juce::Justification::centredLeft, false);
    g.setColour(colour(tokens::green).withAlpha(0.72f));
    g.drawText(waveText, legend.removeFromLeft(widths[2]),
               juce::Justification::centredLeft, false);

    /* The grid follows the visible harmonic span, so two different ranges zoom
       out to a shared axis instead of each drawing on its own scale. */
    g.setColour(colour(tokens::line).withAlpha(0.6f));
    const int divisions = span > 32 ? 8 : 16;
    for (int i = 0; i <= divisions; ++i) {
        const float x = graph.getX() + graph.getWidth() *
                                             static_cast<float>(i) /
                                             static_cast<float>(divisions);
        g.drawVerticalLine(juce::roundToInt(x), graph.getY(), graph.getBottom());
    }
    g.drawHorizontalLine(juce::roundToInt(graph.getBottom()), graph.getX(),
                         graph.getRight());

    auto aLevel = [&](int index) { return levelAt(aIDs_, index); };
    auto bLevel = [&](int index) { return levelAt(bIDs_, index); };

    auto drawSpectrum = [&](std::function<double(int)> level, int first,
                            juce::Colour lineColour) {
        juce::Path path;
        path.startNewSubPath(xAt(first), graph.getBottom());
        const int count = aIDs_.size();
        for (int i = 0; i < count; ++i) {
            const float y = graph.getBottom() -
                            static_cast<float>(level(i)) / 99.0f *
                                graph.getHeight();
            path.lineTo(xAt(first + i), y);
        }
        path.lineTo(xAt(first + count - 1), graph.getBottom());
        path.closeSubPath();
        g.setColour(lineColour.withAlpha(0.18f));
        g.fillPath(path);
        g.setColour(lineColour);
        g.strokePath(path, juce::PathStrokeType(1.2f));
    };
    drawSpectrum(aLevel, firstA, aColour_);
    drawSpectrum(bLevel, firstB, bColour_);

    const double baseMorph = state_.getRawParameterValue(morphID_) != nullptr
                                 ? juce::jlimit(0.0, 99.0,
                                                static_cast<double>(state_
                                                    .getRawParameterValue(
                                                        morphID_)
                                                    ->load())) /
                                       99.0
                                 : 0.0;
    const double morph = liveMorph_ >= 0.0
                             ? juce::jlimit(0.0, 1.0, liveMorph_)
                             : baseMorph;

    /* The faint line is the phase-zero sum of the two spectra at their own
       harmonic offsets, so the display shows the shape the engine will read at
       the live morph. The green cursor sits at the same value and follows the
       envelope and the LFO while a note sounds. */
    constexpr int points = 160;
    std::array<float, points> wave{};
    float peak = 0.0f;
    for (int i = 0; i < points; ++i) {
        const double t = static_cast<double>(i) / (points - 1);
        double sumA = 0.0;
        double sumB = 0.0;
        for (int h = 0; h < aIDs_.size(); ++h) {
            const double phase =
                2.0 * juce::MathConstants<double>::pi * t;
            sumA += aLevel(h) * std::sin(phase * (firstA + h));
            sumB += bLevel(h) * std::sin(phase * (firstB + h));
        }
        const double sum = (1.0 - morph) * sumA + morph * sumB;
        wave[static_cast<std::size_t>(i)] = static_cast<float>(sum);
        peak = juce::jmax(peak, std::abs(wave[static_cast<std::size_t>(i)]));
    }
    if (peak > 1.0e-6f) {
        juce::Path waveform;
        for (int i = 0; i < points; ++i) {
            const float x = graph.getX() +
                            graph.getWidth() * static_cast<float>(i) /
                                static_cast<float>(points - 1);
            const float y = graph.getCentreY() -
                            wave[static_cast<std::size_t>(i)] / peak *
                                graph.getHeight() * 0.42f;
            if (i == 0)
                waveform.startNewSubPath(x, y);
            else
                waveform.lineTo(x, y);
        }
        g.setColour(colour(tokens::green).withAlpha(0.62f));
        g.strokePath(waveform, juce::PathStrokeType(1.2f));
    }

    const float markerX = graph.getX() +
                          static_cast<float>(morph) * graph.getWidth();
    g.setColour(colour(tokens::green));
    g.drawVerticalLine(juce::roundToInt(markerX), graph.getY() - 3.0f,
                       graph.getBottom());
    juce::Path marker;
    marker.addTriangle(markerX - 4.0f, graph.getY() - 5.0f, markerX + 4.0f,
                       graph.getY() - 5.0f, markerX, graph.getY() + 1.0f);
    g.fillPath(marker);

    g.setFont(uiFont(tokens::textLegend));
    g.setColour(colour(tokens::dim));
    for (int k = 0; k < 5; ++k) {
        const int harmonic = minFirst + (span - 1) * k / 4;
        const float x = xAt(harmonic);
        g.drawText(juce::String(harmonic),
                   juce::Rectangle<float>(x - 12.0f, graph.getBottom() + 1.0f,
                                          24.0f, 11.0f),
                   juce::Justification::centred, false);
    }
}

P1Envelope::P1Envelope(APVTS& state, juce::StringArray rateIDs,
                       juce::StringArray levelIDs, juce::Colour curveColour,
                       double levelMin, double levelMax, bool zeroLine,
                       EnvelopeShape shape, juce::String modeID,
                       juce::String startID, double startScale)
    : state_(state),
      rateIDs_(std::move(rateIDs)),
      levelIDs_(std::move(levelIDs)),
      colour_(curveColour),
      levelMin_(levelMin),
      levelMax_(levelMax),
      zeroLine_(zeroLine),
      shape_(shape),
      modeID_(std::move(modeID)),
      startID_(std::move(startID)),
      startScale_(startScale)
{
    setInterceptsMouseClicks(false, false);
}

void P1Envelope::paint(juce::Graphics& g)
{
    g.fillAll(colour(tokens::bg));
    const auto area = getLocalBounds().toFloat().reduced(2.0f, 3.0f);
    if (area.isEmpty())
        return;
    const float mid = zeroLine_ ? area.getCentreY() : area.getBottom();
    if (zeroLine_) {
        g.setColour(colour(tokens::lineStrong));
        g.drawLine(area.getX(), mid, area.getRight(), mid, 1.0f);
    }

    std::array<int32_t, 5> rates{};
    for (int i = 0; i < rateIDs_.size() && i < 5; ++i) {
        if (auto* v = state_.getRawParameterValue(rateIDs_[i]))
            rates[static_cast<size_t>(i)] =
                static_cast<int32_t>(std::lround(v->load()));
    }
    std::array<double, 4> levels{};
    for (int i = 0; i < levelIDs_.size() && i < 4; ++i) {
        if (auto* v = state_.getRawParameterValue(levelIDs_[i]))
            levels[static_cast<size_t>(i)] = static_cast<double>(v->load());
    }
    int mode = 0;
    if (modeID_.isNotEmpty()) {
        if (auto* v = state_.getRawParameterValue(modeID_))
            mode = static_cast<int>(std::lround(v->load()));
    }

    auto levelY = [this, area, mid](double level) {
        if (zeroLine_) {
            const double extreme =
                juce::jmax(std::abs(levelMin_), std::abs(levelMax_));
            const double denom = extreme > 1.0 ? extreme : 32768.0;
            return static_cast<float>(mid - level *
                                               (area.getHeight() * 0.5 - 4.0) /
                                               denom);
        }
        const double range = levelMax_ - levelMin_;
        const double norm = range > 0.0 ? (level - levelMin_) / range : 0.0;
        return static_cast<float>(area.getBottom() - 4.0 -
                                  norm * (area.getHeight() - 8.0));
    };

    /* The legacy four-rate, three-level shape, used when a card passes no
     * explicit envelope shape. */
    if (shape_ == EnvelopeShape::legacyFourStage) {
        /* Stage weight is the square of the rate's distance from 99, so 99 has
         * no horizontal run and reads as vertical whatever its target level.
         * The four weights are normalised to fill the graph, which keeps the
         * usual 50 to 80 settings full width instead of cramming them into the
         * left edge. A four-level envelope uses one stage per rate, so it has
         * no sustain hold. The three-level amplitude and spectral envelopes
         * keep the short hold between the last decay and the release. */
        const auto stageWeight = [](int32_t rate) -> float {
            const float t = static_cast<float>(99 - juce::jlimit(0, 99, rate));
            return t * t;
        };
        const bool fourStage = levelIDs_.size() >= 4;
        const std::array<float, 4> weight = {
            stageWeight(rates[0]), stageWeight(rates[1]), stageWeight(rates[2]),
            stageWeight(rates[3])};
        const float sum = weight[0] + weight[1] + weight[2] + weight[3];
        const float sustain = fourStage ? 0.0f : 8.0f;
        const float scale = sum > 0.0f
                                ? (area.getWidth() - 8.0f - sustain) / sum
                                : 0.0f;
        const std::array<float, 5> len = {
            weight[0] * scale, weight[1] * scale, weight[2] * scale,
            fourStage ? weight[3] * scale : sustain,
            fourStage ? 0.0f : weight[3] * scale};

        const float right = area.getRight() - 4.0f;
        float x = area.getX() + 4.0f;
        const auto advance = [&x, right](float stage) {
            x += stage;
            if (x > right)
                x = right;
            return x;
        };
        juce::Path path;
        path.startNewSubPath(x, levelY(0.0));
        path.lineTo(advance(len[0]), levelY(levels[0]));
        path.lineTo(advance(len[1]), levelY(levels[1]));
        path.lineTo(advance(len[2]), levelY(levels[2]));
        if (fourStage) {
            path.lineTo(advance(len[3]), levelY(levels[3]));
        } else {
            path.lineTo(advance(len[3]), levelY(levels[2]));
            path.lineTo(advance(len[4]), levelY(levels[3]));
        }
        g.setColour(colour_);
        g.strokePath(path, juce::PathStrokeType(1.4f));
        return;
    }

    const float left = area.getX() + 4.0f;
    const float right = area.getRight() - 4.0f;
    const float total = juce::jmax(1.0f, right - left);
    const auto rateWeight = [](int32_t rate) -> float {
        const float t = static_cast<float>(99 - juce::jlimit(0, 99, rate));
        return t * t;
    };
    const auto boundary = [&g, &area](float bx) {
        const float dashes[] = {2.0f, 2.0f};
        g.setColour(colour(tokens::pink));
        g.drawDashedLine(juce::Line<float>(bx, area.getY(), bx, area.getBottom()),
                         dashes, 2, 1.0f);
    };
    const auto loopArrow = [&g, area](float x0, float x1) {
        const float ay = area.getBottom() - 1.0f;
        g.setColour(colour(tokens::pink).withAlpha(0.9f));
        g.drawLine(x0, ay, x1, ay, 1.0f);
        g.drawLine(x0, ay, x0 + 4.0f, ay - 3.0f, 1.0f);
        g.drawLine(x0, ay, x0 + 4.0f, ay + 3.0f, 1.0f);
    };

    std::array<float, 5> weight{};
    std::array<double, 4> stageLevels{};
    int rateCount = 0;
    double startLevel = 0.0;
    if (shape_ == EnvelopeShape::operatorAmp) {
        const int32_t warm = rates[0];
        const int32_t surge = rates[1];
        weight[0] = warm > 0 ? juce::jmax(4.0f, rateWeight(warm)) : 0.0f;
        weight[1] = surge > 0 ? juce::jmax(4.0f, rateWeight(surge)) : 0.0f;
        weight[2] = juce::jmax(4.0f, rateWeight(rates[2]));
        weight[3] = juce::jmax(4.0f, rateWeight(rates[3]));
        weight[4] = juce::jmax(4.0f, rateWeight(rates[4]));
        rateCount = 5;
        stageLevels[0] = levels[0];
        stageLevels[1] = levels[1];
    } else {
        for (int i = 0; i < 4; ++i)
            weight[static_cast<size_t>(i)] =
                juce::jmax(4.0f, rateWeight(rates[i]));
        rateCount = 4;
        for (int i = 0; i < 4; ++i)
            stageLevels[static_cast<size_t>(i)] = levels[i];
        if (startID_.isNotEmpty()) {
            if (auto* v = state_.getRawParameterValue(startID_))
                startLevel = static_cast<double>(v->load()) * startScale_;
        }
    }

    const bool hold = mode == 1 &&
                      (shape_ == EnvelopeShape::bipolar ||
                       stageLevels[1] > 0.5);
    const bool loop = mode >= 2;
    const float holdWidth = hold ? 8.0f : 0.0f;
    const float loopWidth = loop ? total * 0.18f : 0.0f;
    const float available = juce::jmax(8.0f, total - holdWidth - loopWidth);
    float sum = 0.0f;
    for (int i = 0; i < rateCount; ++i)
        sum += weight[static_cast<size_t>(i)];
    const float scale =
        sum > 0.0f ? available / sum : available / static_cast<float>(rateCount);

    float x = left;
    float y = levelY(startLevel);
    juce::Path path;
    path.startNewSubPath(x, y);
    const auto lineTo = [&path, &x, &y](float nx, float ny) {
        path.lineTo(nx, ny);
        x = nx;
        y = ny;
    };
    const auto curveTo = [&path, &x, &y](float nx, float ny, bool convex) {
        const float dx = nx - x;
        if (convex)
            path.cubicTo(x + dx * 0.55f, y, nx - dx * 0.10f, ny, nx, ny);
        else
            path.cubicTo(x + dx * 0.10f, ny, nx - dx * 0.45f, ny, nx, ny);
        x = nx;
        y = ny;
    };

    if (shape_ == EnvelopeShape::operatorAmp) {
        if (weight[0] > 0.0f) {
            const double target = weight[1] > 0.0f ? 49.5 : 99.0;
            curveTo(x + weight[0] * scale, levelY(target), true);
        }
        if (weight[1] > 0.0f)
            curveTo(x + weight[1] * scale, levelY(99.0), false);
        lineTo(x + weight[2] * scale, levelY(stageLevels[0]));
        lineTo(x + weight[3] * scale, levelY(stageLevels[1]));
        const float releaseTarget = 0.0f;
        if (mode == 0) {
            lineTo(x + weight[4] * scale, levelY(releaseTarget));
        } else if (hold) {
            const float bx = x;
            lineTo(x + holdWidth, y);
            boundary(bx);
            boundary(x);
            lineTo(x + weight[4] * scale, levelY(releaseTarget));
        } else if (loop) {
            const float bx = x;
            lineTo(x + loopWidth * 0.5f, levelY(stageLevels[0]));
            boundary(bx);
            boundary(x);
            lineTo(x + loopWidth * 0.5f, levelY(stageLevels[1]));
            boundary(x);
            loopArrow(bx, x);
            lineTo(x + weight[4] * scale, levelY(releaseTarget));
        }
    } else {
        lineTo(x + weight[0] * scale, levelY(stageLevels[0]));
        lineTo(x + weight[1] * scale, levelY(stageLevels[1]));
        lineTo(x + weight[2] * scale, levelY(stageLevels[2]));
        if (mode == 0) {
            lineTo(x + weight[3] * scale, levelY(stageLevels[3]));
        } else if (hold) {
            const float bx = x;
            lineTo(x + holdWidth, y);
            boundary(bx);
            boundary(x);
            lineTo(x + weight[3] * scale, levelY(stageLevels[3]));
        } else if (loop) {
            const float bx = x;
            lineTo(x + loopWidth * 0.5f, levelY(stageLevels[1]));
            boundary(bx);
            boundary(x);
            lineTo(x + loopWidth * 0.5f, levelY(stageLevels[2]));
            boundary(x);
            loopArrow(bx, x);
            lineTo(x + weight[3] * scale, levelY(stageLevels[3]));
        }
    }

    g.setColour(colour_);
    g.strokePath(path, juce::PathStrokeType(1.4f));
}

P1Wave::P1Wave(APVTS& state, juce::String waveID, juce::Colour curveColour)
    : state_(state), waveID_(std::move(waveID)), colour_(curveColour)
{
    setInterceptsMouseClicks(false, false);
}

void P1Wave::paint(juce::Graphics& g)
{
    g.fillAll(colour(tokens::bg));
    const auto area = getLocalBounds().toFloat().reduced(4.0f, 5.0f);
    int wave = 0;
    if (auto* v = state_.getRawParameterValue(waveID_))
        wave = static_cast<int>(std::lround(v->load()));
    juce::Path path;
    const int points = 120;
    for (int i = 0; i <= points; ++i) {
        const float p = static_cast<float>(i) / points;
        const float phase = std::fmod(p * 2.0f, 1.0f);
        float value = 0.0f;
        if (wave == 1)
            value = 1.0f - 4.0f * std::abs(phase - 0.5f);
        else if (wave == 2)
            value = 1.0f - 2.0f * phase;
        else if (wave == 3)
            value = 2.0f * phase - 1.0f;
        else if (wave == 4)
            value = phase < 0.5f ? 1.0f : -1.0f;
        else if (wave == 5)
            value = std::sin(static_cast<float>(i) * 1.7f) *
                    std::cos(static_cast<float>(i) * 0.37f);
        else
            value = std::sin(phase * juce::MathConstants<float>::twoPi);
        const float x = area.getX() + p * area.getWidth();
        const float y = area.getCentreY() - value * area.getHeight() * 0.42f;
        if (i == 0)
            path.startNewSubPath(x, y);
        else
            path.lineTo(x, y);
    }
    g.setColour(colour_);
    g.strokePath(path, juce::PathStrokeType(1.4f));
}

P1LevelSlider::P1LevelSlider(APVTS& state, const juce::String& paramID,
                             juce::String caption, bool bipolar)
    : state_(state), paramID_(paramID), caption_(std::move(caption)),
      bipolar_(bipolar)
{
    spec_ = findSpec(paramID_);
    specIndex_ = spec_ != nullptr
                     ? static_cast<int>(spec_ - kP1ParamSpecs.data())
                     : -1;
    if (auto* param = state_.getParameter(paramID_)) {
        attach_ = std::make_unique<juce::ParameterAttachment>(
            *param, [this](float value) {
                if (spec_ != nullptr)
                    value_ = juce::jlimit(static_cast<float>(spec_->min),
                                          static_cast<float>(spec_->max), value);
                repaint();
            });
        attach_->sendInitialUpdate();
    }
    setMouseCursor(juce::MouseCursor::PointingHandCursor);
}

void P1LevelSlider::setMacroFeedback(bool targeted, bool hasEffective,
                                     double effective)
{
    if (macroTargeted_ == targeted && macroHasEffective_ == hasEffective &&
        (!hasEffective || std::abs(macroEffective_ - effective) < 0.5))
        return;
    macroTargeted_ = targeted;
    macroHasEffective_ = hasEffective;
    macroEffective_ = effective;
    repaint();
}

void P1LevelSlider::resized()
{
    track_ = getLocalBounds().withTrimmedTop(20).reduced(0, 1);
}

void P1LevelSlider::paint(juce::Graphics& g)
{
    g.setFont(uiFont(tokens::textInlineLabel));
    g.setColour(colour(tokens::dim));
    g.drawText(caption_, getLocalBounds().removeFromTop(18),
               juce::Justification::centredLeft, false);
    if (spec_ != nullptr) {
        const auto area = getLocalBounds().removeFromTop(18);
        const auto font = uiFont(tokens::textInlineLabel);
        const bool atRest = macroTargeted_ && !macroHasEffective_;
        g.setColour(colour(atRest ? tokens::purple : tokens::pinkBright));
        const auto stored = formatParamValue(*spec_, value_);
        if (!macroHasEffective_) {
            g.setFont(font);
            g.drawText(stored, area, juce::Justification::centredRight, false);
        } else {
            const auto small = font.withHeight(font.getHeight() * 0.62f);
            const auto effective =
                formatParamValuePlain(*spec_, macroEffective_);
            const float storedWidth = textWidth(font, stored);
            const float effectiveWidth = textWidth(small, effective);
            constexpr float gap = 6.0f;
            const float x = static_cast<float>(area.getRight()) - storedWidth -
                            gap - effectiveWidth;
            g.setFont(font);
            g.drawText(stored,
                       juce::Rectangle<float>(
                           x, static_cast<float>(area.getY()), storedWidth,
                           static_cast<float>(area.getHeight()))
                           .toNearestInt(),
                       juce::Justification::centredLeft, false);
            g.setFont(small);
            g.setColour(colour(tokens::green));
            g.drawText(effective,
                       juce::Rectangle<float>(
                           x + storedWidth + gap,
                           static_cast<float>(area.getY()), effectiveWidth,
                           static_cast<float>(area.getHeight()))
                           .toNearestInt(),
                       juce::Justification::centredLeft, false);
        }
    }
    g.setColour(colour(tokens::lineStrong));
    g.drawRect(track_, 1);
    const float range = spec_ != nullptr ? static_cast<float>(spec_->max - spec_->min)
                                         : 1.0f;
    const float norm = range > 0.0f
                           ? juce::jlimit(0.0f, 1.0f,
                                         (value_ - static_cast<float>(spec_->min)) / range)
                           : 0.0f;
    const auto inner = track_.reduced(1);
    const int edge = inner.getX() + static_cast<int>(
                                         std::round(static_cast<float>(inner.getWidth()) * norm));
    g.setColour(colour(tokens::purple));
    if (bipolar_) {
        const int centre = inner.getX() + inner.getWidth() / 2;
        const int x = juce::jmin(centre, edge);
        const int w = juce::jmax(centre, edge) - x;
        g.fillRect(juce::Rectangle<int>(x, inner.getY(), w, inner.getHeight()));
        g.setColour(colour(tokens::lineStrong));
        g.drawVerticalLine(centre, static_cast<float>(inner.getY()),
                           static_cast<float>(inner.getBottom()));
    } else {
        g.fillRect(inner.withWidth(edge - inner.getX()));
    }
}

void P1LevelSlider::mouseDown(const juce::MouseEvent& event)
{
    setFromMouse(event);
}

void P1LevelSlider::mouseDrag(const juce::MouseEvent& event)
{
    setFromMouse(event);
}

void P1LevelSlider::setFromMouse(const juce::MouseEvent& event)
{
    if (attach_ == nullptr || spec_ == nullptr || track_.getWidth() <= 0)
        return;
    const float norm = juce::jlimit(
        0.0f, 1.0f,
        (static_cast<float>(event.x) - static_cast<float>(track_.getX())) /
            static_cast<float>(track_.getWidth()));
    const float value = static_cast<float>(spec_->min) +
                        norm * static_cast<float>(spec_->max - spec_->min);
    attach_->setValueAsCompleteGesture(value);
}

P1VolumeSlider::P1VolumeSlider(APVTS& state, const juce::String& paramID)
    : state_(state), paramID_(paramID)
{
    if (auto* param = state_.getParameter(paramID_)) {
        attach_ = std::make_unique<juce::ParameterAttachment>(
            *param, [this](float value) {
                position_ = juce::jlimit(
                    0, p1::kVolumePositions,
                    static_cast<int>(std::lround(value)));
                repaint();
            });
        attach_->sendInitialUpdate();
    }
    setMouseCursor(juce::MouseCursor::PointingHandCursor);
}

void P1VolumeSlider::resized()
{
    track_ = getLocalBounds().withTrimmedTop(20).reduced(0, 1);
}

void P1VolumeSlider::paint(juce::Graphics& g)
{
    g.setFont(uiFont(tokens::textInlineLabel));
    g.setColour(colour(tokens::dim));
    g.drawText("volume", getLocalBounds().removeFromTop(18),
               juce::Justification::centredLeft, false);
    g.setColour(colour(tokens::pinkBright));
    g.drawText(volumeText(position_), getLocalBounds().removeFromTop(18),
               juce::Justification::centredRight, false);
    g.setColour(colour(tokens::lineStrong));
    g.drawRect(track_, 1);
    const auto inner = track_.reduced(1);
    const float norm = static_cast<float>(position_) /
                       static_cast<float>(p1::kVolumePositions);
    const int edge = inner.getX() + static_cast<int>(
                                         std::round(static_cast<float>(inner.getWidth()) *
                                                    norm));
    g.setColour(colour(tokens::purple));
    g.fillRect(inner.withWidth(edge - inner.getX()));
    /* The unity mark sits where the law reaches 0 dB, so the flat setting can be
       found without reading the value. */
    const int unity =
        inner.getX() + static_cast<int>(std::round(
                           static_cast<float>(inner.getWidth()) *
                           static_cast<float>(p1::kVolumeUnity) /
                           static_cast<float>(p1::kVolumePositions)));
    g.setColour(colour(tokens::dim));
    g.drawVerticalLine(unity, static_cast<float>(inner.getY()),
                       static_cast<float>(inner.getBottom()));
}

void P1VolumeSlider::mouseDown(const juce::MouseEvent& event)
{
    setFromMouse(event);
}

void P1VolumeSlider::mouseDrag(const juce::MouseEvent& event)
{
    setFromMouse(event);
}

void P1VolumeSlider::setFromMouse(const juce::MouseEvent& event)
{
    if (attach_ == nullptr || track_.getWidth() <= 0)
        return;
    const float norm = juce::jlimit(
        0.0f, 1.0f,
        (static_cast<float>(event.x) - static_cast<float>(track_.getX())) /
            static_cast<float>(track_.getWidth()));
    attach_->setValueAsCompleteGesture(
        norm * static_cast<float>(p1::kVolumePositions));
}

P1Card::P1Card(juce::String title, std::function<void()> onCopy,
               std::function<void()> onPaste)
    : title_(std::move(title))
{
    setInterceptsMouseClicks(false, true);
    if (onCopy && onPaste) {
        hasActions_ = true;
        copy_.onClick = std::move(onCopy);
        paste_.onClick = std::move(onPaste);
        addAndMakeVisible(copy_);
        addAndMakeVisible(paste_);
    }
}

void P1Card::paint(juce::Graphics& g)
{
    g.fillAll(colour(tokens::panel));
    drawSectionTitle(g, getLocalBounds().removeFromTop(20).reduced(8, 0), title_);
}

void P1Card::resized()
{
    if (!hasActions_)
        return;
    auto row = getLocalBounds().removeFromTop(20).reduced(4, 1);
    if (trailingWidth_ > 0)
        row.removeFromRight(trailingWidth_ + 6);
    paste_.setBounds(row.removeFromRight(52));
    row.removeFromRight(2);
    copy_.setBounds(row.removeFromRight(52));
}

void P1Card::setHeaderTrailingWidth(int width)
{
    trailingWidth_ = juce::jmax(0, width);
    resized();
}

juce::Rectangle<int> P1Card::headerTrailingBounds() const
{
    auto row = getLocalBounds().removeFromTop(20).reduced(4, 1);
    return row.removeFromRight(trailingWidth_);
}

juce::Rectangle<int> P1Card::contentArea() const
{
    return getLocalBounds().withTrimmedTop(22).reduced(8, 0);
}

P1AlgorithmBlock::P1AlgorithmBlock()
{
    setMouseCursor(juce::MouseCursor::PointingHandCursor);
}

void P1AlgorithmBlock::setAlgorithm(int algorithm)
{
    algorithm_ = juce::jlimit(0, 7, algorithm);
    repaint();
}

void P1AlgorithmBlock::paint(juce::Graphics& g)
{
    const int textTop = (getHeight() - 50) / 2;
    g.setFont(displayFont(tokens::textAlgorithmLabel));
    g.setColour(colour(tokens::dim));
    g.drawText("algorithm", 0, textTop, 90, 12, juce::Justification::centredLeft,
               false);
    g.setFont(displayFont(tokens::textAlgorithmNumber));
    g.setColour(colour(hover_ ? tokens::pinkBright : tokens::pink));
    g.drawText(juce::String(algorithm_), 0, textTop + 16, 54, 34,
               juce::Justification::centredLeft, false);
    g.setFont(uiFont(tokens::textButton));
    g.setColour(colour(tokens::faint));
    g.drawText(juce::String::charToString(0x25be), 50, textTop + 28, 16, 16,
               juce::Justification::centredLeft, false);
    drawAlgorithmGraph(g, juce::Rectangle<float>(78.0f, 0.0f,
                                                 static_cast<float>(getWidth() - 78),
                                                 static_cast<float>(getHeight())),
                       algorithm_);
}

void P1AlgorithmBlock::mouseUp(const juce::MouseEvent& event)
{
    if (event.getDistanceFromDragStart() < 5 && onClick)
        onClick();
}

void P1AlgorithmBlock::mouseEnter(const juce::MouseEvent&)
{
    hover_ = true;
    repaint();
}

void P1AlgorithmBlock::mouseExit(const juce::MouseEvent&)
{
    hover_ = false;
    repaint();
}

void P1AlgorithmBlock::mouseWheelMove(const juce::MouseEvent&,
                                      const juce::MouseWheelDetails& wheel)
{
    if (onWheel)
        onWheel(wheel.deltaY > 0.0f ? 1 : -1);
}

P1Dialog::P1Dialog(juce::String title, std::unique_ptr<juce::Component> body,
                   int width, int height, juce::StringArray buttons,
                   std::function<void(int)> onButton,
                   std::function<void()> onClose)
    : title_(std::move(title)),
      body_(std::move(body)),
      buttons_(std::move(buttons)),
      onButton_(std::move(onButton)),
      onClose_(std::move(onClose))
{
    setInterceptsMouseClicks(true, true);
    panel_ = juce::Rectangle<int>(width, height);
    if (body_ != nullptr)
        addAndMakeVisible(*body_);
    for (int i = 0; i < buttons_.size(); ++i) {
        auto button = std::make_unique<P1Button>(buttons_[i], false);
        juce::Component::SafePointer<P1Dialog> safe(this);
        button->onClick = [safe, i] {
            juce::MessageManager::callAsync([safe, i] {
                if (safe == nullptr)
                    return;
                auto callback = safe->onButton_;
                if (callback)
                    callback(i);
            });
        };
        addAndMakeVisible(*button);
        buttonComponents_.push_back(std::move(button));
    }
    setWantsKeyboardFocus(true);
    grabKeyboardFocus();
}

void P1Dialog::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black.withAlpha(0.72f));
    g.setColour(colour(tokens::popup));
    g.fillRect(panel_);
    g.setColour(colour(tokens::lineStrong));
    g.drawRect(panel_, 1);
    g.setFont(displayFont(tokens::textDialogTitle));
    g.setColour(colour(tokens::white));
    g.drawText(title_, panel_.reduced(14, 0).removeFromTop(38),
               juce::Justification::centredLeft, false);
    g.setFont(uiFont(tokens::textButton));
    g.setColour(colour(tokens::dim));
    g.drawText("x", closeArea_, juce::Justification::centred, false);
    g.setColour(colour(tokens::lineStrong));
    g.fillRect(panel_.getX() + 12, panel_.getY() + 38,
               panel_.getWidth() - 24, 1);
}

void P1Dialog::resized()
{
    panel_.setCentre(getLocalBounds().getCentre());
    closeArea_ = juce::Rectangle<int>(panel_.getRight() - 34, panel_.getY() + 10, 20, 20);
    auto content = panel_.reduced(14);
    content.removeFromTop(34);

    struct Item {
        int index = 0;
        int width = 0;
    };
    std::vector<std::vector<Item>> rows(1);
    int used = 0;
    for (int i = 0; i < buttons_.size(); ++i) {
        const int width = juce::jmax(
            64, buttonComponents_[static_cast<std::size_t>(i)]->getButtonText().length() * 7 + 20);
        if (!rows.back().empty() && used + 8 + width > content.getWidth()) {
            rows.emplace_back();
            used = 0;
        }
        if (!rows.back().empty())
            used += 8;
        rows.back().push_back(Item{i, width});
        used += width;
    }
    const int footerHeight = 42 + static_cast<int>(rows.size() - 1) * 32;
    auto footer = content.removeFromBottom(footerHeight);
    if (body_ != nullptr)
        body_->setBounds(content);
    int y = footer.getBottom() - 33;
    for (int r = static_cast<int>(rows.size()) - 1; r >= 0; --r) {
        int rowWidth = 0;
        for (std::size_t i = 0; i < rows[static_cast<std::size_t>(r)].size(); ++i)
            rowWidth += rows[static_cast<std::size_t>(r)][i].width + (i == 0 ? 0 : 8);
        int x = footer.getRight() - rowWidth;
        for (const auto& item : rows[static_cast<std::size_t>(r)]) {
            buttonComponents_[static_cast<std::size_t>(item.index)]->setBounds(
                x, y, item.width, 24);
            x += item.width + 8;
        }
        y -= 32;
    }
}

void P1Dialog::mouseUp(const juce::MouseEvent& event)
{
    if (closeArea_.contains(event.getPosition())) {
        juce::Component::SafePointer<P1Dialog> safe(this);
        auto callback = onClose_;
        juce::MessageManager::callAsync([safe, callback] {
            if (safe != nullptr && callback)
                callback();
        });
    }
}

void drawAlgorithmGraph(juce::Graphics& g, juce::Rectangle<float> area, int algorithm)
{
    struct Layout {
        float x[4];
        float y[4];
        int edges[4][2];
        int edgeCount;
        int cars[4];
        int carCount;
    };
    static const Layout layouts[8] = {
        {{60, 60, 60, 60}, {6, 34, 62, 90}, {{0, 1}, {1, 2}, {2, 3}, {0, 0}}, 3, {3, 0, 0, 0}, 1},
        {{18, 92, 55, 55}, {6, 6, 44, 82}, {{0, 2}, {1, 2}, {2, 3}, {0, 0}}, 3, {3, 0, 0, 0}, 1},
        {{16, 96, 96, 56}, {6, 6, 44, 82}, {{1, 2}, {0, 3}, {2, 3}, {0, 0}}, 3, {3, 0, 0, 0}, 1},
        {{80, 22, 138, 80}, {6, 46, 46, 86}, {{0, 1}, {0, 2}, {1, 3}, {2, 3}}, 4, {3, 0, 0, 0}, 1},
        {{32, 32, 108, 108}, {10, 50, 10, 50}, {{0, 1}, {2, 3}, {0, 0}, {0, 0}}, 2, {1, 3, 0, 0}, 2},
        {{80, 10, 80, 150}, {8, 58, 58, 58}, {{0, 1}, {0, 2}, {0, 3}, {0, 0}}, 3, {1, 2, 3, 0}, 3},
        {{24, 24, 90, 156}, {8, 58, 58, 58}, {{0, 1}, {0, 0}, {0, 0}, {0, 0}}, 1, {1, 2, 3, 0}, 3},
        {{16, 66, 116, 166}, {38, 38, 38, 38}, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}, 0, {0, 1, 2, 3}, 4},
    };
    const float opw = 36.0f;
    const float oph = 18.0f;
    const auto& layout = layouts[juce::jlimit(0, 7, algorithm)];
    float minX = 1e9f, maxX = -1e9f, minY = 1e9f, maxY = -1e9f;
    for (int i = 0; i < 4; ++i) {
        const float x = layout.x[i];
        const float y = layout.y[i];
        minX = juce::jmin(minX, x);
        maxX = juce::jmax(maxX, x + opw);
        minY = juce::jmin(minY, y);
        maxY = juce::jmax(maxY, y + oph);
        for (int c = 0; c < layout.carCount; ++c) {
            if (layout.cars[c] == i)
                maxY = juce::jmax(maxY, y + oph + 13.0f);
        }
    }
    const float scale = juce::jmin(area.getWidth() / (maxX - minX),
                                   area.getHeight() / (maxY - minY));
    const float originX = area.getX() + (area.getWidth() - (maxX - minX) * scale) * 0.5f -
                          minX * scale;
    const float originY = area.getY() + (area.getHeight() - (maxY - minY) * scale) * 0.5f -
                          minY * scale;
    auto point = [originX, originY, scale](float x, float y) {
        return juce::Point<float>(originX + x * scale, originY + y * scale);
    };

    for (int e = 0; e < layout.edgeCount; ++e) {
        const int from = layout.edges[e][0];
        const int to = layout.edges[e][1];
        const auto p1 = point(layout.x[from] + opw * 0.5f, layout.y[from] + oph);
        const auto p2 = point(layout.x[to] + opw * 0.5f, layout.y[to]);
        g.setColour(colour(0xff666666));
        g.drawLine({p1, p2}, 1.2f * scale);
        const auto angle = std::atan2(p2.y - p1.y, p2.x - p1.x);
        const auto tip = p2;
        const auto left = tip - juce::Point<float>(std::cos(angle - 0.45f),
                                                   std::sin(angle - 0.45f)) * 5.0f * scale;
        const auto right = tip - juce::Point<float>(std::cos(angle + 0.45f),
                                                    std::sin(angle + 0.45f)) * 5.0f * scale;
        juce::Path arrow;
        arrow.addTriangle(tip, left, right);
        g.fillPath(arrow);
    }

    for (int i = 0; i < 4; ++i) {
        const float x = layout.x[i];
        const float y = layout.y[i];
        bool carrier = false;
        for (int c = 0; c < layout.carCount; ++c)
            carrier = carrier || layout.cars[c] == i;
        const auto rect = juce::Rectangle<float>(originX + x * scale, originY + y * scale,
                                                 opw * scale, oph * scale);
        g.setColour(colour(carrier ? tokens::pink : tokens::white));
        g.fillRect(rect);
        g.setFont(uiFont(12.0f * scale));
        g.setColour(colour(tokens::bg));
        g.drawText("op" + juce::String(i + 1), rect,
                   juce::Justification::centred, false);
        if (carrier) {
            const auto start = point(x + opw * 0.5f, y + oph + 1.0f);
            const auto end = point(x + opw * 0.5f, y + oph + 12.0f);
            g.setColour(colour(tokens::purple));
            g.drawLine({start, end}, 1.6f * scale);
            juce::Path arrow;
            arrow.addTriangle(end, end - juce::Point<float>(3.2f * scale, 4.0f * scale),
                              end + juce::Point<float>(3.2f * scale, -4.0f * scale));
            g.fillPath(arrow);
        }
    }
}

} // namespace p1::ui
