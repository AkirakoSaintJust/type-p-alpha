/* SPDX-License-Identifier: GPL-3.0-only */
const startButton = document.querySelector("#start");
const noteButton = document.querySelector("#note");
const level = document.querySelector("#level");
const readout = document.querySelector("#readout");
const status = document.querySelector("#status");
const scope = document.querySelector("#scope");
const context2d = scope.getContext("2d");

let audioContext = null;
let node = null;
let levelSpec = null;
let noteHeld = false;

window.__p1 = {
  ready: false,
  rate: 0,
  paramCount: 0,
  lastParam: null,
  lastScopeMax: 0,
  error: null,
};

function drawScope(samples) {
  const width = scope.width;
  const height = scope.height;
  context2d.fillStyle = "#000000";
  context2d.fillRect(0, 0, width, height);
  context2d.strokeStyle = "#1c1c1c";
  context2d.lineWidth = 1;
  for (let i = 1; i < 8; ++i) {
    const x = (i / 8) * width;
    context2d.beginPath();
    context2d.moveTo(x, 0);
    context2d.lineTo(x, height);
    context2d.stroke();
  }
  context2d.beginPath();
  context2d.moveTo(0, height / 2);
  context2d.lineTo(width, height / 2);
  context2d.strokeStyle = "#2e2e2e";
  context2d.stroke();
  context2d.beginPath();
  for (let i = 0; i < samples.length; ++i) {
    const x = (i / (samples.length - 1)) * width;
    const y = height / 2 - Math.tanh(samples[i] * 4) * height * 0.42;
    if (i === 0) context2d.moveTo(x, y);
    else context2d.lineTo(x, y);
  }
  context2d.strokeStyle = "#3dff9e";
  context2d.lineWidth = 2;
  context2d.stroke();
}

function noteOn() {
  if (node === null || noteHeld) return;
  noteHeld = true;
  node.port.postMessage({
    type: "note",
    on: true,
    note: 60,
    velocity: 100,
    part: 0,
  });
}

function noteOff() {
  if (node === null || !noteHeld) return;
  noteHeld = false;
  node.port.postMessage({ type: "note", on: false, note: 60 });
}

async function start() {
  startButton.disabled = true;
  status.textContent = "loading engine";
  try {
    audioContext = new AudioContext({
      sampleRate: 48000,
      latencyHint: "interactive",
    });
    await audioContext.resume();

    const params = await (await fetch("params.json")).json();
    levelSpec = params.params.find((parameter) => parameter.name === "op1_level.p0");
    if (levelSpec === undefined) throw new Error("op1_level.p0 is not in params.json");
    level.min = String(levelSpec.min);
    level.max = String(levelSpec.max);
    level.value = String(levelSpec.default);
    readout.textContent = String(level.value);

    const bytes = await (await fetch("p1.wasm")).arrayBuffer();
    await audioContext.audioWorklet.addModule("p1-worklet.js");
    node = new AudioWorkletNode(audioContext, "p1-engine", {
      numberOfInputs: 0,
      numberOfOutputs: 1,
      outputChannelCount: [2],
    });
    node.port.onmessage = (event) => {
      const message = event.data;
      if (message.type === "ready") {
        const rate = audioContext.sampleRate;
        const warning = rate === 48000 ? "" : ` · context is ${rate} Hz, engine is 48000 Hz`;
        status.textContent = `ready · ${rate} Hz · ${message.paramCount} parameters${warning}`;
        window.__p1.ready = true;
        window.__p1.rate = rate;
        window.__p1.paramCount = message.paramCount;
        node.port.postMessage({
          type: "param",
          id: levelSpec.id,
          value: Number(level.value),
        });
      } else if (message.type === "scope") {
        let peak = 0;
        for (const sample of message.samples)
          peak = Math.max(peak, Math.abs(sample));
        window.__p1.lastScopeMax = peak;
        drawScope(message.samples);
      } else if (message.type === "param") {
        window.__p1.lastParam = message.value;
      } else if (message.type === "error") {
        window.__p1.error = message.message;
        status.textContent = `engine error: ${message.message}`;
      }
    };
    node.connect(audioContext.destination);
    node.port.postMessage({ type: "module", bytes }, [bytes]);
  } catch (error) {
    window.__p1.error = error.message;
    status.textContent = `start failed: ${error.message}`;
    startButton.disabled = false;
  }
}

startButton.addEventListener("click", start);

noteButton.addEventListener("pointerdown", (event) => {
  event.preventDefault();
  noteOn();
});
noteButton.addEventListener("pointerup", noteOff);
noteButton.addEventListener("pointerleave", noteOff);
noteButton.addEventListener("pointercancel", noteOff);
window.addEventListener("blur", noteOff);

level.addEventListener("input", () => {
  readout.textContent = level.value;
  if (node !== null && levelSpec !== null) {
    node.port.postMessage({
      type: "param",
      id: levelSpec.id,
      value: Number(level.value),
    });
  }
});

drawScope(new Float32Array(512));
