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

  const tabsEl = document.querySelector("#tabs");
  const bodyEl = document.querySelector("#body");
  const statusEl = document.querySelector("#status");
  const partEl = document.querySelector("#part");
  const algorithmEl = document.querySelector("#algorithm");
  const startButton = document.querySelector("#start");
  const noteButton = document.querySelector("#note");
  const scopeEl = document.querySelector("#scope");

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

  window.__p1 = {
    ready: false,
    rate: 0,
    paramCount: 0,
    lastParam: null,
    lastScopeMax: 0,
    lastTapPeak: 0,
    page: "patch",
    controlCount: 0,
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

  function card(title, specs, options) {
    options = options || {};
    if (specs.length === 0) return null;
    const el = document.createElement("section");
    el.className = "card" + (options.className ? " " + options.className : "");
    const heading = document.createElement("h3");
    heading.textContent = title;
    el.append(heading);
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
      const canvas = document.createElement("canvas");
      canvas.className = "panel-opscope";
      canvas.width = 512;
      canvas.height = 44;
      canvas.dataset.op = String(op);
      col.append(canvas);
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
    for (const [title, specs] of [
      ["filter", filter], ["pitch", pitch],
      ["key and velocity follow", follow], ["other", rest],
    ]) {
      const item = card(title, specs);
      if (item) grid.append(item);
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
      for (const [title, specs] of [
        ["oscillator", osc], ["amp env", amp], ["fb env", fb],
        ["key and velocity", follow], ["other", rest],
      ]) {
        const item = card(title, specs);
        if (item) col.append(item);
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
      for (const [title, specs] of [
        ["amp lfo", amp], ["fb lfo", fb], ["detune lfo", det],
      ]) {
        const item = card(title, specs);
        if (item) col.append(item);
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
      const rangeCard = card("range", range);
      const aCard = card("spectrum a", a, { className: "spectrum-card" });
      const bCard = card("spectrum b", b, { className: "spectrum-card" });
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
      const col = opColumn(op, true);
      const envCard = card("morph env", env);
      const lfoCard = card("morph lfo", lfo);
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
    drawWave(scopeEl, outputSamples);
  }

  function drawTapScopes() {
    document.querySelectorAll(".panel-opscope").forEach((canvas) => {
      const op = Number(canvas.dataset.op);
      const samples = tapSamples && tapSamples.channels
        ? tapSamples.channels[op] : null;
      drawWave(canvas, samples);
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
    algorithmEl.onchange = function () {
      setRaw(spec, Number(algorithmEl.value));
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
      node.connect(audioContext.destination);
      node.port.postMessage({ type: "module", bytes }, [bytes]);
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
  noteButton.addEventListener("pointerdown", function (event) {
    event.preventDefault();
    noteOn();
  });
  noteButton.addEventListener("pointerup", noteOff);
  noteButton.addEventListener("pointerleave", noteOff);
  noteButton.addEventListener("pointercancel", noteOff);
  window.addEventListener("blur", noteOff);

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
