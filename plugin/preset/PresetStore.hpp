/* SPDX-License-Identifier: GPL-3.0-only */
#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace p1 {

/* The in-memory device preset bank. The byte layout comes from the firmware's
 * presets.c record functions, so a plugin bank round-trips with the device. */
class PresetStore {
public:
    static constexpr int kPartSlots = 128;
    static constexpr int kMultiSlots = 64;
    static constexpr int kNameLen = 15;

    struct Part {
        bool used = false;
        std::string name;
        std::vector<int32_t> params;
    };

    struct Multi {
        bool used = false;
        std::string name;
        std::vector<uint8_t> partSlot;
        std::vector<int32_t> level;
        std::vector<int32_t> delay;
        std::vector<int32_t> channel;
        std::vector<int32_t> low;
        std::vector<int32_t> high;
    };

    PresetStore(int partParamCount, int partCount,
                std::vector<int32_t> defaults = {},
                std::vector<int> legacyPartMap = {});

    int partParamCount() const { return partParamCount_; }
    int partCount() const { return partCount_; }
    std::size_t partRecordSize() const;
    std::size_t multiRecordSize() const;

    bool savePart(int slot, const std::string& name,
                  const std::vector<int32_t>& params);
    bool loadPart(int slot, std::string& name, std::vector<int32_t>& params) const;
    bool partUsed(int slot) const;
    std::string partName(int slot) const;
    const std::vector<int32_t>& partParams(int slot) const;

    bool saveMulti(int slot, const std::string& name,
                   const std::vector<uint8_t>& partSlot,
                   const std::vector<int32_t>& level,
                   const std::vector<int32_t>& delay,
                   const std::vector<int32_t>& channel,
                   const std::vector<int32_t>& low,
                   const std::vector<int32_t>& high);
    bool loadMulti(int slot, Multi& out) const;
    bool multiUsed(int slot) const;
    std::string multiName(int slot) const;

    std::vector<uint8_t> packPartBank() const;
    bool loadPartBank(const uint8_t* data, std::size_t size);
    std::vector<uint8_t> packMultiBank() const;
    bool loadMultiBank(const uint8_t* data, std::size_t size);

    std::vector<uint8_t> packLibrary() const;
    bool loadLibrary(const uint8_t* data, std::size_t size);

    int findFreePartSlot() const;
    int findFreeMultiSlot() const;

    int currentMulti() const { return currentMulti_; }
    void setCurrentMulti(int slot);
    int partSource(int part) const;
    void setPartSource(int part, int slot);

private:
    static std::string shortName(const std::string& name);
    bool loadLegacyPartBank(const uint8_t* data, std::size_t size);

    int partParamCount_ = 0;
    int partCount_ = 0;
    std::vector<int32_t> defaults_;
    std::vector<int> legacyPartMap_;
    std::vector<Part> parts_;
    std::vector<Multi> multis_;
    int currentMulti_ = -1;
    std::vector<int> partSource_;
};

} // namespace p1
