/* SPDX-License-Identifier: GPL-3.0-only */
#include "PresetStore.hpp"

#include <algorithm>
#include <cstring>
#include <utility>

#include "CutoffLaw.h"

extern "C" {
#include "presets.h"
}

namespace p1 {

namespace {

/* The five envelope blocks that gained a slot in version 13: one pitch and one
 * feedback envelope for each operator. */
constexpr int kOffsetEnvelopeBlocks = 5;

/* The five absolute envelope blocks that gained two slots in version 14: the
 * filter envelope and one spectral envelope for each operator. */
constexpr int kAbsEnvelopeBlocks = 5;
constexpr int kAbsEnvelopeSlots = 2;

/* The version 11 pan envelope block: on, r1, r2, r3, r4, l1, l2, l3, l4, depth.
 * The version 12 block: on, r1, r2, r3, l1, l2, l3, rel rate, rel level, mode.
 * The release rate and level come from the old r4 and l4, and the depth becomes
 * the mode: a non-zero depth was a live envelope, which maps to sustain on. */
void remapPanBlock(std::vector<int32_t>& params, int base)
{
    if (base < 0 || base + 10 > static_cast<int>(params.size()))
        return;
    const auto at = [base, &params](int k) -> int32_t& {
        return params[static_cast<std::size_t>(base + k)];
    };
    const int32_t relRate = at(4);
    const int32_t l1 = at(5);
    const int32_t l2 = at(6);
    const int32_t l3 = at(7);
    const int32_t relLevel = at(8);
    const int32_t depth = at(9);
    at(4) = l1;
    at(5) = l2;
    at(6) = l3;
    at(7) = relRate;
    at(8) = relLevel;
    at(9) = depth != 0 ? 1 : 0;
}

/* The version 12 offset envelope block: on, r1, r2, r3, r4, l1, l2, l3, l4.
 * The version 13 block: on, r1, r2, r3, l1, l2, l3, rel rate, rel level, mode.
 * The old r4 and l4 are the release rate and level. The old envelope always
 * sustained at l3, so the mode is sustain on. */
void remapOffsetBlock(const std::vector<int32_t>& in, int oldBase,
                      std::vector<int32_t>& out, int newBase)
{
    const auto at = [oldBase, &in](int k) -> int32_t {
        return in[static_cast<std::size_t>(oldBase + k)];
    };
    const auto put = [newBase, &out](int k, int32_t value) {
        out[static_cast<std::size_t>(newBase + k)] = value;
    };
    put(0, at(0));
    put(1, at(1));
    put(2, at(2));
    put(3, at(3));
    put(4, at(5));
    put(5, at(6));
    put(6, at(7));
    put(7, at(4));
    put(8, at(8));
    put(9, 1);
}

/* The version 13 spectral envelope block: r1, r2, r3, r4, l1, l2, l3, depth.
 * The version 14 block: on, r1, r2, r3, l1, l2, l3, rel rate, rel level, mode.
 * A non-zero depth meant a live envelope, so the mode is sustain on. */
void remapAbsSpecBlock(const std::vector<int32_t>& in, int oldBase,
                       std::vector<int32_t>& out, int newBase)
{
    const auto at = [oldBase, &in](int k) -> int32_t {
        return in[static_cast<std::size_t>(oldBase + k)];
    };
    const auto put = [newBase, &out](int k, int32_t value) {
        out[static_cast<std::size_t>(newBase + k)] = value;
    };
    put(0, at(7) != 0 ? 1 : 0);
    put(1, at(0));
    put(2, at(1));
    put(3, at(2));
    put(4, at(4));
    put(5, at(5));
    put(6, at(6));
    put(7, at(3));
    put(8, 0);
    put(9, 1);
}

/* The version 13 filter envelope block: depth, r1, r2, r3, r4, l1, l2, l3. The
 * depth sits before the block, so the new block starts where it was. */
void remapAbsFilterBlock(const std::vector<int32_t>& in, int oldBase,
                         std::vector<int32_t>& out, int newBase)
{
    const auto at = [oldBase, &in](int k) -> int32_t {
        return in[static_cast<std::size_t>(oldBase + k)];
    };
    const auto put = [newBase, &out](int k, int32_t value) {
        out[static_cast<std::size_t>(newBase + k)] = value;
    };
    put(0, at(0) != 0 ? 1 : 0);
    put(1, at(1));
    put(2, at(2));
    put(3, at(3));
    put(4, at(5));
    put(5, at(6));
    put(6, at(7));
    put(7, at(4));
    put(8, 0);
    put(9, 1);
}

} // namespace

PresetStore::PresetStore(int partParamCount, int partCount,
                         std::vector<int32_t> defaults, int cutoffIndex)
    : cutoffIndex_(cutoffIndex >= 0 && cutoffIndex < partParamCount
                       ? cutoffIndex
                       : -1),
      partParamCount_(std::max(0, partParamCount)),
      partCount_(std::max(0, partCount)),
      defaults_(std::move(defaults)),
      parts_(static_cast<std::size_t>(kPartSlots)),
      multis_(static_cast<std::size_t>(kMultiSlots)),
      partSource_(static_cast<std::size_t>(partCount_), -1)
{
    if (defaults_.size() != static_cast<std::size_t>(partParamCount_))
        defaults_.assign(static_cast<std::size_t>(partParamCount_), 0);
}

std::size_t PresetStore::partRecordSize() const
{
    return PP_RECORD(static_cast<uint32_t>(partParamCount_));
}

std::size_t PresetStore::multiRecordSize() const
{
    return MP_RECORD(static_cast<uint32_t>(partCount_));
}

bool PresetStore::savePart(int slot, const std::string& name,
                           const std::vector<int32_t>& params)
{
    if (slot < 0 || slot >= kPartSlots ||
        params.size() != static_cast<std::size_t>(partParamCount_))
        return false;
    auto& part = parts_[static_cast<std::size_t>(slot)];
    if (!dirtyParts_[static_cast<std::size_t>(slot)]) {
        baseParts_[static_cast<std::size_t>(slot)] = part;
        dirtyParts_[static_cast<std::size_t>(slot)] = true;
    }
    part.used = true;
    part.name = shortName(name);
    part.params = params;
    return true;
}

bool PresetStore::loadPart(int slot, std::string& name,
                           std::vector<int32_t>& params) const
{
    if (!partUsed(slot))
        return false;
    const auto& part = parts_[static_cast<std::size_t>(slot)];
    name = part.name;
    params = part.params;
    return true;
}

bool PresetStore::partUsed(int slot) const
{
    return slot >= 0 && slot < kPartSlots &&
           parts_[static_cast<std::size_t>(slot)].used;
}

std::string PresetStore::partName(int slot) const
{
    if (!partUsed(slot))
        return {};
    return parts_[static_cast<std::size_t>(slot)].name;
}

const std::vector<int32_t>& PresetStore::partParams(int slot) const
{
    static const std::vector<int32_t> empty;
    if (!partUsed(slot))
        return empty;
    return parts_[static_cast<std::size_t>(slot)].params;
}

bool PresetStore::saveMulti(int slot, const std::string& name,
                            const std::vector<uint8_t>& partSlot,
                            const std::vector<int32_t>& level,
                            const std::vector<int32_t>& delay,
                            const std::vector<int32_t>& channel,
                            const std::vector<int32_t>& low,
                            const std::vector<int32_t>& high)
{
    const auto count = static_cast<std::size_t>(partCount_);
    if (slot < 0 || slot >= kMultiSlots || partSlot.size() != count ||
        level.size() != count || delay.size() != count || channel.size() != count ||
        low.size() != count || high.size() != count)
        return false;
    auto& multi = multis_[static_cast<std::size_t>(slot)];
    if (!dirtyMultis_[static_cast<std::size_t>(slot)]) {
        baseMultis_[static_cast<std::size_t>(slot)] = multi;
        dirtyMultis_[static_cast<std::size_t>(slot)] = true;
    }
    multi.used = true;
    multi.name = shortName(name);
    multi.partSlot = partSlot;
    multi.level = level;
    multi.delay = delay;
    multi.channel = channel;
    multi.low = low;
    multi.high = high;
    return true;
}

bool PresetStore::loadMulti(int slot, Multi& out) const
{
    if (!multiUsed(slot))
        return false;
    out = multis_[static_cast<std::size_t>(slot)];
    return true;
}

bool PresetStore::multiUsed(int slot) const
{
    return slot >= 0 && slot < kMultiSlots &&
           multis_[static_cast<std::size_t>(slot)].used;
}

std::string PresetStore::multiName(int slot) const
{
    if (!multiUsed(slot))
        return {};
    return multis_[static_cast<std::size_t>(slot)].name;
}

std::vector<uint8_t> PresetStore::packPartBank() const
{
    const auto recordSize = partRecordSize();
    std::vector<uint8_t> bank(recordSize * kPartSlots, 0xFF);
    for (int slot = 0; slot < kPartSlots; ++slot) {
        if (!partUsed(slot))
            continue;
        const auto& part = parts_[static_cast<std::size_t>(slot)];
        const auto offset = static_cast<std::size_t>(slot) * recordSize;
        preset_part_pack(bank.data() + offset, part.name.c_str(),
                         part.params.data(), static_cast<uint32_t>(partParamCount_));
    }
    return bank;
}

bool PresetStore::loadPartBank(const uint8_t* data, std::size_t size)
{
    const auto recordSize = partRecordSize();
    if (data == nullptr || size != recordSize * kPartSlots)
        return false;
    for (int slot = 0; slot < kPartSlots; ++slot) {
        auto& part = parts_[static_cast<std::size_t>(slot)];
        part.used = false;
        part.name.clear();
        part.params = defaults_;
        const auto* raw = data + static_cast<std::size_t>(slot) * recordSize;
        if (raw[0] == PRESET_EMPTY)
            continue;
        char name[PRESET_NAME_LEN] = {};
        std::vector<int32_t> params(static_cast<std::size_t>(partParamCount_));
        if (preset_part_unpack(raw, static_cast<uint32_t>(partParamCount_), name,
                               params.data())) {
            if (panEnvBase_ >= 0 && raw[0] < 12u)
                remapPanBlock(params, panEnvBase_);
            if (raw[0] < PRESET_CUTOFF_LAW_VERSION && cutoffIndex_ >= 0)
                params[static_cast<std::size_t>(cutoffIndex_)] =
                    p1::cutoffOldToNew(
                        params[static_cast<std::size_t>(cutoffIndex_)]);
            part.used = true;
            part.name = shortName(name);
            part.params = std::move(params);
        }
    }
    dirtyParts_.fill(false);
    return true;
}

std::vector<uint8_t> PresetStore::packMultiBank() const
{
    const auto recordSize = multiRecordSize();
    std::vector<uint8_t> bank(recordSize * kMultiSlots, 0xFF);
    for (int slot = 0; slot < kMultiSlots; ++slot) {
        if (!multiUsed(slot))
            continue;
        const auto& multi = multis_[static_cast<std::size_t>(slot)];
        const auto offset = static_cast<std::size_t>(slot) * recordSize;
        preset_multi_pack(bank.data() + offset, multi.name.c_str(),
                          multi.partSlot.data(), multi.level.data(), multi.delay.data(),
                          multi.channel.data(), multi.low.data(), multi.high.data(),
                          static_cast<uint32_t>(partCount_));
    }
    return bank;
}

bool PresetStore::loadMultiBank(const uint8_t* data, std::size_t size)
{
    const auto recordSize = multiRecordSize();
    if (data == nullptr || size != recordSize * kMultiSlots)
        return false;
    for (int slot = 0; slot < kMultiSlots; ++slot) {
        auto& multi = multis_[static_cast<std::size_t>(slot)];
        multi.used = false;
        multi.name.clear();
        multi.partSlot.assign(static_cast<std::size_t>(partCount_), 0xFF);
        multi.level.assign(static_cast<std::size_t>(partCount_), 0);
        multi.delay.assign(static_cast<std::size_t>(partCount_), 0);
        multi.channel.assign(static_cast<std::size_t>(partCount_), 0);
        multi.low.assign(static_cast<std::size_t>(partCount_), 0);
        multi.high.assign(static_cast<std::size_t>(partCount_), 127);
        const auto* raw = data + static_cast<std::size_t>(slot) * recordSize;
        if (raw[0] == PRESET_EMPTY)
            continue;
        char name[PRESET_NAME_LEN] = {};
        std::vector<uint8_t> partSlot(static_cast<std::size_t>(partCount_));
        std::vector<int32_t> level(static_cast<std::size_t>(partCount_));
        std::vector<int32_t> delay(static_cast<std::size_t>(partCount_));
        std::vector<int32_t> channel(static_cast<std::size_t>(partCount_));
        std::vector<int32_t> low(static_cast<std::size_t>(partCount_));
        std::vector<int32_t> high(static_cast<std::size_t>(partCount_));
        if (preset_multi_unpack(raw, static_cast<uint32_t>(partCount_), name,
                                partSlot.data(), level.data(), delay.data(),
                                channel.data(), low.data(), high.data())) {
            multi.used = true;
            multi.name = shortName(name);
            multi.partSlot = std::move(partSlot);
            multi.level = std::move(level);
            multi.delay = std::move(delay);
            multi.channel = std::move(channel);
            multi.low = std::move(low);
            multi.high = std::move(high);
        }
    }
    dirtyMultis_.fill(false);
    return true;
}

std::vector<uint8_t> PresetStore::packLibrary() const
{
    const auto part = packPartBank();
    const auto multi = packMultiBank();
    static const uint8_t magic[7] = {'P', '1', 'L', 'I', 'B', '3', '\n'};
    std::vector<uint8_t> out;
    out.reserve(7u + 4u + 4u + part.size() + 4u + multi.size());
    out.insert(out.end(), magic, magic + 7);
    const auto put32 = [&out](uint32_t value) {
        out.push_back(static_cast<uint8_t>(value & 0xFFu));
        out.push_back(static_cast<uint8_t>((value >> 8) & 0xFFu));
        out.push_back(static_cast<uint8_t>((value >> 16) & 0xFFu));
        out.push_back(static_cast<uint8_t>((value >> 24) & 0xFFu));
    };
    put32(revision_);
    put32(static_cast<uint32_t>(part.size()));
    out.insert(out.end(), part.begin(), part.end());
    put32(static_cast<uint32_t>(multi.size()));
    out.insert(out.end(), multi.begin(), multi.end());
    return out;
}

bool PresetStore::loadLibrary(const uint8_t* data, std::size_t size)
{
    static const uint8_t magic2[7] = {'P', '1', 'L', 'I', 'B', '2', '\n'};
    static const uint8_t magic3[7] = {'P', '1', 'L', 'I', 'B', '3', '\n'};
    if (data == nullptr || size < 7u + 4u)
        return false;
    const bool hasRevision = std::memcmp(data, magic3, 7) == 0;
    if (!hasRevision && std::memcmp(data, magic2, 7) != 0)
        return false;
    std::size_t offset = 7u;
    const auto get32 = [data, size, &offset](uint32_t& value) {
        if (offset + 4u > size)
            return false;
        value = static_cast<uint32_t>(data[offset]) |
                (static_cast<uint32_t>(data[offset + 1u]) << 8) |
                (static_cast<uint32_t>(data[offset + 2u]) << 16) |
                (static_cast<uint32_t>(data[offset + 3u]) << 24);
        offset += 4u;
        return true;
    };
    uint32_t revision = 0;
    if (hasRevision && !get32(revision))
        return false;
    uint32_t partSize = 0;
    uint32_t multiSize = 0;
    const auto currentPartSize =
        partRecordSize() * static_cast<std::size_t>(kPartSlots);
    if (!get32(partSize) || partSize == 0u || offset + partSize + 4u > size)
        return false;
    const auto* partData = data + offset;
    offset += partSize;
    std::vector<uint8_t> migrated;
    if (static_cast<std::size_t>(partSize) != currentPartSize) {
        /* A library written before the bend range joined the part record. The
         * old count comes from the record size, and the new params are appended,
         * so the prefix copies across and the tail takes the defaults. */
        const auto oldRecord = static_cast<std::size_t>(partSize) / kPartSlots;
        if (static_cast<std::size_t>(partSize) % kPartSlots != 0u ||
            oldRecord < PP_RECORD(0u))
            return false;
        const auto oldCount =
            (oldRecord - PP_RECORD(0u)) / 4u;
        if ((oldRecord - PP_RECORD(0u)) % 4u != 0u ||
            oldCount == 0u || oldCount >= static_cast<std::size_t>(partParamCount_))
            return false;
        if (!migratePartBank(partData, static_cast<uint32_t>(oldCount), migrated))
            return false;
        partData = migrated.data();
        partSize = static_cast<uint32_t>(currentPartSize);
    }
    if (!get32(multiSize) ||
        static_cast<std::size_t>(multiSize) !=
            multiRecordSize() * static_cast<std::size_t>(kMultiSlots))
        return false;
    if (offset + multiSize != size)
        return false;
    const auto* multiData = data + offset;
    PresetStore staging(partParamCount_, partCount_, defaults_);
    if (!staging.loadPartBank(partData, partSize) ||
        !staging.loadMultiBank(multiData, multiSize))
        return false;
    /* A patch must point at two parts that exist in the same file. Reject a
     * library whose multi slots reference an empty part slot rather than loading
     * a patch that cannot make a sound. */
    for (int slot = 0; slot < kMultiSlots; ++slot) {
        if (!staging.multiUsed(slot))
            continue;
        Multi multi;
        if (!staging.loadMulti(slot, multi))
            return false;
        for (uint8_t source : multi.partSlot) {
            if (source >= kPartSlots || !staging.partUsed(source))
                return false;
        }
    }
    *this = std::move(staging);
    currentMulti_ = -1;
    std::fill(partSource_.begin(), partSource_.end(), -1);
    revision_ = revision;
    dirtyParts_.fill(false);
    dirtyMultis_.fill(false);
    return true;
}

int PresetStore::toVersion13Index(int targetIndex) const
{
    int shift = 0;
    if (filterEnvBase_ >= 0 && filterEnvBase_ < targetIndex)
        shift += kAbsEnvelopeSlots;
    for (int base : specEnvBase_)
        if (base >= 0 && base < targetIndex)
            shift += kAbsEnvelopeSlots;
    return targetIndex - shift;
}

int PresetStore::toVersion12Index(int targetIndex) const
{
    int shift = 0;
    if (pitchEnvBase_ >= 0 && pitchEnvBase_ < targetIndex)
        ++shift;
    for (int base : fbEnvBase_)
        if (base >= 0 && base < targetIndex)
            ++shift;
    return toVersion13Index(targetIndex) - shift;
}

bool PresetStore::migratePartBank(const uint8_t* data, uint32_t oldCount,
                                  std::vector<uint8_t>& out) const
{
    const auto oldRecord = PP_RECORD(oldCount);
    const auto newRecord = partRecordSize();
    out.assign(newRecord * kPartSlots, 0xFF);
    const auto target = static_cast<std::size_t>(partParamCount_);
    const auto v13Count =
        target >= static_cast<std::size_t>(kAbsEnvelopeBlocks * kAbsEnvelopeSlots)
            ? target - static_cast<std::size_t>(kAbsEnvelopeBlocks *
                                               kAbsEnvelopeSlots)
            : target;
    const auto v12Count =
        v13Count >= static_cast<std::size_t>(kOffsetEnvelopeBlocks)
            ? v13Count - static_cast<std::size_t>(kOffsetEnvelopeBlocks)
            : v13Count;

    /* The version 12 operator warm-up indices. */
    std::array<int, 4> opWarm12{{-1, -1, -1, -1}};
    for (int op = 0; op < 4; ++op) {
        const int warm = opEnvWarm_[static_cast<std::size_t>(op)];
        if (warm >= 0)
            opWarm12[static_cast<std::size_t>(op)] = toVersion12Index(warm);
    }
    const bool remapOperators =
        opWarm12[0] >= 0 && opWarm12[1] >= 0 && opWarm12[2] >= 0 &&
        opWarm12[3] >= 0 &&
        static_cast<std::size_t>(oldCount) + 4u == v12Count;

    /* The five offset blocks in version 13 coordinates. */
    std::array<int, kOffsetEnvelopeBlocks> offsetBases{};
    int offsetCount = 0;
    for (int base : fbEnvBase_)
        if (base >= 0)
            offsetBases[static_cast<std::size_t>(offsetCount++)] =
                toVersion13Index(base);
    if (pitchEnvBase_ >= 0)
        offsetBases[static_cast<std::size_t>(offsetCount++)] =
            toVersion13Index(pitchEnvBase_);
    std::sort(offsetBases.begin(),
              offsetBases.begin() + static_cast<std::ptrdiff_t>(offsetCount));

    /* The absolute blocks: target index and version 13 old index. */
    struct AbsBlock {
        int targetBase = -1;
        int oldBase = -1;
        bool filter = false;
    };
    std::array<AbsBlock, kAbsEnvelopeBlocks> absBlocks{};
    int absCount = 0;
    if (filterEnvBase_ >= 0) {
        absBlocks[static_cast<std::size_t>(absCount)] = {
            filterEnvBase_, toVersion13Index(filterEnvBase_), true};
        ++absCount;
    }
    for (int base : specEnvBase_) {
        if (base < 0 || absCount >= kAbsEnvelopeBlocks)
            continue;
        absBlocks[static_cast<std::size_t>(absCount)] = {
            base, toVersion13Index(base), false};
        ++absCount;
    }
    std::sort(absBlocks.begin(),
              absBlocks.begin() + static_cast<std::ptrdiff_t>(absCount),
              [](const AbsBlock& a, const AbsBlock& b) {
                  return a.targetBase < b.targetBase;
              });

    std::vector<int32_t> grown;
    std::vector<int32_t> expanded;
    std::vector<uint8_t> inOperatorEnvelope;
    for (int slot = 0; slot < kPartSlots; ++slot) {
        const auto* raw = data + static_cast<std::size_t>(slot) * oldRecord;
        if (raw[0] == PRESET_EMPTY)
            continue;
        char name[PRESET_NAME_LEN] = {};
        std::vector<int32_t> params(static_cast<std::size_t>(oldCount));
        if (!preset_part_unpack(raw, oldCount, name, params.data()))
            return false;
        if (remapOperators) {
            grown.assign(v12Count, 0);
            inOperatorEnvelope.assign(v12Count, 0);
            for (int op = 0; op < 4; ++op) {
                const int start = opWarm12[static_cast<std::size_t>(op)];
                const int oldStart = start - op;
                if (start < 0 ||
                    static_cast<std::size_t>(start) + 8u > v12Count ||
                    oldStart < 0 ||
                    static_cast<std::size_t>(oldStart) + 7u > params.size())
                    return false;
                /* warm, surge, d1r, d1l, d2r, d2l, rel, mode from the old
                 * r1, r2, r3, r4, l1, l2, l3 block. */
                grown[static_cast<std::size_t>(start)] = 0;
                grown[static_cast<std::size_t>(start) + 1u] =
                    params[static_cast<std::size_t>(oldStart)];
                grown[static_cast<std::size_t>(start) + 2u] =
                    params[static_cast<std::size_t>(oldStart) + 1u];
                grown[static_cast<std::size_t>(start) + 3u] =
                    params[static_cast<std::size_t>(oldStart) + 5u];
                grown[static_cast<std::size_t>(start) + 4u] =
                    params[static_cast<std::size_t>(oldStart) + 2u];
                grown[static_cast<std::size_t>(start) + 5u] =
                    params[static_cast<std::size_t>(oldStart) + 6u];
                grown[static_cast<std::size_t>(start) + 6u] =
                    params[static_cast<std::size_t>(oldStart) + 3u];
                grown[static_cast<std::size_t>(start) + 7u] = 1;
                for (int k = 0; k < 8; ++k)
                    inOperatorEnvelope[static_cast<std::size_t>(start + k)] = 1;
            }
            for (std::size_t i = 0; i < v12Count; ++i) {
                if (inOperatorEnvelope[i])
                    continue;
                std::size_t shift = 0;
                for (int warm : opWarm12)
                    if (warm >= 0 && static_cast<std::size_t>(warm) < i)
                        ++shift;
                const auto oldIndex = i - shift;
                grown[i] = oldIndex < params.size() ? params[oldIndex]
                                                    : defaults_[i];
            }
        } else if (oldCount == static_cast<uint32_t>(v13Count)) {
            grown.assign(v13Count, 0);
            for (std::size_t i = 0; i < v13Count; ++i)
                grown[i] = params[i];
        } else {
            grown.assign(v12Count, 0);
            const auto copy = std::min<std::size_t>(oldCount, v12Count);
            for (std::size_t i = 0; i < copy; ++i)
                grown[i] = params[i];
            for (std::size_t i = copy; i < v12Count; ++i)
                grown[i] = i < defaults_.size() ? defaults_[i] : 0;
        }

        if (grown.size() == v12Count) {
            const int pan12 =
                panEnvBase_ >= 0 ? toVersion12Index(panEnvBase_) : -1;
            if (raw[0] < 12u && pan12 >= 4 &&
                pan12 + 6 <= static_cast<int>(v12Count))
                remapPanBlock(grown, pan12);
            if (raw[0] < 13u) {
                expanded.assign(v13Count, 0);
                if (offsetCount != kOffsetEnvelopeBlocks) {
                    const auto copy =
                        std::min<std::size_t>(grown.size(), v13Count);
                    for (std::size_t i = 0; i < copy; ++i)
                        expanded[i] = grown[i];
                } else {
                    int shift = 0;
                    int next = 0;
                    for (int i = 0; i < static_cast<int>(v13Count); ++i) {
                        if (next < offsetCount && i == offsetBases[next]) {
                            const int oldBase = i - shift;
                            if (oldBase < 0 ||
                                static_cast<std::size_t>(oldBase) + 9u >
                                    grown.size())
                                return false;
                            remapOffsetBlock(grown, oldBase, expanded, i);
                            i += 9;
                            ++shift;
                            ++next;
                            continue;
                        }
                        const int oldIndex = i - shift;
                        expanded[static_cast<std::size_t>(i)] =
                            oldIndex >= 0 &&
                                    static_cast<std::size_t>(oldIndex) <
                                        grown.size()
                                ? grown[static_cast<std::size_t>(oldIndex)]
                                : (static_cast<std::size_t>(i) <
                                           defaults_.size()
                                       ? defaults_[static_cast<std::size_t>(i)]
                                       : 0);
                    }
                }
                grown.swap(expanded);
            }
        }

        if (grown.size() == v13Count && raw[0] < 14u) {
            expanded.assign(target, 0);
            if (absCount != kAbsEnvelopeBlocks) {
                const auto copy = std::min<std::size_t>(grown.size(), target);
                for (std::size_t i = 0; i < copy; ++i)
                    expanded[i] = grown[i];
            } else {
                int shift = 0;
                int next = 0;
                for (int i = 0; i < static_cast<int>(target); ++i) {
                    if (next < absCount &&
                        i == absBlocks[static_cast<std::size_t>(next)].targetBase) {
                        const AbsBlock& block =
                            absBlocks[static_cast<std::size_t>(next)];
                        if (block.oldBase < 0 ||
                            static_cast<std::size_t>(block.oldBase) + 8u >
                                grown.size())
                            return false;
                        if (block.filter)
                            remapAbsFilterBlock(grown, block.oldBase, expanded, i);
                        else
                            remapAbsSpecBlock(grown, block.oldBase, expanded, i);
                        i += 9;
                        shift += kAbsEnvelopeSlots;
                        ++next;
                        continue;
                    }
                    const int oldIndex = i - shift;
                    expanded[static_cast<std::size_t>(i)] =
                        oldIndex >= 0 &&
                                static_cast<std::size_t>(oldIndex) < grown.size()
                            ? grown[static_cast<std::size_t>(oldIndex)]
                            : (static_cast<std::size_t>(i) < defaults_.size()
                                   ? defaults_[static_cast<std::size_t>(i)]
                                   : 0);
                }
            }
            grown.swap(expanded);
        }

        if (grown.size() < target)
            grown.resize(target, 0);
        if (raw[0] < PRESET_CUTOFF_LAW_VERSION && cutoffIndex_ >= 0 &&
            cutoffIndex_ < partParamCount_)
            grown[static_cast<std::size_t>(cutoffIndex_)] =
                p1::cutoffOldToNew(
                    grown[static_cast<std::size_t>(cutoffIndex_)]);
        preset_part_pack(out.data() + static_cast<std::size_t>(slot) * newRecord,
                         name, grown.data(),
                         static_cast<uint32_t>(partParamCount_));
    }
    return true;
}

bool PresetStore::hasChanges() const
{
    return std::any_of(dirtyParts_.begin(), dirtyParts_.end(),
                       [](bool dirty) { return dirty; }) ||
           std::any_of(dirtyMultis_.begin(), dirtyMultis_.end(),
                       [](bool dirty) { return dirty; });
}

std::vector<PresetStore::Conflict> PresetStore::mergeInto(PresetStore& disk)
{
    std::vector<Conflict> conflicts;
    if (disk.partParamCount_ != partParamCount_ || disk.partCount_ != partCount_)
        return conflicts;

    const auto samePart = [](const Part& a, const Part& b) {
        return a.used == b.used && a.name == b.name && a.params == b.params;
    };
    const auto sameMulti = [](const Multi& a, const Multi& b) {
        return a.used == b.used && a.name == b.name &&
               a.partSlot == b.partSlot && a.level == b.level &&
               a.delay == b.delay && a.channel == b.channel &&
               a.low == b.low && a.high == b.high;
    };

    for (int slot = 0; slot < kPartSlots; ++slot) {
        const auto index = static_cast<std::size_t>(slot);
        if (!dirtyParts_[index])
            continue;
        const Part& local = parts_[index];
        const Part& theirs = disk.parts_[index];
        if (samePart(theirs, local))
            continue; /* a retry after a write that already landed */
        if (samePart(theirs, baseParts_[index]))
            disk.parts_[index] = local;
        else
            conflicts.push_back({false, slot, local.name});
    }

    for (int slot = 0; slot < kMultiSlots; ++slot) {
        const auto index = static_cast<std::size_t>(slot);
        if (!dirtyMultis_[index])
            continue;
        const Multi& local = multis_[index];
        const Multi& theirs = disk.multis_[index];
        if (sameMulti(theirs, local))
            continue;
        if (sameMulti(theirs, baseMultis_[index]))
            disk.multis_[index] = local;
        else
            conflicts.push_back({true, slot, local.name});
    }
    return conflicts;
}

void PresetStore::adoptLibrary(const PresetStore& other)
{
    if (other.partParamCount_ != partParamCount_ || other.partCount_ != partCount_)
        return;
    parts_ = other.parts_;
    multis_ = other.multis_;
    revision_ = other.revision_;
    dirtyParts_.fill(false);
    dirtyMultis_.fill(false);
}

void PresetStore::markSaved()
{
    dirtyParts_.fill(false);
    dirtyMultis_.fill(false);
}

int PresetStore::findFreePartSlot() const
{
    for (int slot = 0; slot < kPartSlots; ++slot)
        if (!partUsed(slot))
            return slot;
    return -1;
}

int PresetStore::findFreeMultiSlot() const
{
    for (int slot = 0; slot < kMultiSlots; ++slot)
        if (!multiUsed(slot))
            return slot;
    return -1;
}

void PresetStore::setCurrentMulti(int slot)
{
    currentMulti_ = slot >= 0 && slot < kMultiSlots ? slot : -1;
}

int PresetStore::partSource(int part) const
{
    if (part < 0 || part >= partCount_)
        return -1;
    return partSource_[static_cast<std::size_t>(part)];
}

void PresetStore::setPartSource(int part, int slot)
{
    if (part < 0 || part >= partCount_)
        return;
    partSource_[static_cast<std::size_t>(part)] =
        slot >= 0 && slot < kPartSlots ? slot : -1;
}

std::string PresetStore::shortName(const std::string& name)
{
    return name.substr(0, static_cast<std::size_t>(kNameLen));
}

} // namespace p1
