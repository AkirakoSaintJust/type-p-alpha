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
    const auto legacyRecordSize =
        static_cast<std::size_t>(PP_RECORD_V1(kLegacyCount));
    std::vector<uint8_t> legacyBank(legacyRecordSize * 128u, 0xFF);
    std::vector<int32_t> legacyParams(static_cast<std::size_t>(kLegacyCount));
    for (int i = 0; i < kLegacyCount; ++i)
        legacyParams[static_cast<std::size_t>(i)] = 1000 + i;
    preset_part_pack_v1(legacyBank.data(), "old lead", legacyParams.data(),
                        kLegacyCount);

    p1::PresetStore store{kP1PartParamCount, kP1PartCount,
                          p1::partParamDefaults(), p1::legacyPartParamMaps()};
    const std::vector<uint8_t> multiBank(
        static_cast<std::size_t>(MP_RECORD_V1(kP1PartCount)) * 64u, 0xFF);
    static const uint8_t magic[7] = {'P', '1', 'L', 'I', 'B', '1', '\n'};
    std::vector<uint8_t> library(magic, magic + 7);
    append32(library, static_cast<uint32_t>(legacyBank.size()));
    library.insert(library.end(), legacyBank.begin(), legacyBank.end());
    append32(library, static_cast<uint32_t>(multiBank.size()));
    library.insert(library.end(), multiBank.begin(), multiBank.end());

    const bool loadedOk = store.loadLibrary(library.data(), library.size());
    check(loadedOk, "load a library from the old part layout");
    if (!loadedOk)
        return;
    check(store.partUsed(0) && store.partName(0) == "old lead",
          "the old part name survives the migration");
    const auto& loaded = store.partParams(0);
    check(loaded[0] == legacyParams[0] && loaded[18] == legacyParams[18] &&
              loaded[20] == legacyParams[19] && loaded[48] == legacyParams[47] &&
              loaded[108] == legacyParams[105] &&
              loaded[136] == legacyParams[132],
          "the old head migrates across the feedback depths");
    check(loaded[163] == legacyParams[133] && loaded[170] == legacyParams[140],
          "the moved tail lands at 163..170");
    const auto defaults = p1::partParamDefaults();
    bool filterDefaulted = true;
    for (int i = 137; i < 156; ++i) {
        if (loaded[static_cast<std::size_t>(i)] !=
            defaults[static_cast<std::size_t>(i)])
            filterDefaulted = false;
    }
    check(filterDefaulted, "the filter block takes its defaults");
    bool newDefaulted = true;
    for (int i : {19, 49, 79, 109, 156, 157, 158, 159, 160, 161, 162}) {
        if (loaded[static_cast<std::size_t>(i)] !=
            defaults[static_cast<std::size_t>(i)])
            newDefaulted = false;
    }
    check(newDefaulted, "the feedback depth and envelope take their defaults");
}

void test_legacy_160_library()
{
    constexpr int kOldCount = 160;
    const auto recordSize = static_cast<std::size_t>(PP_RECORD_V1(kOldCount));
    std::vector<uint8_t> oldBank(recordSize * 128u, 0xFF);
    std::vector<int32_t> oldParams(static_cast<std::size_t>(kOldCount));
    for (int i = 0; i < kOldCount; ++i)
        oldParams[static_cast<std::size_t>(i)] = 2000 + i;
    preset_part_pack_v1(oldBank.data(), "old 160", oldParams.data(), kOldCount);

    p1::PresetStore store{kP1PartParamCount, kP1PartCount,
                          p1::partParamDefaults(), p1::legacyPartParamMaps()};
    const std::vector<uint8_t> multiBank(
        static_cast<std::size_t>(MP_RECORD_V1(kP1PartCount)) * 64u, 0xFF);
    static const uint8_t magic[7] = {'P', '1', 'L', 'I', 'B', '1', '\n'};
    std::vector<uint8_t> library(magic, magic + 7);
    append32(library, static_cast<uint32_t>(oldBank.size()));
    library.insert(library.end(), oldBank.begin(), oldBank.end());
    append32(library, static_cast<uint32_t>(multiBank.size()));
    library.insert(library.end(), multiBank.begin(), multiBank.end());

    const bool loadedOk = store.loadLibrary(library.data(), library.size());
    check(loadedOk, "load a library from the 160 layout");
    if (!loadedOk)
        return;
    check(store.partUsed(0) && store.partName(0) == "old 160",
          "the 160 part name survives the migration");
    const auto& loaded = store.partParams(0);
    check(loaded[0] == oldParams[0] && loaded[18] == oldParams[18] &&
              loaded[20] == oldParams[19] && loaded[48] == oldParams[47] &&
              loaded[78] == oldParams[76] && loaded[108] == oldParams[105] &&
              loaded[155] == oldParams[151] && loaded[163] == oldParams[152] &&
              loaded[170] == oldParams[159],
          "the 160 head and tail migrate");
    const auto defaults = p1::partParamDefaults();
    bool newDefaulted = true;
    for (int i : {19, 49, 79, 109, 156, 157, 158, 159, 160, 161, 162}) {
        if (loaded[static_cast<std::size_t>(i)] !=
            defaults[static_cast<std::size_t>(i)])
            newDefaulted = false;
    }
    check(newDefaulted, "the 160 feedback depth and envelope take their defaults");
}

