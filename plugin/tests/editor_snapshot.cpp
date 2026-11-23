/* SPDX-License-Identifier: GPL-3.0-only */
/* Milestone 3: render the designed editor to PNG for visual review. */
#include <cmath>
#include <cstdio>
#include <memory>
#include <vector>

#include <juce_gui_basics/juce_gui_basics.h>

#include "ParamIDs.h"
#include "PluginEditor.h"
#include "PluginProcessor.h"

namespace {

bool saveSnapshot(juce::AudioProcessorEditor& editor, const juce::File& file)
{
    const auto image =
        editor.createComponentSnapshot(editor.getLocalBounds(), true, 1.0f);
    if (!image.isValid())
        return false;
    file.deleteFile();
    juce::FileOutputStream output(file);
    if (!output.openedOk())
        return false;
    juce::PNGImageFormat png;
    return png.writeImageToStream(image, output);
}

} // namespace

int main(int argc, char** argv)
{
    juce::ScopedJuceInitialiser_GUI init;
    const juce::File directory =
        argc > 1 ? juce::File(juce::String(argv[1]))
                 : juce::File::getCurrentWorkingDirectory().getChildFile("snapshots");
    directory.createDirectory();

    TypePAlphaAudioProcessor processor;
    const auto library =
        juce::File::getSpecialLocation(juce::File::tempDirectory)
            .getNonexistentChildFile("p1-editor-snapshot", ".p1lib", false);
    processor.setLibraryFileForTesting(library);
    processor.setRateAndBufferSizeDetails(48000.0, 256);
    processor.prepareToPlay(48000.0, 256);
    {
        auto& store = processor.presets();
        const std::vector<int32_t> params(
            static_cast<std::size_t>(kP1PartParamCount), 0);
        store.savePart(0, "init part", params);
        store.savePart(1, "bass", params);
        store.saveMulti(0, "init multi", std::vector<uint8_t>{0, 1},
                        std::vector<int32_t>{99, 80}, std::vector<int32_t>{0, 20},
                        std::vector<int32_t>{0, 1}, std::vector<int32_t>{0, 0},
                        std::vector<int32_t>{127, 127});
        store.setCurrentMulti(0);
    }
    std::unique_ptr<juce::AudioProcessorEditor> editor(processor.createEditor());
    if (editor == nullptr) {
        std::printf("editor snapshot: createEditor failed\n");
        return 1;
    }
    auto* p1editor = dynamic_cast<PluginEditor*>(editor.get());
    if (p1editor == nullptr) {
        std::printf("editor snapshot: wrong editor type\n");
        return 1;
    }
    editor->setSize(1240, 985);
    editor->setVisible(true);

    juce::AudioBuffer<float> buffer(2, 256);
    juce::MidiBuffer midi;
    midi.addEvent(juce::MidiMessage::noteOn(1, 60, static_cast<juce::uint8>(100)), 0);
    auto fill = [&] {
        for (int i = 0; i < 8; ++i) {
            buffer.clear();
            processor.processBlock(buffer, midi);
        }
        p1editor->updateVisualizers();
    };

    fill();
    bool ok = saveSnapshot(*editor, directory.getChildFile("editor-patch.png"));
    p1editor->showTab(1);
    fill();
    ok = saveSnapshot(*editor, directory.getChildFile("editor-fxmod.png")) && ok;
    /* Bipolar loop1 on the pitch and pan cards draws the turn boundaries and
     * the left arrow against a non-zero level. */
    {
        const std::array<std::pair<const char*, float>, 10> loop{{
            {"penv_l1.p0", 8192.0f}, {"penv_l2.p0", 4096.0f},
            {"penv_l3.p0", 8192.0f}, {"penv_rell.p0", 4096.0f},
            {"penv_mode.p0", 2.0f},
            {"pan_env_l1.p0", 60.0f}, {"pan_env_l2.p0", -60.0f},
            {"pan_env_l3.p0", 60.0f}, {"pan_env_rell.p0", 0.0f},
            {"pan_env_mode.p0", 2.0f}}};
        for (const auto& [id, value] : loop)
            if (auto* parameter = p1editor->state().getParameter(id))
                parameter->setValueNotifyingHost(
                    parameter->convertTo0to1(value));
        fill();
        ok = saveSnapshot(*editor,
                          directory.getChildFile("editor-fxmod-loops.png")) &&
             ok;
        const std::array<std::pair<const char*, float>, 10> restore{{
            {"penv_l1.p0", 5461.0f}, {"penv_l2.p0", 2731.0f},
            {"penv_l3.p0", 0.0f}, {"penv_rell.p0", 0.0f},
            {"penv_mode.p0", 1.0f},
            {"pan_env_l1.p0", 0.0f}, {"pan_env_l2.p0", 0.0f},
            {"pan_env_l3.p0", 0.0f}, {"pan_env_rell.p0", 0.0f},
            {"pan_env_mode.p0", 0.0f}}};
        for (const auto& [id, value] : restore)
            if (auto* parameter = p1editor->state().getParameter(id))
                parameter->setValueNotifyingHost(
                    parameter->convertTo0to1(value));
        fill();
    }
    p1editor->showTab(2);
    fill();
    ok = saveSnapshot(*editor,
                      directory.getChildFile("editor-operator-envs.png")) && ok;
    /* A loop mode draws the repeat boundaries and the left arrow. */
    {
        const std::array<std::pair<const char*, float>, 6> loop{{
            {"op1.eg.mode.p0", 2.0f}, {"op2.eg.mode.p0", 3.0f},
            {"op1_fbenv_mode.p0", 2.0f}, {"op1_fbenv_l1.p0", 8000.0f},
            {"op1_fbenv_l2.p0", 4000.0f}, {"op1_fbenv_l3.p0", 8000.0f}}};
        for (const auto& [id, value] : loop)
            if (auto* parameter = p1editor->state().getParameter(id))
                parameter->setValueNotifyingHost(
                    parameter->convertTo0to1(value));
        fill();
        ok = saveSnapshot(*editor,
                          directory.getChildFile("editor-envelope-loops.png")) &&
             ok;
        const std::array<std::pair<const char*, float>, 6> restore{{
            {"op1.eg.mode.p0", 1.0f}, {"op2.eg.mode.p0", 1.0f},
            {"op1_fbenv_mode.p0", 1.0f}, {"op1_fbenv_l1.p0", 0.0f},
            {"op1_fbenv_l2.p0", 0.0f}, {"op1_fbenv_l3.p0", 0.0f}}};
        for (const auto& [id, value] : restore)
            if (auto* parameter = p1editor->state().getParameter(id))
                parameter->setValueNotifyingHost(
                    parameter->convertTo0to1(value));
        fill();
    }
    p1editor->showTab(3);
    fill();
    ok = saveSnapshot(*editor,
                      directory.getChildFile("editor-operator-lfos.png")) && ok;
    /* Different fade times on the first two operators show the shared scale,
     * the separation and the two time readouts. */
    {
        const std::array<std::pair<const char*, float>, 4> fades{{
            {"op1_trem_rate_fade.p0", 50.0f},
            {"op1_trem_depth_fade.p0", 70.0f},
            {"op2_trem_rate_fade.p0", 90.0f},
            {"op2_trem_depth_fade.p0", 90.0f}}};
        for (const auto& [id, value] : fades)
            if (auto* parameter = p1editor->state().getParameter(id))
                parameter->setValueNotifyingHost(
                    parameter->convertTo0to1(value));
        fill();
        ok = saveSnapshot(
                 *editor,
                 directory.getChildFile("editor-operator-lfos-fades.png")) &&
             ok;
        for (const auto& [id, value] : fades)
            if (auto* parameter = p1editor->state().getParameter(id))
                parameter->setValueNotifyingHost(
                    parameter->convertTo0to1(0.0f));
        fill();
    }
    p1editor->showTab(4);
    fill();
    ok = saveSnapshot(*editor,
                      directory.getChildFile("editor-op-phasemap.png")) && ok;
    {
        const std::array<std::pair<const char*, float>, 26> demo{{
            {"op1_phase_on.p0", 1.0f},
            {"op1_phase_bp.p0", 620.0f},
            {"op1_phase_segments.p0", 8.0f},
            {"op1_phase_flip.p0", 25.0f},
            {"op1_phase_flip_rate.p0", 178956.0f},
            {"op1_phase_reorder.p0", 30.0f},
            {"op1_phase_reorder_rate.p0", 134218.0f},
            {"op1_phase_jump.p0", 2.0f},
            {"op1_phase_drift.p0", 35.0f},
            {"op1_phase_drift_rate.p0", 107374.0f},
            {"op1_phase_lfo_rate.p0", 44739.0f},
            {"op1_phase_lfo_depth.p0", 22.0f},
            {"op1_phase_lfo_wave.p0", 0.0f},
            {"op1_phase_lfo_mode.p0", 1.0f},
            {"op1_phase_lfo_rate_fade.p0", 20.0f},
            {"op1_phase_lfo_depth_fade.p0", 40.0f},
            {"op1_phase_env_on.p0", 1.0f},
            {"op1_phase_env_r1.p0", 80.0f},
            {"op1_phase_env_r2.p0", 60.0f},
            {"op1_phase_env_r3.p0", 50.0f},
            {"op1_phase_env_l1.p0", 12.0f},
            {"op1_phase_env_l2.p0", 28.0f},
            {"op1_phase_env_l3.p0", 0.0f},
            {"op1_phase_env_relr.p0", 80.0f},
            {"op1_phase_env_rell.p0", 0.0f},
            {"op1_phase_env_mode.p0", 1.0f}}};
        auto set = [&](const char* id, float value) {
            if (auto* parameter = p1editor->state().getParameter(id))
                parameter->setValueNotifyingHost(
                    parameter->convertTo0to1(value));
        };
        for (const auto& [id, value] : demo)
            set(id, value);
        fill();
        ok = saveSnapshot(
                 *editor,
                 directory.getChildFile("editor-op-phasemap-live.png")) &&
             ok;
        const std::array<std::pair<const char*, float>, 26> neutral{{
            {"op1_phase_on.p0", 0.0f},
            {"op1_phase_bp.p0", 500.0f},
            {"op1_phase_segments.p0", 8.0f},
            {"op1_phase_flip.p0", 0.0f},
            {"op1_phase_flip_rate.p0", 178956.0f},
            {"op1_phase_reorder.p0", 0.0f},
            {"op1_phase_reorder_rate.p0", 0.0f},
            {"op1_phase_jump.p0", 1.0f},
            {"op1_phase_drift.p0", 0.0f},
            {"op1_phase_drift_rate.p0", 107374.0f},
            {"op1_phase_lfo_rate.p0", 44739.0f},
            {"op1_phase_lfo_depth.p0", 0.0f},
            {"op1_phase_lfo_wave.p0", 0.0f},
            {"op1_phase_lfo_mode.p0", 1.0f},
            {"op1_phase_lfo_rate_fade.p0", 0.0f},
            {"op1_phase_lfo_depth_fade.p0", 0.0f},
            {"op1_phase_env_on.p0", 1.0f},
            {"op1_phase_env_r1.p0", 80.0f},
            {"op1_phase_env_r2.p0", 60.0f},
            {"op1_phase_env_r3.p0", 50.0f},
            {"op1_phase_env_l1.p0", 0.0f},
            {"op1_phase_env_l2.p0", 0.0f},
            {"op1_phase_env_l3.p0", 0.0f},
            {"op1_phase_env_relr.p0", 80.0f},
            {"op1_phase_env_rell.p0", 0.0f},
            {"op1_phase_env_mode.p0", 1.0f}}};
        for (const auto& [id, value] : neutral)
            set(id, value);
        fill();
    }
    p1editor->showTab(5);
    fill();
    ok = saveSnapshot(*editor,
                      directory.getChildFile("editor-operator-spectra.png")) && ok;
    {
        const std::array<std::pair<const char*, float>, 8> levels{{
            {"op1_spec_a1.p0", 99.0f}, {"op1_spec_a2.p0", 62.0f},
            {"op1_spec_a3.p0", 45.0f}, {"op1_spec_a4.p0", 30.0f},
            {"op1_spec_a5.p0", 22.0f}, {"op1_spec_a6.p0", 14.0f},
            {"op1_spec_a7.p0", 9.0f}, {"op1_spec_a8.p0", 5.0f}}};
        for (const auto& [id, value] : levels) {
            if (auto* parameter = p1editor->state().getParameter(id))
                parameter->setValueNotifyingHost(
                    parameter->convertTo0to1(value));
        }
        p1editor->updateVisualizers();
        ok = saveSnapshot(
                 *editor,
                 directory.getChildFile("editor-operator-spectra-levels.png")) &&
             ok;
        p1editor->showTab(6);
        p1editor->updateVisualizers();
        ok = saveSnapshot(
                 *editor,
                 directory.getChildFile("editor-operator-spectra-b.png")) &&
             ok;
        p1editor->showTab(7);
        p1editor->updateVisualizers();
        ok = saveSnapshot(
                 *editor,
                 directory.getChildFile("editor-op-spectra-mod-levels.png")) &&
             ok;
        p1editor->showTab(5);
        if (auto* parameter =
                p1editor->state().getParameter("op1_spec_a1.p0")) {
            parameter->setValueNotifyingHost(parameter->convertTo0to1(99.0f));
        }
        for (int h = 2; h <= 8; ++h) {
            if (auto* parameter = p1editor->state().getParameter(
                    "op1_spec_a" + juce::String(h) + ".p0")) {
                parameter->setValueNotifyingHost(
                    parameter->convertTo0to1(0.0f));
            }
        }
        p1editor->updateVisualizers();
    }
    p1editor->showTab(7);
    fill();
    ok = saveSnapshot(*editor,
                      directory.getChildFile("editor-op-spectra-mod.png")) && ok;
    if (auto* rangeB =
            p1editor->state().getParameter("op1_spec_range_b.p0")) {
        rangeB->setValueNotifyingHost(rangeB->convertTo0to1(1.0f));
        p1editor->updateVisualizers();
        ok = saveSnapshot(
                 *editor,
                 directory.getChildFile("editor-op-spectra-mod-ranges.png")) &&
             ok;
        rangeB->setValueNotifyingHost(rangeB->convertTo0to1(0.0f));
        p1editor->updateVisualizers();
    }
    p1editor->showTab(0);
    fill();
    p1editor->showAlgorithmPicker();
    ok = saveSnapshot(*editor, directory.getChildFile("editor-algorithm.png")) && ok;
    p1editor->closeDialog();
    p1editor->showRenameDialog(false, 0);
    ok = saveSnapshot(*editor, directory.getChildFile("editor-rename.png")) && ok;
    p1editor->closeDialog();
    p1editor->showPatchBrowser();
    ok = saveSnapshot(*editor, directory.getChildFile("editor-patches.png")) && ok;
    p1editor->closeDialog();

    {
        auto& params = processor.parameters();
        auto setRaw = [&params](const char* id, float value) {
            if (auto* parameter = params.getParameter(id))
                parameter->setValueNotifyingHost(
                    parameter->convertTo0to1(value));
        };
        auto readRaw = [&params](const char* id) {
            const auto* value = params.getRawParameterValue(id);
            return value == nullptr ? 0.0f : value->load();
        };
        const float srcAmp = readRaw("op1.eg.d1l.p0");
        const float dstAmp = readRaw("op2.eg.d1l.p0");
        setRaw("op1.eg.d1l.p0", 42.0f);
        p1editor->copyOperatorSection(
            0, 0, PluginEditor::OperatorSection::amplitudeEnvelope);
        p1editor->pasteOperatorSection(
            1, 0, PluginEditor::OperatorSection::amplitudeEnvelope);
        if (std::lround(readRaw("op2.eg.d1l.p0")) != 42) {
            std::printf("editor snapshot: amp envelope section paste failed\n");
            ok = false;
        }
        setRaw("op1.eg.d1l.p0", srcAmp);
        setRaw("op2.eg.d1l.p0", dstAmp);

        const float srcLfo = readRaw("op1_dlfo_depth.p0");
        const float dstLfo = readRaw("op2_dlfo_depth.p0");
        setRaw("op1_dlfo_depth.p0", 1234.0f);
        p1editor->copyOperatorSection(
            0, 0, PluginEditor::OperatorSection::detuneLfo);
        p1editor->pasteOperatorSection(
            1, 0, PluginEditor::OperatorSection::detuneLfo);
        if (std::lround(readRaw("op2_dlfo_depth.p0")) != 1234) {
            std::printf("editor snapshot: detune lfo section paste failed\n");
            ok = false;
        }
        setRaw("op1_dlfo_depth.p0", srcLfo);
        setRaw("op2_dlfo_depth.p0", dstLfo);

        const float srcOsc = readRaw("op1_level.p0");
        const float dstOsc = readRaw("op2_level.p0");
        setRaw("op1_level.p0", 42.0f);
        p1editor->copyOperatorSection(
            0, 0, PluginEditor::OperatorSection::oscillator);
        p1editor->pasteOperatorSection(
            1, 0, PluginEditor::OperatorSection::oscillator);
        if (std::lround(readRaw("op2_level.p0")) != 42) {
            std::printf("editor snapshot: oscillator section paste failed\n");
            ok = false;
        }
        setRaw("op1_level.p0", srcOsc);
        setRaw("op2_level.p0", dstOsc);

        const float srcSpec = readRaw("op1_spec_a1.p0");
        const float dstSpec = readRaw("op2_spec_a1.p0");
        const float srcRange = readRaw("op1_spec_range_a.p0");
        const float dstRange = readRaw("op2_spec_range_a.p0");
        const float srcWave = readRaw("op1_waveform.p0");
        const float dstWave = readRaw("op2_waveform.p0");
        setRaw("op1_spec_a1.p0", 77.0f);
        setRaw("op1_spec_range_a.p0", 1.0f);
        setRaw("op1_waveform.p0", 0.0f);
        setRaw("op2_waveform.p0", 0.0f);
        p1editor->copyOperatorSection(
            0, 0, PluginEditor::OperatorSection::spectrumA);
        p1editor->pasteOperatorSection(
            1, 0, PluginEditor::OperatorSection::spectrumA);
        if (std::lround(readRaw("op2_spec_a1.p0")) != 77 ||
            std::lround(readRaw("op2_spec_range_a.p0")) != 1 ||
            std::lround(readRaw("op2_waveform.p0")) != 13) {
            std::printf("editor snapshot: spectrum paste did not activate the mode\n");
            ok = false;
        }
        setRaw("op1_spec_a1.p0", srcSpec);
        setRaw("op2_spec_a1.p0", dstSpec);
        setRaw("op1_spec_range_a.p0", srcRange);
        setRaw("op2_spec_range_a.p0", dstRange);
        setRaw("op1_waveform.p0", srcWave);
        setRaw("op2_waveform.p0", dstWave);

        const float srcCut = readRaw("cutoff.p0");
        const float dstCut = readRaw("cutoff.p1");
        setRaw("cutoff.p0", 12345.0f);
        p1editor->copyPartFxSection(PluginEditor::PartFxSection::filter);
        if (std::lround(readRaw("cutoff.p1")) != 12345) {
            std::printf("editor snapshot: fx filter copy failed\n");
            ok = false;
        }
        setRaw("cutoff.p0", srcCut);
        setRaw("cutoff.p1", dstCut);

        const float srcFenv = readRaw("fenv_r1.p0");
        const float dstFenv = readRaw("fenv_r1.p1");
        setRaw("fenv_r1.p0", 42.0f);
        p1editor->copyPartFxSection(PluginEditor::PartFxSection::filterEnvelope);
        if (std::lround(readRaw("fenv_r1.p1")) != 42) {
            std::printf("editor snapshot: fx filter envelope copy failed\n");
            ok = false;
        }
        setRaw("fenv_r1.p0", srcFenv);
        setRaw("fenv_r1.p1", dstFenv);

        const float srcPitch = readRaw("pitch_rate.p0");
        const float dstPitch = readRaw("pitch_rate.p1");
        setRaw("pitch_rate.p0", 44739.0f);
        p1editor->copyPartFxSection(PluginEditor::PartFxSection::pitchLfo);
        if (std::lround(readRaw("pitch_rate.p1")) != 44739) {
            std::printf("editor snapshot: fx pitch lfo copy failed\n");
            ok = false;
        }
        setRaw("pitch_rate.p0", srcPitch);
        setRaw("pitch_rate.p1", dstPitch);

        const float srcKs = readRaw("op1_level_ks_lo.p0");
        const float dstKs = readRaw("op1_level_ks_lo.p1");
        setRaw("op1_level_ks_lo.p0", 37.0f);
        p1editor->copyPartFxSection(PluginEditor::PartFxSection::keyFollow);
        if (std::lround(readRaw("op1_level_ks_lo.p1")) != 37) {
            std::printf("editor snapshot: fx key follow copy failed\n");
            ok = false;
        }
        setRaw("op1_level_ks_lo.p0", srcKs);
        setRaw("op1_level_ks_lo.p1", dstKs);
    }

    p1editor->showSinglePartsBrowser();
    ok = saveSnapshot(*editor,
                      directory.getChildFile("editor-single-parts.png")) && ok;
    p1editor->closeDialog();
    p1editor->showConfirm("overwrite patch slot 01?", [] {});
    ok = saveSnapshot(*editor, directory.getChildFile("editor-confirm.png")) && ok;
    p1editor->closeDialog();
    p1editor->showPatchSaveMode(0);
    ok = saveSnapshot(*editor, directory.getChildFile("editor-save-mode.png")) && ok;
    p1editor->closeDialog();

    p1editor->applyAlgorithm(7);
    const auto* algorithm0 = processor.parameters().getRawParameterValue("algorithm.p0");
    if (algorithm0 == nullptr || std::lround(algorithm0->load()) != 7) {
        std::printf("editor snapshot: part 0 algorithm was not applied\n");
        ok = false;
    }
    p1editor->setActivePart(1);
    p1editor->applyAlgorithm(6);
    const auto* algorithm1 = processor.parameters().getRawParameterValue("algorithm.p1");
    if (algorithm1 == nullptr || std::lround(algorithm1->load()) != 6) {
        std::printf("editor snapshot: part 1 algorithm was not applied\n");
        ok = false;
    }
    fill();
    ok = saveSnapshot(*editor, directory.getChildFile("editor-part2.png")) && ok;

    p1editor->togglePartMute(0, true);
    const auto* level0 = processor.parameters().getRawParameterValue("level.p0");
    if (level0 == nullptr || level0->load() > 0.5f) {
        std::printf("editor snapshot: part mute did not set level 0\n");
        ok = false;
    }
    p1editor->togglePartMute(0, false);
    if (level0 == nullptr || level0->load() < 98.5f) {
        std::printf("editor snapshot: part unmute did not restore level\n");
        ok = false;
    }

    p1editor->setActivePart(0);
    processor.setPartName(0, "12345678901234567890123456789012");
    processor.setPatchName("12345678901234567890123456789012");
    p1editor->refreshNames();
    fill();
    ok = saveSnapshot(*editor,
                      directory.getChildFile("editor-long-name.png")) && ok;

    editor->setSize(620, 493);
    fill();
    ok = saveSnapshot(*editor, directory.getChildFile("editor-half.png")) && ok;

    p1editor->setActivePart(0);
    auto setRaw = [&processor](const juce::String& id, int value) {
        if (auto* parameter = processor.parameters().getParameter(id))
            parameter->setValueNotifyingHost(
                parameter->convertTo0to1(static_cast<float>(value)));
    };
    auto raw = [&processor](const juce::String& id) {
        if (auto* value = processor.parameters().getRawParameterValue(id))
            return static_cast<int>(std::lround(value->load()));
        return -999;
    };
    processor.setPartName(0, "bank test part");
    processor.setPatchName("bank test");
    setRaw("op1_ratio.p0", 131072);
    p1editor->savePartToSlot(0, 10);
    const auto& savedParams = processor.presets().partParams(10);
    if (savedParams.size() != static_cast<std::size_t>(kP1PartParamCount) ||
        savedParams[0] != 131072) {
        std::printf("editor snapshot: part slot did not store the parameter\n");
        ok = false;
    }
    setRaw("op1_ratio.p0", 65536);
    p1editor->loadPartFromSlot(0, 10);
    if (raw("op1_ratio.p0") != 131072) {
        std::printf("editor snapshot: part slot did not restore the parameter\n");
        ok = false;
    }
    p1editor->savePatchToSlot(5);
    p1::PresetStore::Multi multi;
    if (!processor.presets().loadMulti(5, multi) ||
        multi.partSlot[0] != 10 || multi.name != "bank test") {
        std::printf("editor snapshot: patch slot did not store the multi\n");
        ok = false;
    }
    setRaw("op1_ratio.p0", 32768);
    processor.setPatchName("changed");
    p1editor->loadPatchFromSlot(5);
    if (raw("op1_ratio.p0") != 131072 ||
        processor.getPatchName() != "bank test") {
        std::printf("editor snapshot: patch slot did not restore the multi\n");
        ok = false;
    }

    setRaw("op1_ratio.p0", 131072);
    setRaw("op1_ratio.p1", 196608);
    processor.setPatchName("both parts new");
    p1editor->savePatchToSlot(6, 1);
    p1::PresetStore::Multi newParts;
    if (!processor.presets().loadMulti(6, newParts) ||
        newParts.partSlot[0] == 0xFF || newParts.partSlot[1] == 0xFF ||
        !processor.presets().partUsed(newParts.partSlot[0]) ||
        !processor.presets().partUsed(newParts.partSlot[1]) ||
        processor.presets().partParams(newParts.partSlot[0])[0] != 131072 ||
        processor.presets().partParams(newParts.partSlot[1])[0] != 196608) {
        std::printf("editor snapshot: save as new parts did not save both parts\n");
        ok = false;
    }
    setRaw("op1_ratio.p0", 163840);
    setRaw("op1_ratio.p1", 229376);
    processor.setPatchName("both parts mutate");
    p1editor->savePatchToSlot(7, 2);
    p1::PresetStore::Multi mutated;
    if (!processor.presets().loadMulti(7, mutated) ||
        mutated.partSlot[0] != newParts.partSlot[0] ||
        mutated.partSlot[1] != newParts.partSlot[1] ||
        processor.presets().partParams(mutated.partSlot[0])[0] != 163840 ||
        processor.presets().partParams(mutated.partSlot[1])[0] != 229376) {
        std::printf("editor snapshot: mutate parts in place did not write both parts\n");
        ok = false;
    }
    setRaw("op1_ratio.p0", 32768);
    setRaw("op1_ratio.p1", 40960);
    processor.setPatchName("both parts discard");
    p1editor->savePatchToSlot(8, 0);
    p1::PresetStore::Multi discarded;
    if (!processor.presets().loadMulti(8, discarded) ||
        discarded.partSlot[0] != mutated.partSlot[0] ||
        discarded.partSlot[1] != mutated.partSlot[1] ||
        processor.presets().partParams(discarded.partSlot[0])[0] != 163840 ||
        processor.presets().partParams(discarded.partSlot[1])[0] != 229376) {
        std::printf("editor snapshot: discard edits to parts changed a part slot\n");
        ok = false;
    }

    {
        /* Every card with copy and paste must move all of its controls. Each
         * case writes a normalised source value, pastes, and compares every
         * destination parameter with the source value it was given. */
        auto& params = processor.parameters();
        using Section = PluginEditor::OperatorSection;
        auto check = [&](const char* name, Section section, int srcOp, int dstOp,
                         const std::vector<juce::String>& src,
                         const std::vector<juce::String>& dst) {
            if (src.size() != dst.size()) {
                std::printf("editor snapshot: %s list lengths differ\n", name);
                ok = false;
                return;
            }
            std::vector<double> expected;
            for (std::size_t i = 0; i < src.size(); ++i) {
                if (auto* parameter = params.getParameter(src[i])) {
                    parameter->setValueNotifyingHost(0.37f);
                    expected.push_back(
                        params.getRawParameterValue(src[i])->load());
                } else {
                    expected.push_back(0.0);
                }
                if (auto* parameter = params.getParameter(dst[i]))
                    parameter->setValueNotifyingHost(0.11f);
            }
            p1editor->copyOperatorSection(srcOp, 0, section);
            p1editor->pasteOperatorSection(dstOp, 0, section);
            for (std::size_t i = 0; i < dst.size(); ++i) {
                const auto* value = params.getRawParameterValue(dst[i]);
                if (value == nullptr ||
                    std::abs(value->load() - expected[i]) > 0.51) {
                    std::printf("editor snapshot: %s copy missed %s\n", name,
                                dst[i].toRawUTF8());
                    ok = false;
                    break;
                }
            }
        };
        auto feedbackEnv = [](int op) {
            std::vector<juce::String> ids;
            ids.push_back(p1::params::opFeedbackOn(op, 0));
            for (int i = 0; i < 3; ++i)
                ids.push_back(p1::params::opFeedbackRate(op, i, 0));
            ids.push_back(p1::params::opFeedbackRelRate(op, 0));
            for (int i = 0; i < 3; ++i)
                ids.push_back(p1::params::opFeedbackLevel(op, i, 0));
            ids.push_back(p1::params::opFeedbackRelLevel(op, 0));
            ids.push_back(p1::params::opFeedbackMode(op, 0));
            return ids;
        };
        auto amplitudeEnv = [](int op) {
            std::vector<juce::String> ids;
            for (const char* field :
                 {"warm", "surge", "d1r", "d1l", "d2r", "d2l", "rel", "mode"})
                ids.push_back(p1::params::opEnv(op, field, 0));
            return ids;
        };
        auto morphEnv = [](int op) {
            std::vector<juce::String> ids;
            ids.push_back(p1::params::opSpecEnvOn(op, 0));
            for (int i = 0; i < 3; ++i)
                ids.push_back(p1::params::opSpecEnvRate(op, i, 0));
            ids.push_back(p1::params::opSpecEnvRelRate(op, 0));
            for (int i = 0; i < 3; ++i)
                ids.push_back(p1::params::opSpecEnvLevel(op, i, 0));
            ids.push_back(p1::params::opSpecEnvRelLevel(op, 0));
            ids.push_back(p1::params::opSpecEnvMode(op, 0));
            return ids;
        };
        auto phase = [](int op) {
            std::vector<juce::String> ids;
            ids.push_back(p1::params::opPhase(op, "on", 0));
            for (const char* field :
                 {"bp", "segments", "flip", "flip_rate", "reorder",
                  "reorder_rate", "jump", "drift", "drift_rate"})
                ids.push_back(p1::params::opPhase(op, field, 0));
            for (const char* field :
                 {"rate", "depth", "wave", "mode", "rate_fade",
                  "depth_fade"})
                ids.push_back(p1::params::opPhaseLfo(op, field, 0));
            for (const char* field : {"on", "r1", "r2", "r3", "l1", "l2",
                                      "l3", "relr", "rell", "mode"})
                ids.push_back(p1::params::opPhaseEnv(op, field, 0));
            return ids;
        };
        auto phaseLfo = [](int op) {
            std::vector<juce::String> ids;
            for (const char* field :
                 {"rate", "depth", "wave", "mode", "rate_fade", "depth_fade"})
                ids.push_back(p1::params::opPhaseLfo(op, field, 0));
            return ids;
        };
        auto phaseEnvelope = [](int op) {
            std::vector<juce::String> ids;
            for (const char* field : {"on", "r1", "r2", "r3", "l1", "l2",
                                      "l3", "relr", "rell", "mode"})
                ids.push_back(p1::params::opPhaseEnv(op, field, 0));
            return ids;
        };
        auto lfo = [](int op, int kind) {
            std::vector<juce::String> ids;
            if (kind == 0) {
                ids.push_back(p1::params::opTremRate(op, 0));
                ids.push_back(p1::params::opTremDepth(op, 0));
                ids.push_back(p1::params::opTremWave(op, 0));
                ids.push_back(p1::params::opTremMode(op, 0));
                ids.push_back(p1::params::opTremRateFade(op, 0));
                ids.push_back(p1::params::opTremDepthFade(op, 0));
            } else if (kind == 1) {
                ids.push_back(p1::params::opFeedbackLfoRate(op, 0));
                ids.push_back(p1::params::opFeedbackLfoDepth(op, 0));
                ids.push_back(p1::params::opFeedbackLfoWave(op, 0));
                ids.push_back(p1::params::opFeedbackLfoMode(op, 0));
                ids.push_back(p1::params::opFeedbackLfoRateFade(op, 0));
                ids.push_back(p1::params::opFeedbackLfoDepthFade(op, 0));
            } else if (kind == 2) {
                ids.push_back(p1::params::opDetuneLfoRate(op, 0));
                ids.push_back(p1::params::opDetuneLfoDepth(op, 0));
                ids.push_back(p1::params::opDetuneLfoWave(op, 0));
                ids.push_back(p1::params::opDetuneLfoMode(op, 0));
                ids.push_back(p1::params::opDetuneLfoRateFade(op, 0));
                ids.push_back(p1::params::opDetuneLfoDepthFade(op, 0));
            } else {
                ids.push_back(p1::params::opSpecLfoRate(op, 0));
                ids.push_back(p1::params::opSpecLfoDepth(op, 0));
                ids.push_back(p1::params::opSpecLfoWave(op, 0));
                ids.push_back(p1::params::opSpecLfoMode(op, 0));
                ids.push_back(p1::params::opSpecLfoRateFade(op, 0));
                ids.push_back(p1::params::opSpecLfoDepthFade(op, 0));
            }
            return ids;
        };
        auto oscillator = [](int op) {
            return std::vector<juce::String>{
                p1::params::opRatio(op, 0), p1::params::opDetune(op, 0),
                p1::params::opWaveform(op, 0), p1::params::opLevel(op, 0),
                p1::params::opFeedback(op, 0), p1::params::opSpecMorph(op, 0)};
        };
        auto spectrumA = [](int op) {
            std::vector<juce::String> ids;
            ids.push_back(p1::params::opSpecRangeA(op, 0));
            for (int h = 1; h <= 64; ++h)
                ids.push_back(p1::params::opSpecA(op, h, 0));
            return ids;
        };
        auto spectrumB = [](int op) {
            std::vector<juce::String> ids;
            ids.push_back(p1::params::opSpecRangeB(op, 0));
            for (int h = 1; h <= 64; ++h)
                ids.push_back(p1::params::opSpecB(op, h, 0));
            return ids;
        };

        check("feedback envelope", Section::feedbackEnvelope, 0, 1,
              feedbackEnv(0), feedbackEnv(1));
        check("amplitude envelope", Section::amplitudeEnvelope, 0, 1,
              amplitudeEnv(0), amplitudeEnv(1));
        check("amplitude envelope back", Section::amplitudeEnvelope, 1, 0,
              amplitudeEnv(1), amplitudeEnv(0));
        check("morph envelope", Section::spectrumEnvelope, 0, 1,
              morphEnv(0), morphEnv(1));
        check("amplitude lfo", Section::amplitudeLfo, 0, 1,
              lfo(0, 0), lfo(1, 0));
        check("feedback lfo", Section::feedbackLfo, 0, 1,
              lfo(0, 1), lfo(1, 1));
        check("detune lfo", Section::detuneLfo, 0, 1,
              lfo(0, 2), lfo(1, 2));
        check("spectrum lfo", Section::spectrumLfo, 0, 1,
              lfo(0, 3), lfo(1, 3));
        check("phase map", Section::phase, 0, 1, phase(0), phase(1));
        check("phase lfo", Section::phaseLfo, 0, 1, phaseLfo(0), phaseLfo(1));
        check("phase envelope", Section::phaseEnvelope, 0, 1, phaseEnvelope(0),
              phaseEnvelope(1));
        check("oscillator", Section::oscillator, 0, 1,
              oscillator(0), oscillator(1));
        check("spectrum a", Section::spectrumA, 0, 1,
              spectrumA(0), spectrumA(1));
        check("spectrum b", Section::spectrumB, 0, 1,
              spectrumB(0), spectrumB(1));
    }

    {
        /* The whole-operator copy has the same op1 attack and release edge. */
        auto& params = processor.parameters();
        auto setNorm = [&params](const juce::String& id, float value) {
            if (auto* parameter = params.getParameter(id))
                parameter->setValueNotifyingHost(value);
        };
        auto raw = [&params](const juce::String& id) {
            const auto* value = params.getRawParameterValue(id);
            return value == nullptr ? 0.0 : value->load();
        };
        p1editor->setActivePart(0);
        setNorm("attack.p0", 0.37f);
        setNorm("op1.eg.d1l.p0", 0.37f);
        setNorm("op2.eg.d1l.p0", 0.11f);
        p1editor->copyOperator(0);
        p1editor->pasteOperator(1);
        if (std::abs(raw("op2.eg.surge.p0") - raw("attack.p0")) > 0.51 ||
            std::abs(raw("op2.eg.d1l.p0") - raw("op1.eg.d1l.p0")) > 0.51) {
            std::printf("editor snapshot: operator copy missed op1 attack or level\n");
            ok = false;
        }
        setNorm("op2.eg.surge.p0", 0.37f);
        setNorm("op1.eg.d1l.p0", 0.11f);
        p1editor->copyOperator(1);
        p1editor->pasteOperator(0);
        if (std::abs(raw("attack.p0") - raw("op2.eg.surge.p0")) > 0.51) {
            std::printf("editor snapshot: operator copy missed the attack edge\n");
            ok = false;
        }
    }

    library.deleteFile();
    std::printf("editor snapshot: %s -> %s\n", ok ? "ok" : "FAIL",
                directory.getFullPathName().toRawUTF8());
    return ok ? 0 : 1;
}
