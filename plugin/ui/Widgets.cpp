/* SPDX-License-Identifier: GPL-3.0-only */
#include "Widgets.h"

#include <algorithm>
#include <cmath>

#include "math_tables.h"

namespace p1::ui {
namespace {

KnobLookAndFeel& knobLookAndFeel()
{
    static KnobLookAndFeel lnf;
    return lnf;
}

const char* const waveNames[12] = {
    "sine",      "triangle", "half sine", "half tri", "2x sine", "2x tri",
    "2x half sine", "2x half tri", "rect sine", "pulse", "square", "saw"};

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
    int pct = range > 0.0 ? static_cast<int>((value - spec.min) * 100.0 / range) : 0;
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
        if (v >= 0 && v < 12)
            return waveNames[v];
        return juce::String(v);
    }
    if (display == "shape") {
        static const char* const names[7] = {"off",      "square", "square neg",
                                             "abs",      "abs neg", "signed",
                                             "signed neg"};
        return v >= 0 && v < 7 ? names[v] : juce::String(v);
    }
    if (display == "off_pct")
        return v <= 0 ? "off" : percentage(spec, value, false);
    if (display == "pct_inv")
        return percentage(spec, value, true);
    if (display == "pct")
        return percentage(spec, value, false);
    if (display == "pct_exp") {
        const double range = static_cast<double>(spec.max - spec.min);
        const double x = range > 0.0 ? (value - spec.min) / range : 0.0;
        constexpr double k = 3.0;
        const double den = 1.0 - std::exp(-k);
        const double p = 100.0 * (1.0 - std::exp(-k * x)) / den;
        return juce::String(p, 2) + "%";
    }
    if (display == "pct_bip")
        return signedNumber(v) + "%";
    if (display == "filter") {
        static const char* const names[3] = {"lpf", "bpf", "hpf"};
        return v >= 0 && v < 3 ? names[v] : juce::String(v);
    }
    if (display == "onoff")
        return v > 0 ? "on" : "off";
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
        const double hz = value * 44100.0 / 4294967296.0;
        return juce::String(hz, hz < 1.0 ? 2 : 1) + "Hz";
    }
    if (juce::String(spec.unit) == "cents")
        return juce::String(v) + "c";
    return juce::String(v) + (spec.unit[0] != '\0' ? juce::String(spec.unit) : "");
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
                            v * 4294967296.0 / 44100.0);
    if (display == "pct" || display == "off_pct") {
        const double range = static_cast<double>(spec.max - spec.min);
        return juce::jlimit(static_cast<double>(spec.min),
                            static_cast<double>(spec.max),
                            spec.min + v * range / 100.0);
    }
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
    return juce::jlimit(static_cast<double>(spec.min),
                        static_cast<double>(spec.max), v);
}

ParamKnob::ParamKnob(APVTS& state, juce::String paramID, juce::String label,
                     float dialSize)
    : state_(state), paramID_(std::move(paramID)), dialSize_(dialSize)
{
    spec_ = findSpec(paramID_);
    slider_.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider_.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    slider_.setRotaryParameters(juce::MathConstants<float>::pi * 1.25f,
                                juce::MathConstants<float>::pi * 2.75f, true);
    slider_.setLookAndFeel(&knobLookAndFeel());
    slider_.setMouseDragSensitivity(220);
    slider_.onDetentKey = [this](const juce::KeyPress& key) {
        return stepRatio(key);
    };
    slider_.onValueChange = [this] {
        snapValue();
        updateValueText();
    };
    addAndMakeVisible(slider_);

    name_.setText(label, juce::dontSendNotification);
    name_.setFont(uiFont(tokens::textKnobName));
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
}

void ParamKnob::setParamID(const juce::String& paramID)
{
    paramID_ = paramID;
    spec_ = findSpec(paramID_);
    attach_.reset();
    attach_ = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        state_, paramID_, slider_);
    updateValueText();
    repaint();
}

