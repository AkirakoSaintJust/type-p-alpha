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

/* `preset_part_pack` seals a current-version record. An older record needs the
 * version byte and then a fresh CRC. */
void setRecordVersion(uint8_t* record, uint32_t count, uint8_t version)
{
    record[0] = version;
    const uint32_t body = 1u + PRESET_NAME_LEN + count * 4u;
    const uint32_t crc = preset_crc32(record, body);
    for (int k = 0; k < 4; ++k)
        record[body + static_cast<std::size_t>(k)] =
            static_cast<uint8_t>((crc >> (8 * k)) & 0xFFu);
}

std::array<int, 34> lfoFadeIndices()
{
    std::array<int, 34> out{};
    out.fill(-1);
    int count = 0;
    for (int i = 0; i < kP1PartParamCount; ++i) {
        const std::string id = kP1ParamSpecs[static_cast<std::size_t>(i)].id;
        if (id.find("_rate_fade.p0") != std::string::npos ||
            id.find("_depth_fade.p0") != std::string::npos) {
            if (count < 34)
                out[static_cast<std::size_t>(count++)] = i;
        }
    }
    return out;
}

} // namespace

int main()
{
    const auto fades = lfoFadeIndices();
    check(fades[33] >= 0, "find the 34 LFO fade parameters");
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

    /* A version 10 part record, before the operator envelope grew from seven to
     * eight parameters. The loader has to remap each operator block and keep the
     * rest of the record aligned. */
    {
        const auto warmIndex = [](int op) {
            const std::string wanted =
                "op" + std::to_string(op + 1) + ".eg.warm.p0";
            for (int i = 0; i < kP1PartParamCount; ++i) {
                if (wanted == kP1ParamSpecs[static_cast<std::size_t>(i)].id)
                    return i;
            }
            return -1;
        };
        const auto indexOf = [](const char* name) {
            for (int i = 0; i < kP1PartParamCount; ++i) {
                if (std::string(name) ==
                    kP1ParamSpecs[static_cast<std::size_t>(i)].id)
                    return i;
            }
            return -1;
        };
        const int w0 = warmIndex(0);
        const int w1 = warmIndex(1);
        const int panBase = indexOf("pan_env_on.p0");
        check(w0 > 0 && w1 > w0 && panBase > w1,
              "find the operator and pan envelope blocks");
        const std::array<int, 4> opWarm = {w0, w1, warmIndex(2), warmIndex(3)};
        const std::array<int, 5> offsetBases = {
            indexOf("op1_fbenv_on.p0"), indexOf("op2_fbenv_on.p0"),
            indexOf("op3_fbenv_on.p0"), indexOf("op4_fbenv_on.p0"),
            indexOf("penv_on.p0")};
        const std::array<int, 5> absBases = {
            indexOf("fenv_on.p0"), indexOf("op1_spec_env_on.p0"),
            indexOf("op2_spec_env_on.p0"), indexOf("op3_spec_env_on.p0"),
            indexOf("op4_spec_env_on.p0")};
        const auto oldIndex = [&fades, &opWarm, &offsetBases, &absBases](int target) {
            int shift = 0;
            for (int base : fades)
                if (base >= 0 && base < target)
                    ++shift;
            for (int base : opWarm)
                if (base >= 0 && base < target)
                    ++shift;
            for (int base : offsetBases)
                if (base >= 0 && base < target)
                    ++shift;
            for (int base : absBases)
                if (base >= 0 && base < target)
                    shift += 2;
            return target - shift;
        };
        /* The version 10 record is 53 params shorter: the 34 fades, the four
         * operator slots and the five offset and ten absolute envelope slots. */
        const uint32_t oldCount = kP1PartParamCount - 19u - 34u;
        const auto oldRecord = PP_RECORD(oldCount);
        std::vector<uint8_t> oldBank(static_cast<std::size_t>(oldRecord) * 128u,
                                     0xFF);
        std::vector<int32_t> oldParams(oldCount, 3);
        const int oldWarm = oldIndex(w0);
        for (int k = 0; k < 7; ++k)
            oldParams[static_cast<std::size_t>(oldWarm) + static_cast<std::size_t>(k)] =
                11 + k;
        oldParams[static_cast<std::size_t>(kP1PartCutoffIndex) - 17u - 34u] =
            12345;
        const int oldPan = oldIndex(panBase);
        oldParams[static_cast<std::size_t>(oldPan)] = 1;
        oldParams[static_cast<std::size_t>(oldPan) + 4u] = 55;
        oldParams[static_cast<std::size_t>(oldPan) + 5u] = 11;
        oldParams[static_cast<std::size_t>(oldPan) + 6u] = 22;
        oldParams[static_cast<std::size_t>(oldPan) + 7u] = 33;
        oldParams[static_cast<std::size_t>(oldPan) + 8u] = -44;
        oldParams[static_cast<std::size_t>(oldPan) + 9u] = 50;
        preset_part_pack(oldBank.data() +
                             static_cast<std::size_t>(3u) * oldRecord,
                         "older", oldParams.data(), oldCount);
        setRecordVersion(
            oldBank.data() + static_cast<std::size_t>(3u) * oldRecord,
            oldCount, 10u);

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
        expanded.setOperatorEnvelopeIndices({w0, w1, warmIndex(2), warmIndex(3)});
        expanded.setPanEnvelopeIndex(panBase);
        expanded.setOffsetEnvelopeIndices(
            {indexOf("op1_fbenv_on.p0"), indexOf("op2_fbenv_on.p0"),
             indexOf("op3_fbenv_on.p0"), indexOf("op4_fbenv_on.p0")},
            indexOf("penv_on.p0"));
        expanded.setAbsEnvelopeIndices(
            indexOf("fenv_on.p0"),
            {indexOf("op1_spec_env_on.p0"), indexOf("op2_spec_env_on.p0"),
             indexOf("op3_spec_env_on.p0"), indexOf("op4_spec_env_on.p0")});
        expanded.setLfoFadeIndices(fades);
        check(expanded.loadLibrary(oldLibrary.data(), oldLibrary.size()),
              "load a version 10 library");
        check(expanded.revision() == 9,
              "the old library keeps its revision through the migration");
        check(expanded.partName(3) == "older",
              "the old part name survives the record migration");
        const auto& grown = expanded.partParams(3);
        check(grown[static_cast<std::size_t>(w0)] == 0 &&
                  grown[static_cast<std::size_t>(w0) + 1u] == 11 &&
                  grown[static_cast<std::size_t>(w0) + 2u] == 12 &&
                  grown[static_cast<std::size_t>(w0) + 3u] == 16 &&
                  grown[static_cast<std::size_t>(w0) + 4u] == 13 &&
                  grown[static_cast<std::size_t>(w0) + 5u] == 17 &&
                  grown[static_cast<std::size_t>(w0) + 6u] == 14 &&
                  grown[static_cast<std::size_t>(w0) + 7u] == 1,
              "the operator envelope maps r1 to surge, l2 to d1l and l3 to d2l");
        check(grown[static_cast<std::size_t>(w0) + 8u] ==
                  oldParams[static_cast<std::size_t>(w0) + 7u],
              "the params after an operator block stay aligned");
        check(grown[static_cast<std::size_t>(panBase)] == 1 &&
                  grown[static_cast<std::size_t>(panBase) + 4u] == 11 &&
                  grown[static_cast<std::size_t>(panBase) + 5u] == 22 &&
                  grown[static_cast<std::size_t>(panBase) + 6u] == 33 &&
                  grown[static_cast<std::size_t>(panBase) + 7u] == 55 &&
                  grown[static_cast<std::size_t>(panBase) + 8u] == -44 &&
                  grown[static_cast<std::size_t>(panBase) + 9u] == 1,
              "the pan envelope maps r4 to the release rate and depth to the mode");
        check(grown[static_cast<std::size_t>(kP1PartCutoffIndex)] == 12345,
              "a current-law cutoff is not converted twice");
    }

    /* A version 12 part record, before the pitch and feedback envelopes gained
     * a mode and swapped the fourth level for a release level. The loader
     * inserts one slot per block and keeps the pan block aligned. */
    {
        const auto indexOf = [](const char* name) {
            for (int i = 0; i < kP1PartParamCount; ++i) {
                if (std::string(name) ==
                    kP1ParamSpecs[static_cast<std::size_t>(i)].id)
                    return i;
            }
            return -1;
        };
        const std::array<int, 5> bases = {
            indexOf("op1_fbenv_on.p0"), indexOf("op2_fbenv_on.p0"),
            indexOf("op3_fbenv_on.p0"), indexOf("op4_fbenv_on.p0"),
            indexOf("penv_on.p0")};
        const int panBase = indexOf("pan_env_on.p0");
        const std::array<int, 5> absBases = {
            indexOf("fenv_on.p0"), indexOf("op1_spec_env_on.p0"),
            indexOf("op2_spec_env_on.p0"), indexOf("op3_spec_env_on.p0"),
            indexOf("op4_spec_env_on.p0")};
        check(bases[0] > 0 && bases[4] > bases[0] && panBase > bases[4],
              "find the offset and pan envelope blocks");
        const auto oldIndex = [&fades, &bases, &absBases](int target) {
            int shift = 0;
            for (int base : fades)
                if (base >= 0 && base < target)
                    ++shift;
            for (int base : bases)
                if (base < target)
                    ++shift;
            for (int base : absBases)
                if (base < target)
                    shift += 2;
            return target - shift;
        };

        const uint32_t oldCount = kP1PartParamCount - 15u - 34u;
        const auto oldRecord = PP_RECORD(oldCount);
        std::vector<uint8_t> oldBank(static_cast<std::size_t>(oldRecord) * 128u,
                                     0xFF);
        std::vector<int32_t> oldParams(oldCount, 3);
        const std::array<int, 9> feedback = {1, 21, 22, 23, 24, 25, 26, 27, 28};
        const std::array<int, 9> pitch = {0, 31, 32, 33, 34, 35, 36, 37, 38};
        const std::array<int, 10> pan = {1, 41, 42, 43, 44, 45, 46, 47, 48, 2};
        const auto writeBlock = [&oldParams, &oldIndex](
                                    int target, const auto& values) {
            const int base = oldIndex(target);
            for (std::size_t k = 0; k < values.size(); ++k)
                oldParams[static_cast<std::size_t>(base) + k] =
                    values[k];
        };
        writeBlock(bases[0], feedback);
        writeBlock(bases[4], pitch);
        writeBlock(panBase, pan);

        const auto slot = static_cast<std::size_t>(5) * oldRecord;
        preset_part_pack(oldBank.data() + slot, "offset", oldParams.data(),
                         oldCount);
        setRecordVersion(oldBank.data() + slot, oldCount, 12u);

        std::vector<uint8_t> oldLibrary;
        const auto put32 = [&oldLibrary](uint32_t value) {
            oldLibrary.push_back(static_cast<uint8_t>(value & 0xFFu));
            oldLibrary.push_back(static_cast<uint8_t>((value >> 8) & 0xFFu));
            oldLibrary.push_back(static_cast<uint8_t>((value >> 16) & 0xFFu));
            oldLibrary.push_back(static_cast<uint8_t>((value >> 24) & 0xFFu));
        };
        static const uint8_t magic[7] = {'P', '1', 'L', 'I', 'B', '3', '\n'};
        oldLibrary.insert(oldLibrary.end(), magic, magic + 7);
        put32(10);
        put32(static_cast<uint32_t>(oldBank.size()));
        oldLibrary.insert(oldLibrary.end(), oldBank.begin(), oldBank.end());
        const auto multiSize =
            MP_RECORD(static_cast<uint32_t>(kP1PartCount)) * 64u;
        put32(static_cast<uint32_t>(multiSize));
        oldLibrary.insert(oldLibrary.end(), multiSize, 0xFF);

        p1::PresetStore loaded{kP1PartParamCount, kP1PartCount,
                               p1::partParamDefaults(), kP1PartCutoffIndex};
        loaded.setOperatorEnvelopeIndices(
            {indexOf("op1.eg.warm.p0"), indexOf("op2.eg.warm.p0"),
             indexOf("op3.eg.warm.p0"), indexOf("op4.eg.warm.p0")});
        loaded.setPanEnvelopeIndex(panBase);
        loaded.setOffsetEnvelopeIndices(
            {bases[0], bases[1], bases[2], bases[3]}, bases[4]);
        loaded.setAbsEnvelopeIndices(
            absBases[0],
            {absBases[1], absBases[2], absBases[3], absBases[4]});
        loaded.setLfoFadeIndices(fades);
        check(loaded.loadLibrary(oldLibrary.data(), oldLibrary.size()),
              "load a version 12 library");
        const auto& grown = loaded.partParams(5);
        check(grown[static_cast<std::size_t>(bases[0])] == 1 &&
                  grown[static_cast<std::size_t>(bases[0]) + 1u] == 21 &&
                  grown[static_cast<std::size_t>(bases[0]) + 2u] == 22 &&
                  grown[static_cast<std::size_t>(bases[0]) + 3u] == 23 &&
                  grown[static_cast<std::size_t>(bases[0]) + 4u] == 25 &&
                  grown[static_cast<std::size_t>(bases[0]) + 5u] == 26 &&
                  grown[static_cast<std::size_t>(bases[0]) + 6u] == 27 &&
                  grown[static_cast<std::size_t>(bases[0]) + 7u] == 24 &&
                  grown[static_cast<std::size_t>(bases[0]) + 8u] == 28 &&
                  grown[static_cast<std::size_t>(bases[0]) + 9u] == 1,
              "the feedback envelope maps r4 to the release rate and l4 to the release level");
        check(grown[static_cast<std::size_t>(bases[4]) + 7u] == 34 &&
                  grown[static_cast<std::size_t>(bases[4]) + 8u] == 38 &&
                  grown[static_cast<std::size_t>(bases[4]) + 9u] == 1,
              "the pitch envelope gains a release level and a sustain mode");
        check(grown[static_cast<std::size_t>(panBase)] == 1 &&
                  grown[static_cast<std::size_t>(panBase) + 4u] == 44 &&
                  grown[static_cast<std::size_t>(panBase) + 7u] == 47 &&
                  grown[static_cast<std::size_t>(panBase) + 9u] == 2,
              "the pan block keeps its version 12 layout after the insert");
    }

    /* A version 13 part record, before the spectral envelope became absolute
     * and gained its on, release and mode controls. */
    {
        const auto indexOf = [](const char* name) {
            for (int i = 0; i < kP1PartParamCount; ++i) {
                if (std::string(name) ==
                    kP1ParamSpecs[static_cast<std::size_t>(i)].id)
                    return i;
            }
            return -1;
        };
        const std::array<int, 5> absBases = {
            indexOf("fenv_on.p0"), indexOf("op1_spec_env_on.p0"),
            indexOf("op2_spec_env_on.p0"), indexOf("op3_spec_env_on.p0"),
            indexOf("op4_spec_env_on.p0")};
        check(absBases[1] > 0 && absBases[4] > absBases[1] &&
                  absBases[0] > absBases[4],
              "find the absolute envelope blocks");
        const auto oldIndex = [&fades, &absBases](int target) {
            int shift = 0;
            for (int base : fades)
                if (base >= 0 && base < target)
                    ++shift;
            for (int base : absBases)
                if (base < target)
                    shift += 2;
            return target - shift;
        };

        const uint32_t oldCount = kP1PartParamCount - 10u - 34u;
        const auto oldRecord = PP_RECORD(oldCount);
        std::vector<uint8_t> oldBank(static_cast<std::size_t>(oldRecord) * 128u,
                                     0xFF);
        std::vector<int32_t> oldParams(oldCount, 3);
        const std::array<int, 8> filter = {2, 51, 52, 53, 54, 55, 56, 57};
        const std::array<int, 8> spectral = {61, 62, 63, 64, 65, 66, 67, 2};
        const auto writeBlock = [&oldParams, &oldIndex](
                                    int target, const auto& values) {
            const int base = oldIndex(target);
            for (std::size_t k = 0; k < values.size(); ++k)
                oldParams[static_cast<std::size_t>(base) + k] = values[k];
        };
        writeBlock(absBases[0], filter);
        writeBlock(absBases[1], spectral);

        const auto slot = static_cast<std::size_t>(5) * oldRecord;
        preset_part_pack(oldBank.data() + slot, "absolute", oldParams.data(),
                         oldCount);
        setRecordVersion(oldBank.data() + slot, oldCount, 13u);

        std::vector<uint8_t> oldLibrary;
        const auto put32 = [&oldLibrary](uint32_t value) {
            oldLibrary.push_back(static_cast<uint8_t>(value & 0xFFu));
            oldLibrary.push_back(static_cast<uint8_t>((value >> 8) & 0xFFu));
            oldLibrary.push_back(static_cast<uint8_t>((value >> 16) & 0xFFu));
            oldLibrary.push_back(static_cast<uint8_t>((value >> 24) & 0xFFu));
        };
        static const uint8_t magic[7] = {'P', '1', 'L', 'I', 'B', '3', '\n'};
        oldLibrary.insert(oldLibrary.end(), magic, magic + 7);
        put32(11);
        put32(static_cast<uint32_t>(oldBank.size()));
        oldLibrary.insert(oldLibrary.end(), oldBank.begin(), oldBank.end());
        const auto multiSize =
            MP_RECORD(static_cast<uint32_t>(kP1PartCount)) * 64u;
        put32(static_cast<uint32_t>(multiSize));
        oldLibrary.insert(oldLibrary.end(), multiSize, 0xFF);

        p1::PresetStore loaded{kP1PartParamCount, kP1PartCount,
                               p1::partParamDefaults(), kP1PartCutoffIndex};
        loaded.setAbsEnvelopeIndices(
            absBases[0],
            {absBases[1], absBases[2], absBases[3], absBases[4]});
        loaded.setLfoFadeIndices(fades);
        check(loaded.loadLibrary(oldLibrary.data(), oldLibrary.size()),
              "load a version 13 library");
        const auto& grown = loaded.partParams(5);
        check(grown[static_cast<std::size_t>(absBases[0])] == 1 &&
                  grown[static_cast<std::size_t>(absBases[0]) + 1u] == 51 &&
                  grown[static_cast<std::size_t>(absBases[0]) + 2u] == 52 &&
                  grown[static_cast<std::size_t>(absBases[0]) + 3u] == 53 &&
                  grown[static_cast<std::size_t>(absBases[0]) + 4u] == 55 &&
                  grown[static_cast<std::size_t>(absBases[0]) + 5u] == 56 &&
                  grown[static_cast<std::size_t>(absBases[0]) + 6u] == 57 &&
                  grown[static_cast<std::size_t>(absBases[0]) + 7u] == 54 &&
                  grown[static_cast<std::size_t>(absBases[0]) + 8u] == 0 &&
                  grown[static_cast<std::size_t>(absBases[0]) + 9u] == 1,
              "the filter envelope maps its depth to on and r4 to the release rate");
        check(grown[static_cast<std::size_t>(absBases[1])] == 1 &&
                  grown[static_cast<std::size_t>(absBases[1]) + 1u] == 61 &&
                  grown[static_cast<std::size_t>(absBases[1]) + 4u] == 65 &&
                  grown[static_cast<std::size_t>(absBases[1]) + 7u] == 64 &&
                  grown[static_cast<std::size_t>(absBases[1]) + 8u] == 0 &&
                  grown[static_cast<std::size_t>(absBases[1]) + 9u] == 1,
              "the spectral envelope maps its depth to on and keeps the levels");
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
