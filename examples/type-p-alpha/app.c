/* SPDX-License-Identifier: GPL-3.0-only */
/* Copyright (C) 2026 P1Onken (umetate) */
/* type-p-alpha's app half of the frozen contract (docs/roadmap.md §6.11.4, F7).
 * It is the glue between the generated engine, the generated panel, the platform,
 * and the framework's synth services, and the framework's src/rib.c includes it
 * in the `umetate build --synth examples/type-p-alpha` composition.
 *
 * The generic preset save/load/init/name flow and the modal popup state machine
 * are the framework's `synth_app.c`, which every project shares. This file keeps
 * only type-p-alpha's control map, its engine<->panel mirror, and its credits page.
 *
 * Two independent-timbre parts. Part 0 is MIDI channel 0, where the panel and
 * the CC surface edit part 0's timbre. Part 1 is channel 1, whose timbre comes
 * from the multi patch or SysEx. The engine owns the param truth, and the
 * panel's values are mirrored in and out every pass, so a control change
 * reaches the sound and a patch load reaches the screen, and the engine stays
 * the only owner of the value. */
#include "dsp.h"            /* the generated seam prototypes (gen/dsp.h) */
#include "glue.h"           /* the generated seam adapters + app_audio (gen/glue.h) */
#include "cc_map.h"         /* the generated cc -> param + [min, max] tables */
#include "synth_layout.h"   /* the generated model byte + param/part counts */
#include "panel.h"          /* the generated UI tables (gen/panel.h) */
#include "presets.h"        /* the framework's record store */
#include "synth_midi.h"     /* the framework's MIDI handler */
#include "synth_sysex.h"    /* the framework's SysEx handler */
#include "synth_runtime.h"  /* the framework's preset apply */
#include "synth_app.h"      /* the framework's generic app core (presets + popups) */

#if defined(UMETATE_BENCH) && !defined(UMETATE_HOST)
/* The bench build's platform.c times the audio path. These two counters time the
   main loop's poll and redraw, which are the UI and USB cost. */
extern volatile uint32_t umetate_bench_poll_ticks;
extern volatile uint32_t umetate_bench_ui_ticks;
extern volatile uint32_t umetate_bench_app_ticks;
#endif

/* ---- the preset store's writable memory (the banks + the settings record) ----
 * The 128 single-part records (40 576 B) do not fit CPU0's 96 KiB .bss alongside
 * the rest of the firmware. The banks sit in .pool, the big-buffer region in
 * app.ld. Flash is the persistence, and RAM is just the working copy reloaded
 * each boot. */
static uint8_t part_bank[PP_BANK(SYNTH_N_PARAMS)] __attribute__((section(".pool")));
static uint8_t multi_bank[MP_BANK(SYNTH_N_PARTS)] __attribute__((section(".pool")));
static uint8_t settings[ST_RECORD];

/* the shared app core: the preset banks + the generated seam, driven by the UI's
 * SAVE/INIT/name/browser actions (synth_app.c). */
static synth_app_ctx app;

/* ---- the engine seam + the audio callback are generated glue (gen/glue.h). */
static uint32_t idle_ok;                            /* MIDI may poll while the screen draws */
static synth_midi_ctx midi_ctx = {
    type_p_alpha_glue_note_on_ch, type_p_alpha_glue_note_off, type_p_alpha_glue_all_off,
    type_p_alpha_glue_set_param, type_p_alpha_glue_bend,
    cc_to_param, param_min, param_max, 0u, {0},
};
static synth_sysex_ctx sysex_ctx = {
    SYNTH_MODEL, SYNTH_N_PARAMS, SYNTH_N_PARTS, type_p_alpha_glue_get_param,
    part_bank, multi_bank, settings,
};

/* the generated init table, read through a callback (no type-punning) */
static void type_p_alpha_get_init(uint32_t i, uint16_t *id, int32_t *value)
{
    *id = type_p_alpha_init[i].id;
    *value = type_p_alpha_init[i].value;
}

