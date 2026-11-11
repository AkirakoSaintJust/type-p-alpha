/* SPDX-License-Identifier: GPL-3.0-only */
/* Copyright (C) 2026 P1Onken */
/* The preset store (docs/roadmap.md §6.11.4): the record byte layouts + the bank
 * save/load over the platform's storage services. The layouts are byte-exact
 * little-endian and must match the framework `_records.py` (proven by the parity test).
 *
 * A record is a self-validating unit: a version byte (0xFF = empty), a body, and
 * a CRC-32 (poly 0xEDB88320, the platform's crc32) over version+body. The banks
 * are chunked across the flash slots (the part bank from slot 0, the multi bank
 * straight after it) at record boundaries. A record never straddles a slot, so the
 * settings record is stored in the settings sector. Save only when silent (the
 * platform's area_save erases with interrupts off). */
#pragma once
#include <stdint.h>

#define PRESET_NAME_LEN 16u
#define PRESET_VERSION 1u
#define PRESET_EMPTY 0xFFu
/* The logical slot counts: 128 single-part slots + 64 multi-patch slots. Each bank
 * is a contiguous byte array chunked across one or more flash slots (a record never
 * straddles a slot boundary). The flash slot byte ceiling is store.h `SLOT_MAX`,
 * mirrored here as `PRESET_SLOT_MAX` so the host tests can build presets.c with
 * `-Isrc` only, and the device build asserts the two agree. The type-p-fm1 317 B part
 * record spans the 128 parts over 3 flash slots (51 per slot). The 63 B multi
 * record fits all 64 in one (roadmap §6.11.4). */
#define PRESET_PART_SLOTS 128u
#define PRESET_MULTI_SLOTS 64u
#define PRESET_SLOT_MAX 16372u

/* The per-part timbre param cap: the preset apply + SysEx dump buffers are sized to
 * this, so a synth's `SYNTH_N_PARAMS` must stay under it. (Raised from 128 when the
 * type-p-fm1 key/velocity follow + per-part algorithm pushed it past 128.) */
#define SYNTH_PARAM_MAX 256u

#define PP_RECORD(N) (1u + PRESET_NAME_LEN + (N) * 4u + 4u)
#define MP_RECORD(P) (1u + PRESET_NAME_LEN + (P) + (P) * 5u * 4u + 4u)
#define ST_RECORD (1u + 4u + 4u + 4u)
#define PP_BANK(N) (PRESET_PART_SLOTS * PP_RECORD(N))
#define MP_BANK(P) (PRESET_MULTI_SLOTS * MP_RECORD(P))

uint32_t preset_crc32(const uint8_t *p, uint32_t n);
uint16_t preset_crc16(const uint8_t *p, uint32_t n); /* the SysEx envelope's CRC-16-CCITT */

/* pack -> the record length, the name is truncated to PRESET_NAME_LEN-1 chars. */
uint32_t preset_part_pack(uint8_t *out, const char *name, const int32_t *params, uint32_t n);
uint32_t preset_multi_pack(uint8_t *out, const char *name, const uint8_t *part_slot,
                           const int32_t *level, const int32_t *delay, const int32_t *channel,
                           const int32_t *low, const int32_t *high, uint32_t p);
uint32_t preset_settings_pack(uint8_t *out, int32_t base_channel, int32_t master_tune);

/* unpack -> 1 and fills the outputs, 0 when the slot is empty or the CRC fails
 * (the outputs are then left untouched). */
int preset_part_unpack(const uint8_t *raw, uint32_t n, char *name, int32_t *params);
int preset_multi_unpack(const uint8_t *raw, uint32_t p, char *name, uint8_t *part_slot,
                        int32_t *level, int32_t *delay, int32_t *channel, int32_t *low,
                        int32_t *high);
int preset_settings_unpack(const uint8_t *raw, int32_t *base_channel, int32_t *master_tune);

/* banks, over the platform's `slot_save`/`slot_load` services. `n` = the part-patch
 * param count (PP_RECORD) and `p` = the multi-patch part count (MP_RECORD). The
 * multi bank's `n` is carried too so its flash slot can follow the part bank. */
int preset_part_bank_save(const uint8_t *bank, uint32_t n);
int preset_part_bank_load(uint8_t *bank, uint32_t n);
int preset_multi_bank_save(const uint8_t *bank, uint32_t n, uint32_t p);
int preset_multi_bank_load(uint8_t *bank, uint32_t n, uint32_t p);
int preset_settings_save(const uint8_t *rec);
int preset_settings_load(uint8_t *rec);