void test_legacy_179_library()
{
    constexpr int kOldCount = 179;
    const auto recordSize = static_cast<std::size_t>(PP_RECORD_V1(kOldCount));
    std::vector<uint8_t> oldBank(recordSize * 128u, 0xFF);
    std::vector<int32_t> oldParams(static_cast<std::size_t>(kOldCount));
    for (int i = 0; i < kOldCount; ++i)
        oldParams[static_cast<std::size_t>(i)] = 3000 + i;
    preset_part_pack_v1(oldBank.data(), "old 179", oldParams.data(), kOldCount);

    p1::PresetStore store{kP1PartParamCount, kP1PartCount,
                          p1::partParamDefaults(), p1::legacyPartParamMaps()};
    const std::vector<uint8_t> multiBank(
        static_cast<std::size_t>(MP_RECORD_V1(kP1PartCount)) * 64u, 0xFF);
    static const uint8_t magic[7] = {'P', '1', 'L', 'I', 'B', '1', '\n'};
    std::vector<uint8_t> library(magic, magic + 7);
    append32(library, static_cast<uint32_t>(oldBank.size()));
    library.insert(library.end(), oldBank.begin(), oldBank.end());
    append32(library, static_cast<uint32_t>(multiBank.size()));
    library.insert(library.end(), multiBank.begin(), multiBank.end());

    const bool loadedOk = store.loadLibrary(library.data(), library.size());
    check(loadedOk, "load a library from the 179 layout");
    if (!loadedOk)
        return;
    check(store.partUsed(0) && store.partName(0) == "old 179",
          "the 179 part name survives the migration");
    const auto& loaded = store.partParams(0);
    check(loaded[0] == oldParams[0] && loaded[19] == oldParams[19] &&
              loaded[20] == oldParams[22] && loaded[49] == oldParams[51] &&
              loaded[50] == oldParams[54] && loaded[79] == oldParams[83] &&
              loaded[80] == oldParams[86] && loaded[109] == oldParams[115] &&
              loaded[110] == oldParams[118] && loaded[156] == oldParams[164] &&
              loaded[162] == oldParams[170] && loaded[163] == oldParams[171] &&
              loaded[170] == oldParams[178],
          "the 179 head, envelope and mode block migrate");
}

void test_v1_name_library()
{
    const auto recordSize = static_cast<std::size_t>(
        PP_RECORD_V1(static_cast<uint32_t>(kP1PartParamCount)));
    std::vector<uint8_t> bank(recordSize * 128u, 0xFF);
    const auto params = pattern(7);
    preset_part_pack_v1(bank.data(), "fifteen chars", params.data(),
                        static_cast<uint32_t>(kP1PartParamCount));
    const std::vector<uint8_t> multi(
        static_cast<std::size_t>(MP_RECORD_V1(kP1PartCount)) * 64u, 0xFF);
    static const uint8_t magic[7] = {'P', '1', 'L', 'I', 'B', '1', '\n'};
    std::vector<uint8_t> library(magic, magic + 7);
    append32(library, static_cast<uint32_t>(bank.size()));
    library.insert(library.end(), bank.begin(), bank.end());
    append32(library, static_cast<uint32_t>(multi.size()));
    library.insert(library.end(), multi.begin(), multi.end());

    p1::PresetStore store{kP1PartParamCount, kP1PartCount,
                          p1::partParamDefaults(), p1::legacyPartParamMaps()};
    const bool loadedOk = store.loadLibrary(library.data(), library.size());
    check(loadedOk, "load a v1-name library with the current part layout");
    if (!loadedOk)
        return;
    check(store.partUsed(0) && store.partName(0) == "fifteen chars",
          "the v1-name part name survives");
    check(store.partParams(0) == params, "the v1-name part params survive");
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
    check(libraryLoaded.multiUsed(7), "the library keeps the multi records");
    check(!libraryLoaded.loadLibrary(library.data(), 12),
          "reject a truncated library file");
    check(!libraryLoaded.loadLibrary(library.data(), library.size() - 1),
          "reject a library file with a short tail");

    test_legacy_library();
    test_legacy_160_library();
    test_legacy_179_library();
    test_v1_name_library();

    std::printf("preset: %s\n", failures ? "FAILED" : "passed");
    return failures ? 1 : 0;
}
