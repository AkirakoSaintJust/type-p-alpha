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
#include "part_migration.h"

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

void append32(std::vector<uint8_t>& out, uint32_t value)
{
    out.push_back(static_cast<uint8_t>(value & 0xFFu));
    out.push_back(static_cast<uint8_t>((value >> 8) & 0xFFu));
    out.push_back(static_cast<uint8_t>((value >> 16) & 0xFFu));
    out.push_back(static_cast<uint8_t>((value >> 24) & 0xFFu));
}

void test_legacy_library()
{
    constexpr int kLegacyCount = 146;
    const auto legacyRecordSize = static_cast<std::size_t>(PP_RECORD(kLegacyCount));
    std::vector<uint8_t> legacyBank(legacyRecordSize * 128u, 0xFF);
    std::vector<int32_t> legacyParams(static_cast<std::size_t>(kLegacyCount));
    for (int i = 0; i < kLegacyCount; ++i)
        legacyParams[static_cast<std::size_t>(i)] = 1000 + i;
    preset_part_pack(legacyBank.data(), "old lead", legacyParams.data(),
                     kLegacyCount);

    p1::PresetStore store{kP1PartParamCount, kP1PartCount,
                          p1::partParamDefaults(), p1::legacyPartParamMap()};
    const auto multiBank = store.packMultiBank();
    static const uint8_t magic[7] = {'P', '1', 'L', 'I', 'B', '1', '\n'};
    std::vector<uint8_t> library(magic, magic + 7);
    append32(library, static_cast<uint32_t>(legacyBank.size()));
    library.insert(library.end(), legacyBank.begin(), legacyBank.end());
    append32(library, static_cast<uint32_t>(multiBank.size()));
    library.insert(library.end(), multiBank.begin(), multiBank.end());

    check(store.loadLibrary(library.data(), library.size()),
          "load a library from the old part layout");
    check(store.partUsed(0) && store.partName(0) == "old lead",
          "the old part name survives the migration");
    const auto& loaded = store.partParams(0);
    check(loaded[0] == legacyParams[0] && loaded[132] == legacyParams[132],
          "the unchanged head keeps its index");
    check(loaded[152] == legacyParams[133] && loaded[159] == legacyParams[140],
          "the moved tail lands at 152..159");
    const auto defaults = p1::partParamDefaults();
    bool filterDefaulted = true;
    for (int i = 133; i < 152; ++i) {
        if (loaded[static_cast<std::size_t>(i)] !=
            defaults[static_cast<std::size_t>(i)])
            filterDefaulted = false;
    }
    check(filterDefaulted, "the new filter block takes its defaults");
}

} // namespace

int main()
{
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

    check(store.savePart(5, "12345678901234567890", lead),
          "save a name longer than the device limit");
    check(store.partName(5).size() == p1::PresetStore::kNameLen,
          "the store truncates the name to the device limit");

    const auto library = store.packLibrary();
    p1::PresetStore libraryLoaded{kP1PartParamCount, kP1PartCount};
    check(libraryLoaded.loadLibrary(library.data(), library.size()),
          "load a packed library file");
    check(libraryLoaded.partUsed(3) && libraryLoaded.partName(3) == "lead",
          "the library keeps the part records");
    check(libraryLoaded.multiUsed(7), "the library keeps the multi records");
    check(!libraryLoaded.loadLibrary(library.data(), 12),
          "reject a truncated library file");
    check(!libraryLoaded.loadLibrary(library.data(), library.size() - 1),
          "reject a library file with a short tail");

    test_legacy_library();

    std::printf("preset: %s\n", failures ? "FAILED" : "passed");
    return failures ? 1 : 0;
}
