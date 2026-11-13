/* SPDX-License-Identifier: GPL-3.0-only */
class P1Processor extends AudioWorkletProcessor {
  constructor() {
    super();
    this.exports = null;
    this.scope = new Float32Array(1024);
    this.scopePosition = 0;
    this.port.onmessage = (event) => this.handle(event.data);
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
    return true;
  }
}

registerProcessor("p1-engine", P1Processor);
