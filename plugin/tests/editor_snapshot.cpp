/* SPDX-License-Identifier: GPL-3.0-only */
/* Milestone 3: render the designed editor to PNG for visual review. */
#include <cmath>
#include <cstdio>
#include <memory>
#include <vector>

#include <juce_gui_basics/juce_gui_basics.h>

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
    processor.setRateAndBufferSizeDetails(44100.0, 256);
    processor.prepareToPlay(44100.0, 256);
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
    ok = saveSnapshot(*editor, directory.getChildFile("editor-operators.png")) && ok;
    p1editor->showTab(2);
    fill();
    ok = saveSnapshot(*editor, directory.getChildFile("editor-fxmod.png")) && ok;
    p1editor->showTab(3);
    fill();
    ok = saveSnapshot(*editor, directory.getChildFile("editor-feedback.png")) && ok;
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
    p1editor->showPartBrowser(0, true);
    ok = saveSnapshot(*editor, directory.getChildFile("editor-parts.png")) && ok;
    p1editor->closeDialog();
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

    library.deleteFile();
    std::printf("editor snapshot: %s -> %s\n", ok ? "ok" : "FAIL",
                directory.getFullPathName().toRawUTF8());
    return ok ? 0 : 1;
}
