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

bool PresetStore::migratePartBank(const uint8_t* data, uint32_t oldCount,
                                  std::vector<uint8_t>& out) const
{
    const auto oldRecord = PP_RECORD(oldCount);
    const auto newRecord = partRecordSize();
    out.assign(newRecord * kPartSlots, 0xFF);
    const auto target = static_cast<std::size_t>(partParamCount_);
    const bool remapOperators =
        opEnvWarm_[0] >= 0 && opEnvWarm_[1] >= 0 && opEnvWarm_[2] >= 0 &&
        opEnvWarm_[3] >= 0 && static_cast<std::size_t>(oldCount) + 4u == target;
    std::vector<int32_t> grown;
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
            grown.assign(target, 0);
            inOperatorEnvelope.assign(target, 0);
            for (int op = 0; op < 4; ++op) {
                const int start = opEnvWarm_[static_cast<std::size_t>(op)];
                const int oldStart = start - op;
                if (start < 0 || static_cast<std::size_t>(start) + 8u > target ||
                    oldStart < 0 ||
                    static_cast<std::size_t>(oldStart) + 6u >= params.size())
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
            for (std::size_t i = 0; i < target; ++i) {
                if (inOperatorEnvelope[i])
                    continue;
                std::size_t shift = 0;
                for (int warm : opEnvWarm_)
                    if (static_cast<std::size_t>(warm) < i)
                        ++shift;
                const auto oldIndex = i - shift;
                grown[i] = oldIndex < params.size() ? params[oldIndex]
                                                    : defaults_[i];
            }
        } else {
            grown = defaults_;
            const auto copy = std::min<std::size_t>(oldCount, target);
            for (std::size_t i = 0; i < copy; ++i)
                grown[i] = params[i];
        }
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
