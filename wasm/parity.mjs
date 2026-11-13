#!/usr/bin/env node
/* SPDX-License-Identifier: GPL-3.0-only */
/* The first WASM gate: the engine render must match the native 48 kHz capture
 * sample for sample before any audio or panel work. */
import fs from "node:fs";

const [wasmPath, referencePath] = process.argv.slice(2);
if (wasmPath === undefined || referencePath === undefined) {
  console.error("usage: node wasm/parity.mjs <p1.wasm> <reference.json>");
  process.exit(2);
}

const bytes = fs.readFileSync(wasmPath);
const module = await WebAssembly.compile(bytes);
const imports = WebAssembly.Module.imports(module);
if (imports.length > 0) {
  console.error("wasm: unexpected imports", imports);
  process.exit(1);
}
const instance = await WebAssembly.instantiate(module, {});
const e = instance.exports;
const reference = JSON.parse(fs.readFileSync(referencePath, "utf8"));

const fail = (message) => {
  console.error(`wasm: ${message}`);
  process.exit(1);
};

if (typeof e.memory !== "object")
  fail("the module does not export memory");

e.p1_create();
if (e.p1_param_count() !== 846)
  fail(`parameter count is ${e.p1_param_count()}, expected 846`);

e.p1_reset();
/* The native capture uses the omni channel, so the parity call does too. */
e.p1_note_on(60, 100, 0xFFFFFFFF);
e.p1_render(256);
const output = new Int32Array(e.memory.buffer, e.p1_output(), 512);
if (output.length !== reference.noteOn.length)
  fail("the note-on render length does not match the reference");
for (let i = 0; i < output.length; ++i) {
  if (output[i] !== reference.noteOn[i])
    fail(`note-on sample ${i}: ${output[i]} != ${reference.noteOn[i]}`);
}

e.p1_note_off(60);
e.p1_render(256);
const release = new Int32Array(e.memory.buffer, e.p1_output(), 512);
if (release.length !== reference.release.length)
  fail("the release render length does not match the reference");
for (let i = 0; i < release.length; ++i) {
  if (release[i] !== reference.release[i])
    fail(`release sample ${i}: ${release[i]} != ${reference.release[i]}`);
}

console.log("wasm: 846 parameters, note-on and release match the 48 kHz capture");
