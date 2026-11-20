/* SPDX-License-Identifier: GPL-3.0-only */
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace p1 {

/* The in-memory preset bank. The byte layout comes from the engine's
 * presets.c record functions, so a plugin bank round-trips with the device. */
class PresetStore {
public:
    static constexpr int kPartSlots = 128;
    static constexpr int kMultiSlots = 64;
    static constexpr int kNameLen = 32;

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
                std::vector<int32_t> defaults = {}, int cutoffIndex = -1);

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

    /* The library revision written into the file header. A v2 file has no
     * revision field, so it loads as revision 0. */
    uint32_t revision() const { return revision_; }
    void setRevision(uint32_t revision) { revision_ = revision; }

    /* A slot that changed on disk since this instance loaded it. `multi` says
     * which bank, and the name is the local name so the message can quote it. */
    struct Conflict {
        bool multi = false;
        int slot = -1;
        std::string name;
    };

    /* Copy the slots this instance changed into `disk`, which must be a fresh
     * store loaded from the current file. A slot the other side changed comes
     * back as a conflict and is left out, so the disk version stays. The local
     * store keeps its value and its queued change is cleared, so the edit can be
     * saved to another slot. */
    std::vector<Conflict> mergeInto(PresetStore& disk);

    /* Take the merged banks back, keeping the routing state the user chose. */
    void adoptLibrary(const PresetStore& other);

    /* The part-block indices of the four operator warm-up rates, used to remap
     * a version 10 record where the operator envelope grew by one parameter.
     * -1 in any slot means the caller has no layout, so the older append-only
     * migration runs instead. */
    void setOperatorEnvelopeIndices(const std::array<int, 4>& indices)
    {
        opEnvWarm_ = indices;
    }

    /* The part-block index of `pan_env_on`, used to remap a version 11 record
     * where the pan envelope dropped the depth and gained a release level and a
     * mode. -1 means no layout was supplied. */
    void setPanEnvelopeIndex(int index) { panEnvBase_ = index; }

    /* Drop the queued changes after the file holds them. */
    void markSaved();

    bool hasChanges() const;

    int findFreePartSlot() const;
    int findFreeMultiSlot() const;

    int currentMulti() const { return currentMulti_; }
    void setCurrentMulti(int slot);
    int partSource(int part) const;
    void setPartSource(int part, int slot);

private:
    static std::string shortName(const std::string& name);

    /* Expand a part bank saved with `oldCount` params per record to the current
     * count. The new params are appended at the end of the record, which is how
     * the bend range joined the mode group. */
    bool migratePartBank(const uint8_t* data, uint32_t oldCount,
                         std::vector<uint8_t>& out) const;

    int cutoffIndex_ = -1;

    int partParamCount_ = 0;
    int partCount_ = 0;
    std::vector<int32_t> defaults_;
    std::vector<Part> parts_;
    std::vector<Multi> multis_;
    int currentMulti_ = -1;
    std::vector<int> partSource_;
    uint32_t revision_ = 0;

    /* The change set: which slots this instance wrote since it loaded, and the
     * value each had before the first write, so a merge can tell a local edit
     * from an edit that arrived on disk. */
    std::array<bool, kPartSlots> dirtyParts_{};
    std::array<bool, kMultiSlots> dirtyMultis_{};
    std::array<Part, kPartSlots> baseParts_{};
    std::array<Multi, kMultiSlots> baseMultis_{};
    std::array<int, 4> opEnvWarm_{{ -1, -1, -1, -1 }};
    int panEnvBase_ = -1;
};

} // namespace p1