/* ---- the panel <-> engine mirror: the engine owns the truth */
static int32_t last_ui[UI_PARAM_MAX];
static void ui_sync(void)
{
    int32_t *v = ui_params();
    uint32_t i;
    for (i = 0; i < PANEL.n_params; i++) {
        int32_t e = type_p_alpha_glue_get_param(i);
        if (v[i] != e) {
            if (last_ui[i] != v[i]) {                 /* the panel wrote it */
                type_p_alpha_glue_set_param(i, v[i]);
                e = type_p_alpha_glue_get_param(i);      /* the clamped value */
            }
            v[i] = e;
        }
        last_ui[i] = v[i];
    }
    /* part mute: force a muted part's `level` routing param to 0 (the UI's level
     * value is preserved, unmuting restores it through the normal mirror). */
    for (i = 0; i < SYNTH_N_PARTS; i++) {
        if (ui_part_muted((uint8_t)i))
            type_p_alpha_glue_set_param(SYNTH_ROUTING_BASE + 3u * SYNTH_N_PARTS + i, 0);
    }
}

/* ---- the app's action hooks (called by ui.c). The generic bodies are the
 * framework's synth_app_* ---- */
void ui_act_note_on(uint8_t note, uint8_t vel)
{
    type_p_alpha_glue_note_on(note, vel);               /* omni: every matching part */
}
void ui_act_note_off(uint8_t note)
{
    type_p_alpha_glue_note_off(note);
}
void ui_act_set_step(uint8_t step) { (void)step; }
void ui_act_panic(void) { type_p_alpha_glue_all_off(); }
void ui_act_play(void) { }
void ui_act_stop(void) { }
void ui_act_save(void) { synth_app_save(&app); }
void ui_act_init_patch(void)
{
    synth_app_init_patch(&app);
    ui_part_mute_set(1u, 1u);                         /* the spec: part 2 muted on init */
}
void ui_act_init_part(void) { synth_app_init_part(&app); }
void ui_act_name(uint8_t which) { ui_name_editor_begin(which); }
void ui_save_slot_name(uint8_t kind, uint16_t slot, char *out, uint32_t cap)
{
    synth_app_slot_name(&app, kind, slot, out, cap);
}
void ui_preset_apply(uint8_t kind, uint16_t slot) { synth_app_apply(&app, kind, slot); }

/* the credits page (a UI_CUSTOM widget): the firmware version + credits, drawn
 * static (it fits the widget rows, no SELECT-scroll). The typeface credit is
 * required by the font licence. */
static const char *const CREDITS[] = {
    "umetate",
    "Type P.alpha  (4-op FM)",
    "firmware  FM-1_990",
    "",
    "Logo typeface:",
    "Digital Desolation Plus",
    "by ukiyomoji.booth.pm",
    "",
    "Copyright (C) 2026",
    "P1Onken",
    "",
    "Licence: GPL-3.0-or-later",
};
#define CREDITS_N ((int32_t)(sizeof(CREDITS) / sizeof(CREDITS[0])))

void ui_draw_custom(const ui_widget *w)
{
    /* the widget's pixel region (col 0 = full width, 16 px rows, 120 px cols). */
    uint32_t x = (uint32_t)w->col * 120u;
    uint32_t y = (uint32_t)w->row * 16u;
    uint32_t wd = x ? 120u : 240u;
    uint32_t ht = (uint32_t)(w->span ? w->span : 1u) * 16u;
    uint32_t rows = ht / 16u;
    int32_t i;
    (void)wd;
    cv_begin(wd, ht, C_BLACK);
    for (i = 0; i < (int32_t)rows && i < CREDITS_N; i++)
        cv_text(0, i * 16, &FONT_S, CREDITS[i], C_TEXT);   /* the body: white */
    cv_blit(x, y);
}

/* ---- MIDI: the framework handlers, from the main loop and while the screen draws */
static void app_idle(void)
{
    if (idle_ok)
        synth_midi_poll(&midi_ctx);
}

#define KEY0 53u   /* key 0 = F3 */
#define GLO_CTX 5u /* the GLO context id (panel.json key order: home/edit/env/lfo/fx/glo) */

/* ---- the main loop, split for the browser emulator: app_init (the setup) + app_step
   (one millisecond of the loop). app_main keeps the frozen contract for the
   host and target builds. A web build drives app_init + app_step from an
   AudioWorklet instead of the blocking loop. */
