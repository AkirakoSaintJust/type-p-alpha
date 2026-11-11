/* SPDX-License-Identifier: GPL-3.0-only */
/* Host build of the engine preset record code. The plugin keeps its banks in
 * memory, so the flash bank services are stubs. The pack and unpack functions are
 * the engine's own code. */
#include <stdint.h>
#include <string.h>

int slot_save(uint32_t i, const void *p, uint32_t n)
{
    (void)i;
    (void)p;
    (void)n;
    return -1;
}

int slot_load(uint32_t i, void *p, uint32_t n)
{
    (void)i;
    (void)p;
    (void)n;
    return -1;
}

int settings_save(const void *p, uint32_t n)
{
    (void)p;
    (void)n;
    return -1;
}

int settings_load(void *p, uint32_t n)
{
    (void)p;
    (void)n;
    return -1;
}

#include "../../engine/runtime/presets.c"
