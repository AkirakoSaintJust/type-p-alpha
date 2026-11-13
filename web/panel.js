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
  const partEl = document.querySelector("#part");
  const algorithmEl = document.querySelector("#algorithm");
  const startButton = document.querySelector("#start");
  const noteButton = document.querySelector("#note");
  const scopeEl = document.querySelector("#scope");
  const algGraphEl = document.querySelector("#algGraph");
  const visSeg = document.querySelector("#visSeg");

  const graphUpdaters = [];
  const spectrumUpdaters = [];

  let params = [];
  let byName = new Map();
  let values = new Map();
  let activePart = 0;
  let currentPage = "patch";
  let audioContext = null;
  let node = null;
  let noteHeld = false;
  let outputSamples = null;
  let tapSamples = null;
  let visMode = "scope";
  let analyser = null;

  window.__p1 = {
    ready: false,
    rate: 0,
    paramCount: 0,
    lastParam: null,
    lastScopeMax: 0,
    lastTapPeak: 0,
    page: "patch",
    controlCount: 0,
    visMode: "scope",
    error: null,
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
    const match = base.match(/^op([1-4])/);
    return match === null ? -1 : Number(match[1]) - 1;
  }

  function isWaveform(base) {
    return /^op[1-4]_waveform$/.test(base);
  }

  function pageOf(base) {
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
      if (/^spec_range_[ab]$/.test(tail)) return tail.replace("_", " ");
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
      "dc.r": "dc r",
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

  function controlFor(spec) {
    const base = baseName(spec.name);
    const raw = rawOf(spec);
    const enums = enumValues(spec);
    if (enums !== null) {
      const wrapper = div("param-enum");
      const label = div("knob-name");
      label.textContent = humanLabel(base);
      label.title = base;
      const select = makeSelect(enums, enums[raw] ?? enums[0], function (choice) {
        setRaw(spec, enums.indexOf(choice));
      });
      wrapper.title = base;
      wrapper.append(label, select);
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
      name: humanLabel(base),
      title: base,
      min: range.min,
      max: range.max,
      val: range.toUser(raw),
      step: range.step,
      fmt: function (value) { return formatUser(value, spec); },
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
        : envSvg(rates, levels);
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

  function card(title, specs, options) {
    options = options || {};
    if (specs.length === 0) return null;
    const el = document.createElement("section");
    el.className = "card" + (options.className ? " " + options.className : "");
    const heading = document.createElement("h3");
    heading.textContent = title;
    el.append(heading);
    if (options.graph) {
      const graph = div("envbox");
      graph.innerHTML = options.graph();
      el.append(graph);
      graphUpdaters.push(() => {
        const html = options.graph();
        if (graph.innerHTML !== html) graph.innerHTML = html;
      });
    }
    if (options.note) {
      const note = div("panel-card-note");
      note.textContent = options.note;
      el.append(note);
    }
    const knobs = div("knobs");
    for (const spec of specs) knobs.append(controlFor(spec));
    el.append(knobs);
    return el;
  }

  function clampLevel(value) {
    return Math.max(0, Math.min(99, Math.round(value)));
  }

  function spectrumBars(specs, kind, rangeSpec) {
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

  function spectrumCard(title, specs, otherSpecs, kind, rangeSpec) {
    const cardEl = document.createElement("section");
    cardEl.className = "card speccard";
    const heading = document.createElement("h3");
    heading.textContent = title;
    cardEl.append(heading);
    const bars = spectrumBars(specs, kind, rangeSpec);
    cardEl.append(bars.el, bars.labels);

    let base = null;
    function begin() { base = bars.levels(); }
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
      const levels = bars.levels();
      for (let i = 0; i < 16 && i < otherSpecs.length; ++i)
        setRaw(otherSpecs[i], levels[i]);
    });
    foot.append(norm, to);
    cardEl.append(foot);
    return cardEl;
  }


  function spectrumDisplay(aSpecs, bSpecs, rangeA, rangeB, morphSpec) {
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
      const aLevels = aSpecs.map((spec) => values.get(spec.id) ?? 0);
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
        context.strokeStyle = "rgba(61,255,158,0.62)";
        context.lineWidth = 1.2;
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

  function buildPatch(page) {
    const all = specsForPart(activePart).filter((spec) =>
      pageFor(spec) === "patch" && baseName(spec.name) !== "algorithm");
    const voiceNames = new Set([
      "mode", "legato", "unison_count", "unison_detune", "portamento",
      "note_priority",
    ]);
    const part = all.filter((spec) => !voiceNames.has(baseName(spec.name)));
    const voice = all.filter((spec) => voiceNames.has(baseName(spec.name)));
    const grid = div("panel-grid");
    const partCard = card("part " + (activePart + 1), part);
    const voiceCard = card("voice mode", voice);
    if (partCard) grid.append(partCard);
    if (voiceCard) grid.append(voiceCard);
    page.append(grid);
  }

  function buildFx(page) {
    const all = specsForPart(activePart).filter((spec) => pageFor(spec) === "fx");
    const used = new Set();
    const take = (test) => all.filter((spec) => {
      const base = baseName(spec.name);
      if (used.has(spec.name) || !test(base)) return false;
      used.add(spec.name);
      return true;
    });
    const filter = take((base) =>
      !/_(ks_lo|ks_hi|vs)$/.test(base) && /cutoff|res|filter|fenv_/.test(base));
    const pitch = take((base) =>
      !/_(ks_lo|ks_hi|vs)$/.test(base) && /^(pitch_|penv_)/.test(base));
    const follow = take((base) =>
      /_(ks_lo|ks_hi|vs)$/.test(base) ||
      base === "breakpoint" || base === "key_sens_on" || base === "vel_sens_on");
    const rest = take(() => true);
    const grid = div("panel-grid");
    const items = [
      {
        title: "filter", specs: filter,
        graph: graphFor(filterRateNames(activePart),
                        filterLevelNames(activePart)),
      },
      {
        title: "pitch", specs: pitch,
        graph: graphFor(pitchRateNames(activePart),
                        pitchLevelNames(activePart),
                        { signed: true, scaleMax: 12,
                          transform: (value) => value * 12 / 32768 }),
      },
      { title: "key and velocity follow", specs: follow },
      { title: "other", specs: rest },
    ];
    for (const item of items) {
      const section = card(item.title, item.specs, { graph: item.graph });
      if (section) grid.append(section);
    }
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
      const amp = take((base) => /^op[1-4]\.eg\./.test(base));
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
      const rangeCard = card("range", range);
      const aCard = a.length === 16
        ? spectrumCard("spectrum a", a, b, "a", rangeA) : null;
      const bCard = b.length === 16
        ? spectrumCard("spectrum b", b, a, "b", rangeB) : null;
      if (rangeCard) col.append(rangeCard);
      if (aCard) col.append(aCard);
      if (bCard) col.append(bCard);
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
        spectrumDisplay(aSpecs, bSpecs, rangeA, rangeB, morph));
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
    try {
      audioContext = new AudioContext({
        sampleRate: 48000,
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
          window.__p1.ready = true;
          window.__p1.rate = rate;
          window.__p1.paramCount = message.paramCount;
          const warning = rate === 48000
            ? "" : ` · context is ${rate} Hz, engine is 48000 Hz`;
          setStatus(`ready · ${rate} Hz · ${message.paramCount} parameters${warning}`);
          node.port.postMessage({
            type: "params",
            values: Array.from(values.entries()),
          });
          node.port.postMessage({ type: "part", part: activePart });
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
            drawTapScopes();
          }
        } else if (message.type === "param") {
          window.__p1.lastParam = message.value;
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
  }, 120);

  partEl.addEventListener("change", function () {
    noteOff();
    activePart = Number(partEl.value) === 1 ? 1 : 0;
    tapSamples = null;
    buildAlgorithm();
    buildPage();
    if (node !== null)
      node.port.postMessage({ type: "part", part: activePart });
  });

  async function init() {
    try {
      const response = await fetch("params.json");
      if (!response.ok) throw new Error(`params.json: ${response.status}`);
      const data = await response.json();
      params = data.params;
      byName = new Map(params.map((spec) => [spec.name, spec]));
      values = new Map(params.map((spec) => [spec.id, spec.default]));
      window.__p1.paramCount = params.length;
      partEl.value = "0";
      buildAlgorithm();
      buildTabs();
      buildPage();
      drawOutput();
      setStatus(`loaded ${params.length} parameters; press start audio`);
    } catch (error) {
      window.__p1.error = error.message;
      setStatus(`load failed: ${error.message}`);
    }
  }

  init();
})();
