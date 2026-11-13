/* SPDX-License-Identifier: GPL-3.0-only */
class P1Processor extends AudioWorkletProcessor {
  constructor() {
    super();
    this.exports = null;
    this.scope = new Float32Array(1024);
    this.scopePosition = 0;
    this.activePart = 0;
    this.tapRings = [];
    this.tapPosition = 0;
    this.tapBlocks = 0;
    this.resetTapRings();
    this.port.onmessage = (event) => this.handle(event.data);
  }

  resetTapRings() {
    this.tapRings = Array.from({ length: 5 }, () => new Float32Array(512));
    this.tapPosition = 0;
    this.tapBlocks = 0;
  }

  async handle(message) {
    if (message.type === "module") {
      try {
        const { instance } = await WebAssembly.instantiate(message.bytes, {});
        this.exports = instance.exports;
        this.exports.p1_create();
        this.port.postMessage({
          type: "ready",
          paramCount: this.exports.p1_param_count(),
        });
      } catch (error) {
        this.port.postMessage({ type: "error", message: String(error) });
      }
      return;
    }
    if (this.exports === null) return;
    const e = this.exports;
    switch (message.type) {
      case "param":
        e.p1_set_param(message.id, message.value);
        this.port.postMessage({
          type: "param",
          id: message.id,
          value: e.p1_get_param(message.id),
        });
        break;
      case "note":
        if (message.on)
          e.p1_note_on(message.note, message.velocity, message.part);
        else
          e.p1_note_off(message.note);
        break;
      case "allNotesOff":
        e.p1_all_notes_off();
        break;
      case "bend":
        e.p1_set_bend(message.value);
        break;
      case "part":
        this.activePart = message.part === 1 ? 1 : 0;
        this.resetTapRings();
        break;
      case "params":
        for (const [id, value] of message.values)
          e.p1_set_param(id, value);
        break;
      default:
        break;
    }
  }

  process(_inputs, outputs) {
    const out = outputs[0];
    const left = out[0];
    const right = out.length > 1 ? out[1] : left;
    if (this.exports === null) {
      left.fill(0);
      if (right !== left) right.fill(0);
      return true;
    }
    const frames = left.length;
    this.exports.p1_render(frames);
    const samples = new Int32Array(
      this.exports.memory.buffer,
      this.exports.p1_output(),
      frames * 2,
    );
    for (let i = 0; i < frames; ++i) {
      left[i] = samples[i * 2] / 32768;
      right[i] = samples[i * 2 + 1] / 32768;
    }
    for (let i = 0; i < frames; ++i) {
      this.scope[this.scopePosition++] = left[i];
      if (this.scopePosition === this.scope.length) {
        this.port.postMessage({
          type: "scope",
          samples: this.scope.slice(512),
        });
        this.scopePosition = 0;
      }
    }
    const tapChannels = [
      this.activePart,
      2 + this.activePart,
      4 + this.activePart,
      6 + this.activePart,
      8 + this.activePart,
    ];
    for (let c = 0; c < tapChannels.length; ++c) {
      const pointer = this.exports.p1_tap(tapChannels[c]);
      const tap = new Int32Array(this.exports.memory.buffer, pointer, frames);
      const ring = this.tapRings[c];
      for (let i = 0; i < frames; ++i)
        ring[this.tapPosition + i] = tap[i] / 32768;
    }
    this.tapPosition += frames;
    if (this.tapPosition >= 512) {
      this.tapPosition = 0;
      ++this.tapBlocks;
      if (this.tapBlocks >= 8) {
        this.port.postMessage({
          type: "taps",
          part: this.activePart,
          channels: this.tapRings.map((ring) => ring.slice()),
        });
        this.tapBlocks = 0;
      }
    }
    return true;
  }
}

registerProcessor("p1-engine", P1Processor);
