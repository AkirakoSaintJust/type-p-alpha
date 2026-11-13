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

  await page.locator(".tab").nth(4).click();
  await page.waitForTimeout(200);
  const bars = await page.locator(".specbars").first().boundingBox();
  await page.mouse.click(bars.x + bars.width * 0.5, bars.y + bars.height * 0.25);
  await page.waitForTimeout(200);
  const barValue = await page.evaluate(() => window.__p1.lastParam);
  console.log("spectrum bar value:", barValue);
  if (!(barValue > 0)) throw new Error("the spectrum bar did not reach the engine");

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

  await page.selectOption("#part", "1");
  await page.waitForTimeout(200);
  const partState = await page.evaluate(() => window.__p1);
  console.log("part two:", partState.page, partState.controlCount);
  if (partState.controlCount <= 0)
    throw new Error("part two page has no controls");
  if (errors.length > 0) throw new Error(errors.join("; "));
  console.log("panel smoke: passed");
} finally {
  await browser.close();
}
