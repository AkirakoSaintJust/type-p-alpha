/* SPDX-License-Identifier: GPL-3.0-only */
/* Copyright (C) 2026 P1Onken */
/* The preset store implementation (presets.h): byte-exact little-endian layouts that
 * match the framework `_records.py`, over the bank save/load hooks
 * (`slot_load/slot_save/settings_load/settings_save`). The host build stubs them. */
#include "presets.h"
/* The host build includes <string.h> before this file. */

static void wr32(uint8_t *p, uint32_t u)
{
    p[0] = (uint8_t)u;
    p[1] = (uint8_t)(u >> 8);
    p[2] = (uint8_t)(u >> 16);
    p[3] = (uint8_t)(u >> 24);
}

static uint32_t rd32(const uint8_t *p)
{
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

uint32_t preset_crc32(const uint8_t *p, uint32_t n)
{
    uint32_t c = 0xFFFFFFFFu, k;
    while (n--) {
        c ^= *p++;
        for (k = 0; k < 8u; k++)
            c = c & 1u ? (c >> 1) ^ 0xEDB88320u : c >> 1;
    }
    return ~c;
}

uint16_t preset_crc16(const uint8_t *p, uint32_t n)
{
    uint16_t c = 0xFFFFu;
    uint32_t k;
    while (n--) {
        c ^= (uint16_t)((uint16_t)*p++ << 8);
        for (k = 0; k < 8u; k++)
            c = c & 0x8000u ? (uint16_t)((uint16_t)c << 1) ^ 0x1021u : (uint16_t)((uint16_t)c << 1);
    }
    return c;
}

static void name_put(uint8_t *out, const char *name)
{
    uint32_t i;
    memset(out, 0, PRESET_NAME_LEN);
    if (!name)
        return;
    for (i = 0; i < PRESET_NAME_LEN - 1u && name[i]; i++)
        out[i] = (uint8_t)name[i];
}

static void seal(uint8_t *out, uint32_t body)
{
    wr32(out + body, preset_crc32(out, body));
}

uint32_t preset_part_pack(uint8_t *out, const char *name, const int32_t *params, uint32_t n)
{
    uint32_t body = 1u + PRESET_NAME_LEN + n * 4u, i;
    out[0] = PRESET_VERSION;
    name_put(out + 1u, name);
    for (i = 0; i < n; i++)
        wr32(out + 1u + PRESET_NAME_LEN + i * 4u, (uint32_t)params[i]);
    seal(out, body);
    return body + 4u;
}

uint32_t preset_multi_pack(uint8_t *out, const char *name, const uint8_t *part_slot,
                           const int32_t *level, const int32_t *delay, const int32_t *channel,
                           const int32_t *low, const int32_t *high, uint32_t p)
{
    uint32_t body = 1u + PRESET_NAME_LEN + p + p * 5u * 4u, i, off = 1u + PRESET_NAME_LEN;
    out[0] = PRESET_VERSION;
    name_put(out + 1u, name);
    for (i = 0; i < p; i++)
        out[off + i] = part_slot[i];
    off += p;
    for (i = 0; i < p; i++) {
        wr32(out + off, (uint32_t)level[i]);
        off += 4u;
    }
    for (i = 0; i < p; i++) {
        wr32(out + off, (uint32_t)delay[i]);
        off += 4u;
    }
    for (i = 0; i < p; i++) {
        wr32(out + off, (uint32_t)channel[i]);
        off += 4u;
    }
    for (i = 0; i < p; i++) {
        wr32(out + off, (uint32_t)low[i]);
        off += 4u;
    }
    for (i = 0; i < p; i++) {
        wr32(out + off, (uint32_t)high[i]);
        off += 4u;
    }
    seal(out, body);
    return body + 4u;
}

uint32_t preset_settings_pack(uint8_t *out, int32_t base_channel, int32_t master_tune)
{
    out[0] = PRESET_VERSION;
    wr32(out + 1u, (uint32_t)base_channel);
    wr32(out + 5u, (uint32_t)master_tune);
    seal(out, 9u);
    return ST_RECORD;
}

int preset_part_unpack(const uint8_t *raw, uint32_t n, char *name, int32_t *params)
{
    uint32_t body = 1u + PRESET_NAME_LEN + n * 4u, i;
    if (raw[0] == PRESET_EMPTY)
        return 0;
    if (preset_crc32(raw, body) != rd32(raw + body))
        return 0;
    if (name) {
        memcpy(name, raw + 1u, PRESET_NAME_LEN - 1u);
        name[PRESET_NAME_LEN - 1u] = 0;
    }
    for (i = 0; i < n; i++)
        params[i] = (int32_t)rd32(raw + 1u + PRESET_NAME_LEN + i * 4u);
    return 1;
}

int preset_multi_unpack(const uint8_t *raw, uint32_t p, char *name, uint8_t *part_slot,
                        int32_t *level, int32_t *delay, int32_t *channel, int32_t *low,
                        int32_t *high)
{
    uint32_t body = 1u + PRESET_NAME_LEN + p + p * 5u * 4u, i, off = 1u + PRESET_NAME_LEN;
    if (raw[0] == PRESET_EMPTY)
        return 0;
    if (preset_crc32(raw, body) != rd32(raw + body))
        return 0;
    if (name) {
        memcpy(name, raw + 1u, PRESET_NAME_LEN - 1u);
        name[PRESET_NAME_LEN - 1u] = 0;
    }
    for (i = 0; i < p; i++)
        part_slot[i] = raw[off + i];
    off += p;
    for (i = 0; i < p; i++) {
        level[i] = (int32_t)rd32(raw + off);
        off += 4u;
    }
    for (i = 0; i < p; i++) {
        delay[i] = (int32_t)rd32(raw + off);
        off += 4u;
    }
    for (i = 0; i < p; i++) {
        channel[i] = (int32_t)rd32(raw + off);
        off += 4u;
    }
    for (i = 0; i < p; i++) {
        low[i] = (int32_t)rd32(raw + off);
        off += 4u;
    }
    for (i = 0; i < p; i++) {
        high[i] = (int32_t)rd32(raw + off);
        off += 4u;
    }
    return 1;
}

int preset_settings_unpack(const uint8_t *raw, int32_t *base_channel, int32_t *master_tune)
{
    if (raw[0] == PRESET_EMPTY)
        return 0;
    if (preset_crc32(raw, 9u) != rd32(raw + 9u))
        return 0;
    *base_channel = (int32_t)rd32(raw + 1u);
    *master_tune = (int32_t)rd32(raw + 5u);
    return 1;
}

/* A bank is a contiguous byte array of records, chunked across consecutive storage
 * slots at record boundaries, so a record never straddles a slot and a corrupt slot
 * never splits a record. `PRESET_SLOT_MAX` gives the records-per-slot byte ceiling. */

static uint32_t bank_per(uint32_t rec) { return PRESET_SLOT_MAX / rec; }

/* the number of storage slots a bank of `count` records of `rec` bytes spans */
static uint32_t bank_slots(uint32_t count, uint32_t rec)
{
    uint32_t per = bank_per(rec);
    return (count + per - 1u) / per;
}

static int bank_save(const uint8_t *bank, uint32_t count, uint32_t rec, uint32_t slot0)
{
    uint32_t per = bank_per(rec), off = 0;
    while (off < count) {
        uint32_t n = count - off < per ? count - off : per;
        if (slot_save(slot0++, bank + off * rec, n * rec) != 0)
            return -1;
        off += n;
    }
    return 0;
}

static int bank_load(uint8_t *bank, uint32_t count, uint32_t rec, uint32_t slot0)
{
    uint32_t per = bank_per(rec), off = 0;
    while (off < count) {
        uint32_t n = count - off < per ? count - off : per;
        if (slot_load(slot0++, bank + off * rec, n * rec) != 0)
            return -1;
        off += n;
    }
    return 0;
}

#define PART_BANK_SLOT 0u /* the part bank starts at flash slot 0, the multi bank follows */

int preset_part_bank_save(const uint8_t *bank, uint32_t n)
{
    return bank_save(bank, PRESET_PART_SLOTS, PP_RECORD(n), PART_BANK_SLOT);
}
int preset_part_bank_load(uint8_t *bank, uint32_t n)
{
    return bank_load(bank, PRESET_PART_SLOTS, PP_RECORD(n), PART_BANK_SLOT);
}
int preset_multi_bank_save(const uint8_t *bank, uint32_t n, uint32_t p)
{
    return bank_save(bank, PRESET_MULTI_SLOTS, MP_RECORD(p),
                     PART_BANK_SLOT + bank_slots(PRESET_PART_SLOTS, PP_RECORD(n)));
}
int preset_multi_bank_load(uint8_t *bank, uint32_t n, uint32_t p)
{
    return bank_load(bank, PRESET_MULTI_SLOTS, MP_RECORD(p),
                     PART_BANK_SLOT + bank_slots(PRESET_PART_SLOTS, PP_RECORD(n)));
}
int preset_settings_save(const uint8_t *rec) { return settings_save(rec, ST_RECORD); }
int preset_settings_load(uint8_t *rec) { return settings_load(rec, ST_RECORD); }