static uint32_t app_prev, app_lit;
static uint8_t app_held[27];
static int32_t app_oct;
static uint32_t app_last_scl_ms, app_scl_pending, app_scl_pending_ms;
static uint32_t app_alg_ms;                         /* the last ALGORITHM-knob turn (popup auto-close) */

static void app_init(void)
{
    app_prev = 0;
    app_lit = 0;
    memset(app_held, 0, sizeof app_held);
    app_oct = 0;
    app_last_scl_ms = app_scl_pending = app_scl_pending_ms = 0;
    app_alg_ms = 0;

    type_p_alpha_glue_reset();
#if FM1_CPU1_USED
    {
        fm1_guard_unlock_top();
        fm1_guard_pc_open();
        (void)fm1_cpu1_start();
        fm1_guard_enable(FM1_GUARD_PC);
        fm1_guard_lock_top();
    }
    lit_base = KEY0;
#endif

    /* the generic app core: the generated seam + layout + init table + banks. */
    app.get_param = type_p_alpha_glue_get_param;
    app.set_param = type_p_alpha_glue_set_param;
    app.reset = type_p_alpha_glue_reset;
    app.n_params = SYNTH_N_PARAMS;
    app.n_parts = SYNTH_N_PARTS;
    app.routing_base = SYNTH_ROUTING_BASE;
    app.get_init = type_p_alpha_get_init;
    app.init_n = type_p_alpha_INIT_N;
    app.part_bank = part_bank;
    app.multi_bank = multi_bank;
    app.settings = settings;

    ui_init(&PANEL);
    synth_app_boot(&app);                              /* load banks, apply multi 0, seed names */
    ui_sync();                                         /* the engine's defaults reach the screen */
#ifdef UMETATE_BENCH_PAGE
    ui_bench_page((uint8_t)UMETATE_BENCH_PAGE);        /* bench: measure a dynamic page */
#endif
}

