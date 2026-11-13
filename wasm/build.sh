#!/bin/sh
# SPDX-License-Identifier: GPL-3.0-only
# Build the standalone web engine and run the native parity gate.
set -eu
cd "$(dirname "$0")/.."

out="build/web"
mkdir -p "$out"

emcc engine/gen/type_p_alpha_dsp.c engine/gen/tables.c wasm/p1_wasm.c \
    -Iengine/gen -Iengine/hal \
    -O3 -flto -sSTANDALONE_WASM=1 --no-entry \
    -sEXPORTED_FUNCTIONS='["_p1_create","_p1_reset","_p1_set_param","_p1_get_param","_p1_note_on","_p1_note_off","_p1_all_notes_off","_p1_set_bend","_p1_render","_p1_output","_p1_tap","_p1_param_count"]' \
    -sINITIAL_MEMORY=33554432 -sALLOW_MEMORY_GROWTH=0 \
    -o "$out/p1.wasm"

python3 wasm/make_reference.py > "$out/reference.json"
node wasm/parity.mjs "$out/p1.wasm" "$out/reference.json"

cp web/index.html web/main.js web/p1-worklet.js "$out"/
cp web/panel.html web/panel.js web/panel.css "$out"/
cp engine/gen/params.json "$out/params.json"
cp design/tokens.css "$out/tokens.css"
