/* SPDX-License-Identifier: GPL-3.0-only */
#include "PresetStore.hpp"

#include <algorithm>
#include <cstring>
#include <utility>

extern "C" {
#include "presets.h"
}

namespace p1 {

PresetStore::PresetStore(int partParamCount, int partCount,
                         std::vector<int32_t> defaults,
                         std::vector<int> legacyPartMap)
    : partParamCount_(std::max(0, partParamCount)),
      partCount_(std::max(0, partCount)),
      defaults_(std::move(defaults)),
      legacyPartMap_(std::move(legacyPartMap)),
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
            part.used = true;
            part.name = shortName(name);
            part.params = std::move(params);
        }
    }
    return true;
}

bool PresetStore::loadLegacyPartBank(const uint8_t* data, std::size_t size)
{
    const auto legacyCount = static_cast<uint32_t>(legacyPartMap_.size());
    const auto recordSize = PP_RECORD(legacyCount);
    if (legacyCount == 0 || data == nullptr || size != recordSize * kPartSlots)
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
        std::vector<int32_t> legacy(legacyCount);
        if (!preset_part_unpack(raw, legacyCount, name, legacy.data()))
            continue;
        part.used = true;
        part.name = shortName(name);
        for (std::size_t i = 0; i < legacyPartMap_.size(); ++i) {
            const int destination = legacyPartMap_[i];
            if (destination >= 0 && destination < partParamCount_)
                part.params[static_cast<std::size_t>(destination)] = legacy[i];
        }
    }
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
    return true;
}

std::vector<uint8_t> PresetStore::packLibrary() const
{
    const auto part = packPartBank();
    const auto multi = packMultiBank();
    static const uint8_t magic[7] = {'P', '1', 'L', 'I', 'B', '1', '\n'};
    std::vector<uint8_t> out;
    out.reserve(7u + 4u + part.size() + 4u + multi.size());
    out.insert(out.end(), magic, magic + 7);
    const auto put32 = [&out](uint32_t value) {
        out.push_back(static_cast<uint8_t>(value & 0xFFu));
        out.push_back(static_cast<uint8_t>((value >> 8) & 0xFFu));
        out.push_back(static_cast<uint8_t>((value >> 16) & 0xFFu));
        out.push_back(static_cast<uint8_t>((value >> 24) & 0xFFu));
    };
    put32(static_cast<uint32_t>(part.size()));
    out.insert(out.end(), part.begin(), part.end());
    put32(static_cast<uint32_t>(multi.size()));
    out.insert(out.end(), multi.begin(), multi.end());
    return out;
}

bool PresetStore::loadLibrary(const uint8_t* data, std::size_t size)
{
    static const uint8_t magic[7] = {'P', '1', 'L', 'I', 'B', '1', '\n'};
    if (data == nullptr || size < 7u + 4u)
        return false;
    if (std::memcmp(data, magic, 7) != 0)
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
    uint32_t partSize = 0;
    uint32_t multiSize = 0;
    const auto currentPartSize =
        partRecordSize() * static_cast<std::size_t>(kPartSlots);
    const auto legacyPartSize =
        legacyPartMap_.empty()
            ? 0u
            : PP_RECORD(static_cast<uint32_t>(legacyPartMap_.size())) *
                  static_cast<std::size_t>(kPartSlots);
    if (!get32(partSize) || (static_cast<std::size_t>(partSize) != currentPartSize &&
                             static_cast<std::size_t>(partSize) != legacyPartSize))
        return false;
    if (offset + partSize + 4u > size)
        return false;
    const auto* partData = data + offset;
    offset += partSize;
    if (!get32(multiSize) ||
        static_cast<std::size_t>(multiSize) !=
            multiRecordSize() * static_cast<std::size_t>(kMultiSlots))
        return false;
    if (offset + multiSize != size)
        return false;
    const auto* multiData = data + offset;
    const bool partsOk =
        static_cast<std::size_t>(partSize) == currentPartSize
            ? loadPartBank(partData, partSize)
            : loadLegacyPartBank(partData, partSize);
    if (!partsOk || !loadMultiBank(multiData, multiSize))
        return false;
    currentMulti_ = -1;
    std::fill(partSource_.begin(), partSource_.end(), -1);
    return true;
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
