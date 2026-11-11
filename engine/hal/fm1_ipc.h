/* SPDX-License-Identifier: GPL-3.0-only
 * Copyright (C) 2026 P1Onken
 *
 * Dual-core DSP transport (docs/roadmap.md §12.1, synchronous one-job model).
 * Core 0 produces note events, core 1 renders each block as a job handed over
 * through hal/fm1_cpu1.h's mailbox. The shared state below sits in plain .bss,
 * 64-byte aligned, with csync barriers, because the cores share the caches (fm1_cpu1.h),
 * so ordering is the only concern. There is no uncached alias on this chip.
 *
 *   note queue    core 0 -> core 1   SPSC, 32-bit indices, acquire/release
 *   status        core 1 -> core 0   quiet / lit (single words)
 *   bend          core 0 -> core 1   shared value, single writer = core 0
 *
 * The ping-pong audio-block ring of §12.1 is deferred to P2-8 (the generator
 * transport), the synchronous model here renders straight into the ISR's buffer.
 */
#pragma once
#include <stdint.h>

#ifndef FM1_CPU1_USED
#define FM1_CPU1_USED 0
#endif

enum {
    UMETATE_DSP_CMD_NOTE_ON  = 1u,
    UMETATE_DSP_CMD_NOTE_OFF = 2u,
    UMETATE_DSP_CMD_ALL_OFF  = 3u,
    UMETATE_DSP_CMD_CAP      = 64u,   /* power of two: index mask is CAP-1 */
    UMETATE_DSP_BLOCK_FRAMES = 256u,
};

/* One note event (6 bytes, and the queue holds 64 of them). */
typedef struct {
    uint8_t  kind;
    uint8_t  ch;       /* the note's MIDI channel (0..15, 0xFF = omni) */
    uint16_t note;
    uint16_t vel;
} dsp_cmd;

/* Reverse status, single words each (core 1 writes, core 0 reads). */
typedef struct {
    volatile uint32_t quiet;    /* 1 = all voices silent */
    volatile uint32_t lit;      /* sounding note keys, 27-bit (FM-1) */
} dsp_status_t;

/* Shared state, defined once (src/rib_dsp.c), referenced by both cores. */
extern volatile dsp_cmd     dsp_cmd_buf[UMETATE_DSP_CMD_CAP];
extern volatile uint32_t    dsp_cmd_head;                 /* core 0 writes */
extern volatile uint32_t    dsp_cmd_tail;                 /* core 1 writes */
extern volatile uint32_t    dsp_cmd_overrun;              /* dropped note-ons */
extern volatile uint32_t    dsp_all_off_pending;          /* a note-off coalesced */
extern volatile dsp_status_t dsp_status;
extern volatile int32_t     bend_q16;                     /* core 0 writes */
extern volatile uint32_t    lit_base;                     /* core 0 writes: the octave's base note */

/* ------------------------------------------------------------------ mailbox */
/* Thin non-inline wrappers over fm1_cpu1.h's static-inline mailbox, so a generated
 * TU (the split render) can hand core 1 work without pulling in fm1_cpu1_main (the
 * core-1 loop must stay a single TU). Defined in platform.c (which owns fm1_cpu1.h). */
#if FM1_CPU1_USED
int fm1_cpu1_dispatch(void (*fn)(uint32_t), uint32_t arg);   /* 1 = handed off */
int fm1_cpu1_join(void);                                     /* 0 = done, -1 = gave up */
#endif

/* ------------------------------------------------------------------ barrier */
#if FM1_CPU1_USED
#define FM1_CSYNC() __asm__ volatile("csync" ::: "memory")
#else
#define FM1_CSYNC() ((void)0)
#endif

/* -------------------------------------------------------- command queue (SPSC) */
/* Producer = core 0 (main loop). Never drops a note-off: on overflow a note-on is
 * dropped (overrun counted) and a note-off/all-off sets all_off_pending, so a
 * voice can never stick on. */
static inline void dsp_cmd_push(uint8_t kind, uint8_t ch, uint16_t note, uint16_t vel)
{
    uint32_t head = dsp_cmd_head;
    if (head - dsp_cmd_tail >= UMETATE_DSP_CMD_CAP) {          /* full */
        if (kind == UMETATE_DSP_CMD_NOTE_ON) {                 /* droppable */
            dsp_cmd_overrun++;
            return;
        }
        dsp_all_off_pending = 1;                                /* never lose a note-off */
        dsp_cmd_overrun++;
        FM1_CSYNC();
        return;
    }
    {
        dsp_cmd c = {kind, ch, note, vel};
        dsp_cmd_buf[head & (UMETATE_DSP_CMD_CAP - 1u)] = c;
    }
    FM1_CSYNC();                                                /* release */
    dsp_cmd_head = head + 1u;
}

/* Consumer = core 1 (block start). Returns 1 when a command was popped. */
static inline uint32_t dsp_cmd_pop(dsp_cmd *out)
{
    uint32_t tail = dsp_cmd_tail;
    if (tail == dsp_cmd_head)
        return 0;                                               /* empty */
    FM1_CSYNC();                                                /* acquire */
    {
        const volatile dsp_cmd *slot =
            &dsp_cmd_buf[tail & (UMETATE_DSP_CMD_CAP - 1u)];
        out->kind = slot->kind;
        out->ch = slot->ch;
        out->note = slot->note;
        out->vel = slot->vel;
    }
    dsp_cmd_tail = tail + 1u;
    return 1u;
}

static inline int dsp_cmd_empty(void) { return dsp_cmd_head == dsp_cmd_tail; }