static void app_step(void)
{
    uint32_t keys, down, up, b, rel, base, k, input_seen;
    int32_t e, ch0;
#if defined(UMETATE_BENCH) && !defined(UMETATE_HOST)
    uint32_t bench_t0 = fm1_ticks();
    uint32_t bench_start = bench_t0;
#endif

    platform_poll();
#if defined(UMETATE_BENCH) && !defined(UMETATE_HOST)
    umetate_bench_poll_ticks += fm1_ticks() - bench_t0;
#endif

    keys = fm1_in.notes;
    down = keys & ~app_prev;
    up = app_prev & ~keys;
    app_prev = keys;
    input_seen = down | up;
    base = (uint32_t)((int32_t)KEY0 + 12 * app_oct);
    {
        uint32_t now = fm1_ms;
        uint32_t scl = fm1_in.buttons & (1u << BTN_SEL);
        ch0 = type_p_alpha_glue_get_param(SYNTH_ROUTING_BASE + 0u * SYNTH_N_PARTS + 0u);
        if (ch0 < 0 || ch0 > 15)
            ch0 = 0;
        if (synth_app_step(&app)) {
            /* a modal popup consumed the control this step (synth_app drained it) */
        } else {
        for (k = 0; k < 27u; k++) {                /* the keys: play + echo, or SEL chords */
            if (down >> k & 1u) {
                if (scl) {
                    ui_op_mute_active();           /* SEL + key: mute the active op, no note */
                    app_held[k] = 0u;
                } else {
                    app_held[k] = (uint8_t)(base + k);
                    ui_handle(UI_SRC_KEY, app_held[k]);
                    midi_send(0x90u | (uint32_t)ch0, app_held[k], 100);
                }
            }
            if (up >> k & 1u && app_held[k]) {
                ui_act_note_off(app_held[k]);
                midi_send(0x80u | (uint32_t)ch0, app_held[k], 0);
                app_held[k] = 0u;
            }
        }

        b = fm1_input_edges(&rel);
        if (b & (1u << BTN_EDIT)) {
            if (scl && ui_copy_available())
                ui_copy_begin();                    /* SEL+EDIT: copy the page's params */
            else
                ui_handle(UI_SRC_BTN_EDIT, -1);
        }
        if (b & (1u << BTN_HOME)) ui_handle(UI_SRC_BTN_HOME, -1);
        if (b & (1u << BTN_ENV)) ui_handle(UI_SRC_BTN_ENV, -1);
        if (b & (1u << BTN_LFO)) ui_handle(UI_SRC_BTN_LFO, -1);
        if (b & (1u << BTN_FX)) ui_handle(UI_SRC_BTN_FX, -1);
        if (b & (1u << BTN_GLO)) ui_handle(UI_SRC_BTN_GLO, -1);

        /* OCT-/OCT+: shift the keybed base note a whole octave (clamped ±2) */
        if (b & (1u << BTN_OCT_DOWN)) app_oct = app_oct > -2 ? app_oct - 1 : app_oct;
        if (b & (1u << BTN_OCT_UP))   app_oct = app_oct <  2 ? app_oct + 1 : app_oct;

        /* SEL: a tap advances the operator (deferred so a second tap within the
           window is a part switch instead). A double-tap switches the part. */
        if (b & (1u << BTN_SEL)) {
            if (now - app_last_scl_ms < 300u) {
                ui_part_next();
                app_scl_pending = 0;
            } else {
                app_scl_pending = 1;
                app_scl_pending_ms = now;
            }
            app_last_scl_ms = now;
        }

        /* ARP/SEQ/PLAY/REC: plain = operator mutes 1-4, SEL+ARP/PLAY = part 1
           mute, SEL+SEQ/REC = part 2 mute. */
        if (b & (1u << BTN_ARP)) {
            if (scl) ui_part_mute_toggle(0u);
            else ui_handle(UI_SRC_BTN_ARP, -1);
        }
        if (b & (1u << BTN_PLAY)) {
            if (scl) ui_part_mute_toggle(0u);
            else ui_handle(UI_SRC_BTN_PLAY, -1);
        }
        if (b & (1u << BTN_SEQ)) {
            if (scl) ui_part_mute_toggle(1u);
            else ui_handle(UI_SRC_BTN_SEQ, -1);
        }
        if (b & (1u << BTN_REC)) {
            if (scl) ui_part_mute_toggle(1u);
            else ui_handle(UI_SRC_BTN_REC, -1);
        }

        /* SAVE: plain = open the save chooser (single part / multi-patch).
           SEL+SAVE = INIT PATCH (INIT PART in GLO). */
        if (b & (1u << BTN_SAVE)) {
            if (scl) {
                if (ui_top() == GLO_CTX) {
                    app.pending_part = 3u;             /* init a single part */
                    ui_part_chooser_begin(ui_part(), "INIT PART");
                } else
                    ui_confirm_begin("INIT PATCH?");
            } else {
                ui_handle(UI_SRC_BTN_SAVE, -1);
            }
        }

        /* the deferred single-tap (no second tap within the window) */
        if (app_scl_pending && now - app_scl_pending_ms >= 300u) {
            ui_handle(UI_SRC_BTN_SEL, -1);
            app_scl_pending = 0;
        }
        input_seen |= b;                        /* button edges (whichever branch consumed them) */
        }
    }
    if (ui_popup_active()) {
        /* a modal popup captures all control: drain the page + preset encoders so
           a twist during the popup doesn't fire on the page once it closes. */
        fm1_enc_take(ENC_ALGORITHM);
        fm1_enc_take(ENC_SELECT);
        fm1_enc_take(ENC_KNOB1);
        fm1_enc_take(ENC_KNOB2);
        fm1_enc_take(ENC_KNOB3);
        fm1_enc_take(ENC_KNOB4);
        fm1_enc_take(ENC_PRESETS);
    } else {
        if ((e = fm1_enc_take(ENC_ALGORITHM)) != 0) {
            ui_handle(UI_SRC_ENC_ALGORITHM, e);
            input_seen = 1;
            app_alg_ms = fm1_ms;                   /* the algorithm scroll popup */
            ui_popup_alg((uint32_t)ui_params()[PANEL.alg
                           + (uint32_t)ui_part() * PANEL.part_stride]);
        }
        if ((e = fm1_enc_take(ENC_SELECT)) != 0) {
            input_seen = 1;
            if (fm1_in.buttons & (1u << BTN_SEL)) {
                app_scl_pending = 0;               /* the twist is the chord, not a tap */
                ui_op_step(e);
            } else {
                ui_handle(UI_SRC_ENC_SELECT, e);
            }
        }
        if ((e = fm1_enc_take(ENC_KNOB1)) != 0) { input_seen = 1; ui_handle(UI_SRC_ENC_KNOB1, e); }
        if ((e = fm1_enc_take(ENC_KNOB2)) != 0) { input_seen = 1; ui_handle(UI_SRC_ENC_KNOB2, e); }
        if ((e = fm1_enc_take(ENC_KNOB3)) != 0) { input_seen = 1; ui_handle(UI_SRC_ENC_KNOB3, e); }
        if ((e = fm1_enc_take(ENC_KNOB4)) != 0) { input_seen = 1; ui_handle(UI_SRC_ENC_KNOB4, e); }
    }

    /* any input this step resets the popup idle auto-close timer */
    if (input_seen)
        synth_app_input(&app);

    /* the algorithm scroll popup auto-closes ~600 ms after the knob stops */
    if (app_alg_ms && fm1_ms - app_alg_ms >= 600u) {
        ui_popup_hide();
        app_alg_ms = 0u;
    }

    /* the preset browser (framework): the PRESETS encoder opens the multi-patch
       (2-part) browser except in GLO, where it opens the single-part browser. A
       2 s idle loads the selection (or, if already loaded, just clears). */
    {
        int32_t pe = fm1_enc_take(ENC_PRESETS);
        if (pe != 0)
            synth_app_input(&app);
        synth_app_browser_step(&app, pe, fm1_ms, ui_top() == GLO_CTX ? 1u : 0u);
    }

    synth_midi_poll(&midi_ctx);
    synth_sysex_poll(&sysex_ctx);
    {
        /* The mirror walks all 303 params and reads the engine for each one. A
           knob, a key, or a MIDI CC changes a value at human speed, so 100 Hz is
           plenty and still updates the sound within 10 ms. */
        static uint32_t last_sync_ms;
        static uint8_t first_sync = 1;
        if (first_sync || fm1_ms - last_sync_ms >= 10u) {
            first_sync = 0;
            last_sync_ms = fm1_ms;
            ui_sync();
        }
    }

    /* the SAVED linger, the popup idle auto-close, and the silent-only persistence */
    synth_app_tick(&app, fm1_ms, (uint8_t)type_p_alpha_glue_quiet());

#if FM1_CPU1_USED
    lit_base = base;
    app_lit = dsp_status.lit;                    /* published by the render job */
#else
    app_lit = type_p_alpha_glue_lit(base);
#endif
    for (k = 0; k < 27u; k++)
        fm1_led_key(KEY_LED(k), app_lit >> k & 1u);
    fm1_led_key(BTN_OCT_DOWN, app_oct < 0);
    fm1_led_key(BTN_OCT_UP, app_oct > 0);

    idle_ok = 1;
#if defined(UMETATE_BENCH) && !defined(UMETATE_HOST)
    bench_t0 = fm1_ticks();
#endif
    ui_redraw(fm1_ms);
#if defined(UMETATE_BENCH) && !defined(UMETATE_HOST)
    umetate_bench_ui_ticks += fm1_ticks() - bench_t0;
    umetate_bench_app_ticks += fm1_ticks() - bench_start;
#endif
    idle_ok = 0;
}

static void app_main(void)
{
    app_init();
#ifdef UMETATE_HOST
    /* The host's platform_poll() is the time source: it advances fm1_ms once per
       call. The host runner depends on that, so it steps on every pass. */
    for (;;)
        app_step();
#else
    {
        /* The target's 10 kHz timer advances fm1_ms. The input scan and MIDI run
           in the timer ISR, and the screen updates at 30 Hz. One pass per
           millisecond is plenty for the app. The old loop ran the whole body as
           fast as the CPU allowed. It repeated the mirror, LED map, and input
           handling thousands of times a second. */
        uint32_t last = 0xFFFFFFFFu;
        for (;;) {
            if (fm1_ms != last) {
                last = fm1_ms;
                app_step();
            }
        }
    }
#endif
}
