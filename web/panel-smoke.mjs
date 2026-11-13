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
    () => window.__p1 && window.__p1.paramCount === 846, {
    timeout: 20000,
  });
  const tabs = await page.locator(".tab").allTextContents();
  console.log("tabs:", tabs.join(" | "));
  if (tabs.length !== 6) throw new Error(`expected 6 tabs, got ${tabs.length}`);

  await page.click("#start");
  await page.waitForFunction(
    () => window.__p1.ready || window.__p1.error,
    { timeout: 20000 },
  );
  const ready = await page.evaluate(() => window.__p1);
  console.log("ready:", ready);
  if (!ready.ready) throw new Error(ready.error || "engine did not start");
  if (ready.paramCount !== 846)
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

  await page.locator(".tab").nth(2).click();
  await page.waitForTimeout(100);
  const waveSelect = page.locator(".opcol").first()
    .locator(".param-enum").filter({ hasText: "wave" }).locator("select");
  await waveSelect.selectOption({ label: "saw" });
  await page.waitForFunction(() => {
    const preview = window.__p1.analysisFor(0, 0);
    return preview && preview[1] > 10;
  }, { timeout: 5000 });
  const preview = await page.evaluate(() => window.__p1.analysisFor(0, 0));
  console.log("from wave preview:", preview ? preview.slice(0, 3) : null);

  await page.locator(".tab").nth(4).click();
  await page.waitForTimeout(200);
  const badgeVisible = await page.locator(".fromwave").first().isVisible();
  if (!badgeVisible) throw new Error("the from wave badge is not visible");
  const bars = await page.locator(".specbars").first().boundingBox();
  await page.mouse.click(bars.x + bars.width * 0.5, bars.y + bars.height * 0.25);
  await page.waitForTimeout(200);
  const waveform = await page.evaluate(() =>
    window.__p1.paramValue("op1_waveform.p0"));
  const barValue = await page.evaluate(() => window.__p1.lastParam);
  console.log("spectrum bar value:", barValue, "waveform:", waveform);
  if (waveform !== 13)
    throw new Error("the first bar edit did not commit the preview");
  if (!(barValue > 0)) throw new Error("the spectrum bar did not reach the engine");

  await page.locator(".tab").nth(2).click();
  await page.waitForTimeout(100);
  await page.locator(".opcol").first()
    .locator(".param-enum").filter({ hasText: "wave" }).locator("select")
    .selectOption({ label: "saw" });
  await page.waitForFunction(() => {
    const preview = window.__p1.analysisFor(0, 0);
    return preview && preview[1] > 10;
  }, { timeout: 5000 });
  await page.locator(".tab").nth(4).click();
  await page.waitForTimeout(200);
  const morphKnob = page.locator(".knob:has(.knob-name:text-is('morph'))")
    .first();
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
    throw new Error("the morph knob did not commit the preview");

  await page.locator(".tab").nth(2).click();
  const box = await page.locator("#note").boundingBox();
  await page.mouse.move(box.x + box.width / 2, box.y + box.height / 2);
  await page.mouse.down();
  await page.waitForTimeout(1000);
  const peak = await page.evaluate(() => window.__p1.lastScopeMax);
  const tapPeak = await page.evaluate(() => window.__p1.lastTapPeak);
  const routePeak = await page.evaluate(() => window.__p1.lastRoutePeak);
  await page.mouse.up();
  console.log("scope peak:", peak, "tap peak:", tapPeak,
    "route peak:", routePeak);
  if (!(peak > 0.0001)) throw new Error("the panel scope stayed silent");
  if (!(tapPeak > 0.0001)) throw new Error("the operator tap scopes stayed silent");
  if (!(routePeak > 0.0001)) throw new Error("the part route scopes stayed silent");

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

  await page.reload({ waitUntil: "load" });
  await page.waitForFunction(
    () => window.__p1 && window.__p1.paramCount === 846, {
    timeout: 20000,
  });
  const persisted = await page.evaluate(() => window.__p1.patchName());
  const persistedSlots = await page.evaluate(() => window.__p1.partSlots());
  console.log("persisted:", persisted, "part slots:", persistedSlots);
  if (persisted !== "smoke patch" || persistedSlots !== 1)
    throw new Error("the library did not persist");

  const fallbackUrl = new URL("panel.html", new URL(url));
  fallbackUrl.searchParams.set("rate", "44100");
  const fallback = await browser.newPage();
  await fallback.goto(fallbackUrl.toString(), { waitUntil: "load" });
  await fallback.waitForFunction(
    () => window.__p1 && window.__p1.paramCount === 846, {
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
