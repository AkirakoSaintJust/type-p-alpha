/* SPDX-License-Identifier: GPL-3.0-only */
(() => {
  "use strict";

  const WAVES = [
    "sine", "triangle", "half sine", "half tri", "2x sine", "2x tri",
    "2x half sine", "2x half tri", "rect sine", "rect tri", "pulse", "square",
    "saw", "spectrum",
  ];
  const LFO_WAVES = ["sine", "triangle", "saw", "inverse saw", "square", "random"];
  const RANGES = ["1-16", "17-32", "33-48", "49-64", "65-80", "81-96", "97-112", "113-128"];
  const FILTERS = ["lpf", "bpf", "hpf"];
  const PRIORITIES = ["last", "highest", "lowest"];
  const MODES = ["poly", "mono"];
  const TABS = [
    ["patch", "patch settings"],
    ["fx", "part fx/mod"],
    ["envs", "operator envs"],
    ["lfos", "operator lfos"],
    ["spectra", "operator spectra"],
    ["mod", "op spectra mod"],
  ];
  const CARRIERS = [
    [3, -1, -1, -1], [3, -1, -1, -1], [3, -1, -1, -1], [3, -1, -1, -1],
    [1, 3, -1, -1], [1, 2, 3, -1], [1, 2, 3, -1], [0, 1, 2, 3],
  ];
  const ALG_LAYOUT = [
    { pos: [[60, 6], [60, 34], [60, 62], [60, 90]], edges: [[0, 1], [1, 2], [2, 3]], car: [3] },
    { pos: [[18, 6], [92, 6], [55, 44], [55, 82]], edges: [[0, 2], [1, 2], [2, 3]], car: [3] },
    { pos: [[16, 6], [96, 6], [96, 44], [56, 82]], edges: [[1, 2], [0, 3], [2, 3]], car: [3] },
    { pos: [[80, 6], [22, 46], [138, 46], [80, 86]], edges: [[0, 1], [0, 2], [1, 3], [2, 3]], car: [3] },
    { pos: [[32, 10], [32, 50], [108, 10], [108, 50]], edges: [[0, 1], [2, 3]], car: [1, 3] },
    { pos: [[80, 8], [10, 58], [80, 58], [150, 58]], edges: [[0, 1], [0, 2], [0, 3]], car: [1, 2, 3] },
    { pos: [[24, 8], [24, 58], [90, 58], [156, 58]], edges: [[0, 1]], car: [1, 2, 3] },
    { pos: [[16, 38], [66, 38], [116, 38], [166, 38]], edges: [], car: [0, 1, 2, 3] },
  ];
  const OPW = 36;
  const OPH = 18;

  const tabsEl = document.querySelector("#tabs");
  const bodyEl = document.querySelector("#body");
  const statusEl = document.querySelector("#status");
  const algorithmEl = document.querySelector("#algorithm");
  const startButton = document.querySelector("#start");
  const noteButton = document.querySelector("#note");
  const scopeEl = document.querySelector("#scope");
  const algGraphEl = document.querySelector("#algGraph");
  const visSeg = document.querySelector("#visSeg");
  const toolbarEl = document.querySelector("#toolbar");
  const patchNameEl = document.querySelector("#patchName");
  const patchButton = document.querySelector("#patchBtn");
  const patchRenameButton = document.querySelector("#patchRename");
  const toolbarNodes = [];

  const graphUpdaters = [];
  const spectrumUpdaters = [];
  const previewCache = new Map();
  const previewRequests = new Set();

  let params = [];
  let byName = new Map();
  let values = new Map();
  let activePart = 0;
  let currentPage = "patch";
  const partMuted = [false, false];
  const partLevelBeforeMute = [99, 99];
  let audioContext = null;
  let node = null;
  let workletReady = false;
  let noteHeld = false;
  let outputSamples = null;
  let tapSamples = null;
  let tapRoutes = null;
  let visMode = "scope";
  let analyser = null;

  window.__p1 = {
    ready: false,
    rate: 0,
    paramCount: 0,
    lastParam: null,
    lastScopeMax: 0,
    lastTapPeak: 0,
    lastRoutePeak: 0,
    lastAnalysis: null,
    page: "patch",
    controlCount: 0,
    visMode: "scope",
    resampled: false,
    error: null,
    paramValue: (name) => {
      const spec = byName.get(name);
      return spec === undefined ? null : values.get(spec.id) ?? null;
    },
    analysisFor: (part, op) => getPreview(part, op),
    patchName: () => patchName,
    partName: (part) => partNames[part],
    partSlots: () => partLibrary.filter((slot) => slot !== null).length,
    multiSlots: () => multiLibrary.filter((multi) => multi.used).length,
  };

  const A0 = 135;
  const SWEEP = 270;
  function angleFor(f) { return A0 + SWEEP * f; }
  function pt(cx, cy, r, deg) {
    const a = deg * Math.PI / 180;
    return [cx + r * Math.cos(a), cy + r * Math.sin(a)];
  }
  function arc(cx, cy, r, f0, f1) {
    const a0 = angleFor(f0);
    const a1 = angleFor(f1);
    const p0 = pt(cx, cy, r, a0);
    const p1 = pt(cx, cy, r, a1);
    const large = (a1 - a0) > 180 ? 1 : 0;
    return "M " + p0[0].toFixed(2) + " " + p0[1].toFixed(2) +
      " A " + r + " " + r + " 0 " + large + " 1 " +
      p1[0].toFixed(2) + " " + p1[1].toFixed(2);
  }

  function wire(x1, y1, x2, y2) {
    const a = Math.atan2(y2 - y1, x2 - x1);
    const bx = x2 - 4.4 * Math.cos(a);
    const by = y2 - 4.4 * Math.sin(a);
    const p1x = x2 - 5.8 * Math.cos(a) + 2.4 * Math.sin(a);
    const p1y = y2 - 5.8 * Math.sin(a) - 2.4 * Math.cos(a);
    const p2x = x2 - 5.8 * Math.cos(a) - 2.4 * Math.sin(a);
    const p2y = y2 - 5.8 * Math.sin(a) + 2.4 * Math.cos(a);
    return '<line x1="' + x1 + '" y1="' + y1 + '" x2="' + bx.toFixed(1) +
      '" y2="' + by.toFixed(1) + '" stroke="#666" stroke-width="1.2"/>' +
      '<polygon points="' + x2 + "," + y2 + " " + p1x.toFixed(1) + "," +
      p1y.toFixed(1) + " " + p2x.toFixed(1) + "," + p2y.toFixed(1) +
      '" fill="#666"/>';
  }

  function arrowDown(x, y, len) {
    return '<line x1="' + x + '" y1="' + y + '" x2="' + x + '" y2="' +
      (y + len - 4) + '" stroke="#c780ff" stroke-width="1.6"/>' +
      '<polygon points="' + x + "," + (y + len) + " " + (x - 3.2) + "," +
      (y + len - 4) + " " + (x + 3.2) + "," + (y + len - 4) +
      '" fill="#c780ff"/>';
  }

  function algSvg(algorithm) {
    const layout = ALG_LAYOUT[algorithm] || ALG_LAYOUT[0];
    let graph = "";
    layout.edges.forEach((edge) => {
      const a = layout.pos[edge[0]];
      const b = layout.pos[edge[1]];
      graph += wire(a[0] + OPW / 2, a[1] + OPH, b[0] + OPW / 2, b[1]);
    });
    let minX = Infinity;
    let maxX = -Infinity;
    let minY = Infinity;
    let maxY = -Infinity;
    function fit(x, y) {
      minX = Math.min(minX, x);
      maxX = Math.max(maxX, x);
      minY = Math.min(minY, y);
      maxY = Math.max(maxY, y);
    }
    layout.pos.forEach((position, index) => {
      const isCarrier = layout.car.indexOf(index) >= 0;
      fit(position[0], position[1]);
      fit(position[0] + OPW, position[1] + OPH);
      if (isCarrier) fit(position[0] + OPW / 2, position[1] + OPH + 13);
      graph += '<rect x="' + position[0] + '" y="' + position[1] +
        '" width="' + OPW + '" height="' + OPH + '" fill="' +
        (isCarrier ? "#ff2e7e" : "#ffffff") + '"/>' +
        '<text x="' + (position[0] + OPW / 2) + '" y="' +
        (position[1] + 12.5) + '" fill="#000" font-size="9" font-weight="700" ' +
        'text-anchor="middle" font-family="DotGothic16, monospace">op' +
        (index + 1) + "</text>";
      if (isCarrier)
        graph += arrowDown(position[0] + OPW / 2, position[1] + OPH + 1, 12);
    });
    const dx = (240 - (maxX - minX)) / 2 - minX;
    const dy = (128 - (maxY - minY)) / 2 - minY;
    return '<svg viewBox="0 0 240 128" preserveAspectRatio="xMidYMid meet">' +
      '<g transform="translate(' + dx.toFixed(1) + "," + dy.toFixed(1) + ')">' +
      graph + "</g></svg>";
  }

  function makeKnob(spec) {
    const el = document.createElement("div");
    el.className = "knob";
    const dial = document.createElement("div");
    dial.className = "dial";
    dial.innerHTML =
      '<svg viewBox="0 0 42 42">' +
      '<path class="track" d="' + arc(21, 21, 16, 0, 1) + '"/>' +
      '<path class="arc" d="' + arc(21, 21, 16, 0, 0.0001) + '"/>' +
      '<line class="ptr" x1="21" y1="21" x2="21" y2="21"/>' +
      "</svg>";
    const name = document.createElement("div");
    name.className = "knob-name";
    name.textContent = spec.name;
    name.title = spec.title || spec.name;
    const val = document.createElement("div");
    val.className = "knob-val";
    el.append(dial, name, val);
    el.__spec = spec;
    el.__dial = dial;
    refreshKnob(el);

    function stepFor() {
      return spec.step != null ? spec.step : ((spec.max - spec.min) > 30 ? 1 : 0.01);
    }
    val.title = "Click to type a value";
    val.addEventListener("click", editValue);

    function editValue() {
      if (val.querySelector("input")) return;
      const input = document.createElement("input");
      input.className = "knob-input";
      input.type = "text";
      input.value = String(spec.val);
      val.textContent = "";
      val.appendChild(input);
      input.focus();
      input.select();
      let done = false;
      function finish(commit) {
        if (done) return;
        done = true;
        if (commit) {
          const n = parseFloat(input.value);
          if (isFinite(n)) {
            if (spec.onStart) spec.onStart();
            const step = stepFor();
            let v = Math.max(spec.min, Math.min(spec.max, n));
            v = Math.round(v / step) * step;
            spec.val = +v.toFixed(6);
            if (spec.onChange) spec.onChange(spec.val);
          }
        }
        refreshKnob(el);
      }
      input.addEventListener("keydown", function (event) {
        if (event.key === "Enter") {
          event.preventDefault();
          finish(true);
        } else if (event.key === "Escape") {
          event.preventDefault();
          finish(false);
        }
      });
      input.addEventListener("blur", function () { finish(true); });
    }

    dial.addEventListener("pointerdown", function (event) {
      event.preventDefault();
      dial.setPointerCapture(event.pointerId);
      if (spec.onStart) spec.onStart();
      const startY = event.clientY;
      const startVal = spec.val;
      const range = spec.max - spec.min;
      const step = stepFor();
      function move(ev) {
        const scale = ev.shiftKey ? 0.12 : 1;
        let v = startVal + (startY - ev.clientY) / 160 * range * scale;
        v = Math.max(spec.min, Math.min(spec.max, v));
        spec.val = Math.round(v / step) * step;
        refreshKnob(el);
        if (spec.onChange) spec.onChange(spec.val);
      }
      function up() {
        dial.removeEventListener("pointermove", move);
        dial.removeEventListener("pointerup", up);
      }
      dial.addEventListener("pointermove", move);
      dial.addEventListener("pointerup", up);
    });
    return el;
  }

  function refreshKnob(el) {
    const s = el.__spec;
    const frac = (s.val - s.min) / (s.max - s.min);
    const svg = el.__dial.querySelector("svg");
    svg.querySelector(".arc").setAttribute(
      "d", arc(21, 21, 16, 0, Math.max(frac, 0.0001)));
    const p = pt(21, 21, 15, angleFor(frac));
    const q = pt(21, 21, 6, angleFor(frac));
    const line = svg.querySelector(".ptr");
    line.setAttribute("x1", q[0].toFixed(2));
    line.setAttribute("y1", q[1].toFixed(2));
    line.setAttribute("x2", p[0].toFixed(2));
    line.setAttribute("y2", p[1].toFixed(2));
    el.querySelector(".knob-val").textContent = s.fmt ? s.fmt(s.val) : Math.round(s.val);
  }

  function makeSelect(options, current, onchange) {
    const select = document.createElement("select");
    select.className = "sel";
    options.forEach(function (option) {
      const item = document.createElement("option");
      item.value = option;
      item.textContent = option;
      if (option === current) item.selected = true;
      select.appendChild(item);
    });
    if (onchange)
      select.addEventListener("change", function () { onchange(select.value); });
    return select;
  }

  function makeToggle(name, on, onchange) {
    const el = document.createElement("div");
    el.className = "toggle" + (on ? " on" : "");
    el.innerHTML = '<div class="trk"><div class="thb"></div></div><div class="tname"></div>';
    const caption = el.querySelector(".tname");
    function paint() {
      const isOn = el.classList.contains("on");
      caption.textContent = (name === "on" ? "" : name + " ") + (isOn ? "on" : "off");
    }
    paint();
    el.addEventListener("click", function () {
      el.classList.toggle("on");
      paint();
      if (onchange) onchange(el.classList.contains("on"));
    });
    return el;
  }

  function envSvg(r, l, cls) {
    const W = 240;
    const H = 62;
    const widths = r.map(function (rr) { return 8 + (99 - rr) / 99 * 26; });
    const total = widths.reduce(function (a, b) { return a + b; }, 0);
    const scale = (W - 12) / total;
    let x = 6;
    let d = "M 6 " + H;
    const levels = [l[0], l[1], l[2], l[2]];
    for (let i = 0; i < 4; ++i) {
      x += widths[i] * scale;
      const y = H - 3 - levels[i] / 99 * (H - 10);
      d += " L " + x.toFixed(1) + " " + y.toFixed(1);
    }
    x += widths[3] * scale;
    d += " L " + x.toFixed(1) + " " + H;
    return '<svg viewBox="0 0 ' + W + " " + H + '">' +
      '<line class="egrid" x1="6" y1="' + (H - 3) + '" x2="' + (W - 6) +
      '" y2="' + (H - 3) + '"/>' +
      '<path class="' + (cls || "ecurve") + '" d="' + d + '"/></svg>';
  }

  function penvSvg(r, l, scaleMax) {
    const W = 240;
    const H = 62;
    const mid = H / 2;
    const top = scaleMax || 12;
    const widths = r.map(function (rr) { return 8 + (99 - rr) / 99 * 26; });
    const total = widths.reduce(function (a, b) { return a + b; }, 0);
    const scale = (W - 12) / total;
    let x = 6;
    let d = "M 6 " + mid;
    const levels = l.length >= 4 ? [l[0], l[1], l[2], l[3]]
      : [l[0], l[1], l[2], 0];
    levels.forEach(function (level, i) {
      x += widths[i] * scale;
      const y = mid - level / top * (mid - 5);
      d += " L " + x.toFixed(1) + " " + y.toFixed(1);
    });
    return '<svg viewBox="0 0 ' + W + " " + H + '">' +
      '<line class="ezero" x1="6" y1="' + mid + '" x2="' + (W - 6) +
      '" y2="' + mid + '"/>' +
      '<path class="ecurve" d="' + d + '"/></svg>';
  }

  function lfoSvg(wave) {
    let d = "M 4 31";
    const N = 232;
    for (let x = 0; x <= N; ++x) {
      const p = x / N;
      const phase = (p * 2) % 1;
      let v;
      if (wave === "triangle") v = 1 - 4 * Math.abs(phase - 0.5);
      else if (wave === "saw") v = 1 - 2 * phase;
      else if (wave === "inverse saw") v = 2 * phase - 1;
      else if (wave === "square") v = phase < 0.5 ? 1 : -1;
      else if (wave === "random") v = Math.sin(x * 1.7) * Math.cos(x * 0.37);
      else v = Math.sin(p * Math.PI * 4);
      d += " L " + (4 + x) + " " + (31 - v * 22).toFixed(1);
    }
    return '<svg viewBox="0 0 240 62"><path d="' + d +
      '" fill="none" stroke="#3dff9e" stroke-width="1.4"/></svg>';
  }

  function makeMacro(label, options) {
    const row = document.createElement("div");
    row.className = "macro";
    const name = document.createElement("span");
    name.className = "mlabel";
    name.textContent = label;
    const track = document.createElement("div");
    track.className = "mtrack";
    if (options.min < 0) track.classList.add("bip");
    const fill = document.createElement("i");
    track.appendChild(fill);
    const value = document.createElement("b");
    value.className = "mval";
    row.append(name);
    if (options.extra) row.appendChild(options.extra);
    row.append(track, value);
    function paint(v) {
      const f = (v - options.min) / (options.max - options.min);
      if (options.min < 0) {
        fill.style.left = Math.min(f, 0.5) * 100 + "%";
        fill.style.width = Math.abs(f - 0.5) * 100 + "%";
      } else {
        fill.style.left = "0%";
        fill.style.width = f * 100 + "%";
      }
      value.textContent = options.format ? options.format(v) : Math.round(v);
    }
    paint(options.value || 0);
    function valueFrom(event) {
      const rect = track.getBoundingClientRect();
      const f = Math.max(0, Math.min(1, (event.clientX - rect.left) / rect.width));
      return options.min + f * (options.max - options.min);
    }
    track.addEventListener("pointerdown", function (event) {
      event.preventDefault();
      track.setPointerCapture(event.pointerId);
      if (options.onStart) options.onStart();
      const first = valueFrom(event);
      paint(first);
      options.onInput(first);
      function move(ev) {
        const v = valueFrom(ev);
        paint(v);
        options.onInput(v);
      }
      function up() {
        track.removeEventListener("pointermove", move);
        track.removeEventListener("pointerup", up);
        track.removeEventListener("pointercancel", up);
        if (options.onEnd) options.onEnd();
      }
      track.addEventListener("pointermove", move);
      track.addEventListener("pointerup", up);
      track.addEventListener("pointercancel", up);
    });
    return row;
  }

  function div(className) {
    const el = document.createElement("div");
    if (className) el.className = className;
    return el;
  }

  function baseName(name) {
    return name.replace(/\.p[01]$/, "");
  }

  function partOf(name) {
    return name.endsWith(".p1") ? 1 : 0;
  }

  function opOf(base) {
    if (base === "attack" || base === "release") return 0;
    const match = base.match(/^op([1-4])/);
    return match === null ? -1 : Number(match[1]) - 1;
  }

  function isWaveform(base) {
    return /^op[1-4]_waveform$/.test(base);
  }

  function pageOf(base) {
    if (base === "attack" || base === "release") return "envs";
    if (/^op[1-4]_spec_(env_|lfo_)/.test(base) ||
        /^op[1-4]_spec_env_depth$/.test(base))
      return "mod";
    if (/^op[1-4]_spec_(a|b|range|morph)/.test(base)) return "spectra";
    if (/^op[1-4]_(trem_|fblfo_|dlfo_)/.test(base)) return "lfos";
    if (/^op[1-4]_/.test(base) || /^op[1-4]\./.test(base)) return "envs";
    if (/^(algorithm|channel|low|high|level|delay|detune|transpose|mode|legato|unison_count|unison_detune|portamento|note_priority)$/.test(base))
      return "patch";
    return "fx";
  }

  function humanLabel(base) {
    const velocity = base.match(/^op[1-4]\.(velamp|velfm|velrate)\.depth$/);
    if (velocity !== null)
      return ({ velamp: "v amp", velfm: "v fm", velrate: "v rate" })[velocity[1]];
    if (/^op[1-4]\.eg\.[rl][1-4]$/.test(base)) return base.slice(-2);
    const opMatch = base.match(/^op[1-4]_(.+)$/);
    if (opMatch !== null) {
      const tail = opMatch[1];
      if (/^spec_range_[ab]$/.test(tail))
        return "range " + tail.slice(-1);
      if (/^spec_[ab]\d+$/.test(tail)) return "h" + tail.match(/\d+/)[0];
      if (/^spec_env_/.test(tail)) return tail.replace("spec_env_", "");
      if (/^spec_lfo_/.test(tail)) return tail.replace("spec_lfo_", "");
      if (tail === "spec_morph") return "morph";
      if (/^fbenv_/.test(tail)) return tail.slice(6);
      if (/^trem_/.test(tail)) return tail.slice(5);
      if (/^fblfo_/.test(tail)) return tail.slice(6);
      if (/^dlfo_/.test(tail)) return tail.slice(5);
      if (tail === "waveform") return "wave";
      if (tail === "feedback") return "fb";
      const short = {
        level_ks_lo: "lvl lo", level_ks_hi: "lvl hi", level_vs: "lvl vs",
        fb_ks_lo: "fb lo", fb_ks_hi: "fb hi", fb_vs: "fb vs",
        aenv_ks_lo: "a lo", aenv_ks_hi: "a hi", aenv_vs: "a vs",
      };
      if (short[tail] !== undefined) return short[tail];
      return tail.replace(/_/g, " ");
    }
    const names = {
      channel: "midi", low: "key low", high: "key high", detune: "tune",
      transpose: "transpose", note_priority: "priority", unison_count: "unison",
      unison_detune: "unison detune", portamento: "porta", mode: "mode",
      cutoff: "cutoff", res: "res", filter_type: "type", filter_on: "on",
      fenv_depth: "env depth", key_sens_on: "key sens", vel_sens_on: "vel sens",
      breakpoint: "breakpoint", pitch_rate: "rate", pitch_depth: "depth",
      pitch_wave: "wave", pitch_mode: "bipolar", penv_on: "penv on",
      penv_ks_lo: "penv lo", penv_ks_hi: "penv hi", penv_vs: "penv vs",
      f1_cutoff_ks_lo: "cut lo", f1_cutoff_ks_hi: "cut hi",
      f1_cutoff_vs: "cut vs", fenv_ks_lo: "env lo",
      fenv_ks_hi: "env hi", fenv_vs: "env vs",
      pitch_ks_lo: "pitch lo", pitch_ks_hi: "pitch hi", pitch_vs: "pitch vs",
      "lp.comp": "comp", "dc.r": "dc r",
    };
    if (names[base] !== undefined) return names[base];
    return base.replace(/_/g, " ");
  }

  function noteName(note) {
    const names = ["c", "c#", "d", "d#", "e", "f", "f#", "g", "g#", "a", "a#", "b"];
    const value = Math.round(note);
    return names[((value % 12) + 12) % 12] + (Math.floor(value / 12) - 1);
  }

  function userSpec(spec) {
    const display = spec.display;
    if (baseName(spec.name).startsWith("channel")) {
      return {
        min: spec.min + 1, max: spec.max + 1, step: 1,
        toUser: (raw) => raw + 1, toRaw: (user) => user - 1,
      };
    }
    if (display === "ratio") {
      return {
        min: spec.min / 65536, max: spec.max / 65536, step: 0.01,
        toUser: (raw) => raw / 65536, toRaw: (user) => user * 65536,
      };
    }
    if (display === "hz") {
      const scale = 48000 / 4294967296;
      return {
        min: spec.min * scale, max: spec.max * scale, step: 0.01,
        toUser: (raw) => raw * scale, toRaw: (user) => user / scale,
      };
    }
    if (display === "pct_bip") {
      return {
        min: spec.min / 327.67, max: spec.max / 327.67, step: 1,
        toUser: (raw) => raw / 327.67, toRaw: (user) => user * 327.67,
      };
    }
    if (display === "pct" || display === "pct_exp" || display === "off_pct") {
      return {
        min: 0, max: 100, step: 1,
        toUser: (raw) => (raw / spec.max) * 100,
        toRaw: (user) => (user / 100) * spec.max,
      };
    }
    if (display === "semi_lfo" || display === "semi_fine") {
      return {
        min: spec.min * 12 / 32768, max: spec.max * 12 / 32768, step: 0.01,
        toUser: (raw) => raw * 12 / 32768,
        toRaw: (user) => user * 32768 / 12,
      };
    }
    return {
      min: spec.min, max: spec.max, step: 1,
      toUser: (raw) => raw, toRaw: (user) => user,
    };
  }

  function formatUser(user, spec) {
    const display = spec.display;
    if (display === "ratio") return user.toFixed(2);
    if (display === "hz")
      return user < 1 ? user.toFixed(2) + "Hz" : user.toFixed(1) + "Hz";
    if (display === "pct_bip")
      return (user > 0 ? "+" : "") + Math.round(user) + "%";
    if (display === "pct" || display === "pct_exp" || display === "off_pct")
      return Math.round(user) + "%";
    if (display === "semi_lfo" || display === "semi_fine")
      return (user > 0 ? "+" : "") + user.toFixed(2) + "st";
    if (display === "cent")
      return (user > 0 ? "+" : "") + Math.round(user) + "c";
    if (display === "semitones")
      return (user > 0 ? "+" : "") + Math.round(user) + "st";
    if (display === "porta")
      return user <= 0 ? "off" : Math.round(user) + "ms";
    if (display === "note") return noteName(user);
    return String(Math.round(user));
  }

  function enumValues(spec) {
    if (spec.display === "wave")
      return spec.max === 13 ? WAVES : LFO_WAVES;
    if (spec.display === "range") return RANGES;
    if (spec.display === "filter") return FILTERS;
    if (spec.display === "priority") return PRIORITIES;
    if (spec.display === "mode") return MODES;
    return null;
  }

  function rawOf(spec) {
    const value = values.get(spec.id);
    return value === undefined ? spec.default : value;
  }

  function setRaw(spec, raw) {
    const value = Math.max(spec.min, Math.min(spec.max, Math.round(raw)));
    values.set(spec.id, value);
    if (node !== null) {
      node.port.postMessage({ type: "param", id: spec.id, value });
    }
  }

  function controlFor(spec, labelOverride, onStart, formatOverride) {
    const base = baseName(spec.name);
    const raw = rawOf(spec);
    const enums = enumValues(spec);
    const label = labelOverride || humanLabel(base);
    if (enums !== null) {
      const wrapper = div("param-enum");
      const name = div("knob-name");
      name.textContent = label;
      name.title = base;
      const select = makeSelect(enums, enums[raw] ?? enums[0], function (choice) {
        setRaw(spec, enums.indexOf(choice));
      });
      wrapper.title = base;
      wrapper.append(name, select);
      return wrapper;
    }
    if (spec.display === "onoff" || spec.display === "onoff_inv") {
      const invert = spec.display === "onoff_inv";
      const active = invert ? raw === 0 : raw === 1;
      const name = invert ? "active"
        : (/_mode$/.test(base) ? "bipolar"
          : (base === "legato" ? "legato" : "on"));
      const wrapper = div("param-toggle");
      wrapper.title = base;
      wrapper.append(makeToggle(name, active, function (on) {
        setRaw(spec, invert ? (on ? 0 : 1) : (on ? 1 : 0));
      }));
      return wrapper;
    }
    const range = userSpec(spec);
    const knob = makeKnob({
      name: label,
      title: base,
      min: range.min,
      max: range.max,
      val: range.toUser(raw),
      step: range.step,
      fmt: formatOverride || function (value) { return formatUser(value, spec); },
      onStart: onStart,
      onChange: function (value) {
        setRaw(spec, range.toRaw(value));
      },
    });
    return knob;
  }

  function idOf(names) {
    return names
      .map((name) => byName.get(name))
      .filter((spec) => spec !== undefined)
      .map((spec) => spec.id);
  }

  function namePart(base, part) {
    return base + ".p" + part;
  }

  function graphFor(rateNames, levelNames, options) {
    options = options || {};
    const rateIDs = idOf(rateNames);
    const levelIDs = idOf(levelNames);
    return function () {
      const rates = rateIDs.map((id) => values.get(id) ?? 0);
      let levels = levelIDs.map((id) => values.get(id) ?? 0);
      if (options.transform) levels = levels.map(options.transform);
      return options.signed
        ? penvSvg(rates, levels, options.scaleMax)
        : envSvg(rates, levels, options.curve);
    };
  }

  function ampRateNames(op, part) {
    const n = op + 1;
    return op === 0
      ? [namePart("attack", part), namePart("op1.eg.r2", part),
         namePart("op1.eg.r3", part), namePart("release", part)]
      : [namePart("op" + n + ".eg.r1", part),
         namePart("op" + n + ".eg.r2", part),
         namePart("op" + n + ".eg.r3", part),
         namePart("op" + n + ".eg.r4", part)];
  }

  function ampLevelNames(op, part) {
    const n = op + 1;
    return [namePart("op" + n + ".eg.l1", part),
            namePart("op" + n + ".eg.l2", part),
            namePart("op" + n + ".eg.l3", part)];
  }

  function fbRateNames(op, part) {
    const n = op + 1;
    return [1, 2, 3, 4].map((i) =>
      namePart("op" + n + "_fbenv_r" + i, part));
  }

  function fbLevelNames(op, part) {
    const n = op + 1;
    return [1, 2, 3, 4].map((i) =>
      namePart("op" + n + "_fbenv_l" + i, part));
  }

  function morphRateNames(op, part) {
    const n = op + 1;
    return [1, 2, 3, 4].map((i) =>
      namePart("op" + n + "_spec_env_r" + i, part));
  }

  function morphLevelNames(op, part) {
    const n = op + 1;
    return [1, 2, 3].map((i) =>
      namePart("op" + n + "_spec_env_l" + i, part));
  }

  function pitchRateNames(part) {
    return [1, 2, 3, 4].map((i) => namePart("penv_r" + i, part));
  }

  function pitchLevelNames(part) {
    return [1, 2, 3, 4].map((i) => namePart("penv_l" + i, part));
  }

  function filterRateNames(part) {
    return [1, 2, 3, 4].map((i) => namePart("fenv_r" + i, part));
  }

  function filterLevelNames(part) {
    return [1, 2, 3].map((i) => namePart("fenv_l" + i, part));
  }

  function appendGraph(el, graph) {
    const box = div("envbox");
    box.innerHTML = graph();
    el.append(box);
    graphUpdaters.push(() => {
      const html = graph();
      if (box.innerHTML !== html) box.innerHTML = html;
    });
  }

  function sectionCard(title, options) {
    options = options || {};
    const el = document.createElement("section");
    el.className = "card" + (options.className ? " " + options.className : "");
    if (options.headExtra) {
      const head = div("sec-head");
      const heading = document.createElement("h3");
      heading.textContent = title;
      head.append(heading, options.headExtra);
      el.append(head);
    } else {
      const heading = document.createElement("h3");
      heading.textContent = title;
      el.append(heading);
    }
    if (options.before) el.append(options.before);
    return el;
  }

  function card(title, specs, options) {
    options = options || {};
    if (specs.length === 0) return null;
    const el = sectionCard(title, options);
    if (options.graph) appendGraph(el, options.graph);
    if (options.note) {
      const note = div("panel-card-note");
      note.textContent = options.note;
      el.append(note);
    }
    const knobs = div("knobs");
    for (const spec of specs) {
      const before = options.beforeChange
        ? options.beforeChange(spec) : null;
      knobs.append(controlFor(spec,
        options.label ? options.label(spec) : null, before));
    }
    el.append(knobs);
    return el;
  }

  function clampLevel(value) {
    return Math.max(0, Math.min(99, Math.round(value)));
  }

  function previewKey(part, op) {
    return part + ":" + op;
  }

  function previewSpecs(part, op) {
    return specsForOp(part, op)
      .filter((spec) => /^op[1-4]_spec_a\d+$/.test(baseName(spec.name)))
      .sort(naturalSort);
  }

  function requestPreview(part, op) {
    if (!workletReady || node === null) return;
    const waveSpec = byName.get(`op${op + 1}_waveform.p${part}`);
    const rangeSpec = byName.get(`op${op + 1}_spec_range_a.p${part}`);
    if (waveSpec === undefined || rangeSpec === undefined) return;
    const wave = values.get(waveSpec.id) ?? 0;
    const key = previewKey(part, op);
    if (wave === 13) {
      previewCache.delete(key);
      return;
    }
    const first = ((values.get(rangeSpec.id) ?? 0) * 16) + 1;
    const cached = previewCache.get(key);
    if (cached !== undefined && cached.wave === wave &&
        cached.first === first)
      return;
    const request = `${key}:${wave}:${first}`;
    if (previewRequests.has(request))
      return;
    previewRequests.add(request);
    node.port.postMessage({ type: "analyze", key, wave, first, request });
  }

  function getPreview(part, op) {
    const key = previewKey(part, op);
    const cached = previewCache.get(key);
    if (cached === undefined) return null;
    const waveSpec = byName.get(`op${op + 1}_waveform.p${part}`);
    const rangeSpec = byName.get(`op${op + 1}_spec_range_a.p${part}`);
    if (waveSpec === undefined || rangeSpec === undefined) return null;
    const wave = values.get(waveSpec.id) ?? 0;
    const first = ((values.get(rangeSpec.id) ?? 0) * 16) + 1;
    if (wave === 13 || cached.wave !== wave || cached.first !== first)
      return null;
    return cached.levels;
  }

  function commitPreview(part, op) {
    const key = previewKey(part, op);
    const preview = previewCache.get(key);
    if (preview === undefined) return;
    const aSpecs = previewSpecs(part, op);
    if (aSpecs.length !== 16) {
      previewCache.delete(key);
      return;
    }
    for (let i = 0; i < 16; ++i)
      setRaw(aSpecs[i], preview.levels[i]);
    const waveSpec = byName.get(`op${op + 1}_waveform.p${part}`);
    if (waveSpec !== undefined)
      setRaw(waveSpec, 13);
    previewCache.delete(key);
    for (const update of spectrumUpdaters) update();
  }

  function spectrumBars(specs, kind, rangeSpec, previewLevels, onEdit) {
    const wrap = div("specbars");
    const fills = [];
    for (let i = 0; i < 16; ++i) {
      const bar = div("sbar" + (kind === "b" ? " b" : ""));
      const fill = document.createElement("i");
      bar.append(fill);
      wrap.append(bar);
      fills.push(fill);
    }
    const labels = div("speclabels");
    const labelSpans = [];
    for (let i = 0; i < 16; ++i) {
      const span = document.createElement("span");
      labels.append(span);
      labelSpans.push(span);
    }
    function levels() {
      const preview = previewLevels ? previewLevels() : null;
      if (preview !== null && preview !== undefined) return preview;
      return specs.map((spec) => values.get(spec.id) ?? 0);
    }
    function draw() {
      const data = levels();
      for (let i = 0; i < 16; ++i)
        fills[i].style.height = (data[i] / 99 * 100) + "%";
      const range = rangeSpec === undefined || rangeSpec === null
        ? 0 : (values.get(rangeSpec.id) ?? 0);
      const first = range * 16 + 1;
      for (let i = 0; i < 16; ++i) {
        labelSpans[i].textContent =
          (i === 0 || i === 3 || i === 7 || i === 11 || i === 15)
            ? "h" + (first + i) : "";
      }
    }
    function setFrom(event) {
      const rect = wrap.getBoundingClientRect();
      const cell = rect.width / 16;
      const index = Math.max(0, Math.min(15,
        Math.floor((event.clientX - rect.left) / cell)));
      const y = 1 - (event.clientY - rect.top) / rect.height;
      setRaw(specs[index], clampLevel(y * 99));
      draw();
    }
    wrap.addEventListener("pointerdown", function (event) {
      event.preventDefault();
      if (onEdit) onEdit();
      wrap.setPointerCapture(event.pointerId);
      setFrom(event);
      function move(ev) { setFrom(ev); }
      function up() {
        wrap.removeEventListener("pointermove", move);
        wrap.removeEventListener("pointerup", up);
        wrap.removeEventListener("pointercancel", up);
      }
      wrap.addEventListener("pointermove", move);
      wrap.addEventListener("pointerup", up);
      wrap.addEventListener("pointercancel", up);
    });
    spectrumUpdaters.push(draw);
    draw();
    return { el: wrap, labels: labels, draw: draw, levels: levels };
  }

  function spectrumCard(title, specs, otherSpecs, kind, rangeSpec, options) {
    options = options || {};
    const cardEl = document.createElement("section");
    cardEl.className = "card speccard";
    const head = div("secheadleft");
    const heading = document.createElement("h3");
    heading.textContent = title;
    head.append(heading);
    let badge = null;
    if (options.fromWave) {
      badge = document.createElement("span");
      badge.className = "fromwave";
      badge.textContent = "from wave";
      head.append(badge);
    }
    cardEl.append(head);
    const bars = spectrumBars(specs, kind, rangeSpec, options.preview,
      options.onEdit);
    cardEl.append(bars.el, bars.labels);

    let base = null;
    function begin() {
      if (options.onEdit) options.onEdit();
      base = bars.levels();
    }
    function end() { base = null; }
    function apply(fn) {
      if (base === null) return;
      for (let i = 0; i < 16; ++i)
        setRaw(specs[i], clampLevel(fn(base[i], i + 1)));
      bars.draw();
    }
    const odd = makeMacro("odd", {
      min: 0, max: 1, value: 0,
      format: (v) => Math.round(v * 100) + "%",
      onStart: begin, onEnd: end,
      onInput: (a) => apply((level, n) => n % 2 === 0 ? level * (1 - a) : level),
    });
    const even = makeMacro("even", {
      min: 0, max: 1, value: 0,
      format: (v) => Math.round(v * 100) + "%",
      onStart: begin, onEnd: end,
      onInput: (a) => apply((level, n) =>
        n > 1 && n % 2 === 1 ? level * (1 - a) : level),
    });
    const tilt = makeMacro("tilt", {
      min: -1, max: 1, value: 0,
      format: (v) => (v > 0 ? "+" : "") + Math.round(v * 100) + "%",
      onStart: begin, onEnd: end,
      onInput: (a) => apply((level, n) =>
        level * Math.max(0, Math.min(1, 1 + a * (n - 1) / 15))),
    });
    let centre = 5;
    const formantSelect = makeSelect(
      ["h1", "h2", "h3", "h4", "h5", "h6", "h7", "h8", "h9", "h10", "h11",
       "h12", "h13", "h14", "h15", "h16"],
      "h5",
      (value) => { centre = parseInt(value.slice(1), 10); });
    formantSelect.classList.add("msel");
    const formant = makeMacro("formant", {
      min: 0, max: 1, value: 0, extra: formantSelect,
      format: (v) => Math.round(v * 100) + "%",
      onStart: begin, onEnd: end,
      onInput: (a) => apply((level, n) =>
        level + a * 99 * Math.exp(-Math.pow(n - centre, 2) / 4.5)),
    });
    const row1 = div("macro-row");
    row1.append(odd, even);
    const row2 = div("macro-row");
    row2.append(tilt, formant);
    cardEl.append(row1, row2);

    const foot = div("specfoot");
    const norm = document.createElement("button");
    norm.className = "mini";
    norm.textContent = "norm";
    norm.addEventListener("click", () => {
      if (options.onEdit) options.onEdit();
      const levels = bars.levels();
      const max = Math.max(...levels);
      if (max > 0) {
        for (let i = 0; i < 16; ++i)
          setRaw(specs[i], clampLevel(levels[i] * 99 / max));
        bars.draw();
      }
    });
    const to = document.createElement("button");
    to.className = "mini";
    to.textContent = kind === "a" ? "to b" : "to a";
    to.addEventListener("click", () => {
      if (options.onEdit) options.onEdit();
      const levels = bars.levels();
      for (let i = 0; i < 16 && i < otherSpecs.length; ++i)
        setRaw(otherSpecs[i], levels[i]);
    });
    foot.append(norm, to);
    cardEl.append(foot);
    if (badge !== null) {
      spectrumUpdaters.push(() => {
        badge.style.display = options.preview && options.preview() ? "" : "none";
      });
    }
    return cardEl;
  }


  function spectrumDisplay(aSpecs, bSpecs, rangeA, rangeB, morphSpec,
                           previewLevels) {
    const canvas = document.createElement("canvas");
    canvas.className = "panel-spectrum-display";
    canvas.width = 512;
    canvas.height = 120;
    function draw() {
      const context = canvas.getContext("2d");
      const width = canvas.width;
      const height = canvas.height;
      context.fillStyle = "#000000";
      context.fillRect(0, 0, width, height);
      const firstA = ((rangeA === undefined ? 0 : values.get(rangeA.id) ?? 0)
        * 16) + 1;
      const firstB = ((rangeB === undefined ? 0 : values.get(rangeB.id) ?? 0)
        * 16) + 1;
      const minFirst = Math.min(firstA, firstB);
      const maxLast = Math.max(firstA + 15, firstB + 15);
      const span = maxLast - minFirst + 1;
      const left = 8;
      const right = width - 8;
      const top = 20;
      const bottom = height - 18;
      const graphWidth = right - left;
      const graphHeight = bottom - top;
      function xAt(harmonic) {
        return left + (harmonic - minFirst + 0.5) / span * graphWidth;
      }
      context.strokeStyle = "#1c1c1c";
      context.lineWidth = 1;
      for (let i = 0; i <= 8; ++i) {
        const x = left + graphWidth * i / 8;
        context.beginPath();
        context.moveTo(x, top);
        context.lineTo(x, bottom);
        context.stroke();
      }
      context.beginPath();
      context.moveTo(left, bottom);
      context.lineTo(right, bottom);
      context.strokeStyle = "#2e2e2e";
      context.stroke();
      const aLevels = (previewLevels && previewLevels()) ||
        aSpecs.map((spec) => values.get(spec.id) ?? 0);
      const bLevels = bSpecs.map((spec) => values.get(spec.id) ?? 0);
      function outline(levels, first, color) {
        context.beginPath();
        context.moveTo(xAt(first), bottom);
        for (let i = 0; i < 16; ++i) {
          const y = bottom - levels[i] / 99 * graphHeight;
          context.lineTo(xAt(first + i), y);
        }
        context.lineTo(xAt(first + 15), bottom);
        context.closePath();
        context.fillStyle = color + "2e";
        context.fill();
        context.strokeStyle = color;
        context.lineWidth = 1.2;
        context.stroke();
      }
      outline(aLevels, firstA, "#c780ff");
      outline(bLevels, firstB, "#ff2e7e");
      const morph = morphSpec === undefined ? 0
        : Math.max(0, Math.min(1, (values.get(morphSpec.id) ?? 0) / 99));
      const points = 256;
      const wave = new Float32Array(points);
      let peak = 0;
      for (let i = 0; i < points; ++i) {
        const t = i / (points - 1);
        const phase = 2 * Math.PI * t;
        let sumA = 0;
        let sumB = 0;
        for (let h = 0; h < 16; ++h) {
          sumA += aLevels[h] * Math.sin(phase * (firstA + h));
          sumB += bLevels[h] * Math.sin(phase * (firstB + h));
        }
        const sum = (1 - morph) * sumA + morph * sumB;
        wave[i] = sum;
        peak = Math.max(peak, Math.abs(sum));
      }
      if (peak > 1e-6) {
        context.beginPath();
        for (let i = 0; i < points; ++i) {
          const x = left + graphWidth * i / (points - 1);
          const y = (top + bottom) / 2 -
            wave[i] / peak * graphHeight * 0.42;
          if (i === 0) context.moveTo(x, y);
          else context.lineTo(x, y);
        }
        context.strokeStyle = "rgba(61,255,158,0.85)";
        context.lineWidth = 1.5;
        context.stroke();
      }
      const markerX = left + morph * graphWidth;
      context.strokeStyle = "#3dff9e";
      context.beginPath();
      context.moveTo(markerX, top - 3);
      context.lineTo(markerX, bottom);
      context.stroke();
      context.font = '10px "DotGothic16", monospace';
      context.textBaseline = "top";
      context.fillStyle = "#c780ff";
      context.fillText("spectrum a (" + firstA + "-" + (firstA + 15) + ")",
        left, 2);
      context.fillStyle = "#ff2e7e";
      context.fillText("spectrum b (" + firstB + "-" + (firstB + 15) + ")",
        left + 150, 2);
      context.fillStyle = "rgba(61,255,158,0.8)";
      context.fillText("waveform", left + 300, 2);
      context.fillStyle = "#4a4a4a";
      for (let k = 0; k < 5; ++k) {
        const harmonic = minFirst + Math.round((span - 1) * k / 4);
        context.fillText(String(harmonic), xAt(harmonic) - 8, bottom + 2);
      }
    }
    spectrumUpdaters.push(draw);
    draw();
    return canvas;
  }


  function pageFor(spec) {
    return pageOf(baseName(spec.name));
  }

  function specsForPart(part) {
    return params.filter((spec) => {
      if (spec.name.endsWith(".p0") || spec.name.endsWith(".p1"))
        return spec.name.endsWith(".p" + part);
      return true;
    });
  }

  function specsForOp(part, op) {
    return specsForPart(part).filter((spec) => opOf(baseName(spec.name)) === op);
  }

  function isCarrier(algorithm, op) {
    const row = CARRIERS[Math.max(0, Math.min(7, algorithm))];
    return row.includes(op);
  }

  function opColumn(op, withScope) {
    const col = div("opcol");
    const head = div("opcol-head");
    const name = document.createElement("span");
    const algorithmSpec = byName.get("algorithm.p" + activePart);
    const algorithm = algorithmSpec === undefined ? 4 : rawOf(algorithmSpec);
    name.className = "opname" + (isCarrier(algorithm, op) ? " car" : "");
    name.style.color = isCarrier(algorithm, op) ? "var(--pink)" : "var(--white)";
    name.textContent = "op" + (op + 1);
    head.append(name);
    const acts = div("opcol-acts");
    const copy = document.createElement("button");
    copy.className = "mini";
    copy.textContent = "copy";
    copy.addEventListener("click", () => copyOp(activePart, op));
    const paste = document.createElement("button");
    paste.className = "mini";
    paste.textContent = "paste";
    paste.disabled = clipboard === null || clipboard.kind !== "op";
    paste.addEventListener("click", () => pasteOp(activePart, op));
    acts.append(copy, paste);
    head.append(acts);
    col.append(head);
    if (withScope) {
      const vis = div("opvis");
      const scope = document.createElement("canvas");
      scope.className = "panel-opscope";
      scope.width = 256;
      scope.height = 44;
      scope.dataset.op = String(op);
      const spectro = document.createElement("canvas");
      spectro.className = "panel-opspectro";
      spectro.width = 256;
      spectro.height = 44;
      spectro.dataset.op = String(op);
      vis.append(scope, spectro);
      col.append(vis);
    }
    return col;
  }

  function selectPart(part) {
    part = part === 1 ? 1 : 0;
    if (part === activePart) return;
    noteOff();
    activePart = part;
    tapSamples = null;
    tapRoutes = null;
    buildAlgorithm();
    buildToolbar();
    buildPage();
    if (node !== null)
      node.port.postMessage({ type: "part", part: activePart });
  }

  function togglePartMute(part, muted) {
    const levelSpec = byName.get("level.p" + part);
    if (levelSpec === undefined) return;
    if (muted) {
      partLevelBeforeMute[part] = values.get(levelSpec.id) ?? 99;
      partMuted[part] = true;
      setRaw(levelSpec, 0);
    } else {
      partMuted[part] = false;
      setRaw(levelSpec, partLevelBeforeMute[part]);
    }
    buildPage();
    buildToolbar();
  }

  /* ---------------- patch and part library ---------------- */
  const PATCH_SAVE_MODES = [
    "discard edits to parts",
    "save as new parts",
    "mutate parts in place",
  ];
  const PART_SLOT_COUNT = 128;
  const MULTI_SLOT_COUNT = 64;
  const LIBRARY_KEY = "type-p-alpha-web-library";
  const partNames = ["init", "init"];
  const partSource = [-1, -1];
  const partLibrary = new Array(PART_SLOT_COUNT).fill(null);
  const multiLibrary = new Array(MULTI_SLOT_COUNT).fill(null).map(() => ({
    used: false, name: "empty", partSlots: [-1, -1], routing: [],
  }));
  let patchName = "init patch";
  let currentMulti = -1;
  let clipboard = null;

  function partSnapshot(part) {
    const entries = [];
    for (const spec of params) {
      if (!spec.name.endsWith(".p" + part)) continue;
      entries.push([spec.id, values.get(spec.id) ?? spec.default]);
    }
    return { name: partNames[part], params: entries };
  }

  function applyValues(entries) {
    for (const [id, value] of entries) values.set(id, value);
    if (node !== null)
      node.port.postMessage({ type: "params", values: entries });
  }

  function applyPartSnapshot(part, snapshot) {
    if (snapshot === null || !Array.isArray(snapshot.params)) return;
    if (typeof snapshot.name === "string") partNames[part] = snapshot.name;
    applyValues(snapshot.params);
  }

  function defaultPartEntries(part) {
    const entries = [];
    for (const spec of params) {
      if (!spec.name.endsWith(".p" + part)) continue;
      entries.push([spec.id, spec.default]);
    }
    return entries;
  }

  function initPart(part) {
    applyValues(defaultPartEntries(part));
    partNames[part] = "init";
    partSource[part] = -1;
    partMuted[part] = false;
    buildAlgorithm();
    buildToolbar();
    buildPage();
  }

  function initMulti() {
    initPart(0);
    initPart(1);
    patchName = "init multi";
    currentMulti = -1;
    showPatchName();
    saveLibrary();
    if (typeof toast === "function") toast("multi-patch initialised");
  }

  function showPatchName() {
    if (patchNameEl !== null) patchNameEl.textContent = patchName;
  }

  function routingFor(part) {
    const read = (base) => {
      const spec = byName.get(base + ".p" + part);
      return spec === undefined ? 0 : (values.get(spec.id) ?? spec.default);
    };
    return {
      channel: read("channel"), low: read("low"), high: read("high"),
      level: read("level"), delay: read("delay"),
    };
  }

  function multiSnapshot() {
    return {
      used: true,
      name: patchName,
      partSlots: partSource.slice(),
      routing: [routingFor(0), routingFor(1)],
    };
  }

  function findFreePartSlot() {
    return partLibrary.findIndex((slot) => slot === null);
  }

  function saveMulti(slot, mode) {
    for (let part = 0; part < 2; ++part) {
      let source = partSource[part];
      if (mode !== 0) {
        if (mode === 1 || source < 0 || partLibrary[source] === null) {
          source = findFreePartSlot();
          if (source < 0) {
            toast("no free part slot");
            return;
          }
        }
        partLibrary[source] = partSnapshot(part);
        partSource[part] = source;
      }
    }
    multiLibrary[slot] = multiSnapshot();
    currentMulti = slot;
    showPatchName();
    saveLibrary();
    if (typeof toast === "function")
      toast("saved to multi slot " + String(slot + 1).padStart(2, "0") +
        " (" + PATCH_SAVE_MODES[mode] + ")");
  }

  function applyMulti(slot) {
    const multi = multiLibrary[slot];
    if (!multi || !multi.used) return false;
    patchName = multi.name;
    currentMulti = slot;
    for (let part = 0; part < 2; ++part) {
      const source = Number.isInteger(multi.partSlots[part])
        ? multi.partSlots[part] : -1;
      partSource[part] = source;
      if (source >= 0 && partLibrary[source] !== null)
        applyPartSnapshot(part, partLibrary[source]);
      const routing = multi.routing[part] || {};
      for (const base of ["channel", "low", "high", "level", "delay"]) {
        if (routing[base] === undefined) continue;
        const spec = byName.get(base + ".p" + part);
        if (spec !== undefined) setRaw(spec, routing[base]);
      }
      partMuted[part] = false;
    }
    return true;
  }

  function loadMulti(slot) {
    if (!applyMulti(slot)) return;
    showPatchName();
    buildAlgorithm();
    buildToolbar();
    buildPage();
    saveLibrary();
    toast("loaded " + patchName);
  }

  function savePartToSlot(index, part) {
    partLibrary[index] = partSnapshot(part);
    partSource[part] = index;
    saveLibrary();
    toast("part saved to slot " + String(index + 1).padStart(2, "0"));
  }

  function loadPartFromSlot(index, part) {
    const snapshot = partLibrary[index];
    if (snapshot === null) return;
    applyPartSnapshot(part, snapshot);
    partSource[part] = index;
    buildAlgorithm();
    buildToolbar();
    buildPage();
    saveLibrary();
    toast("part loaded from slot " + String(index + 1).padStart(2, "0"));
  }

  function libraryData() {
    return {
      format: "type-p-alpha-library",
      version: 1,
      partSlots: partLibrary,
      multis: multiLibrary,
      patchName,
      currentMulti,
      partNames,
      partSource,
    };
  }

  function applyLibrary(data) {
    if (!data || data.format !== "type-p-alpha-library")
      throw new Error("not a type P.alpha library");
    if (Array.isArray(data.partSlots)) {
      for (let i = 0; i < PART_SLOT_COUNT; ++i)
        partLibrary[i] = data.partSlots[i] || null;
    }
    if (Array.isArray(data.multis)) {
      for (let i = 0; i < MULTI_SLOT_COUNT; ++i) {
        const multi = data.multis[i];
        multiLibrary[i] = multi && multi.used ? multi
          : { used: false, name: "empty", partSlots: [-1, -1], routing: [] };
      }
    }
    if (typeof data.patchName === "string") patchName = data.patchName;
    if (Number.isInteger(data.currentMulti)) currentMulti = data.currentMulti;
    if (Array.isArray(data.partNames)) {
      for (let i = 0; i < 2; ++i) {
        if (typeof data.partNames[i] === "string")
          partNames[i] = data.partNames[i];
      }
    }
  }

  function saveLibrary() {
    try {
      localStorage.setItem(LIBRARY_KEY, JSON.stringify(libraryData()));
    } catch (error) {
      window.__p1.libraryError = String(error);
    }
  }

  function loadLibrary() {
    try {
      const raw = localStorage.getItem(LIBRARY_KEY);
      if (raw) applyLibrary(JSON.parse(raw));
    } catch (error) {
      window.__p1.libraryError = String(error);
    }
    showPatchName();
  }

  function exportLibrary() {
    const blob = new Blob([JSON.stringify(libraryData(), null, 2)],
      { type: "application/json" });
    const link = document.createElement("a");
    link.href = URL.createObjectURL(blob);
    link.download = "type-p-alpha-library.json";
    document.body.appendChild(link);
    link.click();
    link.remove();
    URL.revokeObjectURL(link.href);
    toast("library exported");
  }

  function importLibrary() {
    const input = document.createElement("input");
    input.type = "file";
    input.accept = ".json,.p1lib,application/json";
    input.addEventListener("change", function () {
      const file = input.files && input.files[0];
      if (!file) return;
      const reader = new FileReader();
      reader.onload = function () {
        try {
          applyLibrary(JSON.parse(String(reader.result)));
          saveLibrary();
          showPatchName();
          buildToolbar();
          buildPage();
          toast("library imported");
        } catch (error) {
          toast("library import failed");
        }
      };
      reader.readAsText(file);
    });
    input.click();
  }

  /* ---------------- dialogs ---------------- */
  function openDialog(title, bodyNode, buttons, wide) {
    const back = div("backdrop");
    const dialog = div("dialog" + (wide ? " wide" : ""));
    const head = div("dialog-head");
    const heading = document.createElement("span");
    heading.textContent = title;
    const closeButton = document.createElement("span");
    closeButton.className = "x";
    closeButton.textContent = "X";
    head.append(heading, closeButton);
    const body = div("dialog-body");
    body.append(bodyNode);
    const foot = div("dialog-foot");
    for (const button of buttons) {
      const el = document.createElement("button");
      el.textContent = button.label;
      if (button.primary) el.className = "primary";
      el.addEventListener("click", () => button.onClick(close));
      foot.append(el);
    }
    dialog.append(head, body, foot);
    back.append(dialog);
    closeButton.addEventListener("click", () => close());
    back.addEventListener("click", (event) => {
      if (event.target === back) close();
    });
    document.body.append(back);
    function close() { back.remove(); }
    return close;
  }

  function toast(message) {
    const el = div("toast");
    el.textContent = message;
    document.body.append(el);
    setTimeout(() => el.remove(), 1600);
  }

  function openRename(title, current, onSave) {
    const body = div("");
    const input = document.createElement("input");
    input.className = "textin";
    input.value = current;
    input.maxLength = 32;
    body.append(input);
    const note = div("note");
    note.textContent = "max 32 characters.";
    body.append(note);
    openDialog(title, body, [
      {
        label: "save", primary: true, onClick: (close) => {
          const value = input.value.trim();
          close();
          if (value) onSave(value);
        },
      },
      { label: "cancel", onClick: (close) => close() },
    ]);
    setTimeout(() => { input.focus(); input.select(); }, 30);
  }

  function renamePart(part) {
    openRename("rename part", partNames[part], (value) => {
      partNames[part] = value;
      if (partSource[part] >= 0)
        partLibrary[partSource[part]] = partSnapshot(part);
      saveLibrary();
      buildPage();
      toast("part " + (part + 1) + " renamed");
    });
  }

  function copyPart(part) {
    clipboard = { kind: "part", part, data: partSnapshot(part) };
    buildPage();
    toast("part " + (part + 1) + " copied");
  }

  function pastePart(part) {
    if (clipboard === null || clipboard.kind !== "part") return;
    applyPartSnapshot(part, clipboard.data);
    partNames[part] = clipboard.data.name;
    buildAlgorithm();
    buildPage();
    toast("part " + (part + 1) + " pasted");
  }

  function copyOp(part, op) {
    const entries = specsForOp(part, op).map((spec) =>
      [spec.name, values.get(spec.id) ?? spec.default]);
    clipboard = { kind: "op", part, op, data: entries };
    buildPage();
    toast("op" + (op + 1) + " copied");
  }

  function pasteOp(part, op) {
    if (clipboard === null || clipboard.kind !== "op") return;
    const entries = [];
    for (const [name, value] of clipboard.data) {
      if (op !== 0 && !/^op[1-4][_.]/.test(name)) continue;
      const renamed = name
        .replace(/^op[1-4]([_.])/, "op" + (op + 1) + "$1")
        .replace(/\.p[01]$/, ".p" + part);
      const spec = byName.get(renamed);
      if (spec !== undefined) entries.push([spec.id, value]);
    }
    applyValues(entries);
    buildAlgorithm();
    buildPage();
    toast("op" + (op + 1) + " pasted");
  }

  function openMultiInitConfirm() {
    const body = div("");
    body.textContent = "init multi-patch? this wipes the current patch.";
    openDialog("init multi-patch", body, [
      { label: "init", primary: true, onClick: (close) => {
        close();
        initMulti();
      } },
      { label: "cancel", onClick: (close) => close() },
    ]);
  }

  function openMultiBrowser() {
    let selection = currentMulti >= 0 ? currentMulti : 0;
    const body = div("");
    const list = div("patchlist");
    function render() {
      list.innerHTML = "";
      multiLibrary.forEach((multi, index) => {
        const row = div("preset-row" + (index === selection ? " sel" : "") +
          (multi.used ? "" : " empty"));
        const number = document.createElement("span");
        number.className = "pno";
        number.textContent = String(index + 1).padStart(2, "0");
        const name = document.createElement("span");
        name.className = "pname";
        name.textContent = multi.used ? multi.name : "empty";
        if (index === currentMulti) {
          const tag = document.createElement("span");
          tag.className = "tag";
          tag.textContent = "active";
          name.append(" ", tag);
        }
        row.append(number, name);
        row.addEventListener("click", () => {
          selection = index;
          render();
        });
        list.append(row);
      });
    }
    render();
    body.append(list);
    openDialog("multi-patches", body, [
      { label: "export bank", onClick: (close) => { close(); exportLibrary(); } },
      { label: "import bank", onClick: (close) => { close(); importLibrary(); } },
      { label: "init multi-patch", onClick: (close) => {
        close();
        openMultiInitConfirm();
      } },
      { label: "save", onClick: (close) => { close(); openMultiSave(selection); } },
      { label: "load", primary: true, onClick: (close) => {
        close();
        loadMulti(selection);
      } },
      { label: "cancel", onClick: (close) => close() },
    ], true);
  }

  function openMultiSave(slot) {
    let mode = 0;
    const body = div("");
    const list = div("partlist");
    const cells = [];
    PATCH_SAVE_MODES.forEach((label, index) => {
      const cell = document.createElement("button");
      cell.className = "slotrow" + (index === mode ? " sel" : "");
      const name = document.createElement("span");
      name.className = "sname";
      name.textContent = label;
      cell.append(name);
      cell.addEventListener("click", () => {
        mode = index;
        cells.forEach((item, i) => item.classList.toggle("sel", i === mode));
      });
      list.append(cell);
      cells.push(cell);
    });
    body.append(list);
    openDialog("save multi-patch", body, [
      { label: "save", primary: true, onClick: (close) => {
        close();
        if (multiLibrary[slot].used) openMultiOverwrite(slot, mode);
        else saveMulti(slot, mode);
      } },
      { label: "cancel", onClick: (close) => close() },
    ]);
  }

  function openMultiOverwrite(slot, mode) {
    const body = div("");
    body.innerHTML = "overwrite multi-patch slot <b>" +
      String(slot + 1).padStart(2, "0") + "</b>?";
    openDialog("confirm save", body, [
      { label: "save", primary: true, onClick: (close) => {
        close();
        saveMulti(slot, mode);
      } },
      { label: "cancel", onClick: (close) => close() },
    ]);
  }

  function openSingleParts() {
    let selection = partSource[activePart] >= 0 ? partSource[activePart] : 0;
    const body = div("");
    const list = div("partlist bank");
    const cells = [];
    for (let i = 0; i < PART_SLOT_COUNT; ++i) {
      const slot = partLibrary[i];
      const cell = document.createElement("button");
      cell.className = "slotrow" + (i === selection ? " sel" : "") +
        (slot === null ? " dead" : "");
      const number = document.createElement("span");
      number.className = "pno";
      number.textContent = String(i + 1).padStart(2, "0");
      const name = document.createElement("span");
      name.className = "sname";
      name.textContent = slot === null ? "empty" : slot.name;
      cell.append(number, name);
      cell.addEventListener("click", () => {
        selection = i;
        cells.forEach((item, index) => item.classList.toggle("sel",
          index === selection));
      });
      list.append(cell);
      cells.push(cell);
    }
    body.append(list);
    const save = (part) => {
      if (partLibrary[selection] !== null) openPartOverwrite(selection, part);
      else {
        savePartToSlot(selection, part);
        buildPage();
      }
    };
    openDialog("single parts", body, [
      { label: "export bank", onClick: (close) => { close(); exportLibrary(); } },
      { label: "import bank", onClick: (close) => { close(); importLibrary(); } },
      { label: "init single part 1", onClick: (close) => {
        close();
        openPartInitConfirm(1);
      } },
      { label: "init single part 2", onClick: (close) => {
        close();
        openPartInitConfirm(2);
      } },
      { label: "save single part 1", onClick: (close) => { close(); save(0); } },
      { label: "save single part 2", onClick: (close) => { close(); save(1); } },
      { label: "load single part 1", onClick: (close) => {
        close();
        if (partLibrary[selection] !== null) loadPartFromSlot(selection, 0);
      } },
      { label: "load single part 2", onClick: (close) => {
        close();
        if (partLibrary[selection] !== null) loadPartFromSlot(selection, 1);
      } },
      { label: "cancel", onClick: (close) => close() },
    ], true);
  }

  function openPartOverwrite(index, part) {
    const body = div("");
    body.innerHTML = "overwrite part slot <b>" +
      String(index + 1).padStart(2, "0") + "</b>?";
    openDialog("confirm save", body, [
      { label: "save", primary: true, onClick: (close) => {
        close();
        savePartToSlot(index, part);
        buildPage();
      } },
      { label: "cancel", onClick: (close) => close() },
    ]);
  }

  function openPartInitConfirm(part) {
    const body = div("");
    body.innerHTML = "init part <b>" + part + "</b>? this wipes the current part.";
    openDialog("init part", body, [
      { label: "init", primary: true, onClick: (close) => {
        close();
        initPart(part - 1);
        toast("part initialised");
      } },
      { label: "cancel", onClick: (close) => close() },
    ]);
  }

  function buildToolbar() {
    if (toolbarEl === null) return;
    for (const node of toolbarNodes.splice(0)) node.remove();
    const legend = div("legend");
    legend.innerHTML = '<span class="sq muted">\u732b</span>' +
      '<span class="lbl">muted</span>' +
      '<span class="sq active">\u732b</span><span class="lbl">active</span>';

    const group = div("tgroup");
    for (let op = 0; op < 4; ++op) {
      const spec = byName.get("op" + (op + 1) + "_mute.p" + activePart);
      const muted = spec !== undefined &&
        (values.get(spec.id) ?? spec.default) === 1;
      const button = document.createElement("button");
      button.className = "mutebox" + (muted ? " muted" : "");
      button.textContent = "op" + (op + 1);
      button.title = (muted ? "unmute " : "mute ") + "op" + (op + 1);
      button.addEventListener("click", () => {
        if (spec === undefined) return;
        const current = values.get(spec.id) ?? spec.default;
        setRaw(spec, current === 1 ? 0 : 1);
        buildToolbar();
      });
      group.append(button);
    }
    group.append(div("tdiv"));
    for (let part = 0; part < 2; ++part) {
      const button = document.createElement("button");
      button.className = "mutebox" + (partMuted[part] ? " muted" : "");
      button.textContent = "p" + (part + 1);
      button.title = (partMuted[part] ? "unmute " : "mute ") +
        "part " + (part + 1);
      button.addEventListener("click", () =>
        togglePartMute(part, !partMuted[part]));
      group.append(button);
    }

    const editing = div("tgroup");
    const label = document.createElement("span");
    label.className = "tlabel";
    label.textContent = "now editing part:";
    editing.append(label);
    for (let part = 0; part < 2; ++part) {
      const button = document.createElement("button");
      button.className = "tbtn partsel" +
        (activePart === part ? " active" : "");
      button.textContent = "part " + (part + 1);
      button.addEventListener("click", () => selectPart(part));
      editing.append(button);
    }
    const library = document.createElement("button");
    library.className = "tbtn partlib";
    library.id = "partLibrary";
    library.textContent = "part library actions...";
    library.title = "Single part library";
    library.addEventListener("click", openSingleParts);
    editing.append(library);

    const nodes = [legend, group, editing];
    toolbarEl.append(...nodes);
    toolbarNodes.push(...nodes);
  }

  function levelSlider(part, spec) {
    const wrap = div("lvl");
    const cap = div("lvlcap");
    const label = document.createElement("span");
    label.textContent = "level";
    const value = document.createElement("span");
    value.className = "lvlval";
    cap.append(label, value);
    const track = div("lvltrack");
    const fill = document.createElement("i");
    track.append(fill);
    function paint(raw) {
      value.textContent = String(raw);
      fill.style.width = ((raw - spec.min) / (spec.max - spec.min) * 100) + "%";
    }
    paint(values.get(spec.id) ?? spec.default);
    track.addEventListener("pointerdown", (event) => {
      event.preventDefault();
      track.setPointerCapture(event.pointerId);
      function set(ev) {
        const rect = track.getBoundingClientRect();
        const fraction = Math.max(0,
          Math.min(1, (ev.clientX - rect.left) / rect.width));
        const raw = Math.round(spec.min + fraction * (spec.max - spec.min));
        partMuted[part] = false;
        setRaw(spec, raw);
        paint(raw);
      }
      set(event);
      function move(ev) { set(ev); }
      function up() {
        track.removeEventListener("pointermove", move);
        track.removeEventListener("pointerup", up);
        track.removeEventListener("pointercancel", up);
      }
      track.addEventListener("pointermove", move);
      track.addEventListener("pointerup", up);
      track.addEventListener("pointercancel", up);
    });
    wrap.append(cap, track);
    return wrap;
  }

  function voiceModeBar(part) {
    const bar = div("vmode");
    const head = div("vh");
    const title = document.createElement("h3");
    title.textContent = "voice mode";
    head.append(title);
    const legato = byName.get("legato.p" + part);
    if (legato !== undefined) {
      head.append(makeToggle("legato", rawOf(legato) === 1,
        (on) => setRaw(legato, on ? 1 : 0)));
    }
    bar.append(head);
    const row = div("vrow");
    const buttons = div("vbtns");
    const mode = byName.get("mode.p" + part);
    if (mode !== undefined) {
      MODES.forEach((name, index) => {
        const button = document.createElement("button");
        button.textContent = name;
        if (rawOf(mode) === index) button.className = "active";
        button.addEventListener("click", () => {
          setRaw(mode, index);
          buildPage();
        });
        buttons.append(button);
      });
    }
    row.append(buttons);
    const knobs = div("knobs");
    const voiceKnobs = [
      { base: "unison_count", label: "unison" },
      { base: "unison_detune", label: "detune" },
      { base: "portamento", label: "porta" },
      { base: "note_priority", label: "priority" },
    ];
    for (const item of voiceKnobs) {
      const spec = byName.get(item.base + ".p" + part);
      if (spec !== undefined) knobs.append(controlFor(spec, item.label));
    }
    row.append(knobs);
    bar.append(row);
    return bar;
  }

  function buildPatch(page) {
    const wrap = div("patchpage");
    const parts = div("parts");
    const partKnobs = [
      { base: "channel", label: "midi channel" },
      { base: "low", label: "keyzone start",
        format: (value) => Math.round(value) + "/" + noteName(value) },
      { base: "high", label: "keyzone end",
        format: (value) => Math.round(value) + "/" + noteName(value) },
      { base: "detune", label: "tune" },
      { base: "transpose", label: "transpose" },
      { base: "delay", label: "delay",
        format: (value) => Math.round(value) + "ms" },
    ];
    for (let part = 0; part < 2; ++part) {
      const cardEl = div("part" + (activePart === part ? " active" : ""));
      const header = div("ph");
      const nameWrap = div("");
      nameWrap.style.cursor = "pointer";
      const partLabel = document.createElement("span");
      partLabel.className = "pn";
      partLabel.textContent = "part " + (part + 1) + ": " + partNames[part];
      nameWrap.append(partLabel);
      nameWrap.addEventListener("click", () => selectPart(part));
      const acts = div("pacts");
      const rename = document.createElement("button");
      rename.className = "mini";
      rename.textContent = "rename";
      rename.addEventListener("click", () => renamePart(part));
      const copy = document.createElement("button");
      copy.className = "mini";
      copy.textContent = "copy";
      copy.addEventListener("click", () => copyPart(part));
      const paste = document.createElement("button");
      paste.className = "mini";
      paste.textContent = "paste";
      paste.disabled = clipboard === null || clipboard.kind !== "part";
      paste.addEventListener("click", () => pastePart(part));
      const mute = makeToggle("active", !partMuted[part], (on) =>
        togglePartMute(part, !on));
      acts.append(rename, copy, paste, mute);
      header.append(nameWrap, acts);
      cardEl.append(header);

      const knobs = div("knobs");
      for (const item of partKnobs) {
        const spec = byName.get(item.base + ".p" + part);
        if (spec !== undefined)
          knobs.append(controlFor(spec, item.label, null, item.format));
      }
      cardEl.append(knobs);

      const levelSpec = byName.get("level.p" + part);
      if (levelSpec !== undefined) cardEl.append(levelSlider(part, levelSpec));

      const vis = div("pvis");
      for (const className of ["pscope", "pspectro"]) {
        const cell = div("pcell");
        const canvas = document.createElement("canvas");
        canvas.className = className;
        canvas.width = 320;
        canvas.height = 200;
        canvas.dataset.part = String(part);
        cell.append(canvas);
        vis.append(cell);
      }
      cardEl.append(vis);
      parts.append(cardEl);
    }
    wrap.append(parts, voiceModeBar(activePart));
    page.append(wrap);
  }

  const FOLLOW_TARGETS = (() => {
    const list = [];
    for (let op = 1; op <= 4; ++op) {
      list.push({
        name: "op" + op + " level",
        lo: "op" + op + "_level_ks_lo",
        hi: "op" + op + "_level_ks_hi",
        vs: "op" + op + "_level_vs",
      });
      list.push({
        name: "op" + op + " feedback",
        lo: "op" + op + "_fb_ks_lo",
        hi: "op" + op + "_fb_ks_hi",
        vs: "op" + op + "_fb_vs",
      });
      list.push({
        name: "op" + op + " amp env",
        lo: "op" + op + "_aenv_ks_lo",
        hi: "op" + op + "_aenv_ks_hi",
        vs: "op" + op + "_aenv_vs",
      });
    }
    list.push({
      name: "pitch envelope",
      lo: "penv_ks_lo", hi: "penv_ks_hi", vs: "penv_vs",
    });
    list.push({
      name: "filter 1 cutoff",
      lo: "f1_cutoff_ks_lo", hi: "f1_cutoff_ks_hi", vs: "f1_cutoff_vs",
    });
    list.push({
      name: "filter env",
      lo: "fenv_ks_lo", hi: "fenv_ks_hi", vs: "fenv_vs",
    });
    return list;
  })();

  const followSelection = { key: 0, vel: 0 };

  function specByBase(base) {
    return byName.get(base + ".p" + activePart) ?? byName.get(base);
  }

  function buildFx(page) {
    const grid = div("fxmod-grid");

    const filterCard = sectionCard("filter 1");
    const filterKnobs = div("knobs");
    for (const base of ["cutoff", "res", "fenv_depth", "lp.comp", "dc.r"]) {
      const spec = specByBase(base);
      if (spec !== undefined) filterKnobs.append(controlFor(spec));
    }
    filterCard.append(filterKnobs);
    const typeSpec = specByBase("filter_type");
    if (typeSpec !== undefined) {
      const typeRow = div("inline");
      const typeLabel = document.createElement("span");
      typeLabel.className = "lbl";
      typeLabel.textContent = "type";
      const typeSelect = makeSelect(FILTERS,
        FILTERS[rawOf(typeSpec)] ?? FILTERS[0],
        (choice) => setRaw(typeSpec, FILTERS.indexOf(choice)));
      typeRow.append(typeLabel, typeSelect);
      filterCard.append(typeRow);
    }
    const filterOn = specByBase("filter_on");
    if (filterOn !== undefined) {
      const toggle = makeToggle("on", rawOf(filterOn) === 1,
        (on) => setRaw(filterOn, on ? 1 : 0));
      toggle.style.marginTop = "10px";
      filterCard.append(toggle);
    }
    grid.append(filterCard);

    const fenvSpecs = ["fenv_r1", "fenv_r2", "fenv_r3", "fenv_r4",
      "fenv_l1", "fenv_l2", "fenv_l3"]
      .map(specByBase).filter((spec) => spec !== undefined);
    const fenvCard = card("filter envelope", fenvSpecs, {
      graph: graphFor(filterRateNames(activePart), filterLevelNames(activePart),
        { curve: "ecurve2" }),
    });
    if (fenvCard) grid.append(fenvCard);

    const pitchWave = specByBase("pitch_wave");
    const plfoGraph = pitchWave === undefined
      ? null : () => lfoSvg(LFO_WAVES[rawOf(pitchWave)] ?? "sine");
    const plfoCard = sectionCard("pitch lfo");
    if (plfoGraph) appendGraph(plfoCard, plfoGraph);
    const plfoKnobs = div("knobs");
    for (const base of ["pitch_rate", "pitch_depth"]) {
      const spec = specByBase(base);
      if (spec !== undefined) plfoKnobs.append(controlFor(spec));
    }
    plfoCard.append(plfoKnobs);
    if (pitchWave !== undefined) {
      const waveRow = div("inline");
      const waveLabel = document.createElement("span");
      waveLabel.className = "lbl";
      waveLabel.textContent = "wave";
      waveRow.append(waveLabel, makeSelect(LFO_WAVES,
        LFO_WAVES[rawOf(pitchWave)] ?? LFO_WAVES[0],
        (choice) => setRaw(pitchWave, LFO_WAVES.indexOf(choice))));
      const modeSpec = specByBase("pitch_mode");
      if (modeSpec !== undefined) {
        const bipolar = makeToggle("bipolar", rawOf(modeSpec) === 1,
          (on) => setRaw(modeSpec, on ? 1 : 0));
        bipolar.style.marginLeft = "auto";
        waveRow.append(bipolar);
      }
      plfoCard.append(waveRow);
    }
    grid.append(plfoCard);

    const penvOn = specByBase("penv_on");
    const penvHead = div("follow-actions");
    if (penvOn !== undefined) {
      penvHead.append(makeToggle("on", rawOf(penvOn) === 1,
        (on) => setRaw(penvOn, on ? 1 : 0)));
    }
    const penvCard = sectionCard("pitch envelope", {
      className: "penvcard",
      headExtra: penvHead.childElementCount > 0 ? penvHead : null,
    });
    appendGraph(penvCard, graphFor(pitchRateNames(activePart),
      pitchLevelNames(activePart),
      { signed: true, scaleMax: 12,
        transform: (value) => value * 12 / 32768 }));
    const penvKnobs = div("knobs");
    for (const base of ["penv_r1", "penv_r2", "penv_r3", "penv_r4",
      "penv_l1", "penv_l2", "penv_l3", "penv_l4"]) {
      const spec = specByBase(base);
      if (spec !== undefined) penvKnobs.append(controlFor(spec));
    }
    penvCard.append(penvKnobs);
    grid.append(penvCard);

    function followCard(kind) {
      const isKey = kind === "key";
      const onSpec = specByBase(isKey ? "key_sens_on" : "vel_sens_on");
      const actions = div("follow-actions");
      const wipe = document.createElement("button");
      wipe.className = "mini";
      wipe.textContent = isKey ? "wipe all key" : "wipe all vel";
      wipe.addEventListener("click", () => {
        for (const target of FOLLOW_TARGETS) {
          const lo = specByBase(isKey ? target.lo : target.vs);
          if (lo !== undefined) setRaw(lo, 0);
          if (isKey) {
            const hi = specByBase(target.hi);
            if (hi !== undefined) setRaw(hi, 0);
          }
        }
        buildPage();
      });
      actions.append(wipe);
      if (onSpec !== undefined) {
        actions.append(makeToggle("on", rawOf(onSpec) === 1, (on) => {
          setRaw(onSpec, on ? 1 : 0);
          buildPage();
        }));
      }
      const cardEl = sectionCard(isKey ? "key follow" : "velocity follow", {
        className: "followcard" +
          (onSpec !== undefined && rawOf(onSpec) === 0 ? " off" : ""),
        headExtra: actions,
      });

      const targetRow = div("inline");
      const targetLabel = document.createElement("span");
      targetLabel.className = "lbl";
      targetLabel.textContent = "target";
      const names = FOLLOW_TARGETS.map((target) => target.name);
      const selection = followSelection[kind];
      targetRow.append(targetLabel, makeSelect(names, names[selection],
        (value) => {
          followSelection[kind] = Math.max(0, names.indexOf(value));
          buildPage();
        }));
      cardEl.append(targetRow);

      const target = FOLLOW_TARGETS[selection];
      const knobs = div("knobs");
      if (isKey) {
        const breakpoint = specByBase("breakpoint");
        if (breakpoint !== undefined) knobs.append(controlFor(breakpoint));
        const lo = specByBase(target.lo);
        const hi = specByBase(target.hi);
        if (lo !== undefined) knobs.append(controlFor(lo, "below"));
        if (hi !== undefined) knobs.append(controlFor(hi, "above"));
      } else {
        const amount = specByBase(target.vs);
        if (amount !== undefined) knobs.append(controlFor(amount, "amount"));
      }
      cardEl.append(knobs);
      return cardEl;
    }
    grid.append(followCard("key"));
    grid.append(followCard("vel"));

    page.append(grid);
  }

  function buildEnvs(page) {
    const columns = div("panel-opcols");
    for (let op = 0; op < 4; ++op) {
      const all = specsForOp(activePart, op)
        .filter((spec) => pageFor(spec) === "envs");
      const used = new Set();
      const take = (test) => all.filter((spec) => {
        const base = baseName(spec.name);
        if (used.has(spec.name) || !test(base)) return false;
        used.add(spec.name);
        return true;
      });
      const osc = take((base) =>
        /^op[1-4]_(ratio|detune|waveform|level|feedback|mute)$/.test(base));
      const amp = take((base) =>
        /^op[1-4]\.eg\./.test(base) ||
        (op === 0 && (base === "attack" || base === "release")));
      const fb = take((base) => /^op[1-4]_fbenv_/.test(base));
      const follow = take((base) =>
        /^op[1-4]_.*_(ks_lo|ks_hi|vs)$/.test(base) ||
        /^op[1-4]\.(velamp|velfm|velrate)\.depth$/.test(base));
      const rest = take(() => true);
      const col = opColumn(op, true);
      const items = [
        { title: "oscillator", specs: osc, className: "osc" },
        {
          title: "amp env", specs: amp, className: "env",
          graph: graphFor(ampRateNames(op, activePart),
                          ampLevelNames(op, activePart)),
        },
        {
          title: "fb env", specs: fb, className: "fbenv",
          graph: graphFor(fbRateNames(op, activePart),
                          fbLevelNames(op, activePart),
                          { signed: true, scaleMax: 100,
                            transform: (value) => value / 327.67 }),
        },
        { title: "key and velocity", specs: follow },
        { title: "other", specs: rest },
      ];
      for (const item of items) {
        const section = card(item.title, item.specs,
          { graph: item.graph, className: item.className });
        if (section) col.append(section);
      }
      columns.append(col);
    }
    page.append(columns);
  }

  function buildLfos(page) {
    const columns = div("panel-opcols");
    for (let op = 0; op < 4; ++op) {
      const all = specsForOp(activePart, op);
      const amp = all.filter((spec) => /^op[1-4]_trem_/.test(baseName(spec.name)));
      const fb = all.filter((spec) => /^op[1-4]_fblfo_/.test(baseName(spec.name)));
      const det = all.filter((spec) => /^op[1-4]_dlfo_/.test(baseName(spec.name)));
      const col = opColumn(op, true);
      const items = [
        { title: "amp lfo", specs: amp, className: "amplfo" },
        { title: "fb lfo", specs: fb, className: "fblfo" },
        { title: "detune lfo", specs: det, className: "dlfo" },
      ];
      for (const item of items) {
        const section = card(item.title, item.specs,
          { className: item.className });
        if (section) col.append(section);
      }
      columns.append(col);
    }
    page.append(columns);
  }

  function naturalSort(a, b) {
    const na = Number((a.name.match(/\d+$/) || ["0"])[0]);
    const nb = Number((b.name.match(/\d+$/) || ["0"])[0]);
    return na - nb;
  }

  function buildSpectra(page) {
    const columns = div("panel-opcols");
    for (let op = 0; op < 4; ++op) {
      const all = specsForOp(activePart, op);
      const range = all.filter((spec) =>
        /^op[1-4]_spec_(range_a|range_b|morph)$/.test(baseName(spec.name)));
      const a = all.filter((spec) =>
        /^op[1-4]_spec_a\d+$/.test(baseName(spec.name))).sort(naturalSort);
      const b = all.filter((spec) =>
        /^op[1-4]_spec_b\d+$/.test(baseName(spec.name))).sort(naturalSort);
      const col = opColumn(op, true);
      const rangeA = all.find((spec) =>
        baseName(spec.name) === `op${op + 1}_spec_range_a`);
      const rangeB = all.find((spec) =>
        baseName(spec.name) === `op${op + 1}_spec_range_b`);
      const preview = () => getPreview(activePart, op);
      const commit = () => commitPreview(activePart, op);
      const rangeCard = card("range", range, {
        beforeChange: (spec) =>
          baseName(spec.name) === `op${op + 1}_spec_morph` ? commit : null,
      });
      const aCard = a.length === 16
        ? spectrumCard("spectrum a", a, b, "a", rangeA,
                       { fromWave: true, preview, onEdit: commit })
        : null;
      const bCard = b.length === 16
        ? spectrumCard("spectrum b", b, a, "b", rangeB,
                       { onEdit: commit })
        : null;
      if (rangeCard) col.append(rangeCard);
      if (aCard) col.append(aCard);
      if (bCard) col.append(bCard);
      requestPreview(activePart, op);
      columns.append(col);
    }
    page.append(columns);
  }

  function buildMod(page) {
    const columns = div("panel-opcols");
    for (let op = 0; op < 4; ++op) {
      const all = specsForOp(activePart, op);
      const env = all.filter((spec) =>
        /^op[1-4]_spec_(env_|env_depth$)/.test(baseName(spec.name)));
      const lfo = all.filter((spec) =>
        /^op[1-4]_spec_lfo_/.test(baseName(spec.name)));
      const aSpecs = all.filter((spec) =>
        /^op[1-4]_spec_a\d+$/.test(baseName(spec.name))).sort(naturalSort);
      const bSpecs = all.filter((spec) =>
        /^op[1-4]_spec_b\d+$/.test(baseName(spec.name))).sort(naturalSort);
      const rangeA = all.find((spec) =>
        baseName(spec.name) === `op${op + 1}_spec_range_a`);
      const rangeB = all.find((spec) =>
        baseName(spec.name) === `op${op + 1}_spec_range_b`);
      const morph = all.find((spec) =>
        baseName(spec.name) === `op${op + 1}_spec_morph`);
      const col = opColumn(op, false);
      const displayCard = document.createElement("section");
      displayCard.className = "card";
      const displayTitle = document.createElement("h3");
      displayTitle.textContent = "spectra";
      displayCard.append(displayTitle,
        spectrumDisplay(aSpecs, bSpecs, rangeA, rangeB, morph,
                        () => getPreview(activePart, op)));
      requestPreview(activePart, op);
      const envCard = card("morph env", env, {
        className: "env",
        graph: graphFor(morphRateNames(op, activePart),
                        morphLevelNames(op, activePart)),
      });
      const lfoCard = card("morph lfo", lfo, { className: "amplfo" });
      col.append(displayCard);
      if (envCard) col.append(envCard);
      if (lfoCard) col.append(lfoCard);
      columns.append(col);
    }
    page.append(columns);
  }

  function drawWave(canvas, samples) {
    const context = canvas.getContext("2d");
    const width = canvas.width;
    const height = canvas.height;
    context.fillStyle = "#000000";
    context.fillRect(0, 0, width, height);
    context.strokeStyle = "#1c1c1c";
    context.lineWidth = 1;
    for (let i = 1; i < 8; ++i) {
      const x = (i / 8) * width;
      context.beginPath();
      context.moveTo(x, 0);
      context.lineTo(x, height);
      context.stroke();
    }
    context.beginPath();
    context.moveTo(0, height / 2);
    context.lineTo(width, height / 2);
    context.strokeStyle = "#2e2e2e";
    context.stroke();
    const data = samples || new Float32Array(512);
    context.beginPath();
    for (let i = 0; i < data.length; ++i) {
      const x = (i / Math.max(1, data.length - 1)) * width;
      const y = height / 2 - Math.tanh(data[i] * 4) * height * 0.42;
      if (i === 0) context.moveTo(x, y);
      else context.lineTo(x, y);
    }
    context.strokeStyle = "#3dff9e";
    context.lineWidth = 2;
    context.stroke();
  }

  function drawOutput() {
    if (visMode === "spectro" && analyser !== null) {
      const data = new Uint8Array(analyser.frequencyBinCount);
      analyser.getByteFrequencyData(data);
      const values = [];
      for (let i = 0; i < 32; ++i) {
        const start = Math.floor(Math.pow(i / 32, 2) * data.length);
        const end = Math.max(start + 1,
          Math.floor(Math.pow((i + 1) / 32, 2) * data.length));
        let peak = 0;
        for (let k = start; k < end && k < data.length; ++k)
          peak = Math.max(peak, data[k]);
        values.push(peak / 255);
      }
      drawBars(scopeEl, values, 32);
    } else {
      drawWave(scopeEl, outputSamples);
    }
  }

  function drawBars(canvas, values, count) {
    const context = canvas.getContext("2d");
    const width = canvas.width;
    const height = canvas.height;
    context.fillStyle = "#000000";
    context.fillRect(0, 0, width, height);
    const barWidth = width / count;
    for (let i = 0; i < count; ++i) {
      const level = Math.max(0, Math.min(1, values[i] ?? 0));
      const barHeight = level * (height - 2);
      context.fillStyle = i % 5 === 0 ? "#c780ff" : "#ff2e7e";
      context.fillRect(i * barWidth + 1, height - barHeight - 1,
        Math.max(1, barWidth - 2), barHeight);
    }
  }

  function drawOpSpectrum(canvas, samples) {
    const count = 22;
    const data = samples || new Float32Array(512);
    const values = [];
    for (let bar = 0; bar < count; ++bar) {
      const frequency = 40 * Math.pow(500, bar / (count - 1));
      let re = 0;
      let im = 0;
      for (let i = 0; i < data.length; ++i) {
        const window = 0.5 - 0.5 * Math.cos(
          2 * Math.PI * i / (data.length - 1));
        const phase = 2 * Math.PI * frequency * i / 48000;
        re += data[i] * window * Math.cos(phase);
        im += data[i] * window * Math.sin(phase);
      }
      const magnitude = Math.hypot(re, im) / Math.max(1, data.length);
      values.push(Math.max(0.04, Math.min(1,
        Math.log10(1 + magnitude * 360))));
    }
    drawBars(canvas, values, count);
  }

  function drawTapScopes() {
    document.querySelectorAll(".panel-opscope").forEach((canvas) => {
      const op = Number(canvas.dataset.op);
      const samples = tapSamples && tapSamples.channels
        ? tapSamples.channels[op] : null;
      drawWave(canvas, samples);
    });
    document.querySelectorAll(".panel-opspectro").forEach((canvas) => {
      const op = Number(canvas.dataset.op);
      const samples = tapSamples && tapSamples.channels
        ? tapSamples.channels[op] : null;
      drawOpSpectrum(canvas, samples);
    });
    document.querySelectorAll(".pscope").forEach((canvas) => {
      const part = Number(canvas.dataset.part);
      drawWave(canvas, tapRoutes ? tapRoutes[part] : null);
    });
    document.querySelectorAll(".pspectro").forEach((canvas) => {
      const part = Number(canvas.dataset.part);
      drawOpSpectrum(canvas, tapRoutes ? tapRoutes[part] : null);
    });
  }

  function buildTabs() {
    tabsEl.innerHTML = "";
    for (const [id, label] of TABS) {
      const button = document.createElement("button");
      button.className = "tab" + (id === currentPage ? " active" : "");
      button.textContent = label;
      button.addEventListener("click", () => {
        currentPage = id;
        buildTabs();
        buildPage();
      });
      tabsEl.append(button);
    }
  }

  function buildPage() {
    graphUpdaters.length = 0;
    spectrumUpdaters.length = 0;
    bodyEl.innerHTML = "";
    const page = div("panel-page");
    if (currentPage === "patch") buildPatch(page);
    else if (currentPage === "fx") buildFx(page);
    else if (currentPage === "envs") buildEnvs(page);
    else if (currentPage === "lfos") buildLfos(page);
    else if (currentPage === "spectra") buildSpectra(page);
    else buildMod(page);
    bodyEl.append(page);
    window.__p1.page = currentPage;
    window.__p1.controlCount =
      bodyEl.querySelectorAll(".knob, .param-enum, .param-toggle").length;
    requestAnimationFrame(() => {
      drawTapScopes();
      drawOutput();
    });
  }

  function buildAlgorithm() {
    const spec = byName.get("algorithm.p" + activePart);
    if (spec === undefined) return;
    algorithmEl.innerHTML = "";
    for (let value = 0; value <= 7; ++value) {
      const option = document.createElement("option");
      option.value = String(value);
      option.textContent = String(value);
      algorithmEl.append(option);
    }
    algorithmEl.value = String(rawOf(spec));
    algGraphEl.innerHTML = algSvg(rawOf(spec));
    algorithmEl.onchange = function () {
      const value = Number(algorithmEl.value);
      setRaw(spec, value);
      algGraphEl.innerHTML = algSvg(value);
      buildPage();
    };
  }

  function setStatus(text) {
    statusEl.textContent = text;
  }

  async function startAudio() {
    if (params.length === 0) return;
    startButton.disabled = true;
    setStatus("starting audio");
    workletReady = false;
    try {
      const requested = Number(
        new URLSearchParams(location.search).get("rate")) || 48000;
      audioContext = new AudioContext({
        sampleRate: requested,
        latencyHint: "interactive",
      });
      await audioContext.resume();
      const bytes = await (await fetch("p1.wasm")).arrayBuffer();
      await audioContext.audioWorklet.addModule("p1-worklet.js");
      node = new AudioWorkletNode(audioContext, "p1-engine", {
        numberOfInputs: 0,
        numberOfOutputs: 1,
        outputChannelCount: [2],
      });
      node.port.onmessage = function (event) {
        const message = event.data;
        if (message.type === "ready") {
          const rate = audioContext.sampleRate;
          workletReady = true;
          window.__p1.ready = true;
          window.__p1.rate = rate;
          window.__p1.paramCount = message.paramCount;
          const rateNote = rate === 48000
            ? "" : ` · engine 48000 Hz resampled to ${rate} Hz`;
          window.__p1.resampled = rate !== 48000;
          setStatus(`ready · ${rate} Hz · ${message.paramCount} parameters${rateNote}`);
          node.port.postMessage({
            type: "params",
            values: Array.from(values.entries()),
          });
          node.port.postMessage({ type: "part", part: activePart });
          for (let op = 0; op < 4; ++op)
            requestPreview(activePart, op);
        } else if (message.type === "scope") {
          outputSamples = message.samples;
          let peak = 0;
          for (const sample of message.samples)
            peak = Math.max(peak, Math.abs(sample));
          window.__p1.lastScopeMax = peak;
          drawOutput();
        } else if (message.type === "taps") {
          if (message.part === activePart) {
            tapSamples = message;
            let peak = 0;
            for (const channel of message.channels)
              for (const sample of channel)
                peak = Math.max(peak, Math.abs(sample));
            window.__p1.lastTapPeak = peak;
            if (message.routes) {
              tapRoutes = message.routes;
              let routePeak = 0;
              for (const channel of message.routes)
                for (const sample of channel)
                  routePeak = Math.max(routePeak, Math.abs(sample));
              window.__p1.lastRoutePeak = routePeak;
            }
            drawTapScopes();
          }
        } else if (message.type === "param") {
          window.__p1.lastParam = message.value;
        } else if (message.type === "analysis") {
          previewCache.set(message.key, {
            wave: message.wave,
            first: message.first,
            levels: message.levels,
          });
          previewRequests.delete(message.request);
          window.__p1.lastAnalysis = message;
          for (const update of spectrumUpdaters) update();
        } else if (message.type === "error") {
          window.__p1.error = message.message;
          setStatus(`engine error: ${message.message}`);
        }
      };
      node.port.postMessage({ type: "module", bytes }, [bytes]);
      analyser = audioContext.createAnalyser();
      analyser.fftSize = 1024;
      node.connect(analyser);
      analyser.connect(audioContext.destination);
    } catch (error) {
      window.__p1.error = error.message;
      workletReady = false;
      setStatus(`start failed: ${error.message}`);
      startButton.disabled = false;
    }
  }

  function noteOn() {
    if (node === null || noteHeld) return;
    noteHeld = true;
    node.port.postMessage({
      type: "note", on: true, note: 60, velocity: 100, part: activePart,
    });
  }

  function noteOff() {
    if (node === null || !noteHeld) return;
    noteHeld = false;
    node.port.postMessage({ type: "note", on: false, note: 60 });
  }

  startButton.addEventListener("click", startAudio);
  if (patchButton !== null)
    patchButton.addEventListener("click", openMultiBrowser);
  if (patchRenameButton !== null) {
    patchRenameButton.addEventListener("click", () => {
      openRename("rename patch", patchName, (value) => {
        patchName = value;
        showPatchName();
        saveLibrary();
        toast("patch renamed");
      });
    });
  }
  visSeg.addEventListener("click", function (event) {
    const button = event.target.closest("button");
    if (button === null) return;
    visMode = button.dataset.mode === "spectro" ? "spectro" : "scope";
    window.__p1.visMode = visMode;
    visSeg.querySelectorAll("button").forEach((item) =>
      item.classList.toggle("active", item === button));
    drawOutput();
  });
  noteButton.addEventListener("pointerdown", function (event) {
    event.preventDefault();
    noteOn();
  });
  noteButton.addEventListener("pointerup", noteOff);
  noteButton.addEventListener("pointerleave", noteOff);
  noteButton.addEventListener("pointercancel", noteOff);
  window.addEventListener("blur", noteOff);

  setInterval(function () {
    for (const update of graphUpdaters) update();
    for (const update of spectrumUpdaters) update();
    for (let op = 0; op < 4; ++op)
      requestPreview(activePart, op);
  }, 120);

  async function init() {
    try {
      const response = await fetch("params.json");
      if (!response.ok) throw new Error(`params.json: ${response.status}`);
      const data = await response.json();
      params = data.params;
      byName = new Map(params.map((spec) => [spec.name, spec]));
      values = new Map(params.map((spec) => [spec.id, spec.default]));
      window.__p1.paramCount = params.length;
      loadLibrary();
      if (currentMulti >= 0) applyMulti(currentMulti);
      buildAlgorithm();
      buildTabs();
      buildToolbar();
      buildPage();
      showPatchName();
      drawOutput();
      setStatus(`loaded ${params.length} parameters; press start audio`);
    } catch (error) {
      window.__p1.error = error.message;
      setStatus(`load failed: ${error.message}`);
    }
  }

  init();
})();
