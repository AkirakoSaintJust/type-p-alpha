import { chromium } from "playwright";

const url = process.argv[2] ?? "http://127.0.0.1:8080/panel.html";
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
  await page.waitForFunction(() => window.__p1.paramCount === 846, {
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
  const box = await page.locator("#note").boundingBox();
  await page.mouse.move(box.x + box.width / 2, box.y + box.height / 2);
  await page.mouse.down();
  await page.waitForTimeout(1000);
  const peak = await page.evaluate(() => window.__p1.lastScopeMax);
  const tapPeak = await page.evaluate(() => window.__p1.lastTapPeak);
  await page.mouse.up();
  console.log("scope peak:", peak, "tap peak:", tapPeak);
  if (!(peak > 0.0001)) throw new Error("the panel scope stayed silent");
  if (!(tapPeak > 0.0001)) throw new Error("the operator tap scopes stayed silent");

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
