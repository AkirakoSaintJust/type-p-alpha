/* SPDX-License-Identifier: GPL-3.0-only */
class P1Processor extends AudioWorkletProcessor {
  constructor() {
    super();
    this.exports = null;
    this.scope = new Float32Array(1024);
    this.scopePosition = 0;
    this.activePart = 0;
    this.tapRings = [];
    this.routeRings = [new Float32Array(512), new Float32Array(512)];
    this.tapPosition = 0;
    this.tapBlocks = 0;
    this.engineRate = 48000;
    this.ratio = sampleRate === this.engineRate
      ? 1 : this.engineRate / sampleRate;
    this.phase = 0;
    this.pendingLeft = new Float32Array(0);
    this.pendingRight = new Float32Array(0);
    this.resetTapRings();
    this.port.onmessage = (event) => this.handle(event.data);
  }

  resetTapRings() {
    this.tapRings = Array.from({ length: 5 }, () => new Float32Array(512));
    this.routeRings = [new Float32Array(512), new Float32Array(512)];
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
      case "analyze": {
        e.p1_analyze(message.wave, message.first);
        const pointer = e.p1_analysis();
        const levels = Array.from(
          new Int32Array(e.memory.buffer, pointer, 16));
        this.port.postMessage({
          type: "analysis",
          key: message.key,
          request: message.request,
          wave: message.wave,
          first: message.first,
          levels,
        });
        break;
      }
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
    let engineFrames;
    if (this.ratio === 1) {
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
      engineFrames = frames;
    } else {
      engineFrames = this.renderResampled(frames, left, right);
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
    this.captureTaps(engineFrames);
    return true;
  }

  renderResampled(frames, left, right) {
    const e = this.exports;
    const need = Math.ceil(this.phase + frames * this.ratio) + 1
      - this.pendingLeft.length;
    if (need > 0) {
      e.p1_render(need);
      const samples = new Int32Array(e.memory.buffer, e.p1_output(), need * 2);
      const mergedLeft = new Float32Array(this.pendingLeft.length + need);
      const mergedRight = new Float32Array(this.pendingRight.length + need);
      mergedLeft.set(this.pendingLeft);
      mergedRight.set(this.pendingRight);
      for (let i = 0; i < need; ++i) {
        mergedLeft[this.pendingLeft.length + i] = samples[i * 2] / 32768;
        mergedRight[this.pendingRight.length + i] =
          samples[i * 2 + 1] / 32768;
      }
      this.pendingLeft = mergedLeft;
      this.pendingRight = mergedRight;
    }
    let phase = this.phase;
    const pendingLeft = this.pendingLeft;
    const pendingRight = this.pendingRight;
    for (let i = 0; i < frames; ++i) {
      const index = Math.min(Math.floor(phase), pendingLeft.length - 2);
      const frac = phase - index;
      const a = pendingLeft[index];
      const b = pendingLeft[index + 1];
      const c = pendingRight[index];
      const d = pendingRight[index + 1];
      left[i] = a + (b - a) * frac;
      right[i] = c + (d - c) * frac;
      phase += this.ratio;
    }
    const consumed = Math.min(Math.floor(phase), pendingLeft.length - 1);
    if (consumed > 0) {
      this.pendingLeft = pendingLeft.subarray(consumed).slice();
      this.pendingRight = pendingRight.subarray(consumed).slice();
      phase -= consumed;
    }
    this.phase = phase;
    return Math.max(0, need);
  }

  captureTaps(engineFrames) {
    if (engineFrames <= 0) return;
    const e = this.exports;
    const count = Math.min(engineFrames, this.tapRings[0].length);
    if (this.tapPosition + count > this.tapRings[0].length) {
      this.tapPosition = 0;
      ++this.tapBlocks;
    }
    const tapChannels = [
      this.activePart,
      2 + this.activePart,
      4 + this.activePart,
      6 + this.activePart,
      8 + this.activePart,
    ];
    for (let c = 0; c < tapChannels.length; ++c) {
      const pointer = e.p1_tap(tapChannels[c]);
      const tap = new Int32Array(e.memory.buffer, pointer, engineFrames);
      const ring = this.tapRings[c];
      for (let i = 0; i < count; ++i)
        ring[this.tapPosition + i] = tap[i] / 32768;
    }
    for (let part = 0; part < 2; ++part) {
      const pointer = e.p1_tap(8 + part);
      const tap = new Int32Array(e.memory.buffer, pointer, engineFrames);
      const ring = this.routeRings[part];
      for (let i = 0; i < count; ++i)
        ring[this.tapPosition + i] = tap[i] / 32768;
    }
    this.tapPosition += count;
    if (this.tapPosition >= this.tapRings[0].length) {
      this.tapPosition = 0;
      ++this.tapBlocks;
    }
    if (this.tapBlocks >= 8) {
      this.port.postMessage({
        type: "taps",
        part: this.activePart,
        channels: this.tapRings.map((ring) => ring.slice()),
        routes: this.routeRings.map((ring) => ring.slice()),
      });
      this.tapBlocks = 0;
    }
  }
}

registerProcessor("p1-engine", P1Processor);
