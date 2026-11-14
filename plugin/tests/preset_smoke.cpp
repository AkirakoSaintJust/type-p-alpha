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

    const auto library = store.packLibrary();
    p1::PresetStore libraryLoaded{kP1PartParamCount, kP1PartCount};
    check(libraryLoaded.loadLibrary(library.data(), library.size()),
          "load a packed library file");
    check(libraryLoaded.partUsed(3) && libraryLoaded.partName(3) == "lead",
          "the library keeps the part records");
    check(libraryLoaded.partParams(3)[5] == lead[5],
          "the spectrum level travels in the part record");
    check(libraryLoaded.multiUsed(7), "the library keeps the multi records");
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


    std::printf("preset: %s\n", failures ? "FAILED" : "passed");
    return failures ? 1 : 0;
}
