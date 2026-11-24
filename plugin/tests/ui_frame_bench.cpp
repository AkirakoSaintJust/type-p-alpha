/* SPDX-License-Identifier: GPL-3.0-only */
/* The live display widgets at their real size. The header updates the scope and
 * the spectrogram at 30 Hz; this benchmark paints each one into an image and
 * reports the cost of a frame. It is a console app, not a ctest, because the
 * absolute time depends on the host. */
#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <typeinfo>

#include "Widgets.h"
#include "PluginEditor.h"
#include "PluginProcessor.h"

namespace {

constexpr int kWidth = 330;
constexpr int kHeight = 50;
constexpr int kFrames = 2000;

std::array<float, 512> makeSignal()
{
    std::array<float, 512> samples{};
    for (int i = 0; i < static_cast<int>(samples.size()); ++i) {
        const double phase = 2.0 * 3.14159265358979323846 * 440.0 *
                             static_cast<double>(i) / 48000.0;
        samples[static_cast<std::size_t>(i)] =
            static_cast<float>(0.55 * std::sin(phase) +
                               0.20 * std::sin(2.0 * phase) +
                               0.08 * std::sin(3.0 * phase));
    }
    return samples;
}

template <typename Fn>
double measure(Fn&& fn)
{
    /* Warm the caches and the FFT plan. */
    for (int i = 0; i < 100; ++i)
        fn();
    const auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < kFrames; ++i)
        fn();
    const auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double>(end - start).count() * 1.0e6 /
           static_cast<double>(kFrames);
}

double measureEditor(int page)
{
    TypePAlphaAudioProcessor processor;
    processor.setUiPage(page);
    std::unique_ptr<juce::AudioProcessorEditor> editor(processor.createEditor());
    if (editor == nullptr)
        return -1.0;
    editor->setSize(1240, 985);
    editor->setVisible(true);
    juce::Image image(juce::Image::ARGB, 1240, 985, true);
    juce::Graphics graphics(image);
    auto paint = [&] { editor->paintEntireComponent(graphics, true); };
    for (int i = 0; i < 5; ++i)
        paint();
    const auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < 100; ++i)
        paint();
    const auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double>(end - start).count() * 1.0e6 / 100.0;
}

double measureComponent(juce::Component& component, int frames)
{
    const int w = juce::jmax(1, component.getWidth());
    const int h = juce::jmax(1, component.getHeight());
    juce::Image image(juce::Image::ARGB, w, h, true);
    juce::Graphics graphics(image);
    for (int i = 0; i < 3; ++i)
        component.paintEntireComponent(graphics, true);
    const auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < frames; ++i)
        component.paintEntireComponent(graphics, true);
    const auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double>(end - start).count() * 1.0e6 /
           static_cast<double>(frames);
}

void reportTree(juce::Component& root, int depth)
{
    if (depth > 7)
        return;
    int index = 0;
    for (auto* child : root.getChildren()) {
        if (child == nullptr || !child->isVisible())
            continue;
        const double us = measureComponent(*child, 20);
        if (us >= 200.0) {
            std::printf("  tree %2d.%d %4d x %4d  %7.1f us  children %d  %s\n",
                        depth, index, child->getWidth(), child->getHeight(), us,
                        child->getNumChildComponents(), typeid(*child).name());
            reportTree(*child, depth + 1);
        }
        ++index;
    }
}

void reportEditorTree(int page)
{
    TypePAlphaAudioProcessor processor;
    processor.setUiPage(page);
    std::unique_ptr<juce::AudioProcessorEditor> editor(processor.createEditor());
    if (editor == nullptr)
        return;
    editor->setSize(1240, 985);
    editor->setVisible(true);
    std::printf("ui tree page %d\n", page);
    reportTree(*editor, 0);
}

} // namespace

int main()
{
    juce::ScopedJuceInitialiser_GUI init;
    const auto samples = makeSignal();

    juce::Image scopeImage(juce::Image::ARGB, kWidth, kHeight, true);
    juce::Image spectroImage(juce::Image::ARGB, kWidth, kHeight, true);
    juce::Graphics scopeGraphics(scopeImage);
    juce::Graphics spectroGraphics(spectroImage);

    p1::ui::P1Scope scope;
    scope.setBounds(0, 0, kWidth, kHeight);
    scope.setSamples(samples.data(), static_cast<int>(samples.size()));

    p1::ui::P1Spectro spectro;
    spectro.setBounds(0, 0, kWidth, kHeight);
    spectro.setSamples(samples.data(), static_cast<int>(samples.size()));

    const double scopeUs = measure([&] { scope.paint(scopeGraphics); });
    const double spectroUs = measure([&] { spectro.paint(spectroGraphics); });
    const double updateUs = measure([&] {
        scope.setSamples(samples.data(), static_cast<int>(samples.size()));
        spectro.setSamples(samples.data(), static_cast<int>(samples.size()));
    });
    const double combinedUs = measure([&] {
        scope.setSamples(samples.data(), static_cast<int>(samples.size()));
        spectro.setSamples(samples.data(), static_cast<int>(samples.size()));
        scope.paint(scopeGraphics);
        spectro.paint(spectroGraphics);
    });

    const double editorPhaseUs = measureEditor(5);
    const double editorSpectraUs = measureEditor(6);
    std::printf(
        "ui frame: scope %.1f us, spectro %.1f us, update %.1f us, "
        "combined %.1f us, editor phase %.1f us, editor spectra %.1f us\n",
        scopeUs, spectroUs, updateUs, combinedUs, editorPhaseUs,
        editorSpectraUs);
    reportEditorTree(5);
    return 0;
}
