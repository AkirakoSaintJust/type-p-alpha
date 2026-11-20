import { chromium } from "playwright";

const url = process.argv[2] ?? "http://127.0.0.1:8080/";
const channel = process.env.PLAYWRIGHT_CHANNEL || undefined;
const browser = await chromium.launch({
  ...(channel === undefined ? {} : { channel }),
  headless: true,
  args: ["--autoplay-policy=no-user-gesture-required"],
});
const page = await browser.newPage();
const errors = [];
page.on("pageerror", (error) => errors.push(error.message));
page.on("console", (message) => {
  if (message.type() === "error") console.log("page:", message.text());
});

try {
  await page.goto(url, { waitUntil: "load" });
  await page.waitForFunction(
    () => window.__p1 && window.__p1.paramCount === 1646, {
    timeout: 20000,
  });
  const tabs = await page.locator(".tab").allTextContents();
  console.log("tabs:", tabs.join(" | "));
  if (tabs.length !== 7) throw new Error(`expected 7 tabs, got ${tabs.length}`);

  await page.click("#start");
  await page.waitForFunction(
    () => window.__p1.ready || window.__p1.error,
    { timeout: 20000 },
  );
  const ready = await page.evaluate(() => window.__p1);
  console.log("ready:", ready);
  if (!ready.ready) throw new Error(ready.error || "engine did not start");
  if (ready.paramCount !== 1646)
    throw new Error(`param count ${ready.paramCount}`);

  await page.click('#visSeg button[data-mode="spectro"]');
  await page.waitForTimeout(200);
  const visMode = await page.evaluate(() => window.__p1.visMode);
  if (visMode !== "spectro") throw new Error("output spectro did not engage");
  await page.click('#visSeg button[data-mode="scope"]');

  const counts = [];
  for (let index = 0; index < tabs.length; ++index) {
    await page.locator(".tab").nth(index).click();
    await page.waitForTimeout(100);
    const state = await page.evaluate(() => window.__p1);
    counts.push(`${state.page}:${state.controlCount}`);
    if (state.controlCount <= 0)
      throw new Error(`page ${state.page} has no controls`);
  }
  console.log("pages:", counts.join(" | "));

  for (const [index, name] of [[3, "lfos"], [4, "spectra a"], [5, "spectra b"], [6, "mod"]]) {
    await page.locator(".tab").nth(index).click();
    await page.waitForTimeout(120);
    const oscCount = await page.locator(".opcol .card.osc").count();
    console.log(`${name} oscillator cards:`, oscCount);
    if (oscCount !== 4)
      throw new Error(`expected 4 oscillator cards on ${name}`);
  }

  await page.locator(".tab").nth(3).click();
  await page.waitForTimeout(120);
  await page.locator(".opcol").first()
    .locator(".card.amplfo .sec-actions button", { hasText: "copy" }).click();
  await page.locator(".opcol").nth(1)
    .locator(".card.amplfo .sec-actions button", { hasText: "paste" }).click();
  await page.waitForTimeout(200);
  const lfoRates = await page.evaluate(() => [
    window.__p1.paramValue("op1_trem_rate.p0"),
    window.__p1.paramValue("op2_trem_rate.p0"),
  ]);
  console.log("amp lfo copy:", lfoRates[0], lfoRates[1]);
  if (lfoRates[0] !== lfoRates[1])
    throw new Error("the lfo card paste did not copy the values");

  await page.locator(".tab").nth(2).click();
  await page.waitForTimeout(100);
  await page.locator(".tab").nth(4).click();
  await page.waitForTimeout(200);
  const spectraRow = page.locator(".spectrum-row").first();
  await spectraRow.locator(".template-btn").click();
  await page.locator(".template-menu button", { hasText: "saw" }).first().click();
  await page.waitForFunction(
    () => (window.__p1.paramValue("op1_spec_a2.p0") ?? 0) > 5,
    { timeout: 5000 });
  const templateLevels = await page.evaluate(() => [1, 2, 3].map((h) =>
    window.__p1.paramValue("op1_spec_a" + h + ".p0")));
  console.log("template saw:", templateLevels.join(" "));
  if (!(templateLevels[1] > 5))
    throw new Error("the spectrum template did not load the saw");
  await page.locator(".opcol").first()
    .locator(".param-enum").filter({ hasText: "wave" }).locator("select")
    .selectOption({ label: "spectrum" });
  await page.waitForTimeout(150);
  await page.locator(".specbars").first().scrollIntoViewIfNeeded();
  const bars = await page.locator(".specbars").first().boundingBox();
  await page.mouse.click(bars.x + bars.width * 0.5, bars.y + bars.height * 0.25);
  await page.waitForTimeout(200);
  const waveform = await page.evaluate(() =>
    window.__p1.paramValue("op1_waveform.p0"));
  const barValue = await page.evaluate(() => window.__p1.lastParam);
  console.log("spectrum bar value:", barValue, "waveform:", waveform);
  if (waveform !== 13)
    throw new Error("the bar edit cleared the spectrum mode");
  if (!(barValue > 0)) throw new Error("the spectrum bar did not reach the engine");

  const morphKnob = page.locator(".knob:has(.knob-name:text-is('morph'))")
    .first();
  await morphKnob.scrollIntoViewIfNeeded();
  const morphBox = await morphKnob.locator(".dial").boundingBox();
  await page.mouse.move(morphBox.x + morphBox.width / 2,
    morphBox.y + morphBox.height / 2);
  await page.mouse.down();
  await page.mouse.move(morphBox.x + morphBox.width / 2,
    morphBox.y + morphBox.height / 2 - 24);
  await page.mouse.up();
  await page.waitForTimeout(250);
  const morphWave = await page.evaluate(() =>
    window.__p1.paramValue("op1_waveform.p0"));
  const morphValue = await page.evaluate(() =>
    window.__p1.paramValue("op1_spec_morph.p0"));
  console.log("morph knob:", morphValue, "waveform:", morphWave);
  if (morphWave !== 13)
    throw new Error("the morph knob cleared the spectrum mode");

  await page.locator(".opcol").nth(0)
    .locator(".osc .sec-actions button", { hasText: "copy" }).click();
  await page.locator(".opcol").nth(1)
    .locator(".osc .sec-actions button", { hasText: "paste" }).click();
  await page.waitForTimeout(200);
  const op2Morph = await page.evaluate(() =>
    window.__p1.paramValue("op2_spec_morph.p0"));
  console.log("oscillator copy morph:", op2Morph);
  if (op2Morph !== 15)
    throw new Error("the oscillator card copy missed the morph knob");

  await page.locator(".spectrum-row").first()
    .locator(".sec-actions button", { hasText: "copy" }).click();
  await page.locator(".spectrum-row").nth(1)
    .locator(".sec-actions button", { hasText: "paste" }).click();
  await page.waitForTimeout(250);
  const spectrumCopy = await page.evaluate(() => [
    window.__p1.paramValue("op1_spec_a1.p0"),
    window.__p1.paramValue("op2_spec_a1.p0"),
  ]);
  console.log("spectrum copy:", spectrumCopy[0], spectrumCopy[1]);
  if (spectrumCopy[0] !== spectrumCopy[1])
    throw new Error("the spectrum paste did not copy the levels");

  const op1Row = page.locator(".spectrum-row").first();
  await op1Row.locator(".row-range").selectOption({ label: "65-128" });
  await page.waitForTimeout(150);
  await op1Row.locator(".sec-actions button", { hasText: "copy" }).click();
  await page.locator(".spectrum-row").nth(1)
    .locator(".sec-actions button", { hasText: "paste" }).click();
  await page.waitForTimeout(250);
  const copiedRange = await page.locator(".spectrum-row").nth(1)
    .locator(".row-range").evaluate((el) => el.selectedOptions[0].textContent);
  const copiedWave = await page.evaluate(() =>
    window.__p1.paramValue("op2_waveform.p0"));
  console.log("spectrum copy range:", copiedRange, "waveform:", copiedWave);
  if (copiedRange !== "65-128" || copiedWave !== 13)
    throw new Error("the spectrum copy missed the range or the mode");
  await op1Row.locator(".row-range").selectOption({ label: "1-64" });
  await page.locator(".spectrum-row").nth(1)
    .locator(".row-range").selectOption({ label: "1-64" });
  await page.waitForTimeout(150);

  const firstRow = page.locator(".spectrum-row").first();
  await firstRow.locator(".specbars").scrollIntoViewIfNeeded();
  const zeroBars = await firstRow.locator(".specbars").boundingBox();
  await page.mouse.move(zeroBars.x + 2, zeroBars.y + zeroBars.height - 2);
  await page.mouse.down();
  for (let index = 0; index < 64; ++index) {
    await page.mouse.move(
      zeroBars.x + (index + 0.5) / 64 * zeroBars.width,
      zeroBars.y + zeroBars.height + 2);
  }
  await page.mouse.up();
  await page.waitForTimeout(150);
  const zeroed = await page.evaluate(() => Array.from({ length: 64 },
    (_, index) => window.__p1.paramValue("op1_spec_a" + (index + 1) + ".p0")));
  console.log("zeroed spectrum:", zeroed.join(" "));
  if (zeroed.some((level) => level !== 0))
    throw new Error("the bar editor did not clear the spectrum");
  const selection = firstRow.locator(".row-selection");
  await selection.selectOption({ label: "edit 17-32" });
  await page.waitForTimeout(150);
  await page.mouse.click(zeroBars.x + 4.5 / 64 * zeroBars.width,
    zeroBars.y + zeroBars.height * 0.3);
  await page.mouse.click(zeroBars.x + 16.5 / 64 * zeroBars.width,
    zeroBars.y + zeroBars.height * 0.3);
  await page.waitForTimeout(200);
  const windowed = await page.evaluate(() => [5, 17, 33].map((h) =>
    window.__p1.paramValue("op1_spec_a" + h + ".p0")));
  console.log("selection window h5 h17 h33:", windowed.join(" "));
  if (!(windowed[0] === 0 && windowed[1] > 0 && windowed[2] === 0))
    throw new Error("the selection window did not restrict freehand editing");
  await firstRow.locator(".specmask button", { hasText: "live" }).click();
  await page.mouse.click(zeroBars.x + 19.5 / 64 * zeroBars.width,
    zeroBars.y + zeroBars.height * 0.4);
  await page.waitForTimeout(150);
  const liveBlocked = await page.evaluate(() =>
    window.__p1.paramValue("op1_spec_a20.p0"));
  if (liveBlocked !== 0)
    throw new Error("the live mask did not block a silent bar");
  await firstRow.locator(".specmask button", { hasText: "both" }).click();
  await page.mouse.click(zeroBars.x + 19.5 / 64 * zeroBars.width,
    zeroBars.y + zeroBars.height * 0.4);
  await page.waitForTimeout(150);
  const bothAllowed = await page.evaluate(() =>
    window.__p1.paramValue("op1_spec_a20.p0"));
  if (!(bothAllowed > 0))
    throw new Error("the both mask did not allow the silent bar");
  await selection.selectOption({ label: "edit 1-64" });
  await page.waitForTimeout(150);
  await firstRow.locator(".row-group")
    .selectOption({ label: "third P5" });
  await page.waitForTimeout(150);
  const legendText = (await firstRow.locator(".speclegend").textContent())
    .replace(/\s+/g, " ").trim();
  const members = await firstRow.locator(".speclabels span.in-group")
    .allTextContents();
  console.log("group legend:", legendText, "members:", members.join(" "));
  if (!legendText.includes("harmonic third (P5)"))
    throw new Error("the select group legend is wrong");
  if (!members.includes("3") || !members.includes("6") ||
      !members.includes("12"))
    throw new Error("the select group did not mark its harmonics");

  await firstRow.locator(".specmask button", { hasText: "dead" }).click();
  const levelSpring = firstRow.locator(".spring").nth(1);
  const levelTrack = levelSpring.locator(".spring-track");
  await levelTrack.scrollIntoViewIfNeeded();
  const levelBox = await levelTrack.boundingBox();
  await page.mouse.move(levelBox.x + levelBox.width / 2,
    levelBox.y + levelBox.height / 2);
  await page.mouse.down();
  await page.mouse.move(levelBox.x + levelBox.width / 2,
    levelBox.y + levelBox.height / 2 - 40);
  await page.mouse.up();
  await page.waitForTimeout(250);
  const springLevels = await page.evaluate(() => [3, 4, 6, 12].map((h) =>
    window.__p1.paramValue("op1_spec_a" + h + ".p0")));
  const springValue = (await levelSpring.locator(".spring-val").textContent())
    .trim();
  console.log("spring level:", springLevels, "returned:", springValue);
  if (!(springLevels[0] > 0 && springLevels[2] > 0 && springLevels[3] > 0))
    throw new Error("the level spring did not raise the dead group");
  if (springLevels[1] !== 0)
    throw new Error("the level spring touched a harmonic outside the group");
  if (springValue !== "0%")
    throw new Error("the level spring did not return to centre");

  const formantSpring = firstRow.locator(".spring").nth(2);
  const formantTrack = formantSpring.locator(".spring-track");
  const formantBox = await formantTrack.boundingBox();
  await page.mouse.move(formantBox.x + formantBox.width / 2,
    formantBox.y + formantBox.height / 2);
  await page.mouse.down();
  await page.mouse.move(formantBox.x + formantBox.width / 2,
    formantBox.y + formantBox.height / 2 - 40);
  await page.mouse.up();
  await page.waitForTimeout(250);
  const formantLevel = await page.evaluate(() =>
    window.__p1.paramValue("op1_spec_a5.p0"));
  console.log("formant h5:", formantLevel);
  if (!(formantLevel > 0))
    throw new Error("the formant spring did not reach its harmonic");

  await firstRow.locator(".specmask button", { hasText: "both" }).click();
  const tiltBefore = await page.evaluate(() => [3, 48].map((h) =>
    window.__p1.paramValue("op1_spec_a" + h + ".p0")));
  const tiltSpring = firstRow.locator(".spring").first();
  const tiltTrack = tiltSpring.locator(".spring-track");
  const tiltBox = await tiltTrack.boundingBox();
  await page.mouse.move(tiltBox.x + tiltBox.width / 2,
    tiltBox.y + tiltBox.height / 2);
  await page.mouse.down();
  await page.mouse.move(tiltBox.x + tiltBox.width / 2,
    tiltBox.y + tiltBox.height / 2 + 40);
  await page.mouse.up();
  await page.waitForTimeout(250);
  const tiltAfter = await page.evaluate(() => [3, 48].map((h) =>
    window.__p1.paramValue("op1_spec_a" + h + ".p0")));
  const tiltValue = (await tiltSpring.locator(".spring-val").textContent())
    .trim();
  console.log("tilt:", tiltBefore, "to", tiltAfter, "returned:", tiltValue);
  if (!(tiltAfter[1] < tiltBefore[1]))
    throw new Error("the tilt spring did not lower the high harmonics");
  if (!(tiltAfter[0] >= tiltBefore[0]))
    throw new Error("the tilt spring did not raise the low harmonics");
  if (tiltValue !== "0%")
    throw new Error("the tilt spring did not return to centre");

  await page.locator(".tab").nth(6).click();
  await page.waitForTimeout(200);
  const depthKnob = page.locator(".opcol").nth(1).locator(".env .knob")
    .filter({ hasText: "depth" }).first();
  await depthKnob.scrollIntoViewIfNeeded();
  const depthDial = await depthKnob.locator(".dial").boundingBox();
  await page.mouse.move(depthDial.x + depthDial.width / 2,
    depthDial.y + depthDial.height / 2);
  await page.mouse.down();
  await page.mouse.move(depthDial.x + depthDial.width / 2,
    depthDial.y + depthDial.height / 2 - 24);
  await page.mouse.up();
  await page.waitForTimeout(200);
  const depthWave = await page.evaluate(() =>
    window.__p1.paramValue("op2_waveform.p0"));
  console.log("depth sets spectrum:", depthWave);
  if (depthWave !== 13)
    throw new Error("the morph envelope depth did not set spectrum mode");

  await page.locator(".tab").nth(2).click();
  const box = await page.locator("#note").boundingBox();
  await page.mouse.move(box.x + box.width / 2, box.y + box.height / 2);
  await page.mouse.down();
  await page.waitForTimeout(1000);
  const peak = await page.evaluate(() => window.__p1.lastScopeMax);
  const tapPeak = await page.evaluate(() => window.__p1.lastTapPeak);
  const routePeak = await page.evaluate(() => window.__p1.lastRoutePeak);
  await page.mouse.up();
  const morph = await page.evaluate(() => window.__p1.lastMorph);
  console.log("scope peak:", peak, "tap peak:", tapPeak,
    "route peak:", routePeak, "live morph:", morph);
  if (!(peak > 0.0001)) throw new Error("the panel scope stayed silent");
  if (!(tapPeak > 0.0001)) throw new Error("the operator tap scopes stayed silent");
  if (!(routePeak > 0.0001)) throw new Error("the part route scopes stayed silent");
  if (!Array.isArray(morph) || morph.length !== 4)
    throw new Error("the taps message carries no live morph");

  await page.selectOption("#algorithm", "7");
  await page.waitForFunction(() => window.__p1.lastParam === 7, {
    timeout: 5000,
  });
  console.log("algorithm round trip:", await page.evaluate(() => window.__p1.lastParam));

  await page.locator(".tab").nth(2).click();
  await page.waitForTimeout(150);
  await page.locator(".opcol").first()
    .locator(".opcol-acts button", { hasText: "copy" }).click();
  await page.locator(".opcol").nth(1)
    .locator(".opcol-acts button", { hasText: "paste" }).click();
  await page.waitForTimeout(200);
  const opOneLevel = await page.evaluate(() =>
    window.__p1.paramValue("op1_level.p0"));
  const opTwoLevel = await page.evaluate(() =>
    window.__p1.paramValue("op2_level.p0"));
  console.log("op copy:", opOneLevel, opTwoLevel);
  if (opOneLevel !== opTwoLevel)
    throw new Error("the operator paste did not apply");

  await page.locator(".opcol").first().locator(".toggle.mute").click();
  await page.waitForTimeout(150);
  const mutedValue = await page.evaluate(() =>
    window.__p1.paramValue("op1_mute.p0"));
  const toolbarClass = await page.locator(".toolbar .mutebox").first()
    .getAttribute("class");
  console.log("op mute:", mutedValue, toolbarClass);
  if (mutedValue !== 1 || !toolbarClass.includes("muted"))
    throw new Error("the operator mute did not reach the toolbar");
  await page.locator(".opcol").first().locator(".toggle.mute").click();
  await page.waitForTimeout(150);
  const unmutedValue = await page.evaluate(() =>
    window.__p1.paramValue("op1_mute.p0"));
  if (unmutedValue !== 0)
    throw new Error("the operator unmute did not apply");

  await page.locator(".tbtn.partsel", { hasText: "part 2" }).click();
  await page.waitForTimeout(200);
  const partState = await page.evaluate(() => window.__p1);
  console.log("part two:", partState.page, partState.controlCount);
  if (partState.controlCount <= 0)
    throw new Error("part two page has no controls");

  await page.locator(".tab").nth(1).click();
  await page.waitForTimeout(150);
  const fxCards = await page.locator(".fxmod-grid > .card").count();
  const followCards = await page.locator(".followcard").count();
  const filterTypes = await page.locator(".fxmod-grid select").count();
  console.log("fx cards:", fxCards, "follow cards:", followCards);
  if (fxCards !== 6) throw new Error(`expected 6 fx cards, got ${fxCards}`);
  if (followCards !== 2) throw new Error("the follow cards are missing");
  if (filterTypes < 2) throw new Error("the filter type select is missing");

  await page.locator(".tab").nth(0).click();
  await page.waitForTimeout(150);
  const partOne = await page.locator(".part .pn").first().textContent();
  console.log("part label:", partOne);
  if (!partOne.startsWith("part 1: ")) throw new Error("the part label is wrong");

  await page.locator(".part").first()
    .locator("button.mini", { hasText: "rename" }).click();
  await page.locator(".dialog .textin").fill("lead");
  await page.locator(".dialog-foot button", { hasText: /^save$/ }).click();
  await page.waitForTimeout(150);
  if (await page.evaluate(() => window.__p1.partName(0)) !== "lead")
    throw new Error("the part rename did not apply");

  await page.locator(".part").first()
    .locator("button.mini", { hasText: "copy" }).click();
  await page.locator(".part").nth(1)
    .locator("button.mini", { hasText: "paste" }).click();
  await page.waitForTimeout(150);
  if (await page.evaluate(() => window.__p1.partName(1)) !== "lead")
    throw new Error("the part paste did not apply");

  await page.click("#partLibrary");
  await page.waitForSelector(".dialog .partlist.bank");
  await page.locator(".dialog-foot button",
    { hasText: /^save single part 1$/ }).click();
  await page.waitForTimeout(200);
  const partSlots = await page.evaluate(() => window.__p1.partSlots());
  console.log("part slots:", partSlots);
  if (partSlots !== 1) throw new Error("the part library did not save a slot");

  await page.click("#patchRename");
  await page.locator(".dialog .textin").fill("smoke patch");
  await page.locator(".dialog-foot button", { hasText: /^save$/ }).click();
  await page.waitForTimeout(150);
  const named = await page.evaluate(() => window.__p1.patchName());
  console.log("patch name:", named);
  if (named !== "smoke patch")
    throw new Error("the patch rename did not apply");

  await page.click("#patchBtn");
  await page.waitForSelector(".dialog .patchlist");
  await page.locator(".dialog-foot button", { hasText: /^save$/ }).click();
  await page.waitForSelector(".dialog .partlist");
  await page.locator(".dialog-foot button", { hasText: /^save$/ }).click();
  await page.waitForTimeout(250);
  const multiSlots = await page.evaluate(() => window.__p1.multiSlots());
  console.log("multi slots:", multiSlots);
  if (multiSlots !== 1) throw new Error("the multi library did not save a slot");
  const slotsAfterMulti = await page.evaluate(() => window.__p1.partSlots());
  const savedSpectrum = await page.evaluate(() =>
    window.__p1.paramValue("op1_spec_a3.p0"));
  console.log("parts after the multi save:", slotsAfterMulti,
    "h3:", savedSpectrum);
  if (slotsAfterMulti !== 2)
    throw new Error("the multi save did not save both parts");

  await page.reload({ waitUntil: "load" });
  await page.waitForFunction(
    () => window.__p1 && window.__p1.paramCount === 1646, {
    timeout: 20000,
  });
  const persisted = await page.evaluate(() => window.__p1.patchName());
  const persistedSlots = await page.evaluate(() => window.__p1.partSlots());
  const persistedSpectrum = await page.evaluate(() =>
    window.__p1.paramValue("op1_spec_a3.p0"));
  console.log("persisted:", persisted, "part slots:", persistedSlots,
    "h3:", persistedSpectrum);
  if (persisted !== "smoke patch" || persistedSlots !== 2 ||
      persistedSpectrum !== savedSpectrum)
    throw new Error("the library did not persist");

  await page.evaluate(() => {
    const key = "type-p-alpha-web-library";
    const data = JSON.parse(localStorage.getItem(key));
    data.version = 2;
    const multi = data.multis[data.currentMulti];
    const part = data.partSlots[multi.partSlots[0]];
    for (const entry of part.params) {
      if (entry[0] === "cutoff.p0") entry[1] = 8000;
    }
    localStorage.setItem(key, JSON.stringify(data));
  });
  await page.reload({ waitUntil: "load" });
  await page.waitForFunction(
    () => window.__p1 && window.__p1.paramCount === 1646, {
    timeout: 20000,
  });
  const migratedCutoff = await page.evaluate(() =>
    window.__p1.paramValue("cutoff.p0"));
  console.log("migrated cutoff:", migratedCutoff);
  if (migratedCutoff !== 36958)
    throw new Error("the old library cutoff was not migrated");

  const fallbackUrl = new URL("panel.html", new URL(url));
  fallbackUrl.searchParams.set("rate", "44100");
  const fallback = await browser.newPage();
  await fallback.goto(fallbackUrl.toString(), { waitUntil: "load" });
  await fallback.waitForFunction(
    () => window.__p1 && window.__p1.paramCount === 1646, {
    timeout: 20000,
  });
  await fallback.click("#start");
  await fallback.waitForFunction(
    () => window.__p1.ready || window.__p1.error, { timeout: 20000 });
  const fallbackState = await fallback.evaluate(() => ({
    ready: window.__p1.ready,
    rate: window.__p1.rate,
    resampled: window.__p1.resampled,
    error: window.__p1.error,
  }));
  console.log("fallback:", fallbackState);
  if (!fallbackState.ready) throw new Error(fallbackState.error);
  if (fallbackState.rate !== 44100)
    throw new Error(`the fallback context ran at ${fallbackState.rate} Hz`);
  if (!fallbackState.resampled)
    throw new Error("the fallback did not use the output resampler");
  const fallbackNote = await fallback.locator("#note").boundingBox();
  await fallback.mouse.move(fallbackNote.x + fallbackNote.width / 2,
    fallbackNote.y + fallbackNote.height / 2);
  await fallback.mouse.down();
  await fallback.waitForTimeout(800);
  const fallbackPeak = await fallback.evaluate(() => window.__p1.lastScopeMax);
  await fallback.mouse.up();
  console.log("fallback peak:", fallbackPeak);
  if (!(fallbackPeak > 0.0001))
    throw new Error("the resampled output stayed silent");
  await fallback.close();

  if (errors.length > 0) throw new Error(errors.join("; "));
  console.log("panel smoke: passed");
} finally {
  await browser.close();
}
