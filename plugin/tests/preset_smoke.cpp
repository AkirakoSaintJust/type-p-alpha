/* SPDX-License-Identifier: GPL-3.0-only */
/* Milestone 4: the in-memory preset bank uses the engine record code. */
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include "CutoffLaw.h"
#include "Params.h"
#include "PresetStore.hpp"
#include "part_defaults.h"

extern "C" {
#include "presets.h"
}

namespace {

int failures = 0;

void check(bool ok, const char* what)
{
    std::printf("preset: %-60s %s\n", what, ok ? "ok" : "FAIL");
    if (!ok)
        ++failures;
}

std::vector<int32_t> pattern(int seed)
{
    std::vector<int32_t> values(static_cast<std::size_t>(kP1PartParamCount));
    for (int i = 0; i < kP1PartParamCount; ++i)
        values[static_cast<std::size_t>(i)] = (i * 37 + seed) % 4096 - 2048;
    return values;
}

} // namespace

int main()
{
    std::setvbuf(stdout, nullptr, _IONBF, 0);
    p1::PresetStore store{kP1PartParamCount, kP1PartCount};
    const auto lead = pattern(11);
    const auto bass = pattern(29);
    check(store.savePart(3, "lead", lead), "save a part record");
    check(store.savePart(4, "bass", bass), "save a second part record");
    check(!store.savePart(128, "bad", lead), "reject a part slot out of range");
    check(!store.savePart(0, "bad", std::vector<int32_t>{}),
          "reject a short part record");

    const auto partBank = store.packPartBank();
    const auto partRecordSize = static_cast<std::size_t>(
        PP_RECORD(static_cast<uint32_t>(kP1PartParamCount)));
    check(partBank.size() == partRecordSize * 128,
          "the part bank has 128 device records");
    check(partBank[0] == PRESET_EMPTY, "empty part slots are marked 0xFF");

    p1::PresetStore loaded{kP1PartParamCount, kP1PartCount};
    check(loaded.loadPartBank(partBank.data(), partBank.size()),
          "load the packed part bank");
    check(loaded.partUsed(3) && loaded.partName(3) == "lead",
          "the part name round-trips");
    check(loaded.partParams(3) == lead, "the part parameters round-trip");
    check(!loaded.partUsed(0), "empty part slots stay empty");
    check(!loaded.loadPartBank(partBank.data(), 12),
          "reject a truncated part bank");

    char rawName[PRESET_NAME_LEN] = {};
    std::array<int32_t, kP1PartParamCount> rawParams{};
    check(preset_part_unpack(partBank.data() + 3 * partRecordSize,
                             static_cast<uint32_t>(kP1PartParamCount), rawName,
                             rawParams.data()) == 1,
          "the record is readable by the device unpack");
    check(std::strcmp(rawName, "lead") == 0, "the device record name matches");
    check(std::equal(rawParams.begin(), rawParams.end(), lead.begin()),
          "the device record params match");

    const std::vector<uint8_t> partSlots{3, 4};
    const std::vector<int32_t> level{99, 80};
    const std::vector<int32_t> delay{0, 20};
    const std::vector<int32_t> channel{0, 1};
    const std::vector<int32_t> low{0, 12};
    const std::vector<int32_t> high{127, 96};
    check(store.saveMulti(7, "multi", partSlots, level, delay, channel, low, high),
          "save a multi record");

    const auto multiBank = store.packMultiBank();
    const auto multiRecordSize =
        static_cast<std::size_t>(MP_RECORD(static_cast<uint32_t>(kP1PartCount)));
    check(multiBank.size() == multiRecordSize * 64,
          "the multi bank has 64 device records");
    p1::PresetStore loadedMulti{kP1PartParamCount, kP1PartCount};
    check(loadedMulti.loadMultiBank(multiBank.data(), multiBank.size()),
          "load the packed multi bank");
    p1::PresetStore::Multi multi;
    check(loadedMulti.loadMulti(7, multi), "load the multi slot");
    check(multi.name == "multi", "the multi name round-trips");
    check(multi.partSlot == partSlots, "the part references round-trip");
    check(multi.level == level && multi.delay == delay &&
              multi.channel == channel && multi.low == low && multi.high == high,
          "the routing values round-trip");

    char multiName[PRESET_NAME_LEN] = {};
    std::array<uint8_t, kP1PartCount> rawSlots{};
    std::array<int32_t, kP1PartCount> rawLevel{}, rawDelay{}, rawChannel{},
        rawLow{}, rawHigh{};
    check(preset_multi_unpack(multiBank.data() + 7 * multiRecordSize,
                              static_cast<uint32_t>(kP1PartCount), multiName,
                              rawSlots.data(), rawLevel.data(), rawDelay.data(),
                              rawChannel.data(), rawLow.data(),
                              rawHigh.data()) == 1,
          "the multi record is readable by the device unpack");
    check(std::strcmp(multiName, "multi") == 0, "the device multi name matches");
    check(rawSlots == std::array<uint8_t, kP1PartCount>{3, 4},
          "the device part references match");

    const std::string longName(40, 'a');
    check(store.savePart(5, longName, lead), "save a 40-character name");
    check(store.partName(5) == longName.substr(0, 32),
          "the store truncates the name to 32 characters");
    const auto widePartBank = store.packPartBank();
    p1::PresetStore wideLoaded{kP1PartParamCount, kP1PartCount};
    check(wideLoaded.loadPartBank(widePartBank.data(), widePartBank.size()) &&
              wideLoaded.partName(5) == longName.substr(0, 32),
          "the 32-character name round-trips through the record");

    store.setRevision(4);
    const auto library = store.packLibrary();
    p1::PresetStore libraryLoaded{kP1PartParamCount, kP1PartCount};
    check(libraryLoaded.loadLibrary(library.data(), library.size()),
          "load a packed library file");
    check(libraryLoaded.revision() == 4,
          "the library revision travels in the file header");
    check(libraryLoaded.partUsed(3) && libraryLoaded.partName(3) == "lead",
          "the library keeps the part records");
    check(libraryLoaded.partParams(3)[5] == lead[5],
          "the spectrum level travels in the part record");
    check(libraryLoaded.multiUsed(7), "the library keeps the multi records");

    /* A v2 file has no revision field. It must keep loading, as revision 0, so
     * a library saved before the header change survives the update. */
    std::vector<uint8_t> v2(library.begin(), library.end());
    v2[5] = '2';
    v2.erase(v2.begin() + 7, v2.begin() + 11);
    p1::PresetStore v2Loaded{kP1PartParamCount, kP1PartCount};
    check(v2Loaded.loadLibrary(v2.data(), v2.size()),
          "load a v2 library file");
    check(v2Loaded.revision() == 0, "a v2 library loads as revision 0");
    check(v2Loaded.partUsed(3) && v2Loaded.partName(3) == "lead",
          "a v2 library keeps its part records");

    /* A library whose part record has one fewer param, as every library written
     * before the bend range. The loader has to expand the record and default the
     * new value without touching the old ones. */
    {
        const uint32_t oldCount = kP1PartParamCount - 10u;
        const auto oldRecord = PP_RECORD(oldCount);
        std::vector<uint8_t> oldBank(static_cast<std::size_t>(oldRecord) * 128u,
                                     0xFF);
        std::vector<int32_t> oldParams(oldCount, 3);
        oldParams[static_cast<std::size_t>(kP1PartCutoffIndex)] = 12345;
        preset_part_pack(oldBank.data() +
                             static_cast<std::size_t>(3u) * oldRecord,
                         "older", oldParams.data(), oldCount);

        std::vector<uint8_t> oldLibrary;
        const auto put32 = [&oldLibrary](uint32_t value) {
            oldLibrary.push_back(static_cast<uint8_t>(value & 0xFFu));
            oldLibrary.push_back(static_cast<uint8_t>((value >> 8) & 0xFFu));
            oldLibrary.push_back(static_cast<uint8_t>((value >> 16) & 0xFFu));
            oldLibrary.push_back(static_cast<uint8_t>((value >> 24) & 0xFFu));
        };
        static const uint8_t magic[7] = {'P', '1', 'L', 'I', 'B', '3', '\n'};
        oldLibrary.insert(oldLibrary.end(), magic, magic + 7);
        put32(9);
        put32(static_cast<uint32_t>(oldBank.size()));
        oldLibrary.insert(oldLibrary.end(), oldBank.begin(), oldBank.end());
        const auto multiSize =
            MP_RECORD(static_cast<uint32_t>(kP1PartCount)) * 64u;
        put32(static_cast<uint32_t>(multiSize));
        oldLibrary.insert(oldLibrary.end(), multiSize, 0xFF);

        p1::PresetStore expanded{kP1PartParamCount, kP1PartCount,
                                 p1::partParamDefaults(), kP1PartCutoffIndex};
        check(expanded.loadLibrary(oldLibrary.data(), oldLibrary.size()),
              "load a library with the old part record size");
        check(expanded.revision() == 9,
              "the old library keeps its revision through the migration");
        check(expanded.partName(3) == "older",
              "the old part name survives the record expansion");
        const auto& grown = expanded.partParams(3);
        check(grown[5] == 3 &&
                  grown[800] == 3 && grown[801] == 3 &&
                  grown[802] == 1 && grown[803] == 80 &&
                  grown[static_cast<std::size_t>(kP1PartParamCount - 1)] == 0,
              "the appended pan envelope defaults to on, a slow attack and no depth");
        check(grown[static_cast<std::size_t>(kP1PartCutoffIndex)] == 12345,
              "a current-law cutoff is not converted twice");
    }

    /* Two instances over one file: independent slots merge, a slot both changed
     * comes back as a conflict and the disk version stays. */
    p1::PresetStore shared{kP1PartParamCount, kP1PartCount};
    check(shared.savePart(1, "one", lead), "save the shared slot one");
    check(shared.savePart(2, "two", lead), "save the shared slot two");
    shared.setRevision(1);
    const auto baseBytes = shared.packLibrary();

    p1::PresetStore instanceA{kP1PartParamCount, kP1PartCount};
    p1::PresetStore instanceB{kP1PartParamCount, kP1PartCount};
    check(instanceA.loadLibrary(baseBytes.data(), baseBytes.size()),
          "instance A loads the shared library");
    check(instanceB.loadLibrary(baseBytes.data(), baseBytes.size()),
          "instance B loads the shared library");

    instanceA.savePart(1, "one-a", lead);
    p1::PresetStore merged{kP1PartParamCount, kP1PartCount};
    check(merged.loadLibrary(baseBytes.data(), baseBytes.size()),
          "load the on-disk library for the merge");
    check(instanceA.mergeInto(merged).empty(),
          "a slot only A changed merges without a conflict");
    check(merged.partName(1) == "one-a",
          "A's edit replaces the untouched disk slot");
    instanceA.markSaved();

    instanceB.savePart(2, "two-b", lead);
    check(instanceB.mergeInto(merged).empty(),
          "a slot only B changed merges without a conflict");
    check(merged.partName(1) == "one-a" && merged.partName(2) == "two-b",
          "both instances keep their slot after the merge");
    instanceB.markSaved();

    instanceA.saveMulti(3, "multi-a", {1, 2}, level, delay, channel, low, high);
    check(instanceA.mergeInto(merged).empty(),
          "a patch only A changed merges without a conflict");
    check(merged.multiName(3) == "multi-a",
          "the merged patch reaches the disk bank");
    instanceA.markSaved();

    p1::PresetStore left{kP1PartParamCount, kP1PartCount};
    p1::PresetStore right{kP1PartParamCount, kP1PartCount};
    check(left.loadLibrary(baseBytes.data(), baseBytes.size()) &&
              right.loadLibrary(baseBytes.data(), baseBytes.size()),
          "two instances load the same revision");
    left.savePart(1, "left", lead);
    right.savePart(1, "right", lead);
    p1::PresetStore onDisk{kP1PartParamCount, kP1PartCount};
    check(onDisk.loadLibrary(baseBytes.data(), baseBytes.size()) &&
              left.mergeInto(onDisk).empty(),
          "the left instance saves first");
    const auto conflicts = right.mergeInto(onDisk);
    check(conflicts.size() == 1 && !conflicts[0].multi &&
              conflicts[0].slot == 1 && conflicts[0].name == "right",
          "the right instance reports the slot both changed");
    check(onDisk.partName(1) == "left",
          "the conflicting slot keeps the disk version");

    check(!libraryLoaded.loadLibrary(library.data(), 12),
          "reject a truncated library file");
    check(!libraryLoaded.loadLibrary(library.data(), library.size() - 1),
          "reject a library file with a short tail");

    p1::PresetStore broken{kP1PartParamCount, kP1PartCount};
    check(broken.savePart(3, "lead", lead),
          "save a part for the broken library");
    check(broken.saveMulti(1, "dangling", {3, 9}, level, delay, channel, low,
                           high),
          "save a patch that points at an empty part");
    const auto brokenLibrary = broken.packLibrary();
    p1::PresetStore rejected{kP1PartParamCount, kP1PartCount};
    check(!rejected.loadLibrary(brokenLibrary.data(), brokenLibrary.size()),
          "reject a patch bank with a missing part");
    check(!rejected.partUsed(3) && !rejected.multiUsed(1),
          "the rejected library leaves the store untouched");

    /* A version 6 part record carries the old coefficient law. Loading it must
     * keep its cutoff frequency, not reinterpret the raw value. */
    auto old = pattern(7);
    old[static_cast<std::size_t>(kP1PartCutoffIndex)] = 8000;
    std::vector<uint8_t> oldRecord(PP_RECORD(static_cast<uint32_t>(
        kP1PartParamCount)), 0xFF);
    preset_part_pack(oldRecord.data(), "old", old.data(),
                     static_cast<uint32_t>(kP1PartParamCount));
    oldRecord[0] = 6;
    const std::size_t body = oldRecord.size() - 4u;
    const uint32_t crc = preset_crc32(oldRecord.data(),
                                      static_cast<uint32_t>(body));
    for (int i = 0; i < 4; ++i)
        oldRecord[body + static_cast<std::size_t>(i)] =
            static_cast<uint8_t>((crc >> (8 * i)) & 0xFFu);
    std::vector<uint8_t> oldBank(oldRecord.size() * 128u, 0xFF);
    std::memcpy(oldBank.data(), oldRecord.data(), oldRecord.size());
    p1::PresetStore migrator{kP1PartParamCount, kP1PartCount, {},
                             kP1PartCutoffIndex};
    check(migrator.loadPartBank(oldBank.data(), oldBank.size()),
          "load a version 6 part bank");
    check(migrator.partUsed(0), "the version 6 record is used");
    check(migrator.partParams(0)[static_cast<std::size_t>(kP1PartCutoffIndex)] ==
              p1::cutoffOldToNew(8000),
          "the old cutoff migrates to the new log position");
    check(p1::cutoffOldToNew(8000) == 36958,
          "the migrated default is the 909 Hz position");

    p1::PresetStore current{kP1PartParamCount, kP1PartCount, {},
                            kP1PartCutoffIndex};
    check(current.savePart(0, "current", old),
          "save a current part with the old raw value");
    const auto currentBank = current.packPartBank();
    p1::PresetStore currentLoaded{kP1PartParamCount, kP1PartCount, {},
                                  kP1PartCutoffIndex};
    check(currentLoaded.loadPartBank(currentBank.data(), currentBank.size()),
          "load the current part bank");
    check(currentLoaded.partParams(0)[static_cast<std::size_t>(
              kP1PartCutoffIndex)] == 8000,
          "a current record keeps its raw cutoff");

    std::printf("preset: %s\n", failures ? "FAILED" : "passed");
    return failures ? 1 : 0;
}