void ParamKnob::resized()
{
    auto area = getLocalBounds();
    const int dial = static_cast<int>(std::round(dialSize_));
    auto dialArea = area.removeFromTop(dial).withSizeKeepingCentre(
        juce::jmin(dial, getWidth()), dial);
    slider_.setBounds(dialArea);
    name_.setBounds(0, dialArea.getBottom() + 1, getWidth(), 16);
    value_.setBounds(0, dialArea.getBottom() + 15, getWidth(), 20);
}

void ParamKnob::paint(juce::Graphics&) {}

void ParamKnob::updateValueText()
{
    if (spec_ != nullptr)
        value_.setText(formatParamValue(*spec_, slider_.getValue()),
                       juce::dontSendNotification);
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
    g.setFont(displayFont(tokens::textToolbarLabel));
    g.setColour(on ? colour(tokens::green) : colour(tokens::dim));
    const juce::String state = on ? "on" : "off";
    const juce::String caption =
        (label_ == "on" || label_.isEmpty()) ? state : label_ + " " + state;
    g.drawText(caption, juce::Rectangle<int>(32, 0, getWidth() - 32, getHeight()),
               juce::Justification::centredLeft, false);
}

P1Choice::P1Choice(juce::StringArray items, int selected)
{
    addItemList(items, 1);
    setSelectedItemIndex(selected, juce::dontSendNotification);
    setMouseCursor(juce::MouseCursor::PointingHandCursor);
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
    int x = 0;
    for (auto& tab : tabs_) {
        if (tab == nullptr)
            continue;
        const int width = static_cast<int>(
            std::ceil(textWidth(displayFont(tokens::textTab), tab->getButtonText()))) +
            32;
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

P1Envelope::P1Envelope(APVTS& state, juce::StringArray rateIDs,
                       juce::StringArray levelIDs, juce::Colour curveColour,
                       double levelMin, double levelMax, bool zeroLine)
    : state_(state),
      rateIDs_(std::move(rateIDs)),
      levelIDs_(std::move(levelIDs)),
      colour_(curveColour),
      levelMin_(levelMin),
      levelMax_(levelMax),
      zeroLine_(zeroLine)
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
    std::array<int32_t, 4> rates{};
    std::array<double, 4> levels{};
    for (int i = 0; i < 4; ++i) {
        if (auto* v = state_.getRawParameterValue(rateIDs_[i]))
            rates[static_cast<size_t>(i)] =
                static_cast<int32_t>(std::lround(v->load()));
    }
    for (int i = 0; i < 4 && i < levelIDs_.size(); ++i) {
        if (auto* v = state_.getRawParameterValue(levelIDs_[i]))
            levels[static_cast<size_t>(i)] = static_cast<double>(v->load());
    }

    /* The panel graph shows the log2 of the one-pole time constant, so a faster
     * rate is a shorter stage, and the sustain is a short flat hold. */
    const auto stageLen = [](int32_t rate) -> uint32_t {
        const int32_t r = juce::jlimit(0, 99, rate);
        int32_t c = p1_rate_coeff[r];
        uint32_t lg = 0;
        while (c > 1) {
            c >>= 1;
            ++lg;
        }
        return lg < 16u ? 16u - lg : 1u;
    };
    const std::array<uint32_t, 5> len = {
        stageLen(rates[0]), stageLen(rates[1]), stageLen(rates[2]), 8u,
        stageLen(rates[3])};
    const uint32_t total = len[0] + len[1] + len[2] + len[3] + len[4];

    auto levelY = [this, area, mid](double level) {
        if (zeroLine_) {
            return static_cast<float>(mid - level * (area.getHeight() * 0.5 - 4.0) /
                                                32768.0);
        }
        const double range = levelMax_ - levelMin_;
        const double norm = range > 0.0 ? (level - levelMin_) / range : 0.0;
        return static_cast<float>(area.getBottom() - 4.0 -
                                  norm * (area.getHeight() - 8.0));
    };

    const float width = area.getWidth() - 8.0f;
    const float right = area.getRight() - 4.0f;
    float x = area.getX() + 4.0f;
    const auto advance = [&x, width, right, total](uint32_t stage) {
        x += width * static_cast<float>(stage) / static_cast<float>(total);
        if (x > right)
            x = right;
        return x;
    };
    juce::Path path;
    path.startNewSubPath(x, levelY(0.0));
    path.lineTo(advance(len[0]), levelY(levels[0]));
    path.lineTo(advance(len[1]), levelY(levels[1]));
    path.lineTo(advance(len[2]), levelY(levels[2]));
    path.lineTo(advance(len[3]), levelY(levels[2]));
    path.lineTo(advance(len[4]), levelY(levels[3]));
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

P1LevelSlider::P1LevelSlider(APVTS& state, const juce::String& paramID)
    : state_(state), paramID_(paramID)
{
    spec_ = findSpec(paramID_);
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

void P1LevelSlider::resized()
{
    track_ = getLocalBounds().withTrimmedTop(20).reduced(0, 1);
}

void P1LevelSlider::paint(juce::Graphics& g)
{
    g.setFont(uiFont(tokens::textInlineLabel));
    g.setColour(colour(tokens::dim));
    g.drawText("level", getLocalBounds().removeFromTop(18),
               juce::Justification::centredLeft, false);
    if (spec_ != nullptr) {
        g.setColour(colour(tokens::pinkBright));
        g.drawText(formatParamValue(*spec_, value_),
                   getLocalBounds().removeFromTop(18),
                   juce::Justification::centredRight, false);
    }
    g.setColour(colour(tokens::lineStrong));
    g.drawRect(track_, 1);
    const float range = spec_ != nullptr ? static_cast<float>(spec_->max - spec_->min)
                                         : 1.0f;
    const float norm = range > 0.0f
                           ? juce::jlimit(0.0f, 1.0f,
                                         (value_ - static_cast<float>(spec_->min)) / range)
                           : 0.0f;
    g.setColour(colour(tokens::purple));
    g.fillRect(track_.reduced(1).withWidth(static_cast<int>(
        std::round(static_cast<float>(track_.getWidth() - 2) * norm))));
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

P1Card::P1Card(juce::String title, bool panelBackground)
    : title_(std::move(title)), panel_(panelBackground)
{
    setInterceptsMouseClicks(false, true);
}

void P1Card::paint(juce::Graphics& g)
{
    if (panel_)
        g.fillAll(colour(tokens::panel));
    drawSectionTitle(g, getLocalBounds().removeFromTop(20).reduced(8, 0), title_);
}

juce::Rectangle<int> P1Card::contentArea() const
{
    return getLocalBounds().withTrimmedTop(22).reduced(8, 0);
}

P1AlgorithmGraph::P1AlgorithmGraph(int algorithm) : algorithm_(algorithm)
{
    setInterceptsMouseClicks(false, false);
}

void P1AlgorithmGraph::setAlgorithm(int algorithm)
{
    algorithm_ = juce::jlimit(0, 7, algorithm);
    repaint();
}

void P1AlgorithmGraph::paint(juce::Graphics& g)
{
    drawAlgorithmGraph(g, getLocalBounds().toFloat(), algorithm_);
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
    g.setFont(displayFont(tokens::textAlgorithmLabel));
    g.setColour(colour(tokens::dim));
    g.drawText("algorithm", 0, 0, 90, 12, juce::Justification::centredLeft, false);
    g.setFont(displayFont(tokens::textAlgorithmNumber));
    g.setColour(colour(hover_ ? tokens::pinkBright : tokens::pink));
    g.drawText(juce::String(algorithm_), 0, 16, 54, 34,
               juce::Justification::centredLeft, false);
    g.setFont(uiFont(tokens::textButton));
    g.setColour(colour(tokens::faint));
    g.drawText(juce::String::charToString(0x25be), 50, 28, 16, 16,
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
    auto footer = content.removeFromBottom(42);
    if (body_ != nullptr)
        body_->setBounds(content);
    int x = footer.getRight();
    for (auto it = buttonComponents_.rbegin(); it != buttonComponents_.rend(); ++it) {
        const int width =
            juce::jmax(64, (*it)->getButtonText().length() * 8 + 24);
        x -= width;
        (*it)->setBounds(x, footer.getCentreY() - 12, width, 24);
        x -= 8;
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
